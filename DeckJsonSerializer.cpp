#include <QFile>

#include "DeckJsonSerializer.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

QVector<QSharedPointer<Card>> DeckJsonSerializer::readAllCards
(
	const QString& filePath
)	const
{
	if (QFile::exists(filePath))
	{
		QFile cardsFile{ filePath };
		if (cardsFile.open(QFile::ReadOnly))
		{
			return readAllCards(cardsFile);
		}
	}
	return QVector<QSharedPointer<Card>>();
}

QVector<QSharedPointer<Card>> DeckJsonSerializer::readAllCards
(
	QIODevice& ioDevice
)	const
{
	QVector<QSharedPointer<Card>> result;

	QJsonDocument jsonDocument = QJsonDocument::fromJson(ioDevice.readAll());
	if (jsonDocument.isNull() == false)
	{
		const auto& rootElement = jsonDocument.object();
		for (const auto& cardDefinitionJson : rootElement.value("cards").toArray())
		{
			const auto cardDefinitionObject = cardDefinitionJson.toObject();
			const auto attackPower = static_cast<unsigned int>(cardDefinitionObject.value("attackPower").toInt());
			const auto name = cardDefinitionObject.value("name").toString();
			const auto isLegendary = cardDefinitionObject.value("isLegendary").toBool();
			const auto specialEffect = static_cast<Card::SpecialEffect>(cardDefinitionObject.value("specialEffect").toInt());
			const auto fieldPosition = static_cast<FieldPosition>(cardDefinitionObject.value("fieldPosition").toInt());
			result.append(QSharedPointer<Card>(new Card{ attackPower, specialEffect, name, isLegendary, fieldPosition}));
		}
	}

	return result;
}

QVector<QWeakPointer<Card>> DeckJsonSerializer::readDeck
(
	const QString& filePath,
	const QVector<QSharedPointer<Card>>& availableCards
)	const
{
	
	if (QFile::exists(filePath)) [[likely]]
	{
		QFile file(filePath);
		return readDeck(file, availableCards);
	}
	else [[unlikely]]
	{
		return QVector<QWeakPointer<Card>>();
	}
}

QVector<QWeakPointer<Card>> DeckJsonSerializer::readDeck
(
	QIODevice& ioDevice,
	const QVector<QSharedPointer<Card>>& availableCards
)	const
{
	bool isOpen = ioDevice.isOpen();
	if (isOpen == false)
		isOpen = ioDevice.open(QIODevice::ReadOnly);

	if (isOpen)
	{
		QVector<QWeakPointer<Card>> result;
		QJsonDocument jsonDocument = QJsonDocument::fromJson(ioDevice.readAll());
		if (jsonDocument.isNull() == false)
		{
			const auto& rootElement = jsonDocument.object();
			for (const auto& cardIndexJson : rootElement.value("cards").toArray())
			{
				int cardIndex = cardIndexJson.toInt();
				result.append(availableCards[cardIndex].toWeakRef());
			}
		}
		return result;
	}
	else
	{
		return QVector<QWeakPointer<Card>>();
	}
}