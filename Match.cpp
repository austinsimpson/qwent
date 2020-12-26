#include "Match.h"

Match::Match()
{
	clear();
}

void Match::clear()
{
	_currentPlayerIndex = rand() % 2;
	_rounds.clear();
	for (int i = 0; i < _numberOfRounds; ++i)
	{
		_rounds.push_back(Round{});
	}
	_currentRound = 0;
}

void Match::endRound
(
	const std::array<unsigned int, 2>& scores
)
{
	_rounds[_currentRound].setScores(scores);
	++_currentRound;
}

void Match::endCurrentTurn(bool didForfeit)
{
	_rounds[_currentRound].endTurn(_currentPlayerIndex, didForfeit);
	_currentPlayerIndex = (_currentPlayerIndex + 1) % 2;
	if (_rounds[_currentRound].hasRemainingTurn())
	{
		while (_rounds[_currentRound].hasEndedTurn(_currentPlayerIndex) == true)
		{
			_currentPlayerIndex = (_currentPlayerIndex + 1) % 2;
		}
	}
}

bool Match::hasRemainingTurn() const
{
	return _rounds[_currentRound].hasRemainingTurn();
}

bool Match::isOver() const
{
	constexpr auto winCountLambdaGenerator = [](int playerIndex) {
		return [playerIndex](const Round& round) {
			return round.winningPlayerIndex() == playerIndex && round.isComplete();
		};
	};

	std::array<unsigned int, 2> roundsWon = {
		std::count_if(_rounds.begin(), _rounds.end(), winCountLambdaGenerator(0)), 
		std::count_if(_rounds.begin(), _rounds.end(), winCountLambdaGenerator(1)) 
	};
	return std::any_of(roundsWon.begin(), roundsWon.end(), [this](unsigned int winCount) { return winCount >= _roundsToWin; });
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
	return std::count_if(_rounds.cbegin(), _rounds.cend(), [playerIndex](const Round& round) { return round.isComplete() && round.winningPlayerIndex() != playerIndex; });
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

unsigned int Round::winningPlayerIndex() const
{
	return std::max_element(_playerScores.begin(), _playerScores.end()) - _playerScores.begin();
}

unsigned int Round::winningScore() const
{
	return *std::max_element(_playerScores.begin(), _playerScores.end());
}
