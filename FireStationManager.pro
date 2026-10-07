QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
TARGET = FireStationManager
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    connection.cpp \
    campagne.cpp \
    barchartwidget.cpp

HEADERS += \
    mainwindow.h \
    connection.h \
    campagne.h \
    barchartwidget.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc
