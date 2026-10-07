QT += core gui widgets sql charts printsupport
CONFIG += c++17
TARGET = FireStationManager
TEMPLATE = app

SOURCES += \
    main.cpp \
    logindialog.cpp \
    mainwindow.cpp \
    menupage.cpp \
    rightpanel.cpp \
    employespage.cpp \
    interventionspage.cpp \
    vehiculespage.cpp \
    equipementspage.cpp \
    equipement.cpp \
    equipementmodel.cpp \
    exports.cpp \
    zonespage.cpp \
    zone.cpp \
    cartezoneswidget.cpp \
    campagnespage.cpp \
    campagne.cpp \
    barchartwidget.cpp \
    connection.cpp

HEADERS += \
    logindialog.h \
    mainwindow.h \
    menupage.h \
    rightpanel.h \
    flowlayout.h \
    employespage.h \
    qrgen.h \
    interventionspage.h \
    vehiculespage.h \
    equipementspage.h \
    equipement.h \
    equipementmodel.h \
    exports.h \
    zonespage.h \
    zone.h \
    cartezoneswidget.h \
    campagnespage.h \
    campagne.h \
    barchartwidget.h \
    connection.h

FORMS += \
    employespage.ui \
    vehiculespage.ui \
    equipementspage.ui \
    zonespage.ui \
    campagnespage.ui

RESOURCES += resources.qrc
