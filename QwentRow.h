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

	void scorch();

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

