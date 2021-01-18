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

#include "LearningQwentStrategy.h"
#include "QwentGame.h"
#include "Range.h"
#include "StrategyTrainer.h"

#include <QDebug>

StrategyTrainer::StrategyTrainer() : 
	_numberOfEpochs{ 100 },
	_trainGamesPerEpoch{ 1000 },
	_testGamesPerEpoch{ 100 },
	_learningRate{ 0.005 }, 
	_explorationRate{ 0.15 }, 
	_discountFactor{ 0.1 },
	_winReward { 10. },
	_lossPenalty { -10000. },
	_winCounts{ 0, 0 }
{

}


void StrategyTrainer::train()
{
	//Sets up the training parameters for both strategies.
	for (int strategyIndex : range(2))
	{
		QSharedPointer<LearningQwentStrategy> strategy{ new LearningQwentStrategy{static_cast<size_t>(strategyIndex), static_cast<size_t>(_game.allCards().count())} };//(LearningQwentStrategy*)_game.getStrategy(strategyIndex).data();
		strategy->setLearningRate(_learningRate);
		strategy->setDiscountFactor(_discountFactor);
		strategy->setExplorationRate(_explorationRate);
		strategy->setWinReward(_winReward);
		strategy->setLossPenalty(_lossPenalty);
		_game.setStrategy(strategyIndex, strategy);
	}

	for (int epochNumber : range(_numberOfEpochs))
	{
		((LearningQwentStrategy*)_game.getStrategy(0).data())->setIsLearning(true);
		((LearningQwentStrategy*)_game.getStrategy(0).data())->setIsLearning(true);

		for (int gameNumber : range(_trainGamesPerEpoch))
		{
			_game.startMatch();
		}

		((LearningQwentStrategy*)_game.getStrategy(0).data())->setIsLearning(false);
		((LearningQwentStrategy*)_game.getStrategy(0).data())->setIsLearning(false);

		for (int gameNumber : range(_testGamesPerEpoch))
		{
			const int playerIndex = _game.currentMatch().winningPlayer();
			if (playerIndex < 2) //playerIndex = 2 if they tie
				++_winCounts[playerIndex];
			_game.startMatch();
			
		}
		emit epochFinished(epochNumber + 1);
	}
	emit trainingFinished();
}

QSharedPointer<IQwentStrategy> StrategyTrainer::winningStrategy()
{
	return _winCounts[0] > _winCounts[1] ? _game.getStrategy(0) : _game.getStrategy(1);
}

void StrategyTrainer::setNumberOfEpochs
(
	unsigned int numberOfEpochs
)
{
	_numberOfEpochs = numberOfEpochs;
}

unsigned int StrategyTrainer::numberOfEpochs() const
{
	return _numberOfEpochs;
}

void StrategyTrainer::setTrainGamesPerEpoch
(
	unsigned int trainGamesPerEpoch
)
{
	_trainGamesPerEpoch = trainGamesPerEpoch;
}

unsigned int StrategyTrainer::trainGamesPerEpoch() const
{
	return _trainGamesPerEpoch;
}

void StrategyTrainer::setTestGamesPerEpoch
(
	unsigned int testGamesPerEpoch
)
{
	_testGamesPerEpoch = testGamesPerEpoch;
}

unsigned int StrategyTrainer::testGamesPerEpoch() const
{
	return _trainGamesPerEpoch;
}

void StrategyTrainer::setLearningRate
(
	double learningRate
)
{
	_learningRate = learningRate;
}

double StrategyTrainer::learningRate() const
{
	return _learningRate;
}

void StrategyTrainer::setExplorationRate
(
	double explorationRate
)
{
	_explorationRate = explorationRate;
}

double StrategyTrainer::explorationRate() const
{
	return _explorationRate;
}

void StrategyTrainer::setDiscountFactor
(
	double discountFactor
)
{
	_discountFactor = discountFactor;
}

double StrategyTrainer::discountFactor() const
{
	return _discountFactor;
}

void StrategyTrainer::setWinReward
(
	double winReward
)
{
	_winReward = winReward;
}

double StrategyTrainer::winReward() const
{
	return _winReward;
}

void StrategyTrainer::setLossPenalty
(
	double lossPenalty
)
{
	_lossPenalty = lossPenalty;
}

double StrategyTrainer::lossPenalty() const
{
	return _lossPenalty;
}


