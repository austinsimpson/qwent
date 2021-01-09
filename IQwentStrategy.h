#ifndef IQWENTSTRATEGY_H
#define IQWENTSTRATEGY_H

class QwentGame;

class IQwentStrategy
{
public:
	virtual void performTurn(QwentGame* game) = 0;
	virtual void notifyGameOver(QwentGame* game) = 0;
	virtual ~IQwentStrategy() = default;
};

#endif // IQWENTSTRATEGY_H
