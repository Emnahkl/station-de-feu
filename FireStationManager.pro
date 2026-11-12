QT += core gui sql widgets charts

CONFIG += c++17

TARGET = FireStationManager
TEMPLATE = app

SOURCES += \
    main.cpp \
    connection.cpp \
    campagne.cpp \
    mainwindow.cpp

HEADERS += \
    connection.h \
    campagne.h \
    mainwindow.h
