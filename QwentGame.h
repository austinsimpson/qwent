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

	const QVector<QWeakPointer<Card>>& getHand(unsigned int playerIndex) const;
	const QVector<QWeakPointer<Card>>& getDeck(unsigned int playerIndex) const;

	MatchSnapshot takeSnapshot() const;

	void setDeck(unsigned int playerIndex, QVector<QWeakPointer<Card>> deck);

	QwentRow& getRow(unsigned int playerIndex, FieldPosition rowType);
	const QwentRow& getRow(unsigned int playerIndex, FieldPosition rowType) const;

	const Match& currentMatch() const;

	const QVector<QwentRow>& rows() const;

	QSharedPointer<IQwentStrategy> getStrategy(int playerIndex);

private:
	unsigned int rowIndex(unsigned int playerIndex, FieldPosition rowType) const;

	Match _currentMatch;
	QVector<QwentRow> _rows;
	QVector<Player> _players;
	QVector<QSharedPointer<IQwentStrategy>> _strategies;
	QVector<QSharedPointer<Card>> _allCards;
};

