#include "QwentWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QwentWindow w;

    w.show();
    return a.exec();
}
