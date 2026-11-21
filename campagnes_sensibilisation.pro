QT += core gui widgets sql charts
CONFIG += c++17
TARGET = campagnes_sensibilisation
TEMPLATE = app

SOURCES += main.cpp campagne.cpp campagneswindow.cpp campagnemetiers.cpp \
           gifwriter.cpp connection.cpp menulateral.cpp
HEADERS += campagne.h campagneswindow.h campagnemetiers.h gifwriter.h connection.h menulateral.h
RESOURCES += resources.qrc
