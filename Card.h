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

