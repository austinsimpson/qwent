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

#include "Match.h"

#include <ctime>

Match::Match()
{
	clear();
}

void Match::clear()
{
	srand(time(0));
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
	auto winCountLambdaGenerator = [](unsigned int playerIndex) {
		return [playerIndex](const Round& round) {
			return round.winningPlayerIndex() == playerIndex && round.isComplete();
		};
	};

	std::array<long, 2> roundsWon = {
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

unsigned int Match::getRoundsWonByPlayer
(
	unsigned int playerIndex
)	const
{
	return std::count_if(_rounds.cbegin(), _rounds.cend(), [playerIndex](const Round& round) { return round.isComplete() && round.winningPlayerIndex() == playerIndex; });
}

unsigned int Match::winningPlayer() const
{
	const auto roundsWonByFirst = getRoundsWonByPlayer(0);
	const auto roundsWonBySecond = getRoundsWonByPlayer(1);
	if (roundsWonByFirst == roundsWonBySecond)
		return 2;
	else
	{
		return roundsWonByFirst > roundsWonBySecond ? 0 : 1;
	}
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
