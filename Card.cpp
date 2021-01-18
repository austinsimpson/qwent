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

#include "Card.h"

Card::Card
(
	unsigned int attackValue, 
	SpecialEffect effect,
	const QString& name,
	bool isLegendary,
	FieldPosition fieldPosition,
	unsigned int id
): 
	_attackPower{ attackValue }, 
	_specialEffect{ effect },
	_name{ name },
	_isLegendary{ isLegendary },
	_fieldPosition{ fieldPosition },
	_id{id}
{
}

Card::Card
(
	const Card& other
):
	_attackPower{ other._attackPower },
	_specialEffect{ other._specialEffect },
	_name{ other._name },
	_isLegendary{ other._isLegendary },
	_fieldPosition{ other._fieldPosition },
	_id { other._id }
{
}

Card::Card
(
	Card&& other
):
	_attackPower{ other._attackPower },
	_specialEffect{ other._specialEffect },
	_name{ other._name },
	_isLegendary{ other._isLegendary },
	_fieldPosition{ other._fieldPosition },
	_id { other._id }
{
}

Card& Card::operator=
(
	const Card& other
)
{
	_attackPower = other._attackPower;
	_specialEffect = other._specialEffect;
	_name = other._name;
	_isLegendary = other._isLegendary;
	_fieldPosition = other._fieldPosition;
	_id = other._id;
	return *this;
}

FieldPosition Card::fieldPosition() const
{
	return _fieldPosition;
}

Card::SpecialEffect Card::specialEffect() const
{
	return _specialEffect;
}

unsigned int Card::attackPower() const
{
	return _attackPower;
}

bool Card::isLegendary() const
{
	return _isLegendary;
}

const QString& Card::name() const
{
	return _name;
}

unsigned int Card::id() const
{
	return _id;
}
