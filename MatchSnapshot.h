#ifndef MATCHSNAPSHOT_H
#define MATCHSNAPSHOT_H

#include <stddef.h>
#include <compare>

struct MatchSnapshot
{
	int scoreDifference;
	bool firstPlayerAboutToWin;
	bool secondPlayerAboutToWin;
	//size_t secondPlayerCardCount;
	bool isCloseCombatDemoralized;
	bool isRangedRowDemoralized;
	bool isSeigeRowDemoralized;

	MatchSnapshot() = default;
	MatchSnapshot(const MatchSnapshot& other) = default;

	std::strong_ordering operator <=>(const MatchSnapshot& other) const
	{
		std::strong_ordering result = scoreDifference <=> other.scoreDifference;
		if (result != 0)
		{
			return result;
		}

		result = firstPlayerAboutToWin <=> other.firstPlayerAboutToWin;
		if (result != 0)
		{
			return result;
		}

		result = secondPlayerAboutToWin <=> other.secondPlayerAboutToWin;
		if (result != 0)
		{
			return result;
		}

		result = isCloseCombatDemoralized <=> other.isCloseCombatDemoralized;
		if (result != 0)
		{
			return result;
		}

		result = isRangedRowDemoralized <=> other.isRangedRowDemoralized;
		if (result != 0)
		{
			return result;
		}

		return isSeigeRowDemoralized <=> other.isSeigeRowDemoralized;
	}
};

#endif // MATCHSNAPSHOT_H
