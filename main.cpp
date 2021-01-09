#include "QwentWindow.h"
#include <QtWidgets/QApplication>

#include "QwentGame.h"
#include "LearningQwentStrategy.h"

#include <QStandardPaths>
#include <QDebug>
#include <QFile>
#include <map>
#include <iostream>

#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	//QwentWindow w;

	QwentGame game;

	QFile winMetricsFile("/Users/AustinSimpson/Desktop/wins.csv");
	winMetricsFile.open(QIODevice::WriteOnly);

	QTextStream winMetricsStream(&winMetricsFile);

	constexpr auto numberOfTrainingBlocks = 1000;
	constexpr auto trainGamesPerStep = 1000;
	constexpr auto testGamesPerStep = 100;
	for (int trainingBlockIndex = 0; trainingBlockIndex < numberOfTrainingBlocks; ++trainingBlockIndex)
	{
		((LearningQwentStrategy*)(game.getStrategy(1).data()))->setIsLearning(true);
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
		winMetricsStream << QString("%0,%1\n").arg(trainingBlockIndex).arg(aiWinCount);
		qDebug() << "AI won: " << aiWinCount << " out of" << gamesPlayed << "games";
		qDebug() << "QTable Size: " << ((LearningQwentStrategy*)(game.getStrategy(1).data()))->qTable().size();
	}
	winMetricsFile.close();

	QFile saveFile{ "/Users/AustinSimpson/Desktop/qwentmodel.ai" };
	((LearningQwentStrategy*)(game.getStrategy(1).data()))->saveToFile(&saveFile);

	return a.exec();
}
