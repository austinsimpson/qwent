#include "QwentWindow.h"

#include "DeckJsonSerializer.h"

#include <QDebug>

QwentWindow::QwentWindow
(
    QWidget* parent
): 
    QMainWindow(parent),
    _game(new QwentGame)
{
    setupUi(this);
    _qwentGameWidget->setGame(_game);
    _game->startMatch();
}
