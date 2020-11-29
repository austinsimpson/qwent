QT += core gui widgets
TEMPLATE = app
TARGET = Qwent

CONFIG += c++latest

HEADERS += ./Card.h \
    ./DeckJsonSerializer.h \
    ./Match.h \
    ./Player.h \
    ./QwentGame.h \
    ./QwentRow.h \
    ./QwentWindow.h \
    ./QwentGameWidget.h \
    BraindeadQwentStrategy.h \
    IQwentStrategy.h \
    NullQwentStrategy.h

SOURCES += ./Card.cpp \
    ./DeckJsonSerializer.cpp \
    ./Match.cpp \
    ./Player.cpp \
    ./QwentGame.cpp \
    ./QwentGameWidget.cpp \
    ./QwentRow.cpp \
    ./QwentWindow.cpp \
    ./main.cpp \
    BraindeadQwentStrategy.cpp \
    NullQwentStrategy.cpp
FORMS += ./QwentWindow.ui
RESOURCES += QwentWindow.qrc
