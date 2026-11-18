QT += widgets printsupport sql
QT += multimedia

CONFIG += c++17
TARGET = gestionvehicule

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    integration/integratedwindow.cpp \
    integration/emp_interv/employeswindow.cpp \
    integration/emp_interv/employe.cpp \
    integration/emp_interv/gestioninterventions.cpp \
    integration/emp_interv/intervention.cpp \
    integration/emp_interv/affectation.cpp \
    integration/emp_interv/graphiques.cpp \
    integ1/connexion.cpp \
    integ1/vehicule.cpp

HEADERS += \
    mainwindow.h \
    integration/integratedwindow.h \
    integration/emp_interv/employeswindow.h \
    integration/emp_interv/employe.h \
    integration/emp_interv/gestioninterventions.h \
    integration/emp_interv/intervention.h \
    integration/emp_interv/affectation.h \
    integration/emp_interv/graphiques.h \
    integ1/connexion.h \
    integ1/vehicule.h

FORMS += \
    vehiclewindow.ui \
    integration/emp_interv/employeswindow.ui \
    integration/emp_interv/gestioninterventions.ui

RESOURCES += \
    resources.qrc \
    integration/emp_interv/emp_interv.qrc
