#pragma once
#include <QVector>

struct Round
{
public:
	Round() = default;
	Round(unsigned int playerIndex, unsigned int score) : winningPlayerIndex{ playerIndex }, winningScore{ score } {}
	Round(const Round& other) : winningPlayerIndex{ other.winningPlayerIndex }, winningScore{ other.winningScore } {}

	unsigned int winningPlayerIndex;
	unsigned int winningScore;
};

class Match
{
public:
	Match() = default;
	Match(const Match& other) : _rounds{ other._rounds } {}

	void clear();
	void addRoundResults(unsigned int winningPlayerIndex, unsigned int score);

	unsigned int getRoundsLostByPlayer(unsigned int playerIndex) const;

	void setRoundsToWin(unsigned int roundsToWin);
	unsigned int roundsToWin() const;
private:
	unsigned int _roundsToWin{ 2 };

	QVector<Round> _rounds;
};