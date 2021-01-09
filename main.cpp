#include "QwentWindow.h"
#include <QtWidgets/QApplication>

#include "QwentGame.h"
#include "LearningQwentStrategy.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	QwentGame game;

	constexpr auto numberOfTrainingBlocks = 1000;
	constexpr auto trainGamesPerStep = 100;
	constexpr auto testGamesPerStep = 100;
	for (int trainingBlockIndex = 0; trainingBlockIndex < numberOfTrainingBlocks; ++trainingBlockIndex)
	{
		for (int gameIndex = 0; gameIndex < trainGamesPerStep; ++gameIndex)
		{
			game.startMatch();
		}

		((LearningQwentStrategy*)(game.getStrategy(1).data()))->setIsLearning(false);

		int aiWinCount = 0;
		int gamesPlayed = 0;
		for (int gameIndex = 0; gameIndex < testGamesPerStep; ++gameIndex)
		{
			game.startMatch();
			if (game.winningPlayer() == 1)
			{
				++aiWinCount;
			}
			gamesPlayed++;
		}
		qDebug() << "AI won: " << aiWinCount << " out of" << gamesPlayed << "games";
	}

    return a.exec();
}
