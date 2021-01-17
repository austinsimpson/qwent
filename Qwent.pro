QT += core widgets
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
    ./BasicQwentStrategy.h \
    ./IQwentStrategy.h \
    ./LearningQwentStrategy.h \
    ./MatchSnapshot.h \
    ./NullQwentStrategy.h \
    ./Range.h \
    ./StrategyTrainer.h

SOURCES += ./Card.cpp \
    ./DeckJsonSerializer.cpp \
    ./Match.cpp \
    ./Player.cpp \
    ./QwentGame.cpp \
    ./QwentGameWidget.cpp \
    ./QwentRow.cpp \
    ./QwentWindow.cpp \
    ./main.cpp \
    ./BasicQwentStrategy.cpp \
    ./LearningQwentStrategy.cpp \
    ./NullQwentStrategy.cpp \
    ./StrategyTrainer.cpp

FORMS += ./QwentWindow.ui
RESOURCES += \
    QwentResources.qrc
