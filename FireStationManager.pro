QT       += core gui widgets sql
CONFIG   += c++17
TARGET    = FireStationManager
TEMPLATE  = app
INCLUDEPATH += $$PWD $$PWD/src

SOURCES += \
    src/main.cpp \
    src/core/Zone.cpp \
    src/core/ZoneRepository.cpp \
    src/core/ZoneTableModel.cpp \
    src/core/ZoneFilterProxy.cpp \
    src/core/CoverageAnalyzer.cpp \
    src/core/Exporter.cpp \
    src/widgets/ZoneMapWidget.cpp \
    src/widgets/StatsChartWidget.cpp \
    src/widgets/KpiCard.cpp \
    src/widgets/AlertPanel.cpp \
    src/widgets/ZoneDetailPanel.cpp \
    src/widgets/LogoWidget.cpp \
    src/widgets/SidebarBackground.cpp \
    src/ui/Theme.cpp \
    src/ui/ZoneDialog.cpp \
    src/ui/ZoneManagementPage.cpp \
    src/ui/MainWindow.cpp \
    connection.cpp \
    campagne.cpp \
    barchartwidget.cpp \
    campagnewindow.cpp

HEADERS += \
    src/core/Zone.h \
    src/core/ZoneRepository.h \
    src/core/ZoneTableModel.h \
    src/core/ZoneFilterProxy.h \
    src/core/CoverageAnalyzer.h \
    src/core/Exporter.h \
    src/widgets/ZoneMapWidget.h \
    src/widgets/StatsChartWidget.h \
    src/widgets/KpiCard.h \
    src/widgets/AlertPanel.h \
    src/widgets/ZoneDetailPanel.h \
    src/widgets/LogoWidget.h \
    src/widgets/SidebarBackground.h \
    src/ui/Theme.h \
    src/ui/ZoneDialog.h \
    src/ui/ZoneManagementPage.h \
    src/ui/MainWindow.h \
    connection.h \
    campagne.h \
    barchartwidget.h \
    campagnewindow.h

FORMS += \
    src/ui/ZoneDialog.ui \
    campagnewindow.ui

RESOURCES += \
    resources/resources.qrc \
    campagne_resources.qrc
