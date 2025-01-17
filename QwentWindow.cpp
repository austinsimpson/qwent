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

#include "QwentWindow.h"

#include "BasicQwentStrategy.h"
#include "DeckJsonSerializer.h"
#include "LearningQwentStrategy.h"
#include "StrategyTrainer.h"

#include <QDebug>
#include <QFileDialog>

QwentWindow::QwentWindow
(
    QWidget* parent
): 
    QMainWindow(parent),
    _game(new QwentGame)
{
	_strategies.push_back(QSharedPointer<IQwentStrategy>((IQwentStrategy*)new BasicQwentStrategy{0}));
	_strategies.push_back(QSharedPointer<IQwentStrategy>((IQwentStrategy*)new LearningQwentStrategy{0}));

    setupUi(this);
	hideMachineLearningControls();
    _qwentGameWidget->setGame(_game);

	_strategyTrainer = new StrategyTrainer();
	_strategyTrainer->moveToThread(&_trainingThread);
	_trainingThread.start();

	connect(_strategyTrainer, &StrategyTrainer::trainingFinished, this, &QwentWindow::onTrainingFinished);

	connect(_trainButton, &QPushButton::clicked, this, &QwentWindow::train);
	connect(_strategyTrainer, &StrategyTrainer::epochFinished, _trainingProgressBar, &QProgressBar::setValue);

	loadFile(":/resources/qwentai_1b_games.ai");
}

void QwentWindow::on__strategyToUseComboBox_currentIndexChanged
(
	int index
)
{
	constexpr auto opponentPlayerIndex = 1;
	switch (index)
	{
	case 0:
		_game->setStrategy(opponentPlayerIndex, _strategies[index]);
		hideMachineLearningControls();
		break;
	case 1:
		_game->setStrategy(opponentPlayerIndex, _strategies[index]);
		showMachineLearningControls();
		break;
	default:
		break;
	}
}

void QwentWindow::showMachineLearningControls()
{
	_testGamesCountSpinBox->show();
	_testGamesLabel->show();
	_trainGamesCountSpinBox->show();
	_trainGamesLabel->show();

	_epochCountSpinBox->show();
	_epochsLabel->show();
	_trainButton->show();
	_loadFromFileButton->show();

	_learningRateLabel->show();
	_learningRateSpinBox->show();

	_explorationRateLabel->show();
	_explorationRateSpinBox->show();

	_discountRateLabel->show();
	_discountRateDoubleSpinBox->show();

	_winRewardLabel->show();
	_winRewardSpinBox->show();

	_lossPenaltyLabel->show();
	_lossPenaltySpinBox->show();
}

void QwentWindow::hideMachineLearningControls()
{
	_testGamesCountSpinBox->hide();
	_testGamesLabel->hide();
	_trainGamesCountSpinBox->hide();
	_trainGamesLabel->hide();
	_epochCountSpinBox->hide();
	_epochsLabel->hide();
	_trainButton->hide();
	_loadFromFileButton->hide();

	_learningRateLabel->hide();
	_learningRateSpinBox->hide();

	_explorationRateLabel->hide();
	_explorationRateSpinBox->hide();

	_discountRateLabel->hide();
	_discountRateDoubleSpinBox->hide();

	_winRewardLabel->hide();
	_winRewardSpinBox->hide();

	_lossPenaltyLabel->hide();
	_lossPenaltySpinBox->hide();

	_trainingProgressBar->hide();
}

QwentWindow::~QwentWindow()
{
	_trainingThread.terminate();
}

void QwentWindow::on__loadFromFileButton_clicked()
{
	const QString fileName = QFileDialog::getOpenFileName(this, "Choose a model to load", QDir::home().path(), "*.ai");
	if ((fileName.isNull() || fileName.isEmpty()) == false)
	{
		loadFile(fileName);
	}
}

void QwentWindow::loadFile
(
	const QString& filePath
)
{
	QFile modelFile{ filePath };
	auto* learningStrategy = (LearningQwentStrategy*)_strategies[1].data();
	learningStrategy->loadFromFile(&modelFile);
	learningStrategy->setIsLearning(false);
	learningStrategy->setOwningPlayerIndex(1);

	_game->setStrategy(1, _strategies[1]);
	_learningRateSpinBox->setValue(learningStrategy->learningRate());
	_explorationRateSpinBox->setValue(learningStrategy->explorationRate());
	_discountRateDoubleSpinBox->setValue(learningStrategy->discountFactor());
	_winRewardSpinBox->setValue(learningStrategy->winReward());
	_lossPenaltySpinBox->setValue(learningStrategy->lossPenalty());
}

void QwentWindow::train()
{
	_strategyTrainer->setNumberOfEpochs(_epochCountSpinBox->value());
	_strategyTrainer->setTrainGamesPerEpoch(_trainGamesCountSpinBox->value());
	_strategyTrainer->setTestGamesPerEpoch(_testGamesCountSpinBox->value());
	_strategyTrainer->setLearningRate(_learningRateSpinBox->value());
	_strategyTrainer->setExplorationRate(_explorationRateSpinBox->value());
	_strategyTrainer->setDiscountFactor(_discountRateDoubleSpinBox->value());
	_strategyTrainer->setWinReward(_winRewardSpinBox->value());
	_strategyTrainer->setLossPenalty(_lossPenaltySpinBox->value());
	_trainingProgressBar->setValue(0);
	_trainingProgressBar->setMaximum(_epochCountSpinBox->value());
	_trainingProgressBar->show();

	_loadFromFileButton->setEnabled(false);
	_trainButton->setEnabled(false);

	QMetaObject::invokeMethod(_strategyTrainer, &StrategyTrainer::train);
}

void QwentWindow::onTrainingFinished()
{
	const auto& strategy = _strategyTrainer->winningStrategy();
	strategy->setOwningPlayerIndex(1);
	_trainingProgressBar->hide();

	_trainButton->setEnabled(true);
	_loadFromFileButton->setEnabled(true);

	const QString filePath = QFileDialog::getSaveFileName(this, "Choose a path to save your ai file.", QDir::homePath(), "*.ai");
	if (filePath.isNull() == false && filePath.isEmpty() == false)
	{
		QFile file{ filePath };
		((LearningQwentStrategy*)strategy.data())->saveToFile(&file);
	}

	_game->setStrategy(1, strategy);
}
