#include "NullQwentStrategy.h"

NullQwentStrategy::NullQwentStrategy()
{

}

void NullQwentStrategy::performTurn
(
	QwentGame* game
)
{
	(void) game;
}

void NullQwentStrategy::notifyGameOver
(
	QwentGame* game
)
{
	(void) game;
}
