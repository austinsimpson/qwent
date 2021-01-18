

#pragma once

#include "Card.h"

#include <QIODevice>
#include <QSharedPointer>
#include <QVector>

class DeckJsonSerializer
{
public:
	DeckJsonSerializer() = default;

	[[nodiscard]] QVector<QSharedPointer<Card>> readAllCards(const QString& filePath) const;
	[[nodiscard]] QVector<QSharedPointer<Card>> readAllCards(QIODevice& ioDevice) const;

	[[nodiscard]] QVector<QWeakPointer<Card>> readDeck(const QString& filePath, const QVector<QSharedPointer<Card>>& availableCards) const;
	[[nodiscard]] QVector<QWeakPointer<Card>> readDeck(QIODevice& ioDevice, const QVector<QSharedPointer<Card>>& availableCards) const;
};

