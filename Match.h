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

struct MatchSnapshot
{
	size_t firstPlayerScore;
	size_t secondPlayerScore;
	bool isCloseCombatDemoralized;
	bool isRangedRowDemoralized;
	bool isSeigeRowDemoralized;
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

	void setRoundsToWin(unsigned int roundsToWin);
	unsigned int roundsToWin() const;
private:
	unsigned int _currentPlayerIndex { 0 };
	unsigned int _roundsToWin{ 2 };
	unsigned int _currentRound{ 0 };
	QVector<Round> _rounds;
	unsigned int _numberOfRounds{ 3 };
};
