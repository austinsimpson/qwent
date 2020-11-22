#include "Match.h"

void Match::clear()
{
	_rounds.clear();
}

void Match::addRoundResults
(
	unsigned int winningPlayerIndex, 
	unsigned int score
)
{
	_rounds.append(Round{ winningPlayerIndex, score });
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
