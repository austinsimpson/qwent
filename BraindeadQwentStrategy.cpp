#include "BraindeadQwentStrategy.h"

#include "QwentGame.h"

BraindeadQwentStrategy::BraindeadQwentStrategy
(
	unsigned int owningPlayerIndex
):
	_owningPlayerIndex{ owningPlayerIndex }
{

}

void BraindeadQwentStrategy::performTurn
(
	QwentGame* game
)
{
	const auto cardCount = game->getHand(_owningPlayerIndex).count();
	const auto roundNumber = game->currentMatch().currentRound();
	auto playCardLambda = [this, cardCount, game](){
		unsigned int cardToPlayIndex = rand() % cardCount;
		const auto& cardToPlay = game->getHand(_owningPlayerIndex)[cardToPlayIndex];
		if (cardToPlay.toStrongRef()->specialEffect() != Card::SpecialEffect::DoubleAttackEntireRow)
		{
			game->playCard(_owningPlayerIndex, cardToPlayIndex);
		}
		else
		{
			game->playCard(_owningPlayerIndex, cardToPlayIndex, (FieldPosition)(rand() % 3));
		}
	};

	if (roundNumber == 0)
	{
		if (cardCount > 5)
		{
			playCardLambda();
		}
		else
		{
			game->endTurn(true);
		}
	}
	else
	{
		if (cardCount > 0)
		{
			playCardLambda();
		}
		else
		{
			game->endTurn(true);
		}
	}
}

void BraindeadQwentStrategy::notifyGameOver
(
	QwentGame* game
)
{
	(void) game;
}
