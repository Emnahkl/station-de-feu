QT       += core gui widgets sql charts
CONFIG   += c++11
TARGET    = SafeStation_Equipements
TEMPLATE  = app

SOURCES += main.cpp \
           connexion.cpp \
           equipement.cpp \
           equipementmodel.cpp \
           exports.cpp \
           mainwindow.cpp

HEADERS += connexion.h \
           equipement.h \
           equipementmodel.h \
           exports.h \
           mainwindow.h

FORMS   += mainwindow.ui
RESOURCES += resources.qrc
