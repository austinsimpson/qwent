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

#include <QVector>
#include <QString>

enum class FieldPosition
{
	CloseCombat = 0,
	Ranged = 1,
	Siege = 2,
	Any = 3,
	Every = 4
};

static const QVector<QString> FieldPositionText{ "Close Combat", "Ranged", "Siege"};

class Card
{
public:
	enum class SpecialEffect
	{
		None = 0,
		DoubleAttackEntireRow = 1,
		DoubleAttackOfSameCard = 2,
		Heal = 3,
		Scorch = 4,
		Spy = 5,
		Demoralize = 6,
		ClearDemoralize = 7
	};
	
	Card(unsigned int attackValue, SpecialEffect effect, const QString& name, bool isLegendary, FieldPosition fieldPosition, unsigned int id);
	Card(const Card& other);
	Card(Card&& other);

	Card& operator=(const Card& other);

	FieldPosition fieldPosition() const;
	SpecialEffect specialEffect() const;
	unsigned int attackPower() const;

	bool isLegendary() const;
	const QString& name() const;
	unsigned int id() const;

private:
	QString _name;
	unsigned int _attackPower{ 1 };
	SpecialEffect _specialEffect{ SpecialEffect::None };
	bool _isLegendary{ false };
	FieldPosition _fieldPosition;

	unsigned int _id;
};

