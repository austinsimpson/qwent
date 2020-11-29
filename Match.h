#pragma once
#include <array>
#include <QVector>

struct Round
{
public:
	Round() = default;
	Round(unsigned int playerIndex, unsigned int score) : winningPlayerIndex{ playerIndex }, winningScore{ score } {}
	Round(const Round& other) : winningPlayerIndex{ other.winningPlayerIndex }, winningScore{ other.winningScore } {}

	inline void endTurn(unsigned int playerIndex) { _hasEndedTurn[playerIndex] = true; }
	inline bool hasEndedTurn(unsigned int playerIndex) const { return _hasEndedTurn[playerIndex]; }

	unsigned int winningPlayerIndex;
	unsigned int winningScore;

	std::array<bool, 2> _hasEndedTurn { false, false };
};

class Match
{
public:
	Match() = default;
	Match(const Match& other) : _rounds{ other._rounds } {}

	void clear();
	void addRoundResults(unsigned int winningPlayerIndex, unsigned int score);

	void endTurn();

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
};
