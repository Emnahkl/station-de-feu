QT += widgets printsupport sql

CONFIG += c++17
TARGET = gestionvehicule

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    integ1/connexion.cpp \
    integ1/vehicule.cpp

HEADERS += \
    mainwindow.h \
    integ1/connexion.h \
    integ1/vehicule.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc
