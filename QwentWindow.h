#pragma once

#include <QMainWindow>
#include <QSharedPointer>
#include "ui_QwentWindow.h"

#include "Card.h"
#include "QwentGame.h"

class IQwentStrategy;

class QwentWindow : public QMainWindow, public Ui::QwentWindowClass
{
    Q_OBJECT

public:
    QwentWindow(QWidget* parent = nullptr);

private slots:
	void on__strategyToUseComboBox_currentIndexChanged(int index);
	void on__loadFromFileButton_clicked();

private:
	void train();

	void showMachineLearningControls();
	void hideMachineLearningControls();

	QVector<QSharedPointer<IQwentStrategy>> _strategies;

    QVector<QSharedPointer<Card>> _allCards;
    QSharedPointer<QwentGame> _game;
};
