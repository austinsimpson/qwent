//Copyright(c) 2021 Austin Simpson
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

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

	void setLearningRate(double learningRate);
	double learningRate() const;

	void setExplorationRate(double explorationRate);
	double explorationRate() const;

	void setDiscountFactor(double discountFactor);
	double discountFactor() const;

	void setWinReward(double winReward);
	double winReward() const;

	void setLossPenalty(double lossPenalty);
	double lossPenalty() const;

	void setCardCount(unsigned int cardCount);

	void setOwningPlayerIndex(unsigned int playerIndex);

private:
	double getQValue(const MatchSnapshot& matchSnapshot, Action action);
	void setQValue(const MatchSnapshot& matchSnapshot, Action action, double value);

	double winRoundRewardFunction(const MatchSnapshot& prior, const MatchSnapshot& after) const;

	MatchSnapshot bucketizeSnapshot(MatchSnapshot matchSnapshot) const;

	Action chooseNextAction(const MatchSnapshot& matchSnapshot, const QVector<QWeakPointer<Card>>& currentHand);
	std::tuple<Action, double> getActionWithMaximalOutcome(const MatchSnapshot& matchSnapshot, const QVector<QWeakPointer<Card>>& currentHand);

	bool _isLearning { true };

	double _discountFactor { 0.1 };
	double _learningRate { 0.005 };
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


