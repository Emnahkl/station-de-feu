/********************************************************************************
** Form generated from reading UI file 'vehiculespage.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VEHICULESPAGE_H
#define UI_VEHICULESPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VehiculesPage
{
public:
    QWidget *centralwidget;
    QHBoxLayout *rootLayout;
    QFrame *sidebar;
    QVBoxLayout *sidebarLayout;
    QLabel *logoLabel;
    QLabel *brandLabel;
    QSpacerItem *brandSpacer;
    QPushButton *dashboardButton;
    QPushButton *vehiclesButton;
    QSpacerItem *sidebarStretch;
    QVBoxLayout *contentLayout;
    QFrame *topbar;
    QHBoxLayout *topbarLayout;
    QLabel *topTitle;
    QSpacerItem *topSpacer;
    QLineEdit *globalSearch;
    QSpacerItem *topSpacer2;
    QToolButton *notificationsButton;
    QToolButton *calendarButton;
    QLabel *avatarLabel;
    QLabel *profileLabel;
    QStackedWidget *stackedWidget;
    QWidget *dashboardPage;
    QVBoxLayout *dashboardLayout;
    QLabel *dashboardHeading;
    QLabel *dashboardSubtitle;
    QHBoxLayout *summaryCards;
    QFrame *totalCard;
    QVBoxLayout *totalCardLayout;
    QLabel *totalCaption;
    QLabel *totalValue;
    QFrame *availableCard;
    QVBoxLayout *availableCardLayout;
    QLabel *availableCaption;
    QLabel *availableValue;
    QFrame *maintenanceCard;
    QVBoxLayout *maintenanceCardLayout;
    QLabel *maintenanceCaption;
    QLabel *maintenanceValue;
    QPushButton *dashboardVehiclesButton;
    QSpacerItem *dashboardStretch;
    QWidget *vehiclesPage;
    QHBoxLayout *vehiclesLayout;
    QFrame *listCard;
    QVBoxLayout *listLayout;
    QHBoxLayout *listHeading;
    QLabel *listTitle;
    QSpacerItem *headingSpacer;
    QPushButton *addVehicleButton;
    QHBoxLayout *filterLayout;
    QLineEdit *searchEdit;
    QComboBox *typeFilter;
    QComboBox *statusFilter;
    QHBoxLayout *toolsLayout;
    QComboBox *sortCombo;
    QSpacerItem *toolsSpacer;
    QPushButton *exportPdfButton;
    QPushButton *exportCsvButton;
    QPushButton *statisticsButton;
    QPushButton *maintenanceAlertsButton;
    QPushButton *recommendButton;
    QTableWidget *vehicleTable;
    QLabel *storageHint;
    QFrame *formCard;
    QVBoxLayout *formLayout;
    QLabel *formTitle;
    QLabel *formSubtitle;
    QFormLayout *vehicleForm;
    QLabel *plateLabel;
    QLineEdit *plate;
    QLabel *typeLabel;
    QComboBox *type;
    QLabel *modelLabel;
    QLineEdit *model;
    QLabel *statusLabel;
    QComboBox *status;
    QLabel *zoneLabel;
    QLineEdit *zone;
    QLabel *dateLabel;
    QDateEdit *maintenanceDate;
    QLabel *mileageLabel;
    QSpinBox *mileage;
    QLabel *maintenanceMileageLabel;
    QSpinBox *maintenanceMileage;
    QLabel *notesLabel;
    QTextEdit *notes;
    QPushButton *saveVehicle;
    QPushButton *resetVehicle;
    QSpacerItem *formStretch;

    void setupUi(QMainWindow *VehiculesPage)
    {
        if (VehiculesPage->objectName().isEmpty())
            VehiculesPage->setObjectName("VehiculesPage");
        VehiculesPage->resize(1600, 920);
        VehiculesPage->setMinimumSize(QSize(1180, 720));
        centralwidget = new QWidget(VehiculesPage);
        centralwidget->setObjectName("centralwidget");
        rootLayout = new QHBoxLayout(centralwidget);
        rootLayout->setSpacing(0);
        rootLayout->setObjectName("rootLayout");
        rootLayout->setContentsMargins(0, 0, 0, 0);
        sidebar = new QFrame(centralwidget);
        sidebar->setObjectName("sidebar");
        sidebar->setMinimumSize(QSize(220, 0));
        sidebar->setMaximumSize(QSize(220, 16777215));
        sidebarLayout = new QVBoxLayout(sidebar);
        sidebarLayout->setSpacing(9);
        sidebarLayout->setObjectName("sidebarLayout");
        sidebarLayout->setContentsMargins(14, 20, 14, 20);
        logoLabel = new QLabel(sidebar);
        logoLabel->setObjectName("logoLabel");
        logoLabel->setMinimumSize(QSize(132, 132));
        logoLabel->setMaximumSize(QSize(132, 132));
        logoLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/logo.png")));
        logoLabel->setScaledContents(true);
        logoLabel->setAlignment(Qt::AlignCenter);

        sidebarLayout->addWidget(logoLabel, 0, Qt::AlignHCenter);

        brandLabel = new QLabel(sidebar);
        brandLabel->setObjectName("brandLabel");
        brandLabel->setAlignment(Qt::AlignCenter);

        sidebarLayout->addWidget(brandLabel);

        brandSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        sidebarLayout->addItem(brandSpacer);

        dashboardButton = new QPushButton(sidebar);
        dashboardButton->setObjectName("dashboardButton");
        dashboardButton->setCheckable(true);

        sidebarLayout->addWidget(dashboardButton);

        vehiclesButton = new QPushButton(sidebar);
        vehiclesButton->setObjectName("vehiclesButton");
        vehiclesButton->setCheckable(true);
        vehiclesButton->setChecked(true);

        sidebarLayout->addWidget(vehiclesButton);

        sidebarStretch = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        sidebarLayout->addItem(sidebarStretch);


        rootLayout->addWidget(sidebar);

        contentLayout = new QVBoxLayout();
        contentLayout->setSpacing(0);
        contentLayout->setObjectName("contentLayout");
        contentLayout->setContentsMargins(0, 0, 0, 0);
        topbar = new QFrame(centralwidget);
        topbar->setObjectName("topbar");
        topbar->setMinimumSize(QSize(0, 72));
        topbar->setMaximumSize(QSize(16777215, 72));
        topbarLayout = new QHBoxLayout(topbar);
        topbarLayout->setObjectName("topbarLayout");
        topbarLayout->setContentsMargins(24, 10, 24, 10);
        topTitle = new QLabel(topbar);
        topTitle->setObjectName("topTitle");

        topbarLayout->addWidget(topTitle);

        topSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        topbarLayout->addItem(topSpacer);

        globalSearch = new QLineEdit(topbar);
        globalSearch->setObjectName("globalSearch");
        globalSearch->setMinimumSize(QSize(380, 0));

        topbarLayout->addWidget(globalSearch);

        topSpacer2 = new QSpacerItem(22, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        topbarLayout->addItem(topSpacer2);

        notificationsButton = new QToolButton(topbar);
        notificationsButton->setObjectName("notificationsButton");
        notificationsButton->setMinimumSize(QSize(42, 42));
        const QIcon icon = QIcon(QString::fromUtf8(":/icons/bell_badge.svg"));
        notificationsButton->setIcon(icon);
        notificationsButton->setIconSize(QSize(28, 28));
        notificationsButton->setAutoRaise(true);

        topbarLayout->addWidget(notificationsButton);

        calendarButton = new QToolButton(topbar);
        calendarButton->setObjectName("calendarButton");
        calendarButton->setMinimumSize(QSize(42, 42));
        const QIcon icon1 = QIcon(QString::fromUtf8(":/icons/calendar.png"));
        calendarButton->setIcon(icon1);
        calendarButton->setIconSize(QSize(24, 24));
        calendarButton->setAutoRaise(true);

        topbarLayout->addWidget(calendarButton);

        avatarLabel = new QLabel(topbar);
        avatarLabel->setObjectName("avatarLabel");
        avatarLabel->setMinimumSize(QSize(34, 34));
        avatarLabel->setMaximumSize(QSize(34, 34));
        avatarLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/avatar.png")));
        avatarLabel->setScaledContents(true);

        topbarLayout->addWidget(avatarLabel);

        profileLabel = new QLabel(topbar);
        profileLabel->setObjectName("profileLabel");

        topbarLayout->addWidget(profileLabel);


        contentLayout->addWidget(topbar);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        dashboardPage = new QWidget();
        dashboardPage->setObjectName("dashboardPage");
        dashboardLayout = new QVBoxLayout(dashboardPage);
        dashboardLayout->setObjectName("dashboardLayout");
        dashboardLayout->setContentsMargins(24, 24, 24, 24);
        dashboardHeading = new QLabel(dashboardPage);
        dashboardHeading->setObjectName("dashboardHeading");

        dashboardLayout->addWidget(dashboardHeading);

        dashboardSubtitle = new QLabel(dashboardPage);
        dashboardSubtitle->setObjectName("dashboardSubtitle");

        dashboardLayout->addWidget(dashboardSubtitle);

        summaryCards = new QHBoxLayout();
        summaryCards->setObjectName("summaryCards");
        totalCard = new QFrame(dashboardPage);
        totalCard->setObjectName("totalCard");
        totalCardLayout = new QVBoxLayout(totalCard);
        totalCardLayout->setObjectName("totalCardLayout");
        totalCaption = new QLabel(totalCard);
        totalCaption->setObjectName("totalCaption");

        totalCardLayout->addWidget(totalCaption);

        totalValue = new QLabel(totalCard);
        totalValue->setObjectName("totalValue");

        totalCardLayout->addWidget(totalValue);


        summaryCards->addWidget(totalCard);

        availableCard = new QFrame(dashboardPage);
        availableCard->setObjectName("availableCard");
        availableCardLayout = new QVBoxLayout(availableCard);
        availableCardLayout->setObjectName("availableCardLayout");
        availableCaption = new QLabel(availableCard);
        availableCaption->setObjectName("availableCaption");

        availableCardLayout->addWidget(availableCaption);

        availableValue = new QLabel(availableCard);
        availableValue->setObjectName("availableValue");

        availableCardLayout->addWidget(availableValue);


        summaryCards->addWidget(availableCard);

        maintenanceCard = new QFrame(dashboardPage);
        maintenanceCard->setObjectName("maintenanceCard");
        maintenanceCardLayout = new QVBoxLayout(maintenanceCard);
        maintenanceCardLayout->setObjectName("maintenanceCardLayout");
        maintenanceCaption = new QLabel(maintenanceCard);
        maintenanceCaption->setObjectName("maintenanceCaption");

        maintenanceCardLayout->addWidget(maintenanceCaption);

        maintenanceValue = new QLabel(maintenanceCard);
        maintenanceValue->setObjectName("maintenanceValue");

        maintenanceCardLayout->addWidget(maintenanceValue);


        summaryCards->addWidget(maintenanceCard);


        dashboardLayout->addLayout(summaryCards);

        dashboardVehiclesButton = new QPushButton(dashboardPage);
        dashboardVehiclesButton->setObjectName("dashboardVehiclesButton");

        dashboardLayout->addWidget(dashboardVehiclesButton);

        dashboardStretch = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        dashboardLayout->addItem(dashboardStretch);

        stackedWidget->addWidget(dashboardPage);
        vehiclesPage = new QWidget();
        vehiclesPage->setObjectName("vehiclesPage");
        vehiclesLayout = new QHBoxLayout(vehiclesPage);
        vehiclesLayout->setSpacing(16);
        vehiclesLayout->setObjectName("vehiclesLayout");
        vehiclesLayout->setContentsMargins(18, 18, 18, 18);
        listCard = new QFrame(vehiclesPage);
        listCard->setObjectName("listCard");
        listLayout = new QVBoxLayout(listCard);
        listLayout->setSpacing(10);
        listLayout->setObjectName("listLayout");
        listLayout->setContentsMargins(16, 16, 16, 14);
        listHeading = new QHBoxLayout();
        listHeading->setObjectName("listHeading");
        listTitle = new QLabel(listCard);
        listTitle->setObjectName("listTitle");

        listHeading->addWidget(listTitle);

        headingSpacer = new QSpacerItem(30, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        listHeading->addItem(headingSpacer);

        addVehicleButton = new QPushButton(listCard);
        addVehicleButton->setObjectName("addVehicleButton");

        listHeading->addWidget(addVehicleButton);


        listLayout->addLayout(listHeading);

        filterLayout = new QHBoxLayout();
        filterLayout->setObjectName("filterLayout");
        searchEdit = new QLineEdit(listCard);
        searchEdit->setObjectName("searchEdit");

        filterLayout->addWidget(searchEdit);

        typeFilter = new QComboBox(listCard);
        typeFilter->addItem(QString());
        typeFilter->setObjectName("typeFilter");

        filterLayout->addWidget(typeFilter);

        statusFilter = new QComboBox(listCard);
        statusFilter->addItem(QString());
        statusFilter->setObjectName("statusFilter");

        filterLayout->addWidget(statusFilter);


        listLayout->addLayout(filterLayout);

        toolsLayout = new QHBoxLayout();
        toolsLayout->setObjectName("toolsLayout");
        sortCombo = new QComboBox(listCard);
        sortCombo->addItem(QString());
        sortCombo->addItem(QString());
        sortCombo->addItem(QString());
        sortCombo->addItem(QString());
        sortCombo->addItem(QString());
        sortCombo->setObjectName("sortCombo");

        toolsLayout->addWidget(sortCombo);

        toolsSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        toolsLayout->addItem(toolsSpacer);

        exportPdfButton = new QPushButton(listCard);
        exportPdfButton->setObjectName("exportPdfButton");

        toolsLayout->addWidget(exportPdfButton);

        exportCsvButton = new QPushButton(listCard);
        exportCsvButton->setObjectName("exportCsvButton");

        toolsLayout->addWidget(exportCsvButton);

        statisticsButton = new QPushButton(listCard);
        statisticsButton->setObjectName("statisticsButton");

        toolsLayout->addWidget(statisticsButton);

        maintenanceAlertsButton = new QPushButton(listCard);
        maintenanceAlertsButton->setObjectName("maintenanceAlertsButton");

        toolsLayout->addWidget(maintenanceAlertsButton);

        recommendButton = new QPushButton(listCard);
        recommendButton->setObjectName("recommendButton");

        toolsLayout->addWidget(recommendButton);


        listLayout->addLayout(toolsLayout);

        vehicleTable = new QTableWidget(listCard);
        if (vehicleTable->columnCount() < 9)
            vehicleTable->setColumnCount(9);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        vehicleTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        vehicleTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        vehicleTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        vehicleTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        vehicleTable->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        vehicleTable->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        vehicleTable->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        vehicleTable->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        vehicleTable->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        vehicleTable->setObjectName("vehicleTable");
        vehicleTable->setColumnCount(9);
        vehicleTable->setRowCount(0);

        listLayout->addWidget(vehicleTable);

        storageHint = new QLabel(listCard);
        storageHint->setObjectName("storageHint");

        listLayout->addWidget(storageHint);


        vehiclesLayout->addWidget(listCard);

        formCard = new QFrame(vehiclesPage);
        formCard->setObjectName("formCard");
        formCard->setMinimumSize(QSize(330, 0));
        formCard->setMaximumSize(QSize(390, 16777215));
        formLayout = new QVBoxLayout(formCard);
        formLayout->setSpacing(9);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(18, 16, 18, 16);
        formTitle = new QLabel(formCard);
        formTitle->setObjectName("formTitle");

        formLayout->addWidget(formTitle);

        formSubtitle = new QLabel(formCard);
        formSubtitle->setObjectName("formSubtitle");

        formLayout->addWidget(formSubtitle);

        vehicleForm = new QFormLayout();
        vehicleForm->setObjectName("vehicleForm");
        vehicleForm->setVerticalSpacing(7);
        plateLabel = new QLabel(formCard);
        plateLabel->setObjectName("plateLabel");

        vehicleForm->setWidget(0, QFormLayout::ItemRole::LabelRole, plateLabel);

        plate = new QLineEdit(formCard);
        plate->setObjectName("plate");

        vehicleForm->setWidget(0, QFormLayout::ItemRole::FieldRole, plate);

        typeLabel = new QLabel(formCard);
        typeLabel->setObjectName("typeLabel");

        vehicleForm->setWidget(1, QFormLayout::ItemRole::LabelRole, typeLabel);

        type = new QComboBox(formCard);
        type->setObjectName("type");

        vehicleForm->setWidget(1, QFormLayout::ItemRole::FieldRole, type);

        modelLabel = new QLabel(formCard);
        modelLabel->setObjectName("modelLabel");

        vehicleForm->setWidget(2, QFormLayout::ItemRole::LabelRole, modelLabel);

        model = new QLineEdit(formCard);
        model->setObjectName("model");

        vehicleForm->setWidget(2, QFormLayout::ItemRole::FieldRole, model);

        statusLabel = new QLabel(formCard);
        statusLabel->setObjectName("statusLabel");

        vehicleForm->setWidget(3, QFormLayout::ItemRole::LabelRole, statusLabel);

        status = new QComboBox(formCard);
        status->setObjectName("status");

        vehicleForm->setWidget(3, QFormLayout::ItemRole::FieldRole, status);

        zoneLabel = new QLabel(formCard);
        zoneLabel->setObjectName("zoneLabel");

        vehicleForm->setWidget(4, QFormLayout::ItemRole::LabelRole, zoneLabel);

        zone = new QLineEdit(formCard);
        zone->setObjectName("zone");

        vehicleForm->setWidget(4, QFormLayout::ItemRole::FieldRole, zone);

        dateLabel = new QLabel(formCard);
        dateLabel->setObjectName("dateLabel");

        vehicleForm->setWidget(5, QFormLayout::ItemRole::LabelRole, dateLabel);

        maintenanceDate = new QDateEdit(formCard);
        maintenanceDate->setObjectName("maintenanceDate");
        maintenanceDate->setCalendarPopup(true);

        vehicleForm->setWidget(5, QFormLayout::ItemRole::FieldRole, maintenanceDate);

        mileageLabel = new QLabel(formCard);
        mileageLabel->setObjectName("mileageLabel");

        vehicleForm->setWidget(6, QFormLayout::ItemRole::LabelRole, mileageLabel);

        mileage = new QSpinBox(formCard);
        mileage->setObjectName("mileage");
        mileage->setMaximum(2000000);

        vehicleForm->setWidget(6, QFormLayout::ItemRole::FieldRole, mileage);

        maintenanceMileageLabel = new QLabel(formCard);
        maintenanceMileageLabel->setObjectName("maintenanceMileageLabel");

        vehicleForm->setWidget(7, QFormLayout::ItemRole::LabelRole, maintenanceMileageLabel);

        maintenanceMileage = new QSpinBox(formCard);
        maintenanceMileage->setObjectName("maintenanceMileage");
        maintenanceMileage->setMaximum(2000000);

        vehicleForm->setWidget(7, QFormLayout::ItemRole::FieldRole, maintenanceMileage);

        notesLabel = new QLabel(formCard);
        notesLabel->setObjectName("notesLabel");

        vehicleForm->setWidget(8, QFormLayout::ItemRole::LabelRole, notesLabel);

        notes = new QTextEdit(formCard);
        notes->setObjectName("notes");
        notes->setMaximumSize(QSize(16777215, 62));

        vehicleForm->setWidget(8, QFormLayout::ItemRole::FieldRole, notes);


        formLayout->addLayout(vehicleForm);

        saveVehicle = new QPushButton(formCard);
        saveVehicle->setObjectName("saveVehicle");

        formLayout->addWidget(saveVehicle);

        resetVehicle = new QPushButton(formCard);
        resetVehicle->setObjectName("resetVehicle");

        formLayout->addWidget(resetVehicle);

        formStretch = new QSpacerItem(20, 30, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        formLayout->addItem(formStretch);


        vehiclesLayout->addWidget(formCard);

        stackedWidget->addWidget(vehiclesPage);

        contentLayout->addWidget(stackedWidget);


        rootLayout->addLayout(contentLayout);

        VehiculesPage->setCentralWidget(centralwidget);

        retranslateUi(VehiculesPage);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(VehiculesPage);
    } // setupUi

    void retranslateUi(QMainWindow *VehiculesPage)
    {
        VehiculesPage->setWindowTitle(QCoreApplication::translate("VehiculesPage", "Fire Station \342\200\224 Gestion des v\303\251hicules", nullptr));
        brandLabel->setText(QCoreApplication::translate("VehiculesPage", "FIRE STATION", nullptr));
        dashboardButton->setText(QCoreApplication::translate("VehiculesPage", "\342\214\202   Tableau de bord", nullptr));
        vehiclesButton->setText(QCoreApplication::translate("VehiculesPage", "\360\237\232\222   V\303\251hicules", nullptr));
        topTitle->setText(QCoreApplication::translate("VehiculesPage", "FIRE STATION\n"
"Gestion des v\303\251hicules", nullptr));
        globalSearch->setPlaceholderText(QCoreApplication::translate("VehiculesPage", "Rechercher une immatriculation, un mod\303\250le ou un type\342\200\246", nullptr));
#if QT_CONFIG(tooltip)
        notificationsButton->setToolTip(QCoreApplication::translate("VehiculesPage", "Alertes de maintenance", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        calendarButton->setToolTip(QCoreApplication::translate("VehiculesPage", "Voir les v\303\251hicules \303\240 entretenir", nullptr));
#endif // QT_CONFIG(tooltip)
        profileLabel->setText(QCoreApplication::translate("VehiculesPage", "Chef d\342\200\231\303\251quipe", nullptr));
        dashboardHeading->setText(QCoreApplication::translate("VehiculesPage", "Tableau de bord", nullptr));
        dashboardSubtitle->setText(QCoreApplication::translate("VehiculesPage", "Vue d\342\200\231ensemble de la flotte de v\303\251hicules", nullptr));
        totalCaption->setText(QCoreApplication::translate("VehiculesPage", "V\303\251hicules enregistr\303\251s", nullptr));
        totalValue->setText(QCoreApplication::translate("VehiculesPage", "0", nullptr));
        availableCaption->setText(QCoreApplication::translate("VehiculesPage", "Disponibles", nullptr));
        availableValue->setText(QCoreApplication::translate("VehiculesPage", "0", nullptr));
        maintenanceCaption->setText(QCoreApplication::translate("VehiculesPage", "\303\200 contr\303\264ler", nullptr));
        maintenanceValue->setText(QCoreApplication::translate("VehiculesPage", "0", nullptr));
        dashboardVehiclesButton->setText(QCoreApplication::translate("VehiculesPage", "Ouvrir la gestion des v\303\251hicules", nullptr));
        listTitle->setText(QCoreApplication::translate("VehiculesPage", "Liste des v\303\251hicules", nullptr));
        addVehicleButton->setText(QCoreApplication::translate("VehiculesPage", "\357\274\213  Ajouter un v\303\251hicule", nullptr));
        searchEdit->setPlaceholderText(QCoreApplication::translate("VehiculesPage", "Rechercher immatriculation, mod\303\250le\342\200\246", nullptr));
        typeFilter->setItemText(0, QCoreApplication::translate("VehiculesPage", "Tous les types", nullptr));

        statusFilter->setItemText(0, QCoreApplication::translate("VehiculesPage", "Tous les \303\251tats", nullptr));

        sortCombo->setItemText(0, QCoreApplication::translate("VehiculesPage", "Trier : date d\342\200\231ajout", nullptr));
        sortCombo->setItemText(1, QCoreApplication::translate("VehiculesPage", "Kilom\303\251trage croissant", nullptr));
        sortCombo->setItemText(2, QCoreApplication::translate("VehiculesPage", "Kilom\303\251trage d\303\251croissant", nullptr));
        sortCombo->setItemText(3, QCoreApplication::translate("VehiculesPage", "\303\211tat", nullptr));
        sortCombo->setItemText(4, QCoreApplication::translate("VehiculesPage", "Immatriculation", nullptr));

        exportPdfButton->setText(QCoreApplication::translate("VehiculesPage", "Exporter PDF", nullptr));
        exportCsvButton->setText(QCoreApplication::translate("VehiculesPage", "Exporter Excel / CSV", nullptr));
        statisticsButton->setText(QCoreApplication::translate("VehiculesPage", "Statistiques", nullptr));
        maintenanceAlertsButton->setText(QCoreApplication::translate("VehiculesPage", "Alertes maintenance", nullptr));
        recommendButton->setText(QCoreApplication::translate("VehiculesPage", "Recommander", nullptr));
        QTableWidgetItem *___qtablewidgetitem = vehicleTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("VehiculesPage", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = vehicleTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("VehiculesPage", "Immatriculation", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = vehicleTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("VehiculesPage", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = vehicleTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("VehiculesPage", "Mod\303\250le", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = vehicleTable->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("VehiculesPage", "\303\211tat", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = vehicleTable->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("VehiculesPage", "ID zone", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = vehicleTable->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("VehiculesPage", "Kilom\303\251trage", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = vehicleTable->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("VehiculesPage", "Derni\303\250re maintenance", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = vehicleTable->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("VehiculesPage", "Actions", nullptr));
        storageHint->setText(QCoreApplication::translate("VehiculesPage", "Les donn\303\251es sont sauvegard\303\251es automatiquement sur cet ordinateur.", nullptr));
        formTitle->setText(QCoreApplication::translate("VehiculesPage", "Ajouter / Modifier un v\303\251hicule", nullptr));
        formSubtitle->setText(QCoreApplication::translate("VehiculesPage", "Informations du v\303\251hicule", nullptr));
        plateLabel->setText(QCoreApplication::translate("VehiculesPage", "Immatriculation *", nullptr));
        plate->setPlaceholderText(QCoreApplication::translate("VehiculesPage", "Ex. FT-009", nullptr));
        typeLabel->setText(QCoreApplication::translate("VehiculesPage", "Type *", nullptr));
        modelLabel->setText(QCoreApplication::translate("VehiculesPage", "Mod\303\250le *", nullptr));
        model->setPlaceholderText(QCoreApplication::translate("VehiculesPage", "Marque et mod\303\250le", nullptr));
        statusLabel->setText(QCoreApplication::translate("VehiculesPage", "\303\211tat *", nullptr));
        zoneLabel->setText(QCoreApplication::translate("VehiculesPage", "ID zone", nullptr));
        zone->setPlaceholderText(QCoreApplication::translate("VehiculesPage", "Ex. Z-01", nullptr));
        dateLabel->setText(QCoreApplication::translate("VehiculesPage", "Derni\303\250re maintenance", nullptr));
        maintenanceDate->setDisplayFormat(QCoreApplication::translate("VehiculesPage", "dd/MM/yyyy", nullptr));
        mileageLabel->setText(QCoreApplication::translate("VehiculesPage", "Kilom\303\251trage", nullptr));
        mileage->setSuffix(QCoreApplication::translate("VehiculesPage", " km", nullptr));
        maintenanceMileageLabel->setText(QCoreApplication::translate("VehiculesPage", "Km \303\240 l\342\200\231entretien", nullptr));
        maintenanceMileage->setSuffix(QCoreApplication::translate("VehiculesPage", " km", nullptr));
        notesLabel->setText(QCoreApplication::translate("VehiculesPage", "Notes", nullptr));
        notes->setPlaceholderText(QCoreApplication::translate("VehiculesPage", "Ajouter une note\342\200\246", nullptr));
        saveVehicle->setText(QCoreApplication::translate("VehiculesPage", "Enregistrer le v\303\251hicule", nullptr));
        resetVehicle->setText(QCoreApplication::translate("VehiculesPage", "R\303\251initialiser", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VehiculesPage: public Ui_VehiculesPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VEHICULESPAGE_H
