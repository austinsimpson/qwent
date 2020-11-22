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
		Default,
		SelectRow
	};

private:
	void mouseMoveEvent(QMouseEvent* mouseEvent);
	void mouseReleaseEvent(QMouseEvent* mouseEvent);

	void drawRows(QPainter& painter, const std::array<QwentRow, 6>& rows, const QRect& painterWindow);
	void drawRow(QPainter& painter, const QwentRow& row, const QRect& rowRect);

	void drawCard(QPainter& painter, const Card& card, const QRect& cardRect, unsigned int effectAttackPower);

	void drawPlayerInfoAndScores(QPainter& painter, const QRect& infoRect);
	void drawPlayerLivesLeft(QPainter& painter, QPoint origin, unsigned int radius, unsigned int roundsLost, unsigned int roundsToWin);

	void drawPlayerHand(QPainter& painter, const QVector<QWeakPointer<Card>>& cards, const QRect& painterWindow);

	QWeakPointer<QwentGame> _game;
	QPoint _lastMousePosition;
};
