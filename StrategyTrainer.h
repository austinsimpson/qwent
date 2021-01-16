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

