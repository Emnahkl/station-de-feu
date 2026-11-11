/********************************************************************************
** Form generated from reading UI file 'gestioninterventions.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
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
#include <QtWidgets/QStackedWidget>
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
    QSpacerItem *logoSpacer;
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
    QHBoxLayout *cardsLayout;
    QFrame *cardTotal;
    QVBoxLayout *cardTotalLayout;
    QLabel *lblTotalTitle;
    QLabel *lblTotal;
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
    QPushButton *btnConsulter;
    QPushButton *btnSupprimer;
    QSpacerItem *row1Spacer;
    QPushButton *btnAffectation;
    QPushButton *btnParcours;
    QFrame *sepToolbar;
    QHBoxLayout *actionsRow2;
    QLabel *lblFiltreGravite;
    QComboBox *comboGravite;
    QLabel *lblFiltreStatut;
    QComboBox *comboStatut;
    QLabel *lblTri;
    QHBoxLayout *triLayout;
    QPushButton *btnTriId;
    QPushButton *btnTriDate;
    QSpacerItem *row2Spacer;
    QLabel *lblCompte;
    QPushButton *btnStats;
    QPushButton *btnExportPdf;
    QFrame *bandeau;
    QHBoxLayout *bandeauLayout;
    QLabel *lblMessage;
    QPushButton *btnBandeauOk;
    QPushButton *btnBandeauAnnuler;
    QStackedWidget *stackedMain;
    QWidget *pageListe;
    QVBoxLayout *pageListeLayout;
    QFrame *tableFrame;
    QVBoxLayout *tableFrameLayout;
    QTableWidget *tableInterventions;
    QWidget *pageStats;
    QVBoxLayout *statsLayout;
    QHBoxLayout *statsHeader;
    QLabel *lblStatsTitle;
    QSpacerItem *statsHeaderSpacer;
    QPushButton *btnRetourListe;
    QFrame *statsFrame;
    QHBoxLayout *statsChartsLayout;
    QWidget *pageParcours;
    QVBoxLayout *parcoursLayout;
    QHBoxLayout *parcoursHeader;
    QLabel *lblParcoursTitle;
    QSpacerItem *parcoursHeaderSpacer;
    QPushButton *btnRetourParcours;
    QLabel *lblParcoursResume;
    QLabel *lblParcoursInfo;
    QFrame *parcoursFrame;
    QVBoxLayout *parcoursFrameLayout;
    QTableWidget *tableParcours;
    QWidget *pageAffectation;
    QVBoxLayout *affectationLayout;
    QHBoxLayout *affectationHeader;
    QLabel *lblAffectationTitle;
    QSpacerItem *affectationHeaderSpacer;
    QPushButton *btnValiderAffectation;
    QPushButton *btnRetourAffectation;
    QLabel *lblAffectationResume;
    QLabel *lblAffectationAvert;
    QFrame *affectationFrame;
    QVBoxLayout *affectationFrameLayout;
    QTableWidget *tableAffectation;

    void setupUi(QWidget *GestionInterventions)
    {
        if (GestionInterventions->objectName().isEmpty())
            GestionInterventions->setObjectName("GestionInterventions");
        GestionInterventions->resize(1500, 820);
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
"\n"
"/* ---- Titre ---- */\n"
"QLabel#labelTitle { font-size: 20px; font-weight: bold; color: #1f2937; padding: 4px 2px; }\n"
"\n"
"/* ---- Cartes de synth\303\250se (4 cartes de m\303\252me largeur) ---- */\n"
"QFrame#cardTotal, QFrame#cardEnCours, QFrame#cardEnAttente, QFrame#cardTerminee {\n"
"    background: white; border: 1px solid #e3e6ea; border-radius: 8px;\n"
"}\n"
"QFrame#cardTotal     { borde"
                        "r-left: 5px solid #1f2d3d; }\n"
"QFrame#cardEnCours   { border-left: 5px solid #f59e0b; }\n"
"QFrame#cardEnAttente { border-left: 5px solid #ef4444; }\n"
"QFrame#cardTerminee  { border-left: 5px solid #22c55e; }\n"
"QFrame#cardTotal QLabel, QFrame#cardEnCours QLabel, QFrame#cardEnAttente QLabel, QFrame#cardTerminee QLabel { border: none; }\n"
"QLabel#lblTotalTitle     { color: #1f2d3d; font-weight: bold; }\n"
"QLabel#lblEnCoursTitle   { color: #b45309; font-weight: bold; }\n"
"QLabel#lblEnAttenteTitle { color: #b91c1c; font-weight: bold; }\n"
"QLabel#lblTermineeTitle  { color: #15803d; font-weight: bold; }\n"
"QLabel#lblTotal, QLabel#lblEnCours, QLabel#lblEnAttente, QLabel#lblTerminee { font-size: 26px; font-weight: bold; }\n"
"\n"
"/* ---- Barre d'outils : ligne 1 = actions, ligne 2 = affichage (filtres, tri, stats, PDF) ---- */\n"
"QFrame#actionsFrame { background: white; border: 1px solid #e3e6ea; border-radius: 8px; }\n"
"QFrame#actionsFrame QLabel { border: none; color: #6b7280; font-weight: 600; }\n"
"QF"
                        "rame#sepToolbar { background: #eceef2; border: none; min-height: 1px; max-height: 1px; }\n"
"QLabel#lblFiltreStatut, QLabel#lblTri { padding-left: 10px; }\n"
"QLabel#lblCompte { font-weight: normal; padding-right: 6px; }\n"
"\n"
"QFrame#actionsFrame QComboBox {\n"
"    background: white; border: 1px solid #cfd4da; border-radius: 6px; padding: 7px 10px; min-width: 150px; color: #1f2937;\n"
"}\n"
"QFrame#actionsFrame QComboBox:hover { border-color: #9aa3af; }\n"
"QComboBox::drop-down, QDateEdit::drop-down { border: none; width: 24px; }\n"
"QComboBox::down-arrow, QDateEdit::down-arrow { image: url(:/icons/chevron_down.png); width: 14px; height: 14px; }\n"
"QComboBox QAbstractItemView { background: white; selection-background-color: #fde2e2; selection-color: #1f2937; border: 1px solid #cfd4da; }\n"
"\n"
"/* boutons : secondaire (blanc) par d\303\251faut */\n"
"QFrame#actionsFrame QPushButton {\n"
"    background: white; color: #1f2d3d; border: 1px solid #cfd4da; border-radius: 6px;\n"
"    padding: 8px 14px; font-"
                        "weight: 600; min-height: 20px;\n"
"}\n"
"QFrame#actionsFrame QPushButton:hover    { background: #f3f4f6; border-color: #aab1bb; }\n"
"QFrame#actionsFrame QPushButton:pressed  { background: #e5e7eb; }\n"
"QFrame#actionsFrame QPushButton:focus    { border: 1px solid #1f2d3d; }\n"
"QFrame#actionsFrame QPushButton:disabled { background: #f3f4f6; color: #a3aab5; border-color: #e5e7eb; }\n"
"\n"
"/* bouton principal : Ajouter */\n"
"QFrame#actionsFrame QPushButton#btnAjouter { background: #1f2d3d; color: white; border: 1px solid #1f2d3d; padding: 8px 20px; }\n"
"QFrame#actionsFrame QPushButton#btnAjouter:hover    { background: #2d4157; border-color: #2d4157; }\n"
"QFrame#actionsFrame QPushButton#btnAjouter:pressed  { background: #16212d; }\n"
"QFrame#actionsFrame QPushButton#btnAjouter:disabled { background: #c5ccd5; border-color: #c5ccd5; color: #f3f4f6; }\n"
"\n"
"/* bouton destructif : Supprimer (rouge) */\n"
"QFrame#actionsFrame QPushButton#btnSupprimer { color: #b91c1c; border: 1px solid #f1b9b9; }\n"
"QFrame#a"
                        "ctionsFrame QPushButton#btnSupprimer:hover    { background: #fef2f2; border-color: #b91c1c; }\n"
"QFrame#actionsFrame QPushButton#btnSupprimer:pressed  { background: #fee2e2; }\n"
"QFrame#actionsFrame QPushButton#btnSupprimer:disabled { background: #f3f4f6; color: #d3a9a9; border-color: #e5e7eb; }\n"
"\n"
"/* tri : deux boutons coll\303\251s (ID | Date), celui qui est actif est fonc\303\251 */\n"
"QFrame#actionsFrame QPushButton#btnTriId   { border-top-right-radius: 0; border-bottom-right-radius: 0; padding: 8px 16px; }\n"
"QFrame#actionsFrame QPushButton#btnTriDate { border-top-left-radius: 0; border-bottom-left-radius: 0; border-left: none; padding: 8px 16px; }\n"
"QFrame#actionsFrame QPushButton[actif=\"true\"] { background: #1f2d3d; color: white; border-color: #1f2d3d; }\n"
"QFrame#actionsFrame QPushButton[actif=\"true\"]:hover { background: #2d4157; }\n"
"\n"
"/* ---- Bandeau (messages, Enregistrer / Annuler, confirmation, fiche) ---- */\n"
"QFrame#bandeau { background: white; border: 1px solid #e3e6ea; b"
                        "order-radius: 8px; }\n"
"QFrame#bandeau[etat=\"info\"]    { background: #eff6ff; border-color: #bfdbfe; }\n"
"QFrame#bandeau[etat=\"ok\"]      { background: #f0fdf4; border-color: #bbf7d0; }\n"
"QFrame#bandeau[etat=\"erreur\"]  { background: #fef2f2; border-color: #fecaca; }\n"
"QFrame#bandeau[etat=\"edition\"] { background: #fffbeb; border-color: #fde68a; }\n"
"QFrame#bandeau[etat=\"danger\"]  { background: #fef2f2; border-color: #fca5a5; }\n"
"QFrame#bandeau QLabel { border: none; background: transparent; font-size: 13px; }\n"
"QFrame#bandeau[etat=\"info\"]    QLabel { color: #1e3a8a; }\n"
"QFrame#bandeau[etat=\"ok\"]      QLabel { color: #166534; }\n"
"QFrame#bandeau[etat=\"erreur\"]  QLabel { color: #991b1b; }\n"
"QFrame#bandeau[etat=\"edition\"] QLabel { color: #78350f; }\n"
"QFrame#bandeau[etat=\"danger\"]  QLabel { color: #7f1d1d; }\n"
"QFrame#bandeau QPushButton {\n"
"    background: white; color: #374151; border: 1px solid #cfd4da; border-radius: 6px;\n"
"    padding: 8px 18px; font-weight: 700; min-h"
                        "eight: 20px;\n"
"}\n"
"QFrame#bandeau QPushButton:hover   { background: #f3f4f6; }\n"
"QFrame#bandeau QPushButton:pressed { background: #e5e7eb; }\n"
"QFrame#bandeau QPushButton#btnBandeauOk { background: #15803d; color: white; border: 1px solid #15803d; }\n"
"QFrame#bandeau QPushButton#btnBandeauOk:hover   { background: #166534; }\n"
"QFrame#bandeau QPushButton#btnBandeauOk:pressed { background: #14532d; }\n"
"QFrame#bandeau[etat=\"danger\"] QPushButton#btnBandeauOk { background: #b91c1c; border: 1px solid #b91c1c; }\n"
"QFrame#bandeau[etat=\"danger\"] QPushButton#btnBandeauOk:hover { background: #991b1b; }\n"
"\n"
"/* ---- Tableau ---- */\n"
"QFrame#tableFrame { background: white; border: 1px solid #e3e6ea; border-radius: 4px; }\n"
"QTableWidget { background: white; alternate-background-color: #f5f6f8; border: none; selection-background-color: #fde2e2; selection-color: #1f2937; }\n"
"QTableWidget::item { padding: 6px; border-bottom: 1px solid #e8eaee; }\n"
"QHeaderView { background: #1f2d3d; }\n"
"QHeaderVie"
                        "w::section { background: #1f2d3d; color: white; font-weight: bold; padding: 12px 8px; border: none; border-right: 1px solid #2f4054; }\n"
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
"QFrame#rightPanel QLineEdit { background: white; border: 1px solid #d5d9df; border-radius: 4px; padding: "
                        "8px; }\n"
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
"QFrame#rightPanel QPushButton#btnNotifier:hover { background: #c7dcfb; }\n"
"QFrame#right"
                        "Panel QPushButton#btnAlerte:hover   { background: #fbcfcf; }\n"
"QFrame#rightPanel QPushButton#btnNotifier { background: #dbeafe; color: #1d4ed8; padding: 9px 6px; }\n"
"QFrame#rightPanel QPushButton#btnAlerte   { background: #fee2e2; color: #dc2626; padding: 9px 6px; }\n"
"QFrame#rightPanel QPushButton#btnGenerer  { background: #26364a; }\n"
"QFrame#rightPanel QPushButton#btnEnvoyerEmail { background: #e53935; }\n"
"QFrame#rightPanel QPushButton#btnEnvoyerEmail:hover { background: #c62828; }\n"
"QFrame#rightPanel QPushButton#btnEnvoyerSms { background: #1f6fe0; }\n"
"QFrame#rightPanel QPushButton#btnEnvoyerWhatsapp { background: #22a559; }\n"
"\n"
"/* ---- \303\211dition directement dans le tableau ---- */\n"
"QFrame#celluleEdition { background: #fff7e0; border: none; }\n"
"QFrame#celluleEdition QLineEdit, QFrame#celluleEdition QDateEdit, QFrame#celluleEdition QTimeEdit,\n"
"QFrame#celluleEdition QComboBox {\n"
"    background: white; border: 1px solid #cfd4da; border-radius: 4px; padding: 5px 8px; min-width:"
                        " 0; color: #1f2937;\n"
"}\n"
"QFrame#celluleEdition QLineEdit:focus, QFrame#celluleEdition QDateEdit:focus, QFrame#celluleEdition QTimeEdit:focus,\n"
"QFrame#celluleEdition QComboBox:focus { border: 1px solid #1f2d3d; }\n"
"QFrame#celluleEdition QLineEdit:disabled, QFrame#celluleEdition QDateEdit:disabled,\n"
"QFrame#celluleEdition QTimeEdit:disabled, QFrame#celluleEdition QComboBox:disabled { background: #f3f4f6; color: #6b7280; }\n"
"QFrame#celluleEdition QLineEdit[erreur=\"true\"], QFrame#celluleEdition QDateEdit[erreur=\"true\"],\n"
"QFrame#celluleEdition QTimeEdit[erreur=\"true\"], QFrame#celluleEdition QComboBox[erreur=\"true\"] {\n"
"    border: 1px solid #dc2626; background: #fef2f2;\n"
"}\n"
"\n"
"/* ---- Statistiques (remplacent le tableau) ---- */\n"
"QLabel#lblStatsTitle { font-size: 16px; font-weight: bold; }\n"
"QLabel#lblParcoursTitle { font-size: 16px; font-weight: bold; }\n"
"QLabel#lblParcoursResume { font-weight: 600; color: #1f2937; }\n"
"QLabel#lblParcoursInfo { color: #6b7280; font-size: "
                        "12px; }\n"
"QLabel#lblAffectationTitle { font-size: 16px; font-weight: bold; }\n"
"QLabel#lblAffectationResume { font-weight: 600; color: #1f2937; }\n"
"QLabel#lblAffectationAvert { color: #92400e; background: #fffbeb; border: 1px solid #fde68a; border-radius: 6px; padding: 8px 12px; }\n"
"QPushButton#btnValiderAffectation {\n"
"    background: #15803d; color: white; border: 1px solid #15803d; border-radius: 6px;\n"
"    padding: 8px 18px; font-weight: 700; min-height: 20px;\n"
"}\n"
"QPushButton#btnValiderAffectation:hover    { background: #166534; }\n"
"QPushButton#btnValiderAffectation:pressed  { background: #14532d; }\n"
"QPushButton#btnValiderAffectation:disabled { background: #a7c4b0; border-color: #a7c4b0; }\n"
"QFrame#statsFrame { background: white; border: 1px solid #e3e6ea; border-radius: 8px; }\n"
"QPushButton#btnRetourListe, QPushButton#btnRetourParcours, QPushButton#btnRetourAffectation {\n"
"    background: white; color: #1f2d3d; border: 1px solid #cfd4da; border-radius: 6px;\n"
"    padding: 8px"
                        " 16px; font-weight: 600; min-height: 20px;\n"
"}\n"
"QPushButton#btnRetourListe:hover, QPushButton#btnRetourParcours:hover, QPushButton#btnRetourAffectation:hover   { background: #f3f4f6; border-color: #aab1bb; }\n"
"QPushButton#btnRetourListe:pressed, QPushButton#btnRetourParcours:pressed, QPushButton#btnRetourAffectation:pressed { background: #e5e7eb; }\n"
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
        logoLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/logo.png")));
        logoLabel->setMinimumSize(QSize(124, 124));
        logoLabel->setMaximumSize(QSize(124, 124));
        logoLabel->setScaledContents(true);

        sidebarLayout->addWidget(logoLabel, 0, Qt::AlignHCenter);

        logoSpacer = new QSpacerItem(20, 14, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        sidebarLayout->addItem(logoSpacer);

        btnHome = new QPushButton(sidebar);
        btnHome->setObjectName("btnHome");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/home.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnHome->setIcon(icon);
        btnHome->setIconSize(QSize(22, 22));
        btnHome->setCheckable(true);
        btnHome->setAutoExclusive(true);
        btnHome->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(btnHome);

        btnEmployes = new QPushButton(sidebar);
        btnEmployes->setObjectName("btnEmployes");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/employes.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnEmployes->setIcon(icon1);
        btnEmployes->setIconSize(QSize(22, 22));
        btnEmployes->setCheckable(true);
        btnEmployes->setAutoExclusive(true);
        btnEmployes->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(btnEmployes);

        btnVehicules = new QPushButton(sidebar);
        btnVehicules->setObjectName("btnVehicules");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/vehicules.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnVehicules->setIcon(icon2);
        btnVehicules->setIconSize(QSize(22, 22));
        btnVehicules->setCheckable(true);
        btnVehicules->setAutoExclusive(true);
        btnVehicules->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(btnVehicules);

        btnEquipements = new QPushButton(sidebar);
        btnEquipements->setObjectName("btnEquipements");
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/equipements.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnEquipements->setIcon(icon3);
        btnEquipements->setIconSize(QSize(22, 22));
        btnEquipements->setCheckable(true);
        btnEquipements->setAutoExclusive(true);
        btnEquipements->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(btnEquipements);

        btnInterventions = new QPushButton(sidebar);
        btnInterventions->setObjectName("btnInterventions");
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/interventions.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnInterventions->setIcon(icon4);
        btnInterventions->setIconSize(QSize(22, 22));
        btnInterventions->setCheckable(true);
        btnInterventions->setAutoExclusive(true);
        btnInterventions->setChecked(true);
        btnInterventions->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(btnInterventions);

        btnZones = new QPushButton(sidebar);
        btnZones->setObjectName("btnZones");
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/zones.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnZones->setIcon(icon5);
        btnZones->setIconSize(QSize(22, 22));
        btnZones->setCheckable(true);
        btnZones->setAutoExclusive(true);
        btnZones->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(btnZones);

        btnSensibilisation = new QPushButton(sidebar);
        btnSensibilisation->setObjectName("btnSensibilisation");
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/sensibilisation.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnSensibilisation->setIcon(icon6);
        btnSensibilisation->setIconSize(QSize(22, 22));
        btnSensibilisation->setCheckable(true);
        btnSensibilisation->setAutoExclusive(true);
        btnSensibilisation->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(btnSensibilisation);

        btnParametres = new QPushButton(sidebar);
        btnParametres->setObjectName("btnParametres");
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/parametres.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnParametres->setIcon(icon7);
        btnParametres->setIconSize(QSize(22, 22));
        btnParametres->setCheckable(true);
        btnParametres->setAutoExclusive(true);
        btnParametres->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

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
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/menu.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnMenu->setIcon(icon8);
        btnMenu->setIconSize(QSize(22, 22));
        btnMenu->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        topBarLayout->addWidget(btnMenu);

        searchEdit = new QLineEdit(topBar);
        searchEdit->setObjectName("searchEdit");

        topBarLayout->addWidget(searchEdit);

        topSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        topBarLayout->addItem(topSpacer);

        btnNotif = new QPushButton(topBar);
        btnNotif->setObjectName("btnNotif");
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/icons/bell.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnNotif->setIcon(icon9);
        btnNotif->setIconSize(QSize(22, 22));
        btnNotif->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        topBarLayout->addWidget(btnNotif);

        btnSettings = new QPushButton(topBar);
        btnSettings->setObjectName("btnSettings");
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/icons/gear.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnSettings->setIcon(icon10);
        btnSettings->setIconSize(QSize(22, 22));
        btnSettings->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        topBarLayout->addWidget(btnSettings);

        avatarLabel = new QLabel(topBar);
        avatarLabel->setObjectName("avatarLabel");
        avatarLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/avatar.png")));
        avatarLabel->setMinimumSize(QSize(38, 38));
        avatarLabel->setMaximumSize(QSize(38, 38));
        avatarLabel->setScaledContents(true);

        topBarLayout->addWidget(avatarLabel);

        labelAdmin = new QLabel(topBar);
        labelAdmin->setObjectName("labelAdmin");

        topBarLayout->addWidget(labelAdmin);

        btnPower = new QPushButton(topBar);
        btnPower->setObjectName("btnPower");
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/icons/power.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnPower->setIcon(icon11);
        btnPower->setIconSize(QSize(22, 22));
        btnPower->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

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

        cardsLayout = new QHBoxLayout();
        cardsLayout->setSpacing(14);
        cardsLayout->setObjectName("cardsLayout");
        cardTotal = new QFrame(content);
        cardTotal->setObjectName("cardTotal");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cardTotal->sizePolicy().hasHeightForWidth());
        cardTotal->setSizePolicy(sizePolicy);
        cardTotalLayout = new QVBoxLayout(cardTotal);
        cardTotalLayout->setSpacing(2);
        cardTotalLayout->setObjectName("cardTotalLayout");
        cardTotalLayout->setContentsMargins(16, 12, 16, 12);
        lblTotalTitle = new QLabel(cardTotal);
        lblTotalTitle->setObjectName("lblTotalTitle");

        cardTotalLayout->addWidget(lblTotalTitle);

        lblTotal = new QLabel(cardTotal);
        lblTotal->setObjectName("lblTotal");

        cardTotalLayout->addWidget(lblTotal);


        cardsLayout->addWidget(cardTotal);

        cardEnCours = new QFrame(content);
        cardEnCours->setObjectName("cardEnCours");
        sizePolicy.setHeightForWidth(cardEnCours->sizePolicy().hasHeightForWidth());
        cardEnCours->setSizePolicy(sizePolicy);
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
        sizePolicy.setHeightForWidth(cardEnAttente->sizePolicy().hasHeightForWidth());
        cardEnAttente->setSizePolicy(sizePolicy);
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
        sizePolicy.setHeightForWidth(cardTerminee->sizePolicy().hasHeightForWidth());
        cardTerminee->setSizePolicy(sizePolicy);
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

        cardsLayout->setStretch(0, 1);
        cardsLayout->setStretch(1, 1);
        cardsLayout->setStretch(2, 1);
        cardsLayout->setStretch(3, 1);

        contentLayout->addLayout(cardsLayout);

        actionsFrame = new QFrame(content);
        actionsFrame->setObjectName("actionsFrame");
        actionsLayout = new QVBoxLayout(actionsFrame);
        actionsLayout->setSpacing(10);
        actionsLayout->setObjectName("actionsLayout");
        actionsLayout->setContentsMargins(14, 12, 14, 12);
        actionsRow1 = new QHBoxLayout();
        actionsRow1->setSpacing(8);
        actionsRow1->setObjectName("actionsRow1");
        btnAjouter = new QPushButton(actionsFrame);
        btnAjouter->setObjectName("btnAjouter");
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/icons/add.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnAjouter->setIcon(icon12);
        btnAjouter->setIconSize(QSize(20, 20));
        btnAjouter->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        actionsRow1->addWidget(btnAjouter);

        btnModifier = new QPushButton(actionsFrame);
        btnModifier->setObjectName("btnModifier");
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/icons/edit.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnModifier->setIcon(icon13);
        btnModifier->setIconSize(QSize(20, 20));
        btnModifier->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        actionsRow1->addWidget(btnModifier);

        btnConsulter = new QPushButton(actionsFrame);
        btnConsulter->setObjectName("btnConsulter");
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/icons/view.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnConsulter->setIcon(icon14);
        btnConsulter->setIconSize(QSize(20, 20));
        btnConsulter->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        actionsRow1->addWidget(btnConsulter);

        btnSupprimer = new QPushButton(actionsFrame);
        btnSupprimer->setObjectName("btnSupprimer");
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/icons/delete.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnSupprimer->setIcon(icon15);
        btnSupprimer->setIconSize(QSize(20, 20));
        btnSupprimer->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        actionsRow1->addWidget(btnSupprimer);

        row1Spacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        actionsRow1->addItem(row1Spacer);

        btnAffectation = new QPushButton(actionsFrame);
        btnAffectation->setObjectName("btnAffectation");
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/icons/team.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnAffectation->setIcon(icon16);
        btnAffectation->setIconSize(QSize(20, 20));
        btnAffectation->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        actionsRow1->addWidget(btnAffectation);

        btnParcours = new QPushButton(actionsFrame);
        btnParcours->setObjectName("btnParcours");
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/icons/route.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnParcours->setIcon(icon17);
        btnParcours->setIconSize(QSize(20, 20));
        btnParcours->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        actionsRow1->addWidget(btnParcours);


        actionsLayout->addLayout(actionsRow1);

        sepToolbar = new QFrame(actionsFrame);
        sepToolbar->setObjectName("sepToolbar");
        sepToolbar->setFrameShape(QFrame::HLine);
        sepToolbar->setFrameShadow(QFrame::Plain);

        actionsLayout->addWidget(sepToolbar);

        actionsRow2 = new QHBoxLayout();
        actionsRow2->setSpacing(8);
        actionsRow2->setObjectName("actionsRow2");
        lblFiltreGravite = new QLabel(actionsFrame);
        lblFiltreGravite->setObjectName("lblFiltreGravite");

        actionsRow2->addWidget(lblFiltreGravite);

        comboGravite = new QComboBox(actionsFrame);
        comboGravite->addItem(QString());
        comboGravite->addItem(QString());
        comboGravite->addItem(QString());
        comboGravite->addItem(QString());
        comboGravite->addItem(QString());
        comboGravite->setObjectName("comboGravite");

        actionsRow2->addWidget(comboGravite);

        lblFiltreStatut = new QLabel(actionsFrame);
        lblFiltreStatut->setObjectName("lblFiltreStatut");

        actionsRow2->addWidget(lblFiltreStatut);

        comboStatut = new QComboBox(actionsFrame);
        comboStatut->addItem(QString());
        comboStatut->addItem(QString());
        comboStatut->addItem(QString());
        comboStatut->addItem(QString());
        comboStatut->setObjectName("comboStatut");

        actionsRow2->addWidget(comboStatut);

        lblTri = new QLabel(actionsFrame);
        lblTri->setObjectName("lblTri");

        actionsRow2->addWidget(lblTri);

        triLayout = new QHBoxLayout();
        triLayout->setSpacing(0);
        triLayout->setObjectName("triLayout");
        btnTriId = new QPushButton(actionsFrame);
        btnTriId->setObjectName("btnTriId");
        btnTriId->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        triLayout->addWidget(btnTriId);

        btnTriDate = new QPushButton(actionsFrame);
        btnTriDate->setObjectName("btnTriDate");
        btnTriDate->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        triLayout->addWidget(btnTriDate);


        actionsRow2->addLayout(triLayout);

        row2Spacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        actionsRow2->addItem(row2Spacer);

        lblCompte = new QLabel(actionsFrame);
        lblCompte->setObjectName("lblCompte");

        actionsRow2->addWidget(lblCompte);

        btnStats = new QPushButton(actionsFrame);
        btnStats->setObjectName("btnStats");
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/icons/stats.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnStats->setIcon(icon18);
        btnStats->setIconSize(QSize(20, 20));
        btnStats->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        actionsRow2->addWidget(btnStats);

        btnExportPdf = new QPushButton(actionsFrame);
        btnExportPdf->setObjectName("btnExportPdf");
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/icons/pdf.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        btnExportPdf->setIcon(icon19);
        btnExportPdf->setIconSize(QSize(20, 20));
        btnExportPdf->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        actionsRow2->addWidget(btnExportPdf);


        actionsLayout->addLayout(actionsRow2);


        contentLayout->addWidget(actionsFrame);

        bandeau = new QFrame(content);
        bandeau->setObjectName("bandeau");
        bandeau->setVisible(false);
        bandeauLayout = new QHBoxLayout(bandeau);
        bandeauLayout->setSpacing(10);
        bandeauLayout->setObjectName("bandeauLayout");
        bandeauLayout->setContentsMargins(16, 10, 10, 10);
        lblMessage = new QLabel(bandeau);
        lblMessage->setObjectName("lblMessage");
        lblMessage->setWordWrap(true);
        lblMessage->setTextFormat(Qt::RichText);
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lblMessage->sizePolicy().hasHeightForWidth());
        lblMessage->setSizePolicy(sizePolicy1);

        bandeauLayout->addWidget(lblMessage);

        btnBandeauOk = new QPushButton(bandeau);
        btnBandeauOk->setObjectName("btnBandeauOk");
        btnBandeauOk->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        bandeauLayout->addWidget(btnBandeauOk);

        btnBandeauAnnuler = new QPushButton(bandeau);
        btnBandeauAnnuler->setObjectName("btnBandeauAnnuler");
        btnBandeauAnnuler->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        bandeauLayout->addWidget(btnBandeauAnnuler);


        contentLayout->addWidget(bandeau);

        stackedMain = new QStackedWidget(content);
        stackedMain->setObjectName("stackedMain");
        pageListe = new QWidget();
        pageListe->setObjectName("pageListe");
        pageListeLayout = new QVBoxLayout(pageListe);
        pageListeLayout->setSpacing(0);
        pageListeLayout->setObjectName("pageListeLayout");
        pageListeLayout->setContentsMargins(0, 0, 0, 0);
        tableFrame = new QFrame(pageListe);
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
        tableInterventions->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableInterventions->setAlternatingRowColors(true);
        tableInterventions->setSelectionMode(QAbstractItemView::SingleSelection);
        tableInterventions->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableInterventions->setShowGrid(false);
        tableInterventions->setFrameShape(QFrame::NoFrame);
        tableInterventions->setWordWrap(false);
        tableInterventions->horizontalHeader()->setDefaultSectionSize(140);
        tableInterventions->horizontalHeader()->setStretchLastSection(true);
        tableInterventions->verticalHeader()->setVisible(false);
        tableInterventions->verticalHeader()->setDefaultSectionSize(50);

        tableFrameLayout->addWidget(tableInterventions);


        pageListeLayout->addWidget(tableFrame);

        stackedMain->addWidget(pageListe);
        pageStats = new QWidget();
        pageStats->setObjectName("pageStats");
        statsLayout = new QVBoxLayout(pageStats);
        statsLayout->setSpacing(10);
        statsLayout->setObjectName("statsLayout");
        statsLayout->setContentsMargins(0, 0, 0, 0);
        statsHeader = new QHBoxLayout();
        statsHeader->setSpacing(8);
        statsHeader->setObjectName("statsHeader");
        lblStatsTitle = new QLabel(pageStats);
        lblStatsTitle->setObjectName("lblStatsTitle");

        statsHeader->addWidget(lblStatsTitle);

        statsHeaderSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        statsHeader->addItem(statsHeaderSpacer);

        btnRetourListe = new QPushButton(pageStats);
        btnRetourListe->setObjectName("btnRetourListe");
        btnRetourListe->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        statsHeader->addWidget(btnRetourListe);


        statsLayout->addLayout(statsHeader);

        statsFrame = new QFrame(pageStats);
        statsFrame->setObjectName("statsFrame");
        statsChartsLayout = new QHBoxLayout(statsFrame);
        statsChartsLayout->setSpacing(14);
        statsChartsLayout->setObjectName("statsChartsLayout");
        statsChartsLayout->setContentsMargins(14, 14, 14, 14);

        statsLayout->addWidget(statsFrame);

        stackedMain->addWidget(pageStats);
        pageParcours = new QWidget();
        pageParcours->setObjectName("pageParcours");
        parcoursLayout = new QVBoxLayout(pageParcours);
        parcoursLayout->setSpacing(8);
        parcoursLayout->setObjectName("parcoursLayout");
        parcoursLayout->setContentsMargins(0, 0, 0, 0);
        parcoursHeader = new QHBoxLayout();
        parcoursHeader->setSpacing(8);
        parcoursHeader->setObjectName("parcoursHeader");
        lblParcoursTitle = new QLabel(pageParcours);
        lblParcoursTitle->setObjectName("lblParcoursTitle");

        parcoursHeader->addWidget(lblParcoursTitle);

        parcoursHeaderSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        parcoursHeader->addItem(parcoursHeaderSpacer);

        btnRetourParcours = new QPushButton(pageParcours);
        btnRetourParcours->setObjectName("btnRetourParcours");
        btnRetourParcours->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        parcoursHeader->addWidget(btnRetourParcours);


        parcoursLayout->addLayout(parcoursHeader);

        lblParcoursResume = new QLabel(pageParcours);
        lblParcoursResume->setObjectName("lblParcoursResume");
        lblParcoursResume->setWordWrap(true);

        parcoursLayout->addWidget(lblParcoursResume);

        lblParcoursInfo = new QLabel(pageParcours);
        lblParcoursInfo->setObjectName("lblParcoursInfo");
        lblParcoursInfo->setWordWrap(true);

        parcoursLayout->addWidget(lblParcoursInfo);

        parcoursFrame = new QFrame(pageParcours);
        parcoursFrame->setObjectName("parcoursFrame");
        parcoursFrameLayout = new QVBoxLayout(parcoursFrame);
        parcoursFrameLayout->setSpacing(0);
        parcoursFrameLayout->setObjectName("parcoursFrameLayout");
        parcoursFrameLayout->setContentsMargins(0, 0, 0, 0);
        tableParcours = new QTableWidget(parcoursFrame);
        if (tableParcours->columnCount() < 10)
            tableParcours->setColumnCount(10);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableParcours->setHorizontalHeaderItem(0, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableParcours->setHorizontalHeaderItem(1, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableParcours->setHorizontalHeaderItem(2, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableParcours->setHorizontalHeaderItem(3, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableParcours->setHorizontalHeaderItem(4, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableParcours->setHorizontalHeaderItem(5, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableParcours->setHorizontalHeaderItem(6, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableParcours->setHorizontalHeaderItem(7, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableParcours->setHorizontalHeaderItem(8, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableParcours->setHorizontalHeaderItem(9, __qtablewidgetitem16);
        tableParcours->setObjectName("tableParcours");
        tableParcours->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableParcours->setAlternatingRowColors(true);
        tableParcours->setSelectionMode(QAbstractItemView::NoSelection);
        tableParcours->setShowGrid(false);
        tableParcours->setFrameShape(QFrame::NoFrame);
        tableParcours->setWordWrap(false);
        tableParcours->verticalHeader()->setVisible(false);
        tableParcours->verticalHeader()->setDefaultSectionSize(46);

        parcoursFrameLayout->addWidget(tableParcours);


        parcoursLayout->addWidget(parcoursFrame);

        stackedMain->addWidget(pageParcours);
        pageAffectation = new QWidget();
        pageAffectation->setObjectName("pageAffectation");
        affectationLayout = new QVBoxLayout(pageAffectation);
        affectationLayout->setSpacing(8);
        affectationLayout->setObjectName("affectationLayout");
        affectationLayout->setContentsMargins(0, 0, 0, 0);
        affectationHeader = new QHBoxLayout();
        affectationHeader->setSpacing(8);
        affectationHeader->setObjectName("affectationHeader");
        lblAffectationTitle = new QLabel(pageAffectation);
        lblAffectationTitle->setObjectName("lblAffectationTitle");

        affectationHeader->addWidget(lblAffectationTitle);

        affectationHeaderSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        affectationHeader->addItem(affectationHeaderSpacer);

        btnValiderAffectation = new QPushButton(pageAffectation);
        btnValiderAffectation->setObjectName("btnValiderAffectation");
        btnValiderAffectation->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        affectationHeader->addWidget(btnValiderAffectation);

        btnRetourAffectation = new QPushButton(pageAffectation);
        btnRetourAffectation->setObjectName("btnRetourAffectation");
        btnRetourAffectation->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        affectationHeader->addWidget(btnRetourAffectation);


        affectationLayout->addLayout(affectationHeader);

        lblAffectationResume = new QLabel(pageAffectation);
        lblAffectationResume->setObjectName("lblAffectationResume");
        lblAffectationResume->setWordWrap(true);

        affectationLayout->addWidget(lblAffectationResume);

        lblAffectationAvert = new QLabel(pageAffectation);
        lblAffectationAvert->setObjectName("lblAffectationAvert");
        lblAffectationAvert->setWordWrap(true);
        lblAffectationAvert->setVisible(false);

        affectationLayout->addWidget(lblAffectationAvert);

        affectationFrame = new QFrame(pageAffectation);
        affectationFrame->setObjectName("affectationFrame");
        affectationFrameLayout = new QVBoxLayout(affectationFrame);
        affectationFrameLayout->setSpacing(0);
        affectationFrameLayout->setObjectName("affectationFrameLayout");
        affectationFrameLayout->setContentsMargins(0, 0, 0, 0);
        tableAffectation = new QTableWidget(affectationFrame);
        if (tableAffectation->columnCount() < 4)
            tableAffectation->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableAffectation->setHorizontalHeaderItem(0, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableAffectation->setHorizontalHeaderItem(1, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableAffectation->setHorizontalHeaderItem(2, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        tableAffectation->setHorizontalHeaderItem(3, __qtablewidgetitem20);
        tableAffectation->setObjectName("tableAffectation");
        tableAffectation->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableAffectation->setAlternatingRowColors(false);
        tableAffectation->setSelectionMode(QAbstractItemView::NoSelection);
        tableAffectation->setShowGrid(false);
        tableAffectation->setFrameShape(QFrame::NoFrame);
        tableAffectation->setWordWrap(false);
        tableAffectation->verticalHeader()->setVisible(false);
        tableAffectation->verticalHeader()->setDefaultSectionSize(42);

        affectationFrameLayout->addWidget(tableAffectation);


        affectationLayout->addWidget(affectationFrame);

        stackedMain->addWidget(pageAffectation);

        contentLayout->addWidget(stackedMain);

        contentLayout->setStretch(4, 1);

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
        lblTotalTitle->setText(QCoreApplication::translate("GestionInterventions", "Total", nullptr));
        lblTotal->setText(QCoreApplication::translate("GestionInterventions", "0", nullptr));
        lblEnCoursTitle->setText(QCoreApplication::translate("GestionInterventions", "En cours", nullptr));
        lblEnCours->setText(QCoreApplication::translate("GestionInterventions", "0", nullptr));
        lblEnAttenteTitle->setText(QCoreApplication::translate("GestionInterventions", "En attente", nullptr));
        lblEnAttente->setText(QCoreApplication::translate("GestionInterventions", "0", nullptr));
        lblTermineeTitle->setText(QCoreApplication::translate("GestionInterventions", "Termin\303\251e", nullptr));
        lblTerminee->setText(QCoreApplication::translate("GestionInterventions", "0", nullptr));
        btnAjouter->setText(QCoreApplication::translate("GestionInterventions", "Ajouter", nullptr));
#if QT_CONFIG(tooltip)
        btnAjouter->setToolTip(QCoreApplication::translate("GestionInterventions", "Ajouter une intervention (Ctrl+N)", nullptr));
#endif // QT_CONFIG(tooltip)
        btnModifier->setText(QCoreApplication::translate("GestionInterventions", "Modifier", nullptr));
#if QT_CONFIG(tooltip)
        btnModifier->setToolTip(QCoreApplication::translate("GestionInterventions", "Modifier la ligne s\303\251lectionn\303\251e (F2 ou double-clic)", nullptr));
#endif // QT_CONFIG(tooltip)
        btnConsulter->setText(QCoreApplication::translate("GestionInterventions", "Consulter", nullptr));
#if QT_CONFIG(tooltip)
        btnConsulter->setToolTip(QCoreApplication::translate("GestionInterventions", "Afficher la fiche de l'intervention s\303\251lectionn\303\251e", nullptr));
#endif // QT_CONFIG(tooltip)
        btnSupprimer->setText(QCoreApplication::translate("GestionInterventions", "Supprimer", nullptr));
#if QT_CONFIG(tooltip)
        btnSupprimer->setToolTip(QCoreApplication::translate("GestionInterventions", "Supprimer l'intervention s\303\251lectionn\303\251e (Suppr)", nullptr));
#endif // QT_CONFIG(tooltip)
        btnAffectation->setText(QCoreApplication::translate("GestionInterventions", "Affectation automatique", nullptr));
#if QT_CONFIG(tooltip)
        btnAffectation->setToolTip(QCoreApplication::translate("GestionInterventions", "Proposer automatiquement v\303\251hicules, \303\251quipe et \303\251quipements pour l'intervention s\303\251lectionn\303\251e", nullptr));
#endif // QT_CONFIG(tooltip)
        btnParcours->setText(QCoreApplication::translate("GestionInterventions", "Parcours recommand\303\251", nullptr));
#if QT_CONFIG(tooltip)
        btnParcours->setToolTip(QCoreApplication::translate("GestionInterventions", "Ordre recommand\303\251 de traitement des interventions en cours / en attente", nullptr));
#endif // QT_CONFIG(tooltip)
        lblFiltreGravite->setText(QCoreApplication::translate("GestionInterventions", "Gravit\303\251", nullptr));
        comboGravite->setItemText(0, QCoreApplication::translate("GestionInterventions", "Toutes les gravit\303\251s", nullptr));
        comboGravite->setItemText(1, QCoreApplication::translate("GestionInterventions", "Faible", nullptr));
        comboGravite->setItemText(2, QCoreApplication::translate("GestionInterventions", "Moyenne", nullptr));
        comboGravite->setItemText(3, QCoreApplication::translate("GestionInterventions", "\303\211lev\303\251e", nullptr));
        comboGravite->setItemText(4, QCoreApplication::translate("GestionInterventions", "Critique", nullptr));

        lblFiltreStatut->setText(QCoreApplication::translate("GestionInterventions", "Statut", nullptr));
        comboStatut->setItemText(0, QCoreApplication::translate("GestionInterventions", "Tous les statuts", nullptr));
        comboStatut->setItemText(1, QCoreApplication::translate("GestionInterventions", "En cours", nullptr));
        comboStatut->setItemText(2, QCoreApplication::translate("GestionInterventions", "En attente", nullptr));
        comboStatut->setItemText(3, QCoreApplication::translate("GestionInterventions", "Termin\303\251e", nullptr));

        lblTri->setText(QCoreApplication::translate("GestionInterventions", "Trier par", nullptr));
        btnTriId->setText(QCoreApplication::translate("GestionInterventions", "ID", nullptr));
#if QT_CONFIG(tooltip)
        btnTriId->setToolTip(QCoreApplication::translate("GestionInterventions", "Trier par num\303\251ro : 1er clic croissant, 2e clic d\303\251croissant", nullptr));
#endif // QT_CONFIG(tooltip)
        btnTriDate->setText(QCoreApplication::translate("GestionInterventions", "Date", nullptr));
#if QT_CONFIG(tooltip)
        btnTriDate->setToolTip(QCoreApplication::translate("GestionInterventions", "Trier par date : 1er clic du plus r\303\251cent au plus ancien, 2e clic l'inverse", nullptr));
#endif // QT_CONFIG(tooltip)
        lblCompte->setText(QString());
        btnStats->setText(QCoreApplication::translate("GestionInterventions", "Statistiques", nullptr));
#if QT_CONFIG(tooltip)
        btnStats->setToolTip(QCoreApplication::translate("GestionInterventions", "Afficher les statistiques (Ctrl+T)", nullptr));
#endif // QT_CONFIG(tooltip)
        btnExportPdf->setText(QCoreApplication::translate("GestionInterventions", "Exporter PDF", nullptr));
#if QT_CONFIG(tooltip)
        btnExportPdf->setToolTip(QCoreApplication::translate("GestionInterventions", "Exporter la liste affich\303\251e en PDF (Ctrl+P)", nullptr));
#endif // QT_CONFIG(tooltip)
        lblMessage->setText(QString());
        btnBandeauOk->setText(QCoreApplication::translate("GestionInterventions", "Enregistrer", nullptr));
        btnBandeauAnnuler->setText(QCoreApplication::translate("GestionInterventions", "Annuler", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableInterventions->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("GestionInterventions", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableInterventions->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("GestionInterventions", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableInterventions->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("GestionInterventions", "Adresse", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableInterventions->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("GestionInterventions", "Date et heure", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableInterventions->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("GestionInterventions", "Zone", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableInterventions->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("GestionInterventions", "Gravit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableInterventions->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("GestionInterventions", "Statut", nullptr));
        lblStatsTitle->setText(QCoreApplication::translate("GestionInterventions", "Statistiques", nullptr));
        btnRetourListe->setText(QCoreApplication::translate("GestionInterventions", "\342\206\220  Retour \303\240 la liste", nullptr));
#if QT_CONFIG(tooltip)
        btnRetourListe->setToolTip(QCoreApplication::translate("GestionInterventions", "Revenir au tableau des interventions", nullptr));
#endif // QT_CONFIG(tooltip)
        lblParcoursTitle->setText(QCoreApplication::translate("GestionInterventions", "Parcours recommand\303\251", nullptr));
        btnRetourParcours->setText(QCoreApplication::translate("GestionInterventions", "\342\206\220  Retour \303\240 la liste", nullptr));
#if QT_CONFIG(tooltip)
        btnRetourParcours->setToolTip(QCoreApplication::translate("GestionInterventions", "Revenir au tableau des interventions", nullptr));
#endif // QT_CONFIG(tooltip)
        lblParcoursResume->setText(QString());
        lblParcoursInfo->setText(QString());
        QTableWidgetItem *___qtablewidgetitem7 = tableParcours->horizontalHeaderItem(0);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("GestionInterventions", "Ordre", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableParcours->horizontalHeaderItem(1);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("GestionInterventions", "N\302\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableParcours->horizontalHeaderItem(2);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("GestionInterventions", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableParcours->horizontalHeaderItem(3);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("GestionInterventions", "Adresse", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableParcours->horizontalHeaderItem(4);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("GestionInterventions", "Zone", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tableParcours->horizontalHeaderItem(5);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("GestionInterventions", "Gravit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tableParcours->horizontalHeaderItem(6);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("GestionInterventions", "En attente depuis", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tableParcours->horizontalHeaderItem(7);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("GestionInterventions", "Trajet", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = tableParcours->horizontalHeaderItem(8);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("GestionInterventions", "Arriv\303\251e pr\303\251vue", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = tableParcours->horizontalHeaderItem(9);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("GestionInterventions", "Fin pr\303\251vue", nullptr));
        lblAffectationTitle->setText(QCoreApplication::translate("GestionInterventions", "Affectation automatique", nullptr));
        btnValiderAffectation->setText(QCoreApplication::translate("GestionInterventions", "Valider l'affectation", nullptr));
#if QT_CONFIG(tooltip)
        btnValiderAffectation->setToolTip(QCoreApplication::translate("GestionInterventions", "Enregistrer cette affectation dans la base", nullptr));
#endif // QT_CONFIG(tooltip)
        btnRetourAffectation->setText(QCoreApplication::translate("GestionInterventions", "Annuler", nullptr));
#if QT_CONFIG(tooltip)
        btnRetourAffectation->setToolTip(QCoreApplication::translate("GestionInterventions", "Revenir \303\240 la liste sans rien enregistrer", nullptr));
#endif // QT_CONFIG(tooltip)
        lblAffectationResume->setText(QString());
        lblAffectationAvert->setText(QString());
        QTableWidgetItem *___qtablewidgetitem17 = tableAffectation->horizontalHeaderItem(0);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("GestionInterventions", "Ressource", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = tableAffectation->horizontalHeaderItem(1);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("GestionInterventions", "D\303\251signation", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = tableAffectation->horizontalHeaderItem(2);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("GestionInterventions", "D\303\251tail", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = tableAffectation->horizontalHeaderItem(3);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("GestionInterventions", "V\303\251hicule", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GestionInterventions: public Ui_GestionInterventions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GESTIONINTERVENTIONS_H
