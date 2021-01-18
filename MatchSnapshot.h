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

#ifndef MATCHSNAPSHOT_H
#define MATCHSNAPSHOT_H

#include <stddef.h>
#include <compare>

struct MatchSnapshot
{
	int scoreDifference;
	bool firstPlayerAboutToWin;
	bool secondPlayerAboutToWin;
	int firstPlayerHandCount;
	int secondPlayerHandCount;
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

		result = firstPlayerHandCount <=> other.firstPlayerHandCount;
		if (result != 0)
		{
			return result;
		}

		result = secondPlayerHandCount <=> other.secondPlayerHandCount;
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
