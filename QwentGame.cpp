#include "DeckJsonSerializer.h"
#include "NullQwentStrategy.h"
#include "BasicQwentStrategy.h"
#include "LearningQwentStrategy.h"
#include "QwentGame.h"
#include "MatchSnapshot.h"

#include <QFile>

#include <algorithm>

QwentGame::QwentGame()
{
	DeckJsonSerializer serializer;
	_allCards = serializer.readAllCards(":/resources/card-definitions.json");

	_players.push_back(Player{});
	_players.push_back(Player{});

	auto playerDeck = serializer.readDeck(":/resources/sample-deck.json", _allCards);
	setDeck(0, playerDeck);
	setDeck(1, playerDeck);

	_strategies.push_back(QSharedPointer<IQwentStrategy>((IQwentStrategy*)(new NullQwentStrategy())));
	_strategies.push_back(QSharedPointer<IQwentStrategy>((IQwentStrategy*)(new BasicQwentStrategy(1))));

	for (int index = 0; index < 6; ++index)
	{
		_rows.push_back(QwentRow{});
		_rows[index].setFieldPosition(static_cast<FieldPosition>(index % 3));
	}
}

void QwentGame::startMatch()
{
	_currentMatch.clear();
	_gameState = QwentGame::GameState::GameInProgress;
	for (auto& player : _players)
	{
		player.generateRandomHand(10);
	}
	startRound();
}

void QwentGame::startRound()
{
	for (auto& row : _rows)
	{
		row.clear();
	}

	_strategies[_currentMatch.currentPlayer()]->performTurn(this);
}

void QwentGame::endRound()
{
	std::array<unsigned int, 2> scores{ getPlayerScore(0), getPlayerScore(1) };
	_currentMatch.endRound(scores);
}

void QwentGame::endTurn
(
	bool didForfeit
)
{
	_currentMatch.endCurrentTurn(didForfeit);
	if (_currentMatch.hasRemainingTurn() == false)
	{
		endRound();
		if (!_currentMatch.isOver())
		{
			startRound();
		}
		else
		{
			for (auto strategy : _strategies)
			{
				strategy->notifyGameOver(this);
			}
			_gameState = QwentGame::GameState::EndGame;
		}
	}
	else
	{
		_strategies[_currentMatch.currentPlayer()]->performTurn(this);
	}
}

void QwentGame::playCard
(
	unsigned int playerIndex, 
	unsigned int cardIndex
)
{
	const auto& card = _players[playerIndex].hand()[cardIndex];
	const auto fieldPosition = card.toStrongRef()->fieldPosition();
	playCard(playerIndex, cardIndex, fieldPosition);
}

void QwentGame::playCard
(
	unsigned int playerIndex,
	unsigned int cardIndex,
	FieldPosition fieldPosition
)
{
	const auto& card = _players[playerIndex].hand()[cardIndex];
	const int otherPlayerIndex = 1 - playerIndex;

	assert(card.toStrongRef()->fieldPosition() == fieldPosition || card.toStrongRef()->fieldPosition() == FieldPosition::Any && playerIndex == currentMatch().currentPlayer());
	switch (card.toStrongRef()->specialEffect())
	{
	case Card::SpecialEffect::Spy:
		getRow(otherPlayerIndex, fieldPosition).addCard(card);
		_players[playerIndex].addCardsToHand(2);
		break;
	case Card::SpecialEffect::Demoralize:
		getRow(playerIndex, fieldPosition).addCard(card);
		getRow(otherPlayerIndex, fieldPosition).addCard(card);
		break;
	case Card::SpecialEffect::Scorch:
		getRow(playerIndex, fieldPosition).addCard(card);
		getRow(otherPlayerIndex, fieldPosition).scorch();
		break;
	case Card::SpecialEffect::ClearDemoralize:
		for (auto& row : _rows)
		{
			row.addCard(card);
		}
		break;
	default:
		getRow(playerIndex, fieldPosition).addCard(card);
		break;
	}

	_players[playerIndex].removeCardFromHand(cardIndex);
	endTurn(_players[playerIndex].hand().size() == 0);
}

unsigned int QwentGame::winningPlayer() const
{
	unsigned int firstPlayerScore = getPlayerScore(0);
	unsigned int secondPlayerScore = getPlayerScore(1);
	return firstPlayerScore > secondPlayerScore ? 0u : 1u;
}

unsigned int QwentGame::getPlayerScore(unsigned int playerIndex) const
{
	return std::accumulate(_rows.begin() + (playerIndex * 3), _rows.begin() + ((playerIndex + 1) * 3), 0, [](unsigned int total, const QwentRow& row) {
		return total + row.getTotalAttackPower();
	});
}

const QVector<QSharedPointer<Card>>& QwentGame::allCards() const
{
	return _allCards;
}

const QVector<QWeakPointer<Card>>& QwentGame::getHand
(
	unsigned int playerIndex
)	const
{
	return _players[playerIndex].hand();
}

const QVector<QWeakPointer<Card>>& QwentGame::getDeck
(
	unsigned int playerIndex
)	const
{
	return _players[playerIndex].deck();
}

MatchSnapshot QwentGame::takeSnapshot() const
{
	MatchSnapshot result;
	result.scoreDifference = getPlayerScore(0) - getPlayerScore(1);
	result.firstPlayerAboutToWin = currentMatch().getRoundsWonByPlayer(0) > 0;
	result.secondPlayerAboutToWin = currentMatch().getRoundsWonByPlayer(1) > 0;
	result.firstPlayerHandCount = getHand(0).size();
	result.secondPlayerHandCount = getHand(1).size();
	result.isCloseCombatDemoralized = getRow(0, FieldPosition::CloseCombat).isDemoralized(); //We can choose either player index because of symmetry
	result.isRangedRowDemoralized = getRow(0, FieldPosition::Ranged).isDemoralized(); //We can choose either player index because of symmetry
	result.isSeigeRowDemoralized = getRow(0, FieldPosition::Siege).isDemoralized(); //We can choose either player index because of symmetry
	return result;
}

void QwentGame::setDeck
(
	unsigned int playerIndex, 
	QVector<QWeakPointer<Card>> deck
)
{
	_players[playerIndex].setDeck(deck);
}


QwentRow& QwentGame::getRow
(
	unsigned int playerIndex, 
	FieldPosition fieldPosition
)
{
	return this->_rows[rowIndex(playerIndex, fieldPosition)];
}

const QwentRow& QwentGame::getRow
(
	unsigned int playerIndex, 
	FieldPosition fieldPosition
)	const
{
	return this->_rows[rowIndex(playerIndex, fieldPosition)];
}

const Match& QwentGame::currentMatch() const
{
	return _currentMatch;
}

const QVector<QwentRow>& QwentGame::rows() const
{
	return _rows;
}

unsigned int QwentGame::rowIndex
(
	unsigned int playerIndex, 
	FieldPosition fieldPosition
)	const
{
	return playerIndex * 3 + static_cast<int>(fieldPosition);
}

QSharedPointer<IQwentStrategy> QwentGame::getStrategy
(
	int playerIndex
)
{
	return _strategies[playerIndex];
}

void QwentGame::setStrategy
(
	int playerIndex,
	const QSharedPointer<IQwentStrategy>& strategy
)
{
	_strategies[playerIndex] = strategy;
}

QwentGame::GameState QwentGame::getState() const
{
	return _gameState;
}
