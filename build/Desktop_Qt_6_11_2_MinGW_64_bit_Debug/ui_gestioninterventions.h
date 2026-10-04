/********************************************************************************
** Form generated from reading UI file 'gestioninterventions.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GESTIONINTERVENTIONS_H
#define UI_GESTIONINTERVENTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GestionInterventions
{
public:
    QHBoxLayout *rootLayout;
    QFrame *sidebar;
    QVBoxLayout *sidebarLayout;
    QLabel *logoLabel;
    QPushButton *btnHome;
    QPushButton *btnEmployes;
    QPushButton *btnVehicules;
    QPushButton *btnEquipements;
    QPushButton *btnInterventions;
    QPushButton *btnZones;
    QPushButton *btnSensibilisation;
    QPushButton *btnParametres;
    QSpacerItem *sidebarSpacer;
    QFrame *centerFrame;
    QVBoxLayout *centerLayout;
    QFrame *topBar;
    QHBoxLayout *topBarLayout;
    QPushButton *btnMenu;
    QLineEdit *searchEdit;
    QSpacerItem *topSpacer;
    QPushButton *btnNotif;
    QPushButton *btnSettings;
    QLabel *avatarLabel;
    QLabel *labelAdmin;
    QPushButton *btnPower;
    QFrame *content;
    QVBoxLayout *contentLayout;
    QLabel *labelTitle;
    QFrame *filtersFrame;
    QHBoxLayout *filtersLayout;
    QLabel *lblFiltreGravite;
    QComboBox *comboGravite;
    QLabel *lblFiltreStatut;
    QComboBox *comboStatut;
    QSpacerItem *filtersSpacer;
    QHBoxLayout *cardsLayout;
    QFrame *cardEnCours;
    QVBoxLayout *cardEnCoursLayout;
    QLabel *lblEnCoursTitle;
    QLabel *lblEnCours;
    QFrame *cardEnAttente;
    QVBoxLayout *cardEnAttenteLayout;
    QLabel *lblEnAttenteTitle;
    QLabel *lblEnAttente;
    QFrame *cardTerminee;
    QVBoxLayout *cardTermineeLayout;
    QLabel *lblTermineeTitle;
    QLabel *lblTerminee;
    QFrame *actionsFrame;
    QVBoxLayout *actionsLayout;
    QHBoxLayout *actionsRow1;
    QPushButton *btnAjouter;
    QPushButton *btnModifier;
    QPushButton *btnSupprimer;
    QPushButton *btnConsulter;
    QSpacerItem *row1Spacer;
    QHBoxLayout *actionsRow2;
    QPushButton *btnExportPdf;
    QPushButton *btnTriDate;
    QPushButton *btnTriGravite;
    QPushButton *btnStats;
    QPushButton *btnAffectation;
    QPushButton *btnParcours;
    QSpacerItem *row2Spacer;
    QFrame *tableFrame;
    QVBoxLayout *tableFrameLayout;
    QTableWidget *tableInterventions;

    void setupUi(QWidget *GestionInterventions)
    {
        if (GestionInterventions->objectName().isEmpty())
            GestionInterventions->setObjectName("GestionInterventions");
        GestionInterventions->resize(1800, 900);
        GestionInterventions->setStyleSheet(QString::fromUtf8("* { font-family: \"Segoe UI\", \"Helvetica Neue\", Arial; font-size: 13px; color: #1f2937; }\n"
"QWidget#GestionInterventions { background: #eef0f4; }\n"
"QFrame#centerFrame, QFrame#content { background: #eef0f4; border: none; }\n"
"QLabel { background: transparent; }\n"
"\n"
"/* ---- Sidebar (image + voile rouge) ---- */\n"
"QFrame#sidebar { border-image: url(:/images/sidebar_bg.png) 0 0 0 0 stretch stretch; }\n"
"QFrame#sidebar QLabel { background: transparent; border: none; }\n"
"QFrame#sidebar QPushButton {\n"
"    background: transparent; color: #ffffff; border: none; border-left: 4px solid transparent;\n"
"    text-align: left; padding: 12px 18px 12px 22px; font-size: 14px; min-height: 22px;\n"
"}\n"
"QFrame#sidebar QPushButton:hover   { background: rgba(0,0,0,0.22); }\n"
"QFrame#sidebar QPushButton:checked { background: rgba(0,0,0,0.58); font-weight: bold; border-left: 4px solid #ffffff; }\n"
"\n"
"/* ---- Barre du haut ---- */\n"
"QFrame#topBar { background: #eef0f4; border: none; }\n"
"QLineEdit#searc"
                        "hEdit {\n"
"    background: #e2e5ea url(:/icons/search.png) no-repeat left center;\n"
"    border: 1px solid #d5d9df; border-radius: 6px; padding: 8px 12px 8px 34px; min-width: 360px; max-width: 360px;\n"
"}\n"
"QPushButton#btnMenu, QPushButton#btnNotif, QPushButton#btnSettings, QPushButton#btnPower {\n"
"    background: transparent; border: none; padding: 6px; border-radius: 6px;\n"
"}\n"
"QPushButton#btnMenu:hover, QPushButton#btnNotif:hover, QPushButton#btnSettings:hover, QPushButton#btnPower:hover { background: #dfe3e8; }\n"
"QLabel#labelAdmin { font-weight: bold; font-size: 12px; color: #1f2937; }\n"
"\n"
"QLabel#labelTitle { font-size: 20px; font-weight: bold; color: #1f2937; padding: 4px 2px; }\n"
"\n"
"/* ---- Filtres ---- */\n"
"QFrame#filtersFrame { background: white; border: 1px solid #e3e6ea; border-radius: 6px; }\n"
"QFrame#filtersFrame QLabel { border: none; color: #4b5563; }\n"
"QComboBox { background: white; border: 1px solid #cfd4da; border-radius: 4px; padding: 6px 10px; min-width: 190px; }\n"
""
                        "QComboBox::drop-down { border: none; width: 22px; }\n"
"QComboBox QAbstractItemView { background: white; selection-background-color: #fde2e2; selection-color: #1f2937; border: 1px solid #cfd4da; }\n"
"\n"
"/* ---- Cartes ---- */\n"
"QFrame#cardEnCours, QFrame#cardEnAttente, QFrame#cardTerminee {\n"
"    background: white; border: 1px solid #e3e6ea; border-radius: 8px; min-width: 190px;\n"
"}\n"
"QFrame#cardEnCours   { border-left: 5px solid #f59e0b; }\n"
"QFrame#cardEnAttente { border-left: 5px solid #ef4444; }\n"
"QFrame#cardTerminee  { border-left: 5px solid #22c55e; }\n"
"QFrame#cardEnCours QLabel, QFrame#cardEnAttente QLabel, QFrame#cardTerminee QLabel { border: none; }\n"
"QLabel#lblEnCoursTitle   { color: #b45309; font-weight: bold; }\n"
"QLabel#lblEnAttenteTitle { color: #b91c1c; font-weight: bold; }\n"
"QLabel#lblTermineeTitle  { color: #15803d; font-weight: bold; }\n"
"QLabel#lblEnCours, QLabel#lblEnAttente, QLabel#lblTerminee { font-size: 26px; font-weight: bold; }\n"
"\n"
"/* ---- Actions ---- */\n"
""
                        "QFrame#actionsFrame { background: white; border: 1px solid #e3e6ea; border-radius: 6px; }\n"
"QFrame#actionsFrame QPushButton {\n"
"    background: #d9dce1; color: #374151; border: none; border-radius: 6px;\n"
"    padding: 9px 16px; font-weight: 600; text-align: left;\n"
"}\n"
"QFrame#actionsFrame QPushButton:hover { background: #cdd1d7; }\n"
"QFrame#actionsFrame QPushButton:pressed { background: #b9bec6; }\n"
"QFrame#actionsFrame QPushButton#btnAjouter, QFrame#actionsFrame QPushButton#btnSupprimer { background: #1f2d3d; color: white; }\n"
"QFrame#actionsFrame QPushButton#btnAjouter:hover, QFrame#actionsFrame QPushButton#btnSupprimer:hover { background: #2d4157; }\n"
"\n"
"/* ---- Tableau ---- */\n"
"QFrame#tableFrame { background: white; border: 1px solid #e3e6ea; border-radius: 4px; }\n"
"QTableWidget { background: white; alternate-background-color: #f5f6f8; border: none; selection-background-color: #fde2e2; selection-color: #1f2937; }\n"
"QTableWidget::item { padding: 6px; border-bottom: 1px solid #e8eaee;"
                        " }\n"
"QHeaderView { background: #1f2d3d; }\n"
"QHeaderView::section { background: #1f2d3d; color: white; font-weight: bold; padding: 12px 8px; border: none; border-right: 1px solid #2f4054; }\n"
"QTableCornerButton::section { background: #1f2d3d; border: none; }\n"
"QScrollBar:vertical { background: transparent; width: 10px; }\n"
"QScrollBar::handle:vertical { background: #c3c8d0; border-radius: 5px; min-height: 30px; }\n"
"QScrollBar::add-line, QScrollBar::sub-line { height: 0; width: 0; }\n"
"\n"
"/* ---- Panneau de droite ---- */\n"
"QFrame#rightPanel { background: #eef0f4; border: none; }\n"
"QFrame#panelChat, QFrame#panelDispatch, QFrame#panelComm { background: white; border: 1px solid #e3e6ea; border-radius: 6px; }\n"
"QFrame#rightPanel QLabel { border: none; }\n"
"QLabel#panelChatTitle, QLabel#panelDispatchTitle, QLabel#panelCommTitle { font-weight: bold; font-size: 14px; }\n"
"QTextEdit#chatView { background: white; border: none; min-height: 110px; }\n"
"QFrame#rightPanel QLineEdit { background: white"
                        "; border: 1px solid #d5d9df; border-radius: 4px; padding: 8px; }\n"
"QFrame#rightPanel QPushButton { background: #1f2d3d; color: white; border: none; border-radius: 5px; padding: 9px 16px; font-weight: bold; }\n"
"QFrame#rightPanel QPushButton:hover { background: #2d4157; }\n"
"QLabel#lblUniteChef, QLabel#lblDispo, QLabel#lblProgress, QLabel#lblEmail, QLabel#lblSms { color: #4b5563; font-size: 12px; }\n"
"QLabel#lblJour, QLabel#lblNuit, QLabel#lblWeekend { color: #4b5563; font-size: 12px; }\n"
"\n"
"QProgressBar { background: #e5e7eb; border: none; border-radius: 5px; min-height: 10px; max-height: 10px; }\n"
"QProgressBar::chunk { background: #22a559; border-radius: 5px; }\n"
"QProgressBar#barWeekend::chunk, QProgressBar#barProgress::chunk { background: #d32f2f; }\n"
"\n"
"QPushButton#btnNotifier { background: #dbeafe; color: #1d4ed8; font-weight: 600; padding: 9px 6px; }\n"
"QPushButton#btnAlerte   { background: #fee2e2; color: #dc2626; font-weight: 600; padding: 9px 6px; }\n"
"QFrame#rightPanel QPushButton#b"
                        "tnNotifier:hover { background: #c7dcfb; }\n"
"QFrame#rightPanel QPushButton#btnAlerte:hover   { background: #fbcfcf; }\n"
"QFrame#rightPanel QPushButton#btnNotifier { background: #dbeafe; color: #1d4ed8; padding: 9px 6px; }\n"
"QFrame#rightPanel QPushButton#btnAlerte   { background: #fee2e2; color: #dc2626; padding: 9px 6px; }\n"
"QFrame#rightPanel QPushButton#btnGenerer  { background: #26364a; }\n"
"QFrame#rightPanel QPushButton#btnEnvoyerEmail { background: #e53935; }\n"
"QFrame#rightPanel QPushButton#btnEnvoyerEmail:hover { background: #c62828; }\n"
"QFrame#rightPanel QPushButton#btnEnvoyerSms { background: #1f6fe0; }\n"
"QFrame#rightPanel QPushButton#btnEnvoyerWhatsapp { background: #22a559; }\n"
""));
        rootLayout = new QHBoxLayout(GestionInterventions);
        rootLayout->setSpacing(0);
        rootLayout->setObjectName("rootLayout");
        rootLayout->setContentsMargins(0, 0, 0, 0);
        sidebar = new QFrame(GestionInterventions);
        sidebar->setObjectName("sidebar");
        sidebar->setMinimumSize(QSize(215, 0));
        sidebar->setMaximumSize(QSize(215, 16777215));
        sidebarLayout = new QVBoxLayout(sidebar);
        sidebarLayout->setSpacing(2);
        sidebarLayout->setObjectName("sidebarLayout");
        sidebarLayout->setContentsMargins(0, 24, 0, 24);
        logoLabel = new QLabel(sidebar);
        logoLabel->setObjectName("logoLabel");
        logoLabel->setMinimumSize(QSize(124, 124));
        logoLabel->setMaximumSize(QSize(124, 124));
        logoLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/logo.png")));
        logoLabel->setScaledContents(true);

        sidebarLayout->addWidget(logoLabel, 0, Qt::AlignmentFlag::AlignHCenter);

        btnHome = new QPushButton(sidebar);
        btnHome->setObjectName("btnHome");
        btnHome->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/home.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnHome->setIcon(icon);
        btnHome->setIconSize(QSize(22, 22));
        btnHome->setCheckable(true);
        btnHome->setAutoExclusive(true);

        sidebarLayout->addWidget(btnHome);

        btnEmployes = new QPushButton(sidebar);
        btnEmployes->setObjectName("btnEmployes");
        btnEmployes->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/employes.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnEmployes->setIcon(icon1);
        btnEmployes->setIconSize(QSize(22, 22));
        btnEmployes->setCheckable(true);
        btnEmployes->setAutoExclusive(true);

        sidebarLayout->addWidget(btnEmployes);

        btnVehicules = new QPushButton(sidebar);
        btnVehicules->setObjectName("btnVehicules");
        btnVehicules->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/vehicules.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnVehicules->setIcon(icon2);
        btnVehicules->setIconSize(QSize(22, 22));
        btnVehicules->setCheckable(true);
        btnVehicules->setAutoExclusive(true);

        sidebarLayout->addWidget(btnVehicules);

        btnEquipements = new QPushButton(sidebar);
        btnEquipements->setObjectName("btnEquipements");
        btnEquipements->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/equipements.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnEquipements->setIcon(icon3);
        btnEquipements->setIconSize(QSize(22, 22));
        btnEquipements->setCheckable(true);
        btnEquipements->setAutoExclusive(true);

        sidebarLayout->addWidget(btnEquipements);

        btnInterventions = new QPushButton(sidebar);
        btnInterventions->setObjectName("btnInterventions");
        btnInterventions->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/interventions.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnInterventions->setIcon(icon4);
        btnInterventions->setIconSize(QSize(22, 22));
        btnInterventions->setCheckable(true);
        btnInterventions->setChecked(true);
        btnInterventions->setAutoExclusive(true);

        sidebarLayout->addWidget(btnInterventions);

        btnZones = new QPushButton(sidebar);
        btnZones->setObjectName("btnZones");
        btnZones->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/zones.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnZones->setIcon(icon5);
        btnZones->setIconSize(QSize(22, 22));
        btnZones->setCheckable(true);
        btnZones->setAutoExclusive(true);

        sidebarLayout->addWidget(btnZones);

        btnSensibilisation = new QPushButton(sidebar);
        btnSensibilisation->setObjectName("btnSensibilisation");
        btnSensibilisation->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/sensibilisation.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnSensibilisation->setIcon(icon6);
        btnSensibilisation->setIconSize(QSize(22, 22));
        btnSensibilisation->setCheckable(true);
        btnSensibilisation->setAutoExclusive(true);

        sidebarLayout->addWidget(btnSensibilisation);

        btnParametres = new QPushButton(sidebar);
        btnParametres->setObjectName("btnParametres");
        btnParametres->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/parametres.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnParametres->setIcon(icon7);
        btnParametres->setIconSize(QSize(22, 22));
        btnParametres->setCheckable(true);
        btnParametres->setAutoExclusive(true);

        sidebarLayout->addWidget(btnParametres);

        sidebarSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        sidebarLayout->addItem(sidebarSpacer);


        rootLayout->addWidget(sidebar);

        centerFrame = new QFrame(GestionInterventions);
        centerFrame->setObjectName("centerFrame");
        centerLayout = new QVBoxLayout(centerFrame);
        centerLayout->setSpacing(0);
        centerLayout->setObjectName("centerLayout");
        centerLayout->setContentsMargins(0, 0, 0, 0);
        topBar = new QFrame(centerFrame);
        topBar->setObjectName("topBar");
        topBar->setMinimumSize(QSize(0, 58));
        topBar->setMaximumSize(QSize(16777215, 58));
        topBarLayout = new QHBoxLayout(topBar);
        topBarLayout->setSpacing(10);
        topBarLayout->setObjectName("topBarLayout");
        topBarLayout->setContentsMargins(16, 8, 16, 8);
        btnMenu = new QPushButton(topBar);
        btnMenu->setObjectName("btnMenu");
        btnMenu->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/menu.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnMenu->setIcon(icon8);
        btnMenu->setIconSize(QSize(22, 22));

        topBarLayout->addWidget(btnMenu);

        searchEdit = new QLineEdit(topBar);
        searchEdit->setObjectName("searchEdit");

        topBarLayout->addWidget(searchEdit);

        topSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        topBarLayout->addItem(topSpacer);

        btnNotif = new QPushButton(topBar);
        btnNotif->setObjectName("btnNotif");
        btnNotif->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/icons/bell.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnNotif->setIcon(icon9);
        btnNotif->setIconSize(QSize(22, 22));

        topBarLayout->addWidget(btnNotif);

        btnSettings = new QPushButton(topBar);
        btnSettings->setObjectName("btnSettings");
        btnSettings->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/icons/gear.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnSettings->setIcon(icon10);
        btnSettings->setIconSize(QSize(22, 22));

        topBarLayout->addWidget(btnSettings);

        avatarLabel = new QLabel(topBar);
        avatarLabel->setObjectName("avatarLabel");
        avatarLabel->setMinimumSize(QSize(38, 38));
        avatarLabel->setMaximumSize(QSize(38, 38));
        avatarLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/avatar.png")));
        avatarLabel->setScaledContents(true);

        topBarLayout->addWidget(avatarLabel);

        labelAdmin = new QLabel(topBar);
        labelAdmin->setObjectName("labelAdmin");

        topBarLayout->addWidget(labelAdmin);

        btnPower = new QPushButton(topBar);
        btnPower->setObjectName("btnPower");
        btnPower->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/icons/power.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnPower->setIcon(icon11);
        btnPower->setIconSize(QSize(22, 22));

        topBarLayout->addWidget(btnPower);


        centerLayout->addWidget(topBar);

        content = new QFrame(centerFrame);
        content->setObjectName("content");
        contentLayout = new QVBoxLayout(content);
        contentLayout->setSpacing(12);
        contentLayout->setObjectName("contentLayout");
        contentLayout->setContentsMargins(18, 6, 18, 18);
        labelTitle = new QLabel(content);
        labelTitle->setObjectName("labelTitle");

        contentLayout->addWidget(labelTitle);

        filtersFrame = new QFrame(content);
        filtersFrame->setObjectName("filtersFrame");
        filtersLayout = new QHBoxLayout(filtersFrame);
        filtersLayout->setSpacing(10);
        filtersLayout->setObjectName("filtersLayout");
        filtersLayout->setContentsMargins(14, 10, 14, 10);
        lblFiltreGravite = new QLabel(filtersFrame);
        lblFiltreGravite->setObjectName("lblFiltreGravite");

        filtersLayout->addWidget(lblFiltreGravite);

        comboGravite = new QComboBox(filtersFrame);
        comboGravite->addItem(QString());
        comboGravite->addItem(QString());
        comboGravite->addItem(QString());
        comboGravite->addItem(QString());
        comboGravite->addItem(QString());
        comboGravite->setObjectName("comboGravite");

        filtersLayout->addWidget(comboGravite);

        lblFiltreStatut = new QLabel(filtersFrame);
        lblFiltreStatut->setObjectName("lblFiltreStatut");

        filtersLayout->addWidget(lblFiltreStatut);

        comboStatut = new QComboBox(filtersFrame);
        comboStatut->addItem(QString());
        comboStatut->addItem(QString());
        comboStatut->addItem(QString());
        comboStatut->addItem(QString());
        comboStatut->setObjectName("comboStatut");

        filtersLayout->addWidget(comboStatut);

        filtersSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        filtersLayout->addItem(filtersSpacer);


        contentLayout->addWidget(filtersFrame);

        cardsLayout = new QHBoxLayout();
        cardsLayout->setSpacing(14);
        cardsLayout->setObjectName("cardsLayout");
        cardEnCours = new QFrame(content);
        cardEnCours->setObjectName("cardEnCours");
        cardEnCoursLayout = new QVBoxLayout(cardEnCours);
        cardEnCoursLayout->setSpacing(2);
        cardEnCoursLayout->setObjectName("cardEnCoursLayout");
        cardEnCoursLayout->setContentsMargins(16, 12, 16, 12);
        lblEnCoursTitle = new QLabel(cardEnCours);
        lblEnCoursTitle->setObjectName("lblEnCoursTitle");

        cardEnCoursLayout->addWidget(lblEnCoursTitle);

        lblEnCours = new QLabel(cardEnCours);
        lblEnCours->setObjectName("lblEnCours");

        cardEnCoursLayout->addWidget(lblEnCours);


        cardsLayout->addWidget(cardEnCours);

        cardEnAttente = new QFrame(content);
        cardEnAttente->setObjectName("cardEnAttente");
        cardEnAttenteLayout = new QVBoxLayout(cardEnAttente);
        cardEnAttenteLayout->setSpacing(2);
        cardEnAttenteLayout->setObjectName("cardEnAttenteLayout");
        cardEnAttenteLayout->setContentsMargins(16, 12, 16, 12);
        lblEnAttenteTitle = new QLabel(cardEnAttente);
        lblEnAttenteTitle->setObjectName("lblEnAttenteTitle");

        cardEnAttenteLayout->addWidget(lblEnAttenteTitle);

        lblEnAttente = new QLabel(cardEnAttente);
        lblEnAttente->setObjectName("lblEnAttente");

        cardEnAttenteLayout->addWidget(lblEnAttente);


        cardsLayout->addWidget(cardEnAttente);

        cardTerminee = new QFrame(content);
        cardTerminee->setObjectName("cardTerminee");
        cardTermineeLayout = new QVBoxLayout(cardTerminee);
        cardTermineeLayout->setSpacing(2);
        cardTermineeLayout->setObjectName("cardTermineeLayout");
        cardTermineeLayout->setContentsMargins(16, 12, 16, 12);
        lblTermineeTitle = new QLabel(cardTerminee);
        lblTermineeTitle->setObjectName("lblTermineeTitle");

        cardTermineeLayout->addWidget(lblTermineeTitle);

        lblTerminee = new QLabel(cardTerminee);
        lblTerminee->setObjectName("lblTerminee");

        cardTermineeLayout->addWidget(lblTerminee);


        cardsLayout->addWidget(cardTerminee);


        contentLayout->addLayout(cardsLayout);

        actionsFrame = new QFrame(content);
        actionsFrame->setObjectName("actionsFrame");
        actionsLayout = new QVBoxLayout(actionsFrame);
        actionsLayout->setSpacing(10);
        actionsLayout->setObjectName("actionsLayout");
        actionsLayout->setContentsMargins(14, 12, 14, 12);
        actionsRow1 = new QHBoxLayout();
        actionsRow1->setSpacing(10);
        actionsRow1->setObjectName("actionsRow1");
        btnAjouter = new QPushButton(actionsFrame);
        btnAjouter->setObjectName("btnAjouter");
        btnAjouter->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/icons/add.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnAjouter->setIcon(icon12);
        btnAjouter->setIconSize(QSize(22, 22));

        actionsRow1->addWidget(btnAjouter);

        btnModifier = new QPushButton(actionsFrame);
        btnModifier->setObjectName("btnModifier");
        btnModifier->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/icons/edit.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnModifier->setIcon(icon13);
        btnModifier->setIconSize(QSize(22, 22));

        actionsRow1->addWidget(btnModifier);

        btnSupprimer = new QPushButton(actionsFrame);
        btnSupprimer->setObjectName("btnSupprimer");
        btnSupprimer->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/icons/delete.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnSupprimer->setIcon(icon14);
        btnSupprimer->setIconSize(QSize(22, 22));

        actionsRow1->addWidget(btnSupprimer);

        btnConsulter = new QPushButton(actionsFrame);
        btnConsulter->setObjectName("btnConsulter");
        btnConsulter->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/icons/view.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnConsulter->setIcon(icon15);
        btnConsulter->setIconSize(QSize(22, 22));

        actionsRow1->addWidget(btnConsulter);

        row1Spacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        actionsRow1->addItem(row1Spacer);


        actionsLayout->addLayout(actionsRow1);

        actionsRow2 = new QHBoxLayout();
        actionsRow2->setSpacing(10);
        actionsRow2->setObjectName("actionsRow2");
        btnExportPdf = new QPushButton(actionsFrame);
        btnExportPdf->setObjectName("btnExportPdf");
        btnExportPdf->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/icons/pdf.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnExportPdf->setIcon(icon16);
        btnExportPdf->setIconSize(QSize(22, 22));

        actionsRow2->addWidget(btnExportPdf);

        btnTriDate = new QPushButton(actionsFrame);
        btnTriDate->setObjectName("btnTriDate");
        btnTriDate->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/icons/calendar.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnTriDate->setIcon(icon17);
        btnTriDate->setIconSize(QSize(22, 22));

        actionsRow2->addWidget(btnTriDate);

        btnTriGravite = new QPushButton(actionsFrame);
        btnTriGravite->setObjectName("btnTriGravite");
        btnTriGravite->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnTriGravite->setIcon(icon4);
        btnTriGravite->setIconSize(QSize(22, 22));

        actionsRow2->addWidget(btnTriGravite);

        btnStats = new QPushButton(actionsFrame);
        btnStats->setObjectName("btnStats");
        btnStats->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/icons/stats.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnStats->setIcon(icon18);
        btnStats->setIconSize(QSize(22, 22));

        actionsRow2->addWidget(btnStats);

        btnAffectation = new QPushButton(actionsFrame);
        btnAffectation->setObjectName("btnAffectation");
        btnAffectation->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/icons/team.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnAffectation->setIcon(icon19);
        btnAffectation->setIconSize(QSize(22, 22));

        actionsRow2->addWidget(btnAffectation);

        btnParcours = new QPushButton(actionsFrame);
        btnParcours->setObjectName("btnParcours");
        btnParcours->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/icons/route.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnParcours->setIcon(icon20);
        btnParcours->setIconSize(QSize(22, 22));

        actionsRow2->addWidget(btnParcours);

        row2Spacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        actionsRow2->addItem(row2Spacer);


        actionsLayout->addLayout(actionsRow2);


        contentLayout->addWidget(actionsFrame);

        tableFrame = new QFrame(content);
        tableFrame->setObjectName("tableFrame");
        tableFrameLayout = new QVBoxLayout(tableFrame);
        tableFrameLayout->setSpacing(0);
        tableFrameLayout->setObjectName("tableFrameLayout");
        tableFrameLayout->setContentsMargins(0, 0, 0, 0);
        tableInterventions = new QTableWidget(tableFrame);
        if (tableInterventions->columnCount() < 7)
            tableInterventions->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableInterventions->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableInterventions->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableInterventions->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableInterventions->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableInterventions->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableInterventions->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableInterventions->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        tableInterventions->setObjectName("tableInterventions");
        tableInterventions->setFrameShape(QFrame::Shape::NoFrame);
        tableInterventions->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableInterventions->setAlternatingRowColors(true);
        tableInterventions->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tableInterventions->setShowGrid(false);
        tableInterventions->horizontalHeader()->setDefaultSectionSize(140);
        tableInterventions->horizontalHeader()->setStretchLastSection(true);
        tableInterventions->verticalHeader()->setVisible(false);
        tableInterventions->verticalHeader()->setDefaultSectionSize(52);

        tableFrameLayout->addWidget(tableInterventions);


        contentLayout->addWidget(tableFrame);


        centerLayout->addWidget(content);


        rootLayout->addWidget(centerFrame);


        retranslateUi(GestionInterventions);

        QMetaObject::connectSlotsByName(GestionInterventions);
    } // setupUi

    void retranslateUi(QWidget *GestionInterventions)
    {
        GestionInterventions->setWindowTitle(QCoreApplication::translate("GestionInterventions", "Gestion des interventions", nullptr));
        logoLabel->setText(QString());
        btnHome->setText(QCoreApplication::translate("GestionInterventions", "  Home", nullptr));
        btnEmployes->setText(QCoreApplication::translate("GestionInterventions", "  Employ\303\251s", nullptr));
        btnVehicules->setText(QCoreApplication::translate("GestionInterventions", "  V\303\251hicules", nullptr));
        btnEquipements->setText(QCoreApplication::translate("GestionInterventions", "  \303\211quipements", nullptr));
        btnInterventions->setText(QCoreApplication::translate("GestionInterventions", "  Interventions", nullptr));
        btnZones->setText(QCoreApplication::translate("GestionInterventions", "  Zone de couverture", nullptr));
        btnSensibilisation->setText(QCoreApplication::translate("GestionInterventions", "  Sensibilisation", nullptr));
        btnParametres->setText(QCoreApplication::translate("GestionInterventions", "  Param\303\250tres", nullptr));
        btnMenu->setText(QString());
        searchEdit->setPlaceholderText(QCoreApplication::translate("GestionInterventions", "Rechercher", nullptr));
        btnNotif->setText(QString());
        btnSettings->setText(QString());
        avatarLabel->setText(QString());
        labelAdmin->setText(QCoreApplication::translate("GestionInterventions", "ADMIN", nullptr));
        btnPower->setText(QString());
        labelTitle->setText(QCoreApplication::translate("GestionInterventions", "Gestion des Interventions", nullptr));
        lblFiltreGravite->setText(QCoreApplication::translate("GestionInterventions", "Filtrer par Gravit\303\251", nullptr));
        comboGravite->setItemText(0, QCoreApplication::translate("GestionInterventions", "Toutes les gravit\303\251s", nullptr));
        comboGravite->setItemText(1, QCoreApplication::translate("GestionInterventions", "Faible", nullptr));
        comboGravite->setItemText(2, QCoreApplication::translate("GestionInterventions", "Moyenne", nullptr));
        comboGravite->setItemText(3, QCoreApplication::translate("GestionInterventions", "\303\211lev\303\251e", nullptr));
        comboGravite->setItemText(4, QCoreApplication::translate("GestionInterventions", "Critique", nullptr));

        lblFiltreStatut->setText(QCoreApplication::translate("GestionInterventions", "Filtrer par Statut", nullptr));
        comboStatut->setItemText(0, QCoreApplication::translate("GestionInterventions", "Tous les statuts", nullptr));
        comboStatut->setItemText(1, QCoreApplication::translate("GestionInterventions", "En cours", nullptr));
        comboStatut->setItemText(2, QCoreApplication::translate("GestionInterventions", "En attente", nullptr));
        comboStatut->setItemText(3, QCoreApplication::translate("GestionInterventions", "Termin\303\251e", nullptr));

        lblEnCoursTitle->setText(QCoreApplication::translate("GestionInterventions", "En cours", nullptr));
        lblEnCours->setText(QCoreApplication::translate("GestionInterventions", "0", nullptr));
        lblEnAttenteTitle->setText(QCoreApplication::translate("GestionInterventions", "En attente", nullptr));
        lblEnAttente->setText(QCoreApplication::translate("GestionInterventions", "0", nullptr));
        lblTermineeTitle->setText(QCoreApplication::translate("GestionInterventions", "Termin\303\251e", nullptr));
        lblTerminee->setText(QCoreApplication::translate("GestionInterventions", "0", nullptr));
        btnAjouter->setText(QCoreApplication::translate("GestionInterventions", "Ajouter", nullptr));
        btnModifier->setText(QCoreApplication::translate("GestionInterventions", "Modifier", nullptr));
        btnSupprimer->setText(QCoreApplication::translate("GestionInterventions", "Supprimer", nullptr));
        btnConsulter->setText(QCoreApplication::translate("GestionInterventions", "Consulter", nullptr));
        btnExportPdf->setText(QCoreApplication::translate("GestionInterventions", "Exporter PDF", nullptr));
        btnTriDate->setText(QCoreApplication::translate("GestionInterventions", "Trier selon la date", nullptr));
        btnTriGravite->setText(QCoreApplication::translate("GestionInterventions", "Trier selon la gravit\303\251", nullptr));
        btnStats->setText(QCoreApplication::translate("GestionInterventions", "Statistiques", nullptr));
        btnAffectation->setText(QCoreApplication::translate("GestionInterventions", "Affectation automatique", nullptr));
        btnParcours->setText(QCoreApplication::translate("GestionInterventions", "Parcours recommand\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableInterventions->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("GestionInterventions", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableInterventions->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("GestionInterventions", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableInterventions->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("GestionInterventions", "Adresse", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableInterventions->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("GestionInterventions", "Date et heure", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableInterventions->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("GestionInterventions", "ID_zone", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableInterventions->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("GestionInterventions", "Gravit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableInterventions->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("GestionInterventions", "Statut", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GestionInterventions: public Ui_GestionInterventions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GESTIONINTERVENTIONS_H
