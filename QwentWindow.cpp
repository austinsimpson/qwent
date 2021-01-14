#include "QwentWindow.h"

#include "BraindeadQwentStrategy.h"
#include "DeckJsonSerializer.h"
#include "LearningQwentStrategy.h"

#include <QDebug>

QwentWindow::QwentWindow
(
    QWidget* parent
): 
    QMainWindow(parent),
    _game(new QwentGame)
{
	_strategies.push_back(QSharedPointer<IQwentStrategy>((IQwentStrategy*)new BraindeadQwentStrategy{0}));
	_strategies.push_back(QSharedPointer<IQwentStrategy>((IQwentStrategy*)new LearningQwentStrategy{0}));

    setupUi(this);
	hideMachineLearningControls();
    _qwentGameWidget->setGame(_game);
    _game->startMatch();
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
}

void QwentWindow::on__loadFromFileButton_clicked()
{

}
