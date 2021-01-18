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

#include <array>

#include "Match.h"
#include "Player.h"
#include "QwentRow.h"
#include "MatchSnapshot.h"

class IQwentStrategy;

class QwentGame
{
public:
	QwentGame();

	void startMatch();

	void startRound();
	void endRound();
	void endTurn(bool didForfeit = false);

	void playCard(unsigned int playerIndex, unsigned int cardIndex);
	void playCard(unsigned int playerIndex, unsigned int cardIndex, FieldPosition fieldPosition);

	unsigned int winningPlayer() const;
	unsigned int getPlayerScore(unsigned int playerIndex) const;

	const QVector<QSharedPointer<Card>>& allCards() const;

	const QVector<QWeakPointer<Card>>& getHand(unsigned int playerIndex) const;
	const QVector<QWeakPointer<Card>>& getDeck(unsigned int playerIndex) const;

	MatchSnapshot takeSnapshot() const;

	void setDeck(unsigned int playerIndex, QVector<QWeakPointer<Card>> deck);

	QwentRow& getRow(unsigned int playerIndex, FieldPosition rowType);
	const QwentRow& getRow(unsigned int playerIndex, FieldPosition rowType) const;

	const Match& currentMatch() const;

	const QVector<QwentRow>& rows() const;

	QSharedPointer<IQwentStrategy> getStrategy(int playerIndex);
	void setStrategy(int playerIndex, const QSharedPointer<IQwentStrategy>& strategy);

	enum class GameState { PreGame, GameInProgress, EndGame };
	GameState getState() const;


private:
	unsigned int rowIndex(unsigned int playerIndex, FieldPosition rowType) const;

	Match _currentMatch;
	QVector<QwentRow> _rows;
	QVector<Player> _players;
	QVector<QSharedPointer<IQwentStrategy>> _strategies;
	QVector<QSharedPointer<Card>> _allCards;

	GameState _gameState{ GameState::PreGame };
};

