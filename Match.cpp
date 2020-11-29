#include "Match.h"

void Match::clear()
{
	_currentPlayerIndex = rand() % 2;
	_rounds.clear();
	_currentRound = 0;
}

void Match::addRoundResults
(
	unsigned int winningPlayerIndex, 
	unsigned int score
)
{
	_rounds.append(Round{ winningPlayerIndex, score });
	++_currentRound;
}

void Match::endTurn()
{
	_rounds[_currentRound].endTurn(_currentPlayerIndex);
}

unsigned int Match::currentRound() const
{
	return _currentRound;
}

unsigned int Match::currentPlayer() const
{
	return _currentPlayerIndex;
}

unsigned int Match::getRoundsLostByPlayer
(
	unsigned int playerIndex
)	const
{
	return std::count_if(_rounds.cbegin(), _rounds.cend(), [playerIndex](const Round& round) { return round.winningPlayerIndex != playerIndex; });
}

void Match::setRoundsToWin
(
	unsigned int roundsToWin
)
{
	_roundsToWin = roundsToWin;
}

unsigned int Match::roundsToWin() const
{
	return _roundsToWin;
}
