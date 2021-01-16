#pragma once

#include <QMainWindow>
#include <QSharedPointer>
#include <QThread>
#include "ui_QwentWindow.h"

#include "Card.h"
#include "QwentGame.h"

class IQwentStrategy;
class StrategyTrainer;

class QwentWindow : public QMainWindow, public Ui::QwentWindowClass
{
    Q_OBJECT

public:
    QwentWindow(QWidget* parent = nullptr);
	~QwentWindow();

private slots:
	void on__strategyToUseComboBox_currentIndexChanged(int index);
	void on__loadFromFileButton_clicked();

	void onTrainingFinished();

private:
	void train();

	void showMachineLearningControls();
	void hideMachineLearningControls();

	QVector<QSharedPointer<IQwentStrategy>> _strategies;

    QVector<QSharedPointer<Card>> _allCards;
    QSharedPointer<QwentGame> _game;

	QThread _trainingThread;
	StrategyTrainer* _strategyTrainer;
};
