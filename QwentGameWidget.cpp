#include "QwentGameWidget.h"

#include <QMessageBox>
#include <QPainter>

const int gutterSize = 30;
const int cardSpacing = 5;

QwentGameWidget::QwentGameWidget
(
	QWidget* parent
): 
	QWidget(parent)
{
	setMouseTracking(true);
	grabKeyboard();
}

void QwentGameWidget::paintEvent
(
	QPaintEvent* paintEvent
)
{
	Q_UNUSED(paintEvent);

	QPainter painter;
	painter.begin(this);

	switch (_game.toStrongRef()->getState())
	{
	case QwentGame::GameState::GameInProgress:
		drawRows(painter, _game.toStrongRef()->rows());
		drawPlayerInfoAndScores(painter);
		drawPlayerHand(painter, _game.toStrongRef()->getHand(0));

		drawButton(painter, _endTurnButtonRect, "End Turn");
		break;
	case QwentGame::GameState::PreGame:
		drawButton(painter, _startGameButtonRect, "Start Game");
		break;
	case QwentGame::GameState::EndGame:
		drawButton(painter, _startGameButtonRect, "Play Again");
		drawPostGameSummary(painter);
		break;

	}
	painter.end();
}

void QwentGameWidget::resizeEvent
(
	QResizeEvent* event
)
{
	_rowsWindow = QRectF((width() * _playerInfoRelativeWidth), 0, width() * _rowsRelativeWidth, height() * _rowsRelativeHeight);
	_playerInfoWindow = QRectF(0, 0, width() * _playerInfoRelativeWidth, height() * _playerInfoRelativeHeight);
	_playerHandWindow = QRectF(width() * _playerInfoRelativeWidth + _defaultSpacing, height() * _rowsRelativeHeight, width() * _playerHandRelativeWidth - (2. *_defaultSpacing), height() * _playerHandRelativeHeight);

	const qreal endTurnButtonLeft = _playerHandWindow.left() - _endTurnButtonWidth - (2 * _defaultSpacing);
	const qreal endTurnButtonTop = _playerHandWindow.bottom() - _endTurnButtonHeight;
	_endTurnButtonRect = QRectF(endTurnButtonLeft, endTurnButtonTop, _endTurnButtonWidth, _endTurnButtonHeight);

	const qreal startGameButtonLeft = (width() - _startGameButtonWidth) / 2;
	const qreal startGameButtonTop = (height() - _endTurnButtonHeight) / 2;
	_startGameButtonRect = QRectF(startGameButtonLeft, startGameButtonTop, _startGameButtonWidth, _startGameButtonHeight);
}

void QwentGameWidget::setGame
(
	const QSharedPointer<QwentGame>& game
)
{
	_game = game;
}

void QwentGameWidget::drawRows
(
	QPainter& painter,
	const QVector<QwentRow>& rows
)
{
	for (const QwentRow& row : rows)
	{
		const int rowIndex = &row - &_game.toStrongRef()->rows()[0];
		const QRectF currentRowGeometry = getRowGeometry(rowIndex);
		drawRow(painter, row, currentRowGeometry);
	}
}

void QwentGameWidget::drawRow
(
	QPainter& painter, 
	const QwentRow& row,
	const QRectF& rowRect
)
{
	if (_mouseSelectionMode == MouseSelectionMode::SelectRow && rowRect.contains(_lastMousePosition))
	{
		painter.setBrush(QColor(0, 0, 0, 126));
	}
	else
	{
		painter.setBrush(QColor(0, 0, 0, 0));
	}
	painter.drawRect(rowRect);

	const int textMargins = 5;
	const int cardSpacing = 5;

	const QFontMetrics fontMetrics(painter.font());
	const int fontHeight = QFontMetrics(painter.font()).height();

	const int cardTop = rowRect.top() + cardSpacing;
	const int cardWidth = 100;

	const QString rowText = FieldPositionText[static_cast<int>(row.fieldPosition())];
	painter.drawText(QPoint(rowRect.left() + rowRect.width() - fontMetrics.horizontalAdvance(rowText) - textMargins, rowRect.top() + fontHeight), rowText);

	painter.save();
	const QString rowScoreText = QString::number(row.getTotalAttackPower());
	QFont largerTextFont = painter.font();
	largerTextFont.setPointSize(18);
	painter.setFont(largerTextFont);
	painter.drawText(QPoint(rowRect.left() + rowRect.width() - QFontMetrics(largerTextFont).horizontalAdvance(rowScoreText) - textMargins, rowRect.top() + (rowRect.height() / 2) + QFontMetrics(largerTextFont).capHeight() / 2), rowScoreText);
	painter.restore();

	int currentLeft = rowRect.left() + cardSpacing;
	for (const auto& card : row.cards())
	{	
		QRectF cardBackgroundRect(currentLeft, cardTop, cardWidth, rowRect.height() - 2 * cardSpacing);
		drawCard(painter, *card.toStrongRef(), cardBackgroundRect, row.effectiveAttackPower(card));
		currentLeft += cardWidth + cardSpacing;
	}
}

void QwentGameWidget::drawCard
(
	QPainter& painter,
	const Card& card, 
	const QRectF& cardRect,
	unsigned int effectiveAttackPower
)
{
	QFont font = painter.font();
	font.setPixelSize(12);
	painter.setFont(font);

	QFontMetrics fontMetrics(painter.font());
	const int fontHeight = fontMetrics.height();

	const QString attackPowerString = QString::number(effectiveAttackPower);
	const int attackPowerStringWidth = QFontMetrics(painter.font()).horizontalAdvance(attackPowerString);

	const int textMargins = 5;

	const auto baseColor = card.isLegendary() ? QColor("gold") : QColor("gray");
	const bool cardContainsMouse = cardRect.contains(_lastMousePosition);
	painter.fillRect(cardRect, cardContainsMouse ? baseColor.darker() : baseColor);
	painter.drawText(QPoint(cardRect.left() + textMargins, cardRect.top() + fontHeight), card.name());
	painter.drawText(QPoint(cardRect.left() + cardRect.width() - textMargins - attackPowerStringWidth, cardRect.top() + fontHeight), attackPowerString);

	const auto fieldPositionToText = [](FieldPosition fieldPosition) {
		switch (fieldPosition)
		{
		case FieldPosition::CloseCombat:
			return QString("CC");
		case FieldPosition::Ranged:
			return QString("R");
		case FieldPosition::Siege:
			return QString("S");
		case FieldPosition::Every:
			return QString("Every");
		case FieldPosition::Any:
			return QString("Any");
		}
	};

	const QString fieldPositionText = fieldPositionToText(card.fieldPosition());
	painter.drawText(cardRect.left() + textMargins, cardRect.bottom() - fontHeight, fieldPositionText);

	const auto specialAbilityToText = [](Card::SpecialEffect effect)
	{
		switch (effect)
		{
		case Card::SpecialEffect::DoubleAttackEntireRow:
			return QString("2x");
		case Card::SpecialEffect::DoubleAttackOfSameCard:
			return QString("Reinforce");
		case Card::SpecialEffect::Scorch:
			return QString("Scorch");
		case Card::SpecialEffect::Demoralize:
			return QString("Debuff");
		case Card::SpecialEffect::ClearDemoralize:
			return QString("Clear Debuff");
		default:
			return QString();
		}
	};

	const QString specialEffectText = specialAbilityToText(card.specialEffect());
	painter.drawText(cardRect.right() - textMargins - fontMetrics.horizontalAdvance(specialEffectText), cardRect.bottom() - fontHeight, specialEffectText);

}

void QwentGameWidget::drawPlayerInfoAndScores
(
	QPainter& painter
)
{
	QFont nameFont = painter.font();
	nameFont.setPointSize(18);
	painter.setFont(nameFont);

	QFontMetrics fontMetrics(painter.font());

	const QPointF opponentTextLocation = _playerInfoWindow.center() + QPointF(_playerInfoWindow.width() / 2 - fontMetrics.horizontalAdvance("Opponent"), -gutterSize / 2);
	painter.drawText(opponentTextLocation, "Opponent");
	const QString opponentScoreText = QString::number(_game.toStrongRef()->getPlayerScore(1));
	
	const QPointF opponentScoreLocation = QPoint(_playerInfoWindow.right() - fontMetrics.horizontalAdvance(opponentScoreText), opponentTextLocation.y() - fontMetrics.height());
	painter.drawText(opponentScoreLocation, opponentScoreText);

	const QPointF youTextLocation = _playerInfoWindow.center() + QPointF(_playerInfoWindow.width() / 2 - fontMetrics.horizontalAdvance("You"), fontMetrics.capHeight() + gutterSize / 2);
	painter.drawText(youTextLocation, "You");
	const QString yourScoreText = QString::number(_game.toStrongRef()->getPlayerScore(0));

	const QPointF yourScoreLocation = QPoint(_playerInfoWindow.right() - fontMetrics.horizontalAdvance(yourScoreText), youTextLocation.y() + fontMetrics.height());
	painter.drawText(yourScoreLocation, yourScoreText);

	const Match& currentMatch = _game.toStrongRef()->currentMatch();
	unsigned int roundsToWin = currentMatch.roundsToWin();

	const int circleRadius = 10;

	QPoint opponentRoundsWonIndicatorPosition = QPoint(_playerInfoWindow.right() - circleRadius, opponentScoreLocation.y() - fontMetrics.height() - circleRadius - 5);
	const unsigned int roundsLostByOpponent = currentMatch.getRoundsLostByPlayer(1);
	drawPlayerLivesLeft(painter, opponentRoundsWonIndicatorPosition, circleRadius, roundsLostByOpponent, roundsToWin);

	const QString opponentCardsLeftText = QString::number(_game.toStrongRef()->getHand(1).count());
	const QPointF cardsRemainingLocation{ _playerInfoWindow.right() - fontMetrics.horizontalAdvance(opponentCardsLeftText), opponentRoundsWonIndicatorPosition.y() - _defaultSpacing - fontMetrics.height() };
	painter.drawText(cardsRemainingLocation, opponentCardsLeftText);

	QPoint yourRoundsWonIndicatorPosition = QPoint(_playerInfoWindow.right() - circleRadius, yourScoreLocation.y() + circleRadius + 5);
	const unsigned int roundsLostByYou = currentMatch.getRoundsLostByPlayer(0);
	drawPlayerLivesLeft(painter, yourRoundsWonIndicatorPosition, circleRadius, roundsLostByYou, roundsToWin);
}

void QwentGameWidget::drawPlayerLivesLeft
(
	QPainter& painter,
	QPoint origin,
	unsigned int radius,
	unsigned int roundsLost, 
	unsigned int roundsToWin
)
{
	for (unsigned int index = 0; index < roundsToWin; ++index)
	{
		if (index >= roundsLost)
		{
			painter.setBrush(Qt::green);
		}
		else
		{
			painter.setBrush(Qt::transparent);
		}

		painter.drawEllipse(origin, radius, radius);
		origin -= QPoint((radius * 2) + 5, 0);
	}
}

void QwentGameWidget::drawPlayerHand
(
	QPainter& painter,
	const QVector<QWeakPointer<Card>>& cards
)
{
	painter.setBrush(QColor(0, 0, 0, 0));
	painter.drawRect(_playerHandWindow);
	if (cards.size() > 0)
	{
		const auto cardWidth = ((_playerHandWindow.width() - 1) / cards.size()) - cardSpacing;
		QRect rectOrigin = QRect(_playerHandWindow.left() + cardSpacing, _playerHandWindow.top() + cardSpacing, cardWidth, _playerHandWindow.height() - (cardSpacing * 2));
		for (const auto& card : cards)
		{
			drawCard(painter, *card.toStrongRef(), rectOrigin, card.toStrongRef()->attackPower());
			rectOrigin.adjust(cardWidth + cardSpacing, 0, cardWidth + cardSpacing, 0);
		}
	}
	else
	{
		QFont painterFont = painter.font();
		painterFont.setPointSizeF(14);
		painter.setFont(painterFont);

		QFontMetricsF fontMetrics(painterFont);
		const QString noCardsText = "You are out of cards.";

		const qreal halfTextWidth = fontMetrics.horizontalAdvance(noCardsText) / 2;
		const qreal halfTextHeight = fontMetrics.height() / 2;

		const auto rectCenter = _playerHandWindow.center();
		painter.drawText(rectCenter - QPointF{halfTextWidth, -halfTextHeight}, noCardsText);
	}
}

void QwentGameWidget::mouseMoveEvent
(
	QMouseEvent* mouseEvent
)
{
	_lastMousePosition = mouseEvent->pos();
	repaint();
}

void QwentGameWidget::mouseReleaseEvent
(
	QMouseEvent* mouseEvent
)
{
	if (mouseEvent->button() == Qt::MouseButton::LeftButton)
	{
		switch (_game.toStrongRef()->getState())
		{
		case QwentGame::GameState::GameInProgress:
			switch (_mouseSelectionMode)
			{
				case QwentGameWidget::MouseSelectionMode::SelectCard:
				{
					const unsigned int handSize = _game.toStrongRef()->getHand(0).size();
					if (handSize == 0)
						return;

					const int left = this->_playerInfoRelativeWidth * width();
					const int top = _playerInfoRelativeHeight * height();

					const int playerHandWidth = _playerHandRelativeWidth * width();
					const int playerHandHeight = _playerHandRelativeHeight * height();

					const int cardWidth = (playerHandWidth / _game.toStrongRef()->getHand(0).size()) - cardSpacing;
					const int cardIndex = (mouseEvent->pos().x() - left) / (cardWidth + cardSpacing);

					if (0 <= cardIndex && cardIndex < handSize)
					{
						const auto& selectedCard = _game.toStrongRef()->getHand(0)[cardIndex];

						const QRect selectedCardGeomerty(left + ((cardWidth + cardSpacing) * cardIndex), top, cardWidth, playerHandHeight);
						const auto mousePos = mouseEvent->pos();
						if (selectedCardGeomerty.contains(mousePos))
						{
							if (selectedCard.toStrongRef()->fieldPosition() != FieldPosition::Any || selectedCard.toStrongRef()->specialEffect() == Card::SpecialEffect::ClearDemoralize)
							{
								_game.toStrongRef()->playCard(0, cardIndex);
							}
							else
							{
								_pendingCardIndex = cardIndex;
								_mouseSelectionMode = QwentGameWidget::MouseSelectionMode::SelectRow;
							}
						}
					}
				}
				break;

				case QwentGameWidget::MouseSelectionMode::SelectRow:
				{
					for (unsigned int rowIndex = 0; rowIndex < 6; ++rowIndex)
					{
						const QRectF rowGeometry = getRowGeometry(rowIndex);
						if (rowGeometry.contains(mouseEvent->pos()) && rowIndex < 3)
						{
							_game.toStrongRef()->playCard(0, _pendingCardIndex, (FieldPosition)rowIndex);
							_mouseSelectionMode = MouseSelectionMode::SelectCard;
						}
					}
					break;
				}
				}

				if (_endTurnButtonRect.contains(_lastMousePosition))
				{
					_game.toStrongRef()->endTurn(true);
				}
		case QwentGame::GameState::PreGame:
		case QwentGame::GameState::EndGame:
			if (_startGameButtonRect.contains(_lastMousePosition))
			{
				_game.toStrongRef()->startMatch();
			}
			break;
		}

		update();
	}
}

void QwentGameWidget::keyReleaseEvent
(
	QKeyEvent* keyEvent
)
{
	switch (keyEvent->key())
	{
		case Qt::Key_Escape:
			if (_mouseSelectionMode == MouseSelectionMode::SelectRow)
			{
				_mouseSelectionMode = MouseSelectionMode::SelectCard;
				update();
			}
			break;
		default:
			break;
	}
}

const QRectF QwentGameWidget::getRowGeometry
(
	unsigned int rowIndex
)	const
{
	bool isFirstPlayer = rowIndex < 3;
	const int rowPositionIndex = 5 - (isFirstPlayer ? 2 - rowIndex : rowIndex);

	const int rowHeight = _rowsWindow.height() / 6 - cardSpacing;
	const int rowTop = isFirstPlayer ? rowPositionIndex * rowHeight + gutterSize : rowPositionIndex * rowHeight;
	return QRectF(_rowsWindow.left() + _defaultSpacing, rowTop, _rowsWindow.width() - (2. * _defaultSpacing), rowHeight);
}

void QwentGameWidget::drawButton
(
	QPainter& painter,
	const QRectF& bounds,
	const QString& text
)
{
	painter.setBrush(bounds.contains(_lastMousePosition) ? QColor(0, 0, 0, 126) : QColor(0, 0, 0, 0));
	painter.drawRect(bounds);

	auto font = painter.font();
	font.setPointSizeF(12.);
	painter.setFont(font);

	QFontMetrics metrics(font);
	const auto halfFontWidth = metrics.horizontalAdvance(text) / 2.;
	const auto halfFontHeight = metrics.capHeight() / 2.;

	painter.drawText(QPointF{ bounds.center().x() - halfFontWidth, bounds.top() + (bounds.height() / 2.) + halfFontHeight }, text);
}

void QwentGameWidget::drawPostGameSummary
(
	QPainter& painter
)
{
	const auto& game = _game.toStrongRef();
	bool didWin = game->winningPlayer() == 0;
	unsigned int playerScore = game->getPlayerScore(0);
	unsigned int aiScore = game->getPlayerScore(1);

	const QString roundText = QString(didWin ? "You won." : "You lost.");

	QFontMetrics fontMetrics(painter.font());
	const auto fontHalfWidth = fontMetrics.horizontalAdvance(roundText) / 2.;
	const auto fontHeight = fontMetrics.capHeight();

	painter.drawText(QPointF{ (width() / 2.) - fontHalfWidth, (height() / 2.) - _startGameButtonHeight + fontHeight }, roundText);
}
