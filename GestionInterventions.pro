QT += core gui widgets sql
CONFIG += console

TARGET = GestionInterventions
TEMPLATE = app

# Les fichiers sources sont en UTF-8 (accents dans les textes)
msvc: QMAKE_CXXFLAGS += /utf-8

SOURCES += \
    main.cpp \
    gestioninterventions.cpp \
    connection.cpp \
    intervention.cpp \
    affectation.cpp \
    graphiques.cpp

HEADERS += \
    gestioninterventions.h \
    connection.h \
    intervention.h \
    affectation.h \
    graphiques.h

FORMS += gestioninterventions.ui

RESOURCES += resources.qrc
