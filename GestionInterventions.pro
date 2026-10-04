QT += core gui widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

SOURCES += main.cpp \
           gestioninterventions.cpp

HEADERS += gestioninterventions.h

FORMS += gestioninterventions.ui

RESOURCES += interventions.qrc
