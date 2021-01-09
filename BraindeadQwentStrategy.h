#ifndef BRAINDEADQWENTSTRATEGY_H
#define BRAINDEADQWENTSTRATEGY_H

#include "IQwentStrategy.h"

class BraindeadQwentStrategy : IQwentStrategy
{
public:
	BraindeadQwentStrategy(unsigned int owningPlayerIndex);

	void performTurn(QwentGame* game) override;
	void notifyGameOver(QwentGame* game) override;

private:
	unsigned int _owningPlayerIndex;
};

#endif // BRAINDEADQWENTSTRATEGY_H
