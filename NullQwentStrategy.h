#ifndef NULLQWENTSTRATEGY_H
#define NULLQWENTSTRATEGY_H

#include "IQwentStrategy.h"

class NullQwentStrategy : IQwentStrategy
{
public:
	NullQwentStrategy();

	void performTurn(QwentGame* game) override;
	void notifyGameOver(QwentGame* game) override;
};

#endif // NULLQWENTSTRATEGY_H
