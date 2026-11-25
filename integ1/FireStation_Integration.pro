QT += widgets printsupport sql charts
CONFIG += c++17
TEMPLATE = app
TARGET = FireStation_Integration

SOURCES += \
    main.cpp \
    integratedwindow.cpp \
    vehiclewindow.cpp \
    equipmentwindow.cpp \
    gestioninterventions.cpp \
    connexion.cpp \
    vehicule.cpp \
    equipement.cpp \
    equipementmodel.cpp \
    exports.cpp

HEADERS += \
    integratedwindow.h \
    vehiclewindow.h \
    equipmentwindow.h \
    gestioninterventions.h \
    connexion.h \
    vehicule.h \
    equipement.h \
    equipementmodel.h \
    exports.h

FORMS += \
    vehiclewindow.ui \
    equipmentwindow.ui \
    gestioninterventions.ui

RESOURCES += \
    vehicle_resources.qrc \
    station_resources.qrc \
    interventions.qrc


