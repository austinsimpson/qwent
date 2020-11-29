#pragma once

#include <array>

#include "Match.h"
#include "Player.h"
#include "QwentRow.h"

class IQwentStrategy;

class QwentGame
{
public:
	QwentGame();

	void startMatch();

	void startRound();
	void endRound();
	void endTurn();

	void playCard(unsigned int playerIndex, unsigned int cardIndex);
	void playCard(unsigned int playerIndex, unsigned int cardIndex, FieldPosition fieldPosition);

	unsigned int winningPlayer() const;
	unsigned int getPlayerScore(unsigned int playerIndex) const;

	const QVector<QWeakPointer<Card>>& getHand(unsigned int playerIndex) const;
	const QVector<QWeakPointer<Card>>& getDeck(unsigned int playerIndex) const;

	void setDeck(unsigned int playerIndex, const QVector<QWeakPointer<Card>>& deck);

	QwentRow& getRow(unsigned int playerIndex, FieldPosition rowType);
	const QwentRow& getRow(unsigned int playerIndex, FieldPosition rowType) const;

	const Match& currentMatch() const;

	const std::array<QwentRow, 6>& rows() const;

private:
	unsigned int rowIndex(unsigned int playerIndex, FieldPosition rowType) const;

	Match _currentMatch;
	std::array<QwentRow, 6> _rows;
	std::array<Player, 2> _players;
	QVector<QSharedPointer<IQwentStrategy>> _strategies;
};

