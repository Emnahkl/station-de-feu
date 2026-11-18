QT += core gui widgets network sql printsupport charts multimedia
CONFIG += c++17
TARGET = fire
msvc: QMAKE_CXXFLAGS += /utf-8
SOURCES += main.cpp mainwindow.cpp zone.cpp equipe.cpp alerte.cpp connection.cpp equipement.cpp equipementswindow.cpp exports.cpp
HEADERS += mainwindow.h zone.h equipe.h alerte.h connection.h equipement.h equipementswindow.h exports.h
RESOURCES += resources.qrc resources_equipment.qrc
FORMS += equipements.ui


