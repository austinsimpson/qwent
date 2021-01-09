#pragma once

#include <QMouseEvent>
#include <QPaintEvent>
#include <QWidget>

#include "QwentGame.h"
#include "QwentRow.h"

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

	void drawEndTurnButton(QPainter& painter);

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

	QRectF _playerInfoWindow;
	QRectF _rowsWindow;
	QRectF _playerHandWindow;

	QRectF _endTurnButtonRect;

	unsigned int _pendingCardIndex{0};
};
