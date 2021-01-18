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
#include <array>

#include <QObject>

#include "QwentGame.h"

class IQwentStrategy;

class StrategyTrainer : public QObject
{
	Q_OBJECT
public:
	StrategyTrainer();

	QSharedPointer<IQwentStrategy> winningStrategy();

	unsigned int numberOfEpochs() const;
	unsigned int trainGamesPerEpoch() const;
	unsigned int testGamesPerEpoch() const;
	double learningRate() const;
	double explorationRate() const;
	double discountFactor() const;
	double winReward() const;
	double lossPenalty() const;

public slots:
	void setTrainGamesPerEpoch(unsigned int trainGamesPerEpoch);
	void setNumberOfEpochs(unsigned int numberOfEpochs);
	void setTestGamesPerEpoch(unsigned int testGamesPerEpoch);
	void setLearningRate(double learningRate);
	void setExplorationRate(double explorationRate);
	void setDiscountFactor(double discountFactor);
	void setWinReward(double winReward);
	void setLossPenalty(double lossPenalty);

	void train();

signals:
	void epochFinished(int epochNumber);
	void trainingFinished();

private:
	size_t _numberOfEpochs;
	size_t _trainGamesPerEpoch;
	size_t _testGamesPerEpoch;

	double _learningRate;
	double _explorationRate;
	double _discountFactor;

	double _winReward;
	double _lossPenalty;
	
	std::array<unsigned int, 2> _winCounts;
	QwentGame _game;
};

