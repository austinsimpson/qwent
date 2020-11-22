#pragma once

#include <QMainWindow>
#include <QSharedPointer>
#include "ui_QwentWindow.h"

#include "Card.h"
#include "QwentGame.h"

class QwentWindow : public QMainWindow, public Ui::QwentWindowClass
{
    Q_OBJECT

public:
    QwentWindow(QWidget* parent = nullptr);

private:
    QVector<QSharedPointer<Card>> _allCards;

    QSharedPointer<QwentGame> _game;
};
