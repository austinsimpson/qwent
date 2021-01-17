#ifndef BRAINDEADQWENTSTRATEGY_H
#define BRAINDEADQWENTSTRATEGY_H

#include "IQwentStrategy.h"

class BasicQwentStrategy : IQwentStrategy
{
public:
	BasicQwentStrategy(unsigned int owningPlayerIndex);

	void performTurn(QwentGame* game) override;
	void notifyGameOver(QwentGame* game) override;
	void setOwningPlayerIndex(unsigned int owningPlayerIndex) override;

private:
	unsigned int _owningPlayerIndex;
};

#endif // BRAINDEADQWENTSTRATEGY_H
