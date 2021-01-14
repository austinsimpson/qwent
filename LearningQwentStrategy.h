#pragma once

#include "Card.h"
#include "IQwentStrategy.h"
#include "MatchSnapshot.h"

#include <QIODevice>
#include <QVector>
#include <QWeakPointer>

#include <functional>
#include <random>
#include <map>

struct Action
{
	size_t actionIndex;
	FieldPosition fieldPosition;

	std::strong_ordering operator <=>(const Action& other) const = default;
};

struct StateAction
{
	MatchSnapshot snapshot;
	Action action;

	std::strong_ordering operator <=>(const StateAction& other) const = default;
};

//Is a strategy that employs Q-Learning to figure out the best moves to make in the card game
class LearningQwentStrategy : public IQwentStrategy
{
public:
	LearningQwentStrategy(size_t owningPlayerIndex);
	LearningQwentStrategy(size_t owningPlayerIndex, size_t numberOfCards);
	void performTurn(QwentGame* game) override;
	void notifyGameOver(QwentGame* game) override;

	void setIsLearning(bool isLearning);
	bool isLearning() const;

	void setRewardFunction(std::function<double(const MatchSnapshot&, const MatchSnapshot&)> rewardFunction);

	void saveToFile(QIODevice* ioDevice) const;
	void loadFromFile(QIODevice* ioDevice);

	inline auto& qTable()
	{
		return _qTable;
	}

private:
	double getQValue(const MatchSnapshot& matchSnapshot, Action action);
	void setQValue(const MatchSnapshot& matchSnapshot, Action action, double value);

	double winRoundRewardFunction(const MatchSnapshot& prior, const MatchSnapshot& after) const;

	MatchSnapshot bucketizeSnapshot(MatchSnapshot matchSnapshot) const;

	Action chooseNextAction(const MatchSnapshot& matchSnapshot, const QVector<QWeakPointer<Card>>& currentHand);
	std::tuple<Action, double> getActionWithMaximalOutcome(const MatchSnapshot& matchSnapshot, const QVector<QWeakPointer<Card>>& currentHand);

	bool _isLearning { true };

	double _discountRate { 0.1 };
	double _learningRate { 1. };
	double _explorationRate { 0.1 };
	double _winReward { 10.0 };
	double _lossPenalty { -10000.0 };

	size_t _owningPlayerIndex;

	size_t _forfeitActionIndex;

	bool _hasLastAction { false };
	Action _lastAction { 0, FieldPosition::Any };
	MatchSnapshot _lastSnapshot;

	std::map<StateAction, double> _qTable;
	std::function<double(const MatchSnapshot&, const MatchSnapshot&)> _rewardFunction;

	std::default_random_engine _randomNumberEngine;
};


