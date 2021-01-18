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
#include <QVector>

struct Round
{
public:
	Round() = default;
	Round(std::array<unsigned int, 2>& scores) : _playerScores{ scores } {}
	Round(const Round& other) : _playerScores{ other._playerScores } {}

	inline void endTurn(unsigned int playerIndex, bool didForfeit = false) { _hasEndedTurn[playerIndex] = didForfeit; }
	inline bool hasEndedTurn(unsigned int playerIndex) const { return _hasEndedTurn[playerIndex]; }

	inline bool hasRemainingTurn() const 
	{ 
		return std::any_of(_hasEndedTurn.begin(), _hasEndedTurn.end(), [](bool hasEnded) 
		{ 
			return hasEnded == false; 
		}); 
	};


	inline bool isComplete() const { return _isComplete; }

	void setScores(const std::array<unsigned int, 2>& scores) { _playerScores = scores; _isComplete = true; };

	unsigned int winningPlayerIndex() const;
	unsigned int winningScore() const;

	bool _isComplete{ false };
	std::array<bool, 2> _hasEndedTurn { false, false };
	std::array<unsigned int, 2> _playerScores{ 0, 0 };
};

class Match
{
public:
	Match();

	Match(const Match& other) : _rounds{ other._rounds } {}

	void clear();
	void endRound(const std::array<unsigned int, 2>& playerScores);

	void endCurrentTurn(bool didForfeit = false);

	bool hasRemainingTurn() const;

	bool isOver() const;

	unsigned int currentRound() const;
	unsigned int currentPlayer() const;

	unsigned int getRoundsLostByPlayer(unsigned int playerIndex) const;
	unsigned int getRoundsWonByPlayer(unsigned int playerIndex) const;

	unsigned int winningPlayer() const;

	void setRoundsToWin(unsigned int roundsToWin);
	unsigned int roundsToWin() const;
private:
	unsigned int _currentPlayerIndex { 0 };
	unsigned int _roundsToWin{ 2 };
	unsigned int _currentRound{ 0 };
	QVector<Round> _rounds;
	unsigned int _numberOfRounds{ 3 };
};
