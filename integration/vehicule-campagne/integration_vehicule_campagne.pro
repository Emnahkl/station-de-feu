QT += widgets sql printsupport
CONFIG += c++17
TEMPLATE = app
TARGET = integration_vehicule_campagne
SOURCES += main.cpp mainwindow.cpp vehiclewindow.cpp campaignwindow.cpp connection.cpp campagne.cpp barchartwidget.cpp
HEADERS += mainwindow.h vehiclewindow.h campaignwindow.h connection.h campagne.h barchartwidget.h
FORMS += vehiclewindow.ui campaignwindow.ui
RESOURCES += vehicle_resources.qrc campaign_resources.qrc



