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

    DeckJsonSerializer serializer;
    _allCards = serializer.readAllCards(":/resources/card-definitions.json");

    auto playerDeck = serializer.readDeck(":/resources/sample-deck.json", _allCards);
	_game->setDeck(0, playerDeck);
	_game->setDeck(1, playerDeck);

    _game->startMatch();
}
