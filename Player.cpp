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
//SOFTWARE

#include "Player.h"

#include <algorithm>
#include <random>

constexpr auto kDefaultHandSize = 10;

const QVector<QWeakPointer<Card>>& Player::hand() const
{
	return _hand;
}

void Player::setDeck
(
	QVector<QWeakPointer<Card>> deck
)
{
	_deck = deck;
	generateRandomHand(kDefaultHandSize);
}

const QVector<QWeakPointer<Card>>& Player::deck() const
{
	return _deck;
}

void Player::removeCardFromHand(unsigned int cardIndex)
{
	_hand.removeAt(cardIndex);
}

void Player::moveCardToGraveyard
(
	int cardToMoveIndex
)
{
	QWeakPointer<Card> cardToSendToGrave = _hand.takeAt(cardToMoveIndex);
	_graveYard.push_back(cardToSendToGrave);

}

void Player::addCardsToHand(unsigned int numberOfCards)
{
	if (_nextCardInDeckIndex + numberOfCards < _deck.count())
	{
		for (int i = _nextCardInDeckIndex; i < _nextCardInDeckIndex + numberOfCards; ++i)
		{
			_hand.append(_deck[i]);
		}
		_nextCardInDeckIndex += numberOfCards;
	}
}

void Player::generateRandomHand(unsigned int handSize)
{
	const auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine rng(seed);
	std::shuffle(_deck.begin(), _deck.end(), rng);
	_hand.clear();
	for (int i = 0; i < handSize; ++i)
	{
		_hand.append(_deck[i]);
	}

	_nextCardInDeckIndex = handSize;
}
