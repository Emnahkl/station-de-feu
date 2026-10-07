QT += core gui widgets
CONFIG += c++17
TARGET = integration_employe_inter
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    employespage.cpp \
    interventionspage.cpp \
    rightpanel.cpp

HEADERS += \
    mainwindow.h \
    employespage.h \
    interventionspage.h \
    rightpanel.h \
    flowlayout.h

RESOURCES += resources.qrc
