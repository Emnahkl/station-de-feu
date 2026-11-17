QT += core gui widgets sql charts printsupport multimedia
CONFIG += c++17
msvc: QMAKE_CXXFLAGS += /utf-8
TARGET = FireStationManager
TEMPLATE = app

SOURCES += \
    main.cpp \
    logindialog.cpp \
    mainwindow.cpp \
    menupage.cpp \
    rightpanel.cpp \
    employespage.cpp \
    employe.cpp \
    vehicule.cpp \
    gestioninterventions.cpp \
    intervention.cpp \
    affectation.cpp \
    graphiques.cpp \
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
    employe.h \
    vehicule.h \
    qrgen.h \
    gestioninterventions.h \
    intervention.h \
    affectation.h \
    graphiques.h \
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
    campagnespage.ui \
    gestioninterventions.ui

RESOURCES += resources.qrc

DISTFILES += sql/oracle11g_schema.sql README.md
