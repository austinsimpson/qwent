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
//SOFTWARE.s

#include "QwentRow.h"

#include <algorithm>

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

//Scorch is a special action where a card is played and it vaporizes the strongest cards on the corresponding row for the other player.
void QwentRow::scorch()
{
	QVector<int> result;

	QVector<QWeakPointer<Card>> nonLegendaryCards;
	for (const auto& card : _cards)
	{
		const auto& cardRef = *card.toStrongRef();
		if (cardRef.isLegendary() == false && cardRef.specialEffect() != Card::SpecialEffect::Demoralize)
		{
			nonLegendaryCards.push_back(card);
		}
	}

	if (nonLegendaryCards.count() > 0)
	{
		const auto& card = *std::max_element(nonLegendaryCards.begin(), nonLegendaryCards.end(), [this](const QWeakPointer<Card>& first, const QWeakPointer<Card>& second) -> bool {
			return effectiveAttackPower(first) < effectiveAttackPower(second);
		});

		_cards.removeAll(card);
		recomputeCachedProperties();
	}
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
			return (doubled && card->isLegendary() == false ? 2 : 1) * pow(2, cardsOfSameType - 1) * (demoralized == false ? card->attackPower() : 1u);
		}
		case Card::SpecialEffect::Demoralize:
		case Card::SpecialEffect::DoubleAttackEntireRow:
		{
			return 0;
		}
		case Card::SpecialEffect::None:
		case Card::SpecialEffect::Scorch:
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
			return runningTotal + ((doubled && card->isLegendary() == false ? 2 : 1) * static_cast<unsigned int>(pow(2, cardsOfSameType - 1)) * (demoralized == false ? card->attackPower() : 1u));
		}
		case Card::SpecialEffect::Demoralize:
		case Card::SpecialEffect::DoubleAttackEntireRow:
			return runningTotal;
		case Card::SpecialEffect::None:
		case Card::SpecialEffect::Scorch:
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
