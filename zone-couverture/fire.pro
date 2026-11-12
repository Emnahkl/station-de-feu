QT += core gui widgets network sql printsupport
CONFIG += c++17
TARGET = fire
msvc: QMAKE_CXXFLAGS += /utf-8
SOURCES += main.cpp mainwindow.cpp zone.cpp equipe.cpp alerte.cpp connection.cpp
HEADERS += mainwindow.h zone.h equipe.h alerte.h connection.h
RESOURCES += resources.qrc
