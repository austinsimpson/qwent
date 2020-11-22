#include "QwentGame.h"

#include <algorithm>

QwentGame::QwentGame()
{
	for (int index = 0; index < 6; ++index)
	{
		_rows[index].setFieldPosition(static_cast<FieldPosition>(index % 3));
	}
}

void QwentGame::startMatch()
{
	_currentMatch.clear();
}

void QwentGame::startRound()
{
	for (auto& row : _rows)
	{
		row.clear();
	}
}

void QwentGame::endRound()
{
	unsigned int winningPlayerIndex = winningPlayer();
	unsigned int winningPlayerScore = getPlayerScore(winningPlayerIndex);

	_currentMatch.addRoundResults(winningPlayerIndex, winningPlayerScore);
}

void QwentGame::playCard
(
	unsigned int playerIndex, 
	unsigned int cardIndex
)
{
	const auto& card = _players[playerIndex].hand()[cardIndex];
	const auto fieldPosition = card.data()->fieldPosition();
	playCard(playerIndex, cardIndex, fieldPosition);
}

void QwentGame::playCard
(
	unsigned int playerIndex,
	unsigned int cardIndex,
	FieldPosition fieldPosition
)
{
	const auto& card = _players[playerIndex].hand()[cardIndex];
	assert(card.data()->fieldPosition() == fieldPosition || card.data()->fieldPosition() == FieldPosition::Any);
	switch (card.data()->specialEffect())
	{
	case Card::SpecialEffect::Spy:
		getRow(1 - playerIndex, fieldPosition).addCard(card);
		_players[playerIndex].addCardsToHand(2);
		break;
	case Card::SpecialEffect::Demoralize:
	case Card::SpecialEffect::ClearDemoralize:
		getRow(playerIndex, fieldPosition).addCard(card);
		getRow(1 - playerIndex, fieldPosition).addCard(card);
		break;
	default:
		getRow(playerIndex, fieldPosition).addCard(card);
		break;
	}

	_players[playerIndex].removeCardFromHand(cardIndex);
}

unsigned int QwentGame::winningPlayer() const
{
	auto rowSumLambda = [](unsigned int totalScore, const QwentRow& row) {
		return totalScore + row.getTotalAttackPower();
	};

	unsigned int firstPlayerScore = getPlayerScore(0);
	unsigned int secondPlayerScore = getPlayerScore(1);
	return firstPlayerScore > secondPlayerScore ? 0u : 1u;
}

unsigned int QwentGame::getPlayerScore(unsigned int playerIndex) const
{
	return std::accumulate(_rows.begin() + (playerIndex * 3), _rows.begin() + ((playerIndex + 1) * 3), 0, [](unsigned int total, const QwentRow& row) {
		return total + row.getTotalAttackPower();
	});
}

const QVector<QWeakPointer<Card>>& QwentGame::getHand
(
	unsigned int playerIndex
)	const
{
	return _players[playerIndex].hand();
}

const QVector<QWeakPointer<Card>>& QwentGame::getDeck
(
	unsigned int playerIndex
)	const
{
	return _players[playerIndex].deck();
}

void QwentGame::setDeck
(
	unsigned int playerIndex, 
	const QVector<QWeakPointer<Card>>& deck
)
{
	_players[playerIndex].setDeck(deck);
}


QwentRow& QwentGame::getRow
(
	unsigned int playerIndex, 
	FieldPosition fieldPosition
)
{
	return this->_rows[rowIndex(playerIndex, fieldPosition)];
}

const QwentRow& QwentGame::getRow
(
	unsigned int playerIndex, 
	FieldPosition fieldPosition
)	const
{
	return this->_rows[rowIndex(playerIndex, fieldPosition)];
}

const Match& QwentGame::currentMatch() const
{
	return _currentMatch;
}

const std::array<QwentRow, 6>& QwentGame::rows() const
{
	return _rows;
}

unsigned int QwentGame::rowIndex
(
	unsigned int playerIndex, 
	FieldPosition fieldPosition
)	const
{
	return playerIndex * 3 + static_cast<int>(fieldPosition);
}