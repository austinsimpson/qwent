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

#include "BasicQwentStrategy.h"

#include "QwentGame.h"

BasicQwentStrategy::BasicQwentStrategy
(
	unsigned int owningPlayerIndex
):
	_owningPlayerIndex{ owningPlayerIndex }
{

}

void BasicQwentStrategy::performTurn
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

void BasicQwentStrategy::notifyGameOver
(
	QwentGame* game
)
{
	(void) game;
}

void BasicQwentStrategy::setOwningPlayerIndex
(
	unsigned int owningPlayerIndex
)
{
	_owningPlayerIndex = owningPlayerIndex;
}
