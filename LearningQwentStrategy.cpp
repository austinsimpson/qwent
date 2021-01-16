#include "LearningQwentStrategy.h"

#include "QwentGame.h"
#include "Range.h"

#include <QVector>
#include <QDataStream>

#include <QDebug>

constexpr int kScoreBucketSize = 5;
constexpr auto kFileMagic = 0x71746169; //"qtai" in hex

LearningQwentStrategy::LearningQwentStrategy
(
	size_t owningPlayerIndex
):
	_owningPlayerIndex{ owningPlayerIndex },
	_rewardFunction(std::bind(&LearningQwentStrategy::winRoundRewardFunction, this, std::placeholders::_1, std::placeholders::_2))
{
}

LearningQwentStrategy::LearningQwentStrategy
(
	size_t owningPlayerIndex,
	size_t numberOfCards
):
	_owningPlayerIndex{ owningPlayerIndex },
	_forfeitActionIndex{ numberOfCards },
	_rewardFunction(std::bind(&LearningQwentStrategy::winRoundRewardFunction, this, std::placeholders::_1, std::placeholders::_2))
{
}

void LearningQwentStrategy::performTurn
(
	QwentGame* game
)
{
	MatchSnapshot currentSnapshot = bucketizeSnapshot(game->takeSnapshot());
	const QVector<QWeakPointer<Card>>& currentHand = game->getHand(_owningPlayerIndex);

	if (_hasLastAction)
	{
		double reward = 0;
		double currentQValue = getQValue(_lastSnapshot, _lastAction);
		double maximalQValueInCurrentState = std::get<1>(getActionWithMaximalOutcome(currentSnapshot, currentHand));
		setQValue(_lastSnapshot, _lastAction, (1. - _learningRate) * currentQValue +  _learningRate * (reward + _discountFactor * maximalQValueInCurrentState));
	}

	Action nextAction = chooseNextAction(currentSnapshot, currentHand);

	_lastAction = nextAction;
	_hasLastAction = true;
	_lastSnapshot = currentSnapshot;

	if (nextAction.actionIndex < _forfeitActionIndex)
	{
		const auto findCardLambda = [nextAction](const QWeakPointer<Card>& card)
		{
			return card.toStrongRef()->id() == nextAction.actionIndex;
		};

		const auto& cardIterator = std::find_if(currentHand.cbegin(), currentHand.cend(), findCardLambda);
		size_t cardIndexInHand = cardIterator - currentHand.cbegin();
		game->playCard(_owningPlayerIndex, cardIndexInHand, nextAction.fieldPosition);
	}
	else
	{
		game->endTurn(true);
	}
}

void LearningQwentStrategy::notifyGameOver
(
	QwentGame* game
)
{
	if (_hasLastAction)
	{
		bool didWin = game->currentMatch().winningPlayer() == _owningPlayerIndex;

		double reward = didWin ? _winReward : _lossPenalty;
		double currentQValue = getQValue(_lastSnapshot, _lastAction);
		//Since this is a terminal state, we can ignore max_a'(Q(s,a'))
		setQValue(_lastSnapshot, _lastAction, (1. - _learningRate) * currentQValue +  _learningRate * reward);

		_hasLastAction = false;
		_lastAction.actionIndex = 0;
		_lastAction.fieldPosition = FieldPosition::Any;
	}
}

Action LearningQwentStrategy::chooseNextAction
(
	const MatchSnapshot& matchSnapshot,
	const QVector<QWeakPointer<Card>>& currentHand
)
{
	std::uniform_real_distribution dist(0., 1.);

	if (_isLearning && dist(_randomNumberEngine) < _explorationRate)
	{
		std::uniform_int_distribution actionDistribution(0, currentHand.size());
		int cardIndex = actionDistribution(_randomNumberEngine);
		if (cardIndex < currentHand.count())
		{
			const Card& card = *currentHand[cardIndex].toStrongRef();
			if (card.fieldPosition() == FieldPosition::Any)
			{
				std::uniform_int_distribution fieldPositionDistribution(0, 2);
				return Action{ .actionIndex = card.id(), .fieldPosition = static_cast<FieldPosition>(fieldPositionDistribution(_randomNumberEngine)) };
			}
			else
			{
				return Action{ .actionIndex = card.id(), .fieldPosition = card.fieldPosition() };
			}
		}
		else
		{
			return Action { .actionIndex = _forfeitActionIndex, .fieldPosition = FieldPosition::Any };
		}

	}
	else
	{
		return std::get<0>(getActionWithMaximalOutcome(matchSnapshot, currentHand));
	}
}

std::tuple<Action, double> LearningQwentStrategy::getActionWithMaximalOutcome
(
	const MatchSnapshot& matchSnapshot,
	const QVector<QWeakPointer<Card>>& currentHand
)
{
	double maximalQValue = std::numeric_limits<double>::lowest();
	Action actionToExecute;

	for (const QWeakPointer<Card>& card : currentHand)
	{
		if (card.toStrongRef()->fieldPosition() != FieldPosition::Any)
		{
			Action currentAction{ .actionIndex = card.toStrongRef()->id(), .fieldPosition = card.toStrongRef()->fieldPosition() };
			double currentQValue =  getQValue(matchSnapshot, currentAction);
			if (currentQValue > maximalQValue)
			{
				actionToExecute = currentAction;
				maximalQValue = currentQValue;
			}
		}
		else
		{
			for (int positionIndex = 0; positionIndex < 3; ++positionIndex)
			{
				Action actionForPosition { .actionIndex = card.toStrongRef()->id(), .fieldPosition = static_cast<FieldPosition>(positionIndex)};
				double qValueForPosition = getQValue(matchSnapshot, actionForPosition);
				if (qValueForPosition > maximalQValue)
				{
					actionToExecute = actionForPosition;
					maximalQValue = qValueForPosition;
				}
			}
		}
	}

	const Action forfeitAction = Action {.actionIndex = _forfeitActionIndex, .fieldPosition = FieldPosition::Any };
	double forfeitQValue = getQValue(matchSnapshot, forfeitAction);
	if (forfeitQValue > maximalQValue)
	{
		actionToExecute = forfeitAction;
		maximalQValue = forfeitQValue;
	}
	return std::make_tuple(actionToExecute, maximalQValue);
}

double LearningQwentStrategy::getQValue
(
	const MatchSnapshot& matchSnapshot,
	Action action
)
{
	const auto key = StateAction { .snapshot = matchSnapshot,  .action = action };
	if (_qTable.contains(key) == false)
	{
		_qTable[key] = 0.;
	}
	return _qTable[key];
}

void LearningQwentStrategy::setQValue
(
	const MatchSnapshot& matchSnapshot,
	Action action,
	double value
)
{
	const auto key = StateAction { .snapshot = matchSnapshot,  .action = action };
	_qTable[key] = value;
}

double LearningQwentStrategy::winRoundRewardFunction
(
	const MatchSnapshot& prior, const MatchSnapshot& after
)	const
{
	double reward = 0.;
	if (_owningPlayerIndex == 0)
	{
		reward = after.scoreDifference - prior.scoreDifference;
	}
	else
	{
		reward =  -1 * (after.scoreDifference - prior.scoreDifference);
	}

	return reward;
}

void LearningQwentStrategy::setIsLearning
(
	bool isLearning
)
{
	_isLearning = isLearning;
}

bool LearningQwentStrategy::isLearning() const
{
	return _isLearning;
}

MatchSnapshot LearningQwentStrategy::bucketizeSnapshot
(
	MatchSnapshot snapshot
)	const
{
	snapshot.scoreDifference /= kScoreBucketSize;
	return snapshot;
}

void LearningQwentStrategy::saveToFile
(
	QIODevice* ioDevice
)	const
{
	if ((ioDevice->isOpen() && ioDevice->isWritable()) || ioDevice->open(QIODevice::WriteOnly))
	{
		QDataStream stream{ioDevice};
		stream << (int) kFileMagic;
		stream.setVersion(QDataStream::Qt_5_15);

		stream << _learningRate;
		stream << _explorationRate;
		stream << _discountFactor;
		stream << _winReward;
		stream << _lossPenalty;

		stream << (int) _forfeitActionIndex;

		for (const auto& [stateAction, qValue] : _qTable)
		{
			stream << stateAction.snapshot.scoreDifference;
			stream << stateAction.snapshot.firstPlayerAboutToWin;
			stream << stateAction.snapshot.secondPlayerAboutToWin;
			stream << stateAction.snapshot.isCloseCombatDemoralized;
			stream << stateAction.snapshot.isRangedRowDemoralized;
			stream << stateAction.snapshot.isSeigeRowDemoralized;

			stream << (unsigned int) stateAction.action.actionIndex;
			stream << (unsigned int) stateAction.action.fieldPosition;

			stream << qValue;
		}

		ioDevice->close();
	}
}

void LearningQwentStrategy::loadFromFile
(
	QIODevice* ioDevice
)
{
	_qTable.clear();
	if ((ioDevice->isOpen() && ioDevice->isReadable()) || ioDevice->open(QIODevice::ReadOnly))
	{
		QDataStream stream{ ioDevice };

		int fileMagic;
		stream >> fileMagic;
		if (static_cast<int>(fileMagic) != kFileMagic)
		{
			return;
		}

		stream.setVersion(QDataStream::Qt_5_15);

		stream >> _learningRate;
		stream >> _explorationRate;
		stream >> _discountFactor;
		stream >> _winReward;
		stream >> _lossPenalty;

		int forfeitActionIndex;
		stream >> forfeitActionIndex;
		_forfeitActionIndex = forfeitActionIndex;
	
		while (stream.atEnd() == false)
		{
			int scoreDifference = 0;

			StateAction stateAction{};
			stream >> scoreDifference;
			stream >> stateAction.snapshot.firstPlayerAboutToWin;
			stream >> stateAction.snapshot.secondPlayerAboutToWin;
			stream >> stateAction.snapshot.isCloseCombatDemoralized;
			stream >> stateAction.snapshot.isRangedRowDemoralized;
			stream >> stateAction.snapshot.isSeigeRowDemoralized;

			stateAction.snapshot.scoreDifference = scoreDifference;

			unsigned int actionIndex = 0;
			stream >> actionIndex;
			stateAction.action.actionIndex = actionIndex;
			stream >> stateAction.action.fieldPosition;

			double qValue = 0.;
			stream >> qValue;

			_qTable[stateAction] = qValue;
		}

		ioDevice->close();
	}
}

void LearningQwentStrategy::setLearningRate
(
	double learningRate
)
{
	_learningRate = learningRate;
}

double LearningQwentStrategy::learningRate() const
{
	return _learningRate;
}

void LearningQwentStrategy::setExplorationRate
(
	double explorationRate
)
{
	_explorationRate = explorationRate;
}

double LearningQwentStrategy::explorationRate() const
{
	return _explorationRate;
}

void LearningQwentStrategy::setDiscountFactor
(
	double discountFactor
)
{
	_discountFactor = discountFactor;
}

double LearningQwentStrategy::discountFactor() const
{
	return _discountFactor;
}

void LearningQwentStrategy::setWinReward
(
	double winReward
)
{
	_winReward = winReward;
}

double LearningQwentStrategy::winReward() const
{
	return _winReward;
}

void LearningQwentStrategy::setLossPenalty
(
	double lossPenalty
)
{
	_lossPenalty = lossPenalty;
}

double LearningQwentStrategy::lossPenalty() const
{
	return _lossPenalty;
}

void LearningQwentStrategy::setCardCount
(
	unsigned int cardCount
)
{
	_forfeitActionIndex = cardCount;
}

void LearningQwentStrategy::setOwningPlayerIndex
(
	unsigned int playerIndex
)
{
	_owningPlayerIndex = playerIndex;
}
