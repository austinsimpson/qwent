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
		unsigned int cardId = 0;
		for (const auto cardDefinitionJson : rootElement.value("cards").toArray())
		{
			const auto cardDefinitionObject = cardDefinitionJson.toObject();
			const auto attackPower = static_cast<unsigned int>(cardDefinitionObject.value("attackPower").toInt());
			const auto name = cardDefinitionObject.value("name").toString();
			const auto isLegendary = cardDefinitionObject.value("isLegendary").toBool();
			const auto specialEffect = static_cast<Card::SpecialEffect>(cardDefinitionObject.value("specialEffect").toInt());
			const auto fieldPosition = static_cast<FieldPosition>(cardDefinitionObject.value("fieldPosition").toInt());
			result.append(QSharedPointer<Card>(new Card{ attackPower, specialEffect, name, isLegendary, fieldPosition, cardId}));
			++cardId;
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
			for (const auto cardIndexJson : rootElement.value("cards").toArray())
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
