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
}

void QwentGameWidget::paintEvent
(
	QPaintEvent* paintEvent
)
{
	Q_UNUSED(paintEvent);

	QPainter painter;
	painter.begin(this);

	QRect rowsWindow((width() * .2), 0, width() * .8, height() * .8);
	drawRows(painter, _game.data()->rows(), rowsWindow);

	QRect playerInfoRect(0, 0, width() * .2, height() * .8);
	drawPlayerInfoAndScores(painter, playerInfoRect);

	QRect playerHandWindow(width() * .2, height() * .8, width() * .8, height() * 2);
	drawPlayerHand(painter, _game.data()->getHand(0), playerHandWindow);

	painter.end();
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
	const std::array<QwentRow, 6>& rows,
	const QRect& painterWindow
)
{
	for (const QwentRow& row : rows)
	{
		const int rowIndex = &row - &_game.data()->rows()[0];

		bool isFirstPlayer = rowIndex < 3;
		const int rowPositionIndex = 5 - (isFirstPlayer ? 2 - rowIndex : rowIndex);

		const int cardSpacing = 5;

		const int rowHeight = painterWindow.height() / 6 - cardSpacing;
		const int rowTop = isFirstPlayer ? rowPositionIndex * rowHeight + gutterSize : rowPositionIndex * rowHeight;

		drawRow(painter, row, QRect(painterWindow.left() + cardSpacing, rowTop, painterWindow.width() - cardSpacing * 2, rowHeight));
	}
}

void QwentGameWidget::drawRow
(
	QPainter& painter, 
	const QwentRow& row,
	const QRect& rowRect
)
{
	painter.drawRect(rowRect);

	const int textMargins = 5;
	const int cardSpacing = 5;

	const QFontMetrics fontMetrics(painter.font());
	const int fontHeight = QFontMetrics(painter.font()).height();

	const int cardTop = rowRect.top() + cardSpacing;
	const int cardWidth = 100;

	const int cardHeight = rowRect.height() - 2 * cardSpacing;

	const QString rowText = FieldPositionText[static_cast<int>(row.fieldPosition())];
	painter.drawText(QPoint(rowRect.left() + rowRect.width() - fontMetrics.width(rowText) - textMargins, rowRect.top() + fontHeight), rowText);

	painter.save();
	const QString rowScoreText = QString::number(row.getTotalAttackPower());
	QFont largerTextFont = painter.font();
	largerTextFont.setPointSize(18);
	painter.setFont(largerTextFont);
	painter.drawText(QPoint(rowRect.left() + rowRect.width() - QFontMetrics(largerTextFont).width(rowScoreText) - textMargins, rowRect.top() + (rowRect.height() / 2) + QFontMetrics(largerTextFont).capHeight() / 2), rowScoreText);
	painter.restore();

	int currentLeft = rowRect.left() + cardSpacing;
	for (const auto& card : row.cards())
	{	
		QRect cardBackgroundRect(currentLeft, cardTop, cardWidth, rowRect.height() - 2 * cardSpacing);
		drawCard(painter, *card.data(), cardBackgroundRect, row.effectiveAttackPower(card));
		currentLeft += cardWidth + cardSpacing;
	}
}

void QwentGameWidget::drawCard
(
	QPainter& painter,
	const Card& card, 
	const QRect& cardRect,
	unsigned int effectiveAttackPower
)
{
	QFont font = painter.font();
	font.setPixelSize(12);
	painter.setFont(font);

	QFontMetrics fontMetrics(painter.font());
	const int fontHeight = fontMetrics.height();

	const QString attackPowerString = QString::number(effectiveAttackPower);
	const int attackPowerStringWidth = QFontMetrics(painter.font()).width(attackPowerString);

	const int textMargins = 5;

	const auto baseColor = card.isLegendary() ? QColor("gold") : QColor("gray");
	const bool cardContainsMouse = cardRect.contains(_lastMousePosition);
	painter.fillRect(cardRect, cardContainsMouse ? baseColor.darker() : baseColor);
	painter.drawText(QPoint(cardRect.left() + textMargins, cardRect.top() + fontHeight), card.name());
	painter.drawText(QPoint(cardRect.left() + cardRect.width() - textMargins - attackPowerStringWidth, cardRect.top() + fontHeight), attackPowerString);
}

void QwentGameWidget::drawPlayerInfoAndScores
(
	QPainter& painter, 
	const QRect& infoRect
)
{
	QFont nameFont = painter.font();
	nameFont.setPointSize(18);
	painter.setFont(nameFont);

	QFontMetrics fontMetrics(painter.font());

	const QPoint opponentTextLocation = infoRect.center() + QPoint(infoRect.width() / 2 - fontMetrics.width("Opponent"), -gutterSize / 2);
	painter.drawText(opponentTextLocation, "Opponent");
	const QString opponentScoreText = QString::number(_game.data()->getPlayerScore(1));
	
	const QPoint opponentScoreLocation = QPoint(infoRect.right() - fontMetrics.width(opponentScoreText), opponentTextLocation.y() - fontMetrics.height());
	painter.drawText(opponentScoreLocation, opponentScoreText);

	const QPoint youTextLocation = infoRect.center() + QPoint(infoRect.width() / 2 - fontMetrics.width("You"), fontMetrics.capHeight() + gutterSize / 2);
	painter.drawText(youTextLocation, "You");
	const QString yourScoreText = QString::number(_game.data()->getPlayerScore(0));

	const QPoint yourScoreLocation = QPoint(infoRect.right() - fontMetrics.width(yourScoreText), youTextLocation.y() + fontMetrics.height());
	painter.drawText(yourScoreLocation, yourScoreText);

	const Match& currentMatch = _game.data()->currentMatch();
	unsigned int roundsToWin = currentMatch.roundsToWin();

	const int circleRadius = 10;

	QPoint opponentRoundsWonIndicatorPosition = QPoint(infoRect.right() - circleRadius, opponentScoreLocation.y() - fontMetrics.height() - circleRadius - 5);
	const unsigned int roundsLostByOpponent = currentMatch.getRoundsLostByPlayer(1);
	drawPlayerLivesLeft(painter, opponentRoundsWonIndicatorPosition, circleRadius, roundsLostByOpponent, roundsToWin);

	QPoint yourRoundsWonIndicatorPosition = QPoint(infoRect.right() - circleRadius, yourScoreLocation.y() + circleRadius + 5);
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

		painter.drawEllipse(origin, radius, radius);
		origin -= QPoint((radius * 2) + 5, 0);
	}
}

void QwentGameWidget::drawPlayerHand
(
	QPainter& painter,
	const QVector<QWeakPointer<Card>>& cards,
	const QRect& painterWindow
)
{
	const int cardWidth = painterWindow.width() / cards.size() - cardSpacing;
	QRect rectOrigin = QRect(painterWindow.left() + cardSpacing, painterWindow.top() + cardSpacing, cardWidth, painterWindow.height());
	for (const auto& card : cards)
	{
		drawCard(painter, *card.data(), rectOrigin, card.data()->attackPower());
		rectOrigin.adjust(cardWidth + cardSpacing, 0, cardWidth + cardSpacing, 0);
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
		const int left = 0.2 * width();
		const int top = 0.8 * height();

		const int playerHandWidth = 0.8 * width();
		const int playerHandHeight = 0.2 * height();

		const int cardWidth = playerHandWidth / _game.data()->getHand(0).size() - cardSpacing;
		const int cardIndex = (mouseEvent->pos().x() - left) / (cardWidth + cardSpacing);
		const auto& selectedCard = _game.data()->getHand(0)[cardIndex];
		if (selectedCard.data()->fieldPosition() != FieldPosition::Any || selectedCard.data()->specialEffect() == Card::SpecialEffect::ClearDemoralize)
		{
			_game.data()->playCard(0, cardIndex);
		}
		else
		{

		}
		update();
	}
}