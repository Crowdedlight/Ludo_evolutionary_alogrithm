#-------------------------------------------------
#
# Project created by QtCreator 2016-03-15T10:40:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ludo
TEMPLATE = app


SOURCES += main.cpp\
    dialog.cpp \
    game.cpp \
    ludo_player.cpp \
    ludo_player_random.cpp \
    ludo_player_evol.cpp \
    populationmanager.cpp \
    rl_player.cpp

HEADERS  += dialog.h \
    game.h \
    ludo_player.h \
    positions_and_dice.h \
    ludo_player_random.h \
    ludo_player_evol.h \
    populationmanager.h \
    rl_player.h

FORMS    += dialog.ui

QMAKE_CXXFLAGS += -std=c++14 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wunused

#INCLUDEPATH += $$_PRO_FILE_PWD_/yaml-cpp/
#LIBS += -L$$PWD/libs/ -lyaml-cpp

#win32 {
#    #/* If you compile with QtCreator/gcc: */
#    win32-g++:LIBS += -L"$$_PRO_FILE_PWD_/libs/"
#    win32-g++:LIBS += -lyaml-cpp

#    #/* IF you compile with MSVC:
#    #win32-msvc:LIBS += /path/to/your/libMyLib.lib*/
#}
#macx {
#    LIBS += -L"$$_PRO_FILE_PWD_/libs/"
#    LIBS += -lyaml-cpp
#}


#unix:!macx|win32: LIBS += -L$$PWD/libs/ -lyaml-cpp

#INCLUDEPATH += $$PWD/libs
#DEPENDPATH += $$PWD/libs

#unix:!macx|win32: LIBS += -L$$PWD/libs/ -lyaml-cpp


unix|win32: LIBS += -L$$PWD/libs/ -lyaml-cpp

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
