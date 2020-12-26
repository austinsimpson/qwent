#pragma once

#include "IQwentStrategy.h"

#include <map>

class LearningQwentStrategy : public IQwentStrategy
{
public:
	LearningQwentStrategy(size_t owningPlayerIndex, size_t numberOfCards, size_t maximumScore);
	void performTurn(QwentGame* game) override;

private:
	double _discountRate;
	double _learningRate;
};


