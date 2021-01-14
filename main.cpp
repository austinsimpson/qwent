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

	QwentWindow w;
	w.show();

	return a.exec();
}
