#include <QtWidgets/QApplication>
#include "QwentWindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	QwentWindow w;
	w.show();

	return a.exec();
}
