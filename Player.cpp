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
