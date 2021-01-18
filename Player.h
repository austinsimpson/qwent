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

#pragma once

#include <QVector>
#include <QWeakPointer>

#include "Card.h"

class Player
{
public:
	Player() = default;
	Player(const Player& other) = default;

	void setDeck(QVector<QWeakPointer<Card>> deck);
	const QVector<QWeakPointer<Card>>& deck() const;

	const QVector<QWeakPointer<Card>>& hand() const;

	void removeCardFromHand(unsigned int cardIndex);
	void moveCardToGraveyard(int cardToMoveIndex);

	void addCardsToHand(unsigned int numberOfCards);

	void generateRandomHand(unsigned int handSize);
private:
	QString _name;

	QVector<QWeakPointer<Card>> _deck;
	QVector<QWeakPointer<Card>> _hand;
	QVector<QWeakPointer<Card>> _graveYard;

	unsigned int _currentPlayerIndex;
	unsigned int _nextCardInDeckIndex;
};

