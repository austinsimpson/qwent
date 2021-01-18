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

#include <QMouseEvent>
#include <QPaintEvent>
#include <QWidget>

#include "QwentGame.h"
#include "QwentRow.h"

#include <functional>

class QwentGameWidget : public QWidget
{
	Q_OBJECT

public:
	QwentGameWidget(QWidget* parent = nullptr);
	~QwentGameWidget() = default;

	void setGame(const QSharedPointer<QwentGame>& qwentGame);

	void paintEvent(QPaintEvent* paintEvent) override;

	enum class MouseSelectionMode {
		SelectCard,
		SelectRow
	};

private:
	void resizeEvent(QResizeEvent* event) override;
	void mouseMoveEvent(QMouseEvent* mouseEvent) override;
	void mouseReleaseEvent(QMouseEvent* mouseEvent) override;
	void keyReleaseEvent(QKeyEvent* keyEvent) override;

	void drawRows(QPainter& painter, const QVector<QwentRow>& rows);
	void drawRow(QPainter& painter, const QwentRow& row, const QRectF& rowRect);

	void drawCard(QPainter& painter, const Card& card, const QRectF& cardRect, unsigned int effectAttackPower);

	void drawPlayerInfoAndScores(QPainter& painter);
	void drawPlayerLivesLeft(QPainter& painter, QPoint origin, unsigned int radius, unsigned int roundsLost, unsigned int roundsToWin);

	void drawPlayerHand(QPainter& painter, const QVector<QWeakPointer<Card>>& cards);

	void drawButton(QPainter& painter, const QRectF& rect, const QString& text);

	void drawPostGameSummary(QPainter& painter);

	const QRectF getRowGeometry(unsigned int rowIndex) const;

	MouseSelectionMode _mouseSelectionMode{ MouseSelectionMode::SelectCard };

	QWeakPointer<QwentGame> _game;
	QPoint _lastMousePosition;

	const qreal _playerInfoRelativeWidth { .1 };
	const qreal _playerInfoRelativeHeight { .8 };

	const qreal _rowsRelativeWidth { .9 };
	const qreal _rowsRelativeHeight { .8 };

	const qreal _playerHandRelativeWidth { .9 };
	const qreal _playerHandRelativeHeight { .2 };

	const qreal _defaultSpacing { 4. };

	const qreal _endTurnButtonWidth { 100. };
	const qreal _endTurnButtonHeight { 40. };

	const qreal _startGameButtonWidth{ 160. };
	const qreal _startGameButtonHeight{ 40. };

	QRectF _playerInfoWindow;
	QRectF _rowsWindow;
	QRectF _playerHandWindow;

	QRectF _endTurnButtonRect;
	QRectF _startGameButtonRect;

	unsigned int _pendingCardIndex{0};
};
