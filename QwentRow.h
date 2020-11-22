#pragma once

#include <memory>

#include <QVector>
#include <QWeakPointer>

#include "Card.h"

class QwentRow
{
public:
	QwentRow();

	void clear();
	void addCard(const QWeakPointer<Card>& card);
	void setFieldPosition(FieldPosition fieldPosition);

	const QVector<QWeakPointer<Card>>& cards() const;
	unsigned int effectiveAttackPower(const QWeakPointer<Card>& card) const;
	FieldPosition fieldPosition() const;
	unsigned int getTotalAttackPower() const;
	bool isDemoralized() const;
	bool isDoubled() const;

private:
	QVector<QWeakPointer<Card>> _cards;

	unsigned int calculateTotalAttackPower() const;

	void recomputeCachedProperties() const;

	mutable unsigned int _totalAttackPower;
	mutable bool _isDemoralized;
	mutable bool _isDoubled;
	FieldPosition _fieldPosition;
};

