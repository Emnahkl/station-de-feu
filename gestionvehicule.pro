QT += widgets printsupport sql charts

CONFIG += c++17
TARGET = SafeStation_Integration

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    equipement.cpp \
    equipementswindow.cpp \
    exports.cpp \
    integ1/connexion.cpp \
    integ1/vehicule.cpp

HEADERS += \
    mainwindow.h \
    equipement.h \
    equipementswindow.h \
    exports.h \
    integ1/connexion.h \
    integ1/vehicule.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc \
    resources_equipment.qrc

FORMS += equipements.ui
