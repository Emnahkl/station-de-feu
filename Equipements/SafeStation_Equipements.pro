QT       += core gui widgets sql charts
CONFIG   += c++17
TARGET    = SafeStation_Equipements
TEMPLATE  = app

SOURCES += main.cpp \
           connexion.cpp \
           equipement.cpp \
           exports.cpp \
           mainwindow.cpp

HEADERS += connexion.h \
           equipement.h \
           exports.h \
           mainwindow.h

FORMS   += mainwindow.ui
RESOURCES += resources.qrc

