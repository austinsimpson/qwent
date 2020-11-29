#include "QwentRow.h"

auto isDemoralizingCard = [](const QWeakPointer<Card>& card) -> bool {
	return card.toStrongRef()->specialEffect() == Card::SpecialEffect::Demoralize;
};

auto isDoublingCard = [](const QWeakPointer<Card>& card) -> bool {
	return card.toStrongRef()->specialEffect() == Card::SpecialEffect::DoubleAttackEntireRow;
};

QwentRow::QwentRow() : _totalAttackPower{0u}, _isDemoralized{ false }, _isDoubled{ false }, _fieldPosition{FieldPosition::Any}
{

}

void QwentRow::clear()
{
	_cards.clear();
	recomputeCachedProperties();
}

void QwentRow::addCard
(
	const QWeakPointer<Card>& card
)
{
	int demoralizingCardIndex = std::find_if(_cards.cbegin(), _cards.cend(), isDemoralizingCard) - _cards.cbegin();

	switch (card.toStrongRef()->specialEffect())
	{
	case Card::SpecialEffect::ClearDemoralize:	
		if (0 <= demoralizingCardIndex && demoralizingCardIndex < _cards.size())
		{
			_cards.removeAt(demoralizingCardIndex);
		}
		break;
	case Card::SpecialEffect::Demoralize:
		if (isDemoralized() == false)
		{
			_cards.push_back(card);
		}
		break;
	default:
		_cards.push_back(card);
		break;
	}

	recomputeCachedProperties();
}

void QwentRow::setFieldPosition
(
	FieldPosition fieldPosition
)
{
	_fieldPosition = fieldPosition;
}

const QVector<QWeakPointer<Card>>& QwentRow::cards() const
{
	return _cards;
}

unsigned int QwentRow::effectiveAttackPower
(
	const QWeakPointer<Card>& cardPointer
)	const
{
	const bool demoralized = isDemoralized();
	const bool doubled = isDoubled();

	auto card = cardPointer.toStrongRef();
	switch (card->specialEffect())
	{
		case Card::SpecialEffect::DoubleAttackOfSameCard:
		{
			size_t cardsOfSameType = std::count_if(_cards.cbegin(), _cards.cend(), [card](const QWeakPointer<Card>& other) { return card == other.toStrongRef(); });
			return (doubled && card->isLegendary() == false ? 2 : 1) * pow(2, cardsOfSameType - 1) * card->attackPower();
		}
		case Card::SpecialEffect::Demoralize:
		case Card::SpecialEffect::DoubleAttackEntireRow:
		{
			return 0;
		}
		case Card::SpecialEffect::None:
		case Card::SpecialEffect::Spy:
			return (doubled && card->isLegendary() == false ? 2 : 1) * (demoralized == false || card->isLegendary() ? card->attackPower() : 1u);
		default:
			return 0;
		}
}

FieldPosition QwentRow::fieldPosition() const
{
	return _fieldPosition;
}

unsigned int QwentRow::calculateTotalAttackPower() const
{
	const bool demoralized = isDemoralized();
	const bool doubled = isDoubled();

	auto effectivePowerForCard = [this, demoralized, doubled](unsigned int runningTotal, const QWeakPointer<Card>& cardPointer)
	{
		auto card = cardPointer.toStrongRef();
		switch (card->specialEffect())
		{
		case Card::SpecialEffect::DoubleAttackOfSameCard:
		{
			size_t cardsOfSameType = std::count_if(_cards.cbegin(), _cards.cend(), [card](const QWeakPointer<Card>& other) { return card == other.toStrongRef(); });
			return runningTotal + ((doubled && card->isLegendary() == false ? 2 : 1) * static_cast<unsigned int>(pow(2, cardsOfSameType - 1)) * card->attackPower());
		}
		case Card::SpecialEffect::Demoralize:
		case Card::SpecialEffect::DoubleAttackEntireRow:
			return runningTotal;
		case Card::SpecialEffect::None:
		case Card::SpecialEffect::Spy:
			return runningTotal + ((doubled && card->isLegendary() == false ? 2 : 1) * (demoralized == false || card->isLegendary() ? card->attackPower() : 1u));
		default:
			return runningTotal;
		}
	};

	return std::accumulate(_cards.begin(), _cards.end(), 0, effectivePowerForCard);
}

unsigned int QwentRow::getTotalAttackPower() const
{
	return _totalAttackPower;
}

bool QwentRow::isDemoralized() const
{
	return _isDemoralized;
}

bool QwentRow::isDoubled() const
{
	return _isDoubled;
}

void QwentRow::recomputeCachedProperties() const
{
	_isDemoralized = std::find_if(_cards.cbegin(), _cards.cend(), isDemoralizingCard) != _cards.cend();
	_isDoubled = std::find_if(_cards.cbegin(), _cards.cend(), isDoublingCard) != _cards.cend();
	_totalAttackPower = calculateTotalAttackPower();
}
