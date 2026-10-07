QT += core gui widgets sql charts printsupport
CONFIG += c++17
TARGET = FireStationManager
TEMPLATE = app

SOURCES += \
    main.cpp \
    logindialog.cpp \
    mainwindow.cpp \
    rightpanel.cpp \
    employespage.cpp \
    interventionspage.cpp \
    vehiculespage.cpp \
    equipementspage.cpp \
    equipement.cpp \
    equipementmodel.cpp \
    exports.cpp \
    campagnespage.cpp \
    campagne.cpp \
    barchartwidget.cpp \
    connection.cpp

HEADERS += \
    logindialog.h \
    mainwindow.h \
    rightpanel.h \
    flowlayout.h \
    employespage.h \
    interventionspage.h \
    vehiculespage.h \
    equipementspage.h \
    equipement.h \
    equipementmodel.h \
    exports.h \
    campagnespage.h \
    campagne.h \
    barchartwidget.h \
    connection.h

FORMS += \
    vehiculespage.ui \
    equipementspage.ui \
    campagnespage.ui

RESOURCES += resources.qrc
