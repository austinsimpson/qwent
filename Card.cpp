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
