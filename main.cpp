#include <QtWidgets/QApplication>
#include "QwentWindow.h"

#include <vector>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	QwentWindow w;
	w.show();

	return a.exec();
}
