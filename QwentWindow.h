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
	void loadFile(const QString& filePath);

	void train();

	void showMachineLearningControls();
	void hideMachineLearningControls();

	QVector<QSharedPointer<IQwentStrategy>> _strategies;

    QVector<QSharedPointer<Card>> _allCards;
    QSharedPointer<QwentGame> _game;

	QThread _trainingThread;
	StrategyTrainer* _strategyTrainer;
};
