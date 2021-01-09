#ifndef MATCHSNAPSHOT_H
#define MATCHSNAPSHOT_H

#include <stddef.h>
#include <compare>

struct MatchSnapshot
{
	size_t firstPlayerScore;
	size_t firstPlayerWinCount;
	//size_t firstPlayerCardCount;
	size_t secondPlayerScore;
	size_t secondPlayerWinCount;
	//size_t secondPlayerCardCount;
	bool isCloseCombatDemoralized;
	bool isRangedRowDemoralized;
	bool isSeigeRowDemoralized;

	MatchSnapshot() = default;
	MatchSnapshot(const MatchSnapshot& other) = default;

	std::strong_ordering operator <=>(const MatchSnapshot& other) const
	{
		std::strong_ordering result = firstPlayerScore <=> other.firstPlayerScore;
		if (result != 0)
		{
			return result;
		}

		result = firstPlayerWinCount <=> other.firstPlayerWinCount;
		if (result != 0)
		{
			return result;
		}

		/*result = firstPlayerCardCount <=> other.firstPlayerCardCount;
		if (result != 0)
		{
			return result;
		}*/

		result = secondPlayerScore <=> other.secondPlayerScore;
		if (result != 0)
		{
			return result;
		}

		result = secondPlayerWinCount <=> other.secondPlayerWinCount;
		if (result != 0)
		{
			return result;
		}

		/*result = secondPlayerCardCount <=> other.secondPlayerCardCount;
		if (result != 0)
		{
			return result;
		}*/

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
