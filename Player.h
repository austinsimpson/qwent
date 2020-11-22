#pragma once

#include <QVector>
#include <QWeakPointer>

#include "Card.h"

class Player
{
public:
	Player() = default;

	void setDeck(const QVector<QWeakPointer<Card>>& deck);
	const QVector<QWeakPointer<Card>>& deck() const;

	const QVector<QWeakPointer<Card>>& hand() const;

	void removeCardFromHand(unsigned int cardIndex);
	void moveCardToGraveyard(int cardToMoveIndex);

	void addCardsToHand(unsigned int numberOfCards);

private:
	void generateRandomHand(unsigned int handSize);

	QString _name;

	QVector<QWeakPointer<Card>> _deck;
	QVector<QWeakPointer<Card>> _hand;
	QVector<QWeakPointer<Card>> _graveYard;

	unsigned int _currentPlayerIndex;
	unsigned int _nextCardInDeckIndex;
};

