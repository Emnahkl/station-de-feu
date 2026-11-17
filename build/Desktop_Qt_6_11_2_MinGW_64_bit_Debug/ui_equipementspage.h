/********************************************************************************
** Form generated from reading UI file 'equipementspage.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EQUIPEMENTSPAGE_H
#define UI_EQUIPEMENTSPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EquipementsPage
{
public:
    QWidget *centralwidget;
    QVBoxLayout *layoutRoot;
    QHBoxLayout *layoutCorps;
    QFrame *frameSidebar;
    QVBoxLayout *layoutSidebar;
    QLabel *lblLogo;
    QLabel *lblNomApp;
    QLabel *lblSlogan;
    QSpacerItem *spacerLogo;
    QPushButton *btnNavDashboard;
    QPushButton *btnNavEmployes;
    QPushButton *btnNavVehicules;
    QPushButton *btnNavInterventions;
    QPushButton *btnNavEquipements;
    QPushButton *btnNavSensibilisation;
    QPushButton *btnNavZones;
    QSpacerItem *spacerNav;
    QLabel *lblMotto;
    QWidget *widgetPrincipal;
    QVBoxLayout *layoutPrincipal;
    QFrame *frameHeader;
    QHBoxLayout *layoutHeader;
    QLabel *lblTitreApp;
    QSpacerItem *spacerHeader;
    QLabel *lblUtilisateur;
    QWidget *widgetBody;
    QVBoxLayout *layoutBody;
    QLabel *lblAlertes;
    QHBoxLayout *layoutCartes;
    QFrame *cardListe;
    QVBoxLayout *layoutCardListe;
    QHBoxLayout *layoutListeTitre;
    QLabel *lblListeTitre;
    QSpacerItem *spacerListe;
    QPushButton *btnNouveau;
    QHBoxLayout *layoutRecherche;
    QLabel *lblRechercher;
    QComboBox *cbChampRecherche;
    QLineEdit *leRecherche;
    QLabel *lblTrier;
    QComboBox *cbTri;
    QComboBox *cbOrdre;
    QTableView *tableView;
    QHBoxLayout *layoutExports;
    QPushButton *btnExportPdf;
    QPushButton *btnExportExcel;
    QPushButton *btnStats;
    QPushButton *btnBonCommande;
    QFrame *cardForm;
    QVBoxLayout *layoutCardForm;
    QLabel *lblFormTitre;
    QGridLayout *gridForm;
    QLabel *lblF0;
    QLineEdit *leId;
    QLabel *lblF1;
    QLineEdit *leNom;
    QLabel *lblF2;
    QComboBox *cbType;
    QLabel *lblF3;
    QSpinBox *sbQuantite;
    QLabel *lblF4;
    QSpinBox *sbSeuil;
    QLabel *lblF5;
    QDateEdit *deAcquisition;
    QLabel *lblF6;
    QComboBox *cbEtat;
    QLabel *lblF7;
    QDateEdit *deControle;
    QLabel *lblF8;
    QLineEdit *leAffectation;
    QGridLayout *gridBtns;
    QPushButton *btnAjouter;
    QPushButton *btnModifier;
    QPushButton *btnSupprimer;
    QPushButton *btnVider;
    QSpacerItem *spacerForm;
    QFrame *frameFooter;
    QHBoxLayout *layoutFooter;
    QLabel *lblFooterG;
    QSpacerItem *spacerFooter;
    QLabel *lblFooterD;

    void setupUi(QMainWindow *EquipementsPage)
    {
        if (EquipementsPage->objectName().isEmpty())
            EquipementsPage->setObjectName("EquipementsPage");
        EquipementsPage->resize(1366, 768);
        EquipementsPage->setStyleSheet(QString::fromUtf8("\n"
"* { font-family: \"Roboto\", \"Segoe UI\", sans-serif; font-size: 12pt; color: #2B2B2B; }\n"
"QMainWindow, QWidget#centralwidget { background: #F4F5F7; }\n"
"\n"
"/* ---------- Sidebar ---------- */\n"
"QFrame#frameSidebar { background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #C62828, stop:0.55 #8E1B1B, stop:1 #3A0B0B); }\n"
"QFrame#frameSidebar QLabel { color: white; background: transparent; }\n"
"QLabel#lblNomApp { font-size: 15pt; font-weight: bold; letter-spacing: 1px; }\n"
"QLabel#lblSlogan { font-size: 8pt; letter-spacing: 2px; color: rgba(255,255,255,200); }\n"
"QLabel#lblMotto { font-size: 11pt; font-weight: bold; }\n"
"QFrame#frameSidebar QPushButton { text-align: left; padding: 10px 14px; color: white; background: transparent; border: none; border-radius: 8px; }\n"
"QFrame#frameSidebar QPushButton:disabled { color: rgba(255,255,255,190); }\n"
"QFrame#frameSidebar QPushButton#btnNavEquipements { background: #E53935; font-weight: bold; }\n"
"\n"
"/* ---------- Header / footer ---------- */\n"
""
                        "QFrame#frameHeader { background: white; border-bottom: 1px solid #E6E6E6; }\n"
"QLabel#lblTitreApp { background: transparent; }\n"
"QLabel#lblUtilisateur { color: #555; background: transparent; }\n"
"QFrame#frameFooter { background: #2B2B2B; }\n"
"QFrame#frameFooter QLabel { color: #DDD; font-size: 9pt; background: transparent; }\n"
"\n"
"/* ---------- Cartes ---------- */\n"
"QFrame#cardListe, QFrame#cardForm { background: white; border: 1px solid #ECECEC; border-radius: 10px; }\n"
"QFrame#cardListe QLabel, QFrame#cardForm QLabel { background: transparent; border: none; }\n"
"QLabel#lblListeTitre, QLabel#lblFormTitre { font-size: 14pt; font-weight: bold; }\n"
"QLabel#lblAlertes { padding: 8px 12px; border-radius: 8px; background: #E8F5E9; color: #2E7D32; font-weight: bold; }\n"
"\n"
"/* ---------- Champs ---------- */\n"
"QLineEdit, QComboBox, QSpinBox, QDateEdit { background: white; border: 1px solid #DADADA; border-radius: 6px; padding: 5px 8px; color: #2B2B2B; min-height: 22px; }\n"
"QLineEdit:focus, QComb"
                        "oBox:focus, QSpinBox:focus, QDateEdit:focus { border: 1px solid #C62828; }\n"
"QLineEdit:read-only { background: #F2F2F2; color: #777; }\n"
"QComboBox::drop-down, QDateEdit::drop-down { border: none; width: 26px; subcontrol-origin: padding; subcontrol-position: center right; }\n"
"QComboBox::down-arrow, QDateEdit::down-arrow { image: url(:/images/arrow_down.png); width: 12px; height: 12px; }\n"
"QSpinBox::up-button { subcontrol-origin: border; subcontrol-position: top right; width: 22px; border: none; }\n"
"QSpinBox::down-button { subcontrol-origin: border; subcontrol-position: bottom right; width: 22px; border: none; }\n"
"QSpinBox::up-arrow { image: url(:/images/arrow_up.png); width: 10px; height: 10px; }\n"
"QSpinBox::down-arrow { image: url(:/images/arrow_down.png); width: 10px; height: 10px; }\n"
"\n"
"/* ---------- Boutons ---------- */\n"
"QPushButton#btnAjouter, QPushButton#btnNouveau { background: #C62828; color: white; border: none; border-radius: 8px; padding: 9px 16px; font-weight: bold; }\n"
"QPus"
                        "hButton#btnAjouter:hover, QPushButton#btnNouveau:hover { background: #B71C1C; }\n"
"QPushButton#btnModifier { background: #FF6F00; color: white; border: none; border-radius: 8px; padding: 9px 16px; font-weight: bold; }\n"
"QPushButton#btnModifier:hover { background: #E65100; }\n"
"QPushButton#btnSupprimer, QPushButton#btnVider, QPushButton#btnExportPdf, QPushButton#btnExportExcel, QPushButton#btnStats, QPushButton#btnBonCommande {\n"
"    background: white; color: #C62828; border: 1px solid #C62828; border-radius: 8px; padding: 8px 14px; font-weight: bold; }\n"
"QPushButton#btnSupprimer:hover, QPushButton#btnVider:hover, QPushButton#btnExportPdf:hover, QPushButton#btnExportExcel:hover, QPushButton#btnStats:hover, QPushButton#btnBonCommande:hover { background: #FDECEA; }\n"
"\n"
"/* ---------- Tableau ---------- */\n"
"QTableView { background: white; alternate-background-color: #FAFAFA; border: 1px solid #ECECEC; border-radius: 6px; gridline-color: transparent; selection-background-color: #FDECEA; selection-col"
                        "or: #2B2B2B; }\n"
"QHeaderView::section { background: #F4F5F7; color: #444; padding: 8px; border: none; border-bottom: 1px solid #E0E0E0; font-weight: bold; }\n"
""));
        centralwidget = new QWidget(EquipementsPage);
        centralwidget->setObjectName("centralwidget");
        layoutRoot = new QVBoxLayout(centralwidget);
        layoutRoot->setSpacing(0);
        layoutRoot->setObjectName("layoutRoot");
        layoutRoot->setContentsMargins(0, 0, 0, 0);
        layoutCorps = new QHBoxLayout();
        layoutCorps->setSpacing(0);
        layoutCorps->setObjectName("layoutCorps");
        layoutCorps->setContentsMargins(0, 0, 0, 0);
        frameSidebar = new QFrame(centralwidget);
        frameSidebar->setObjectName("frameSidebar");
        frameSidebar->setMinimumSize(QSize(240, 0));
        frameSidebar->setMaximumSize(QSize(240, 16777215));
        layoutSidebar = new QVBoxLayout(frameSidebar);
        layoutSidebar->setSpacing(6);
        layoutSidebar->setObjectName("layoutSidebar");
        layoutSidebar->setContentsMargins(14, 18, 14, 14);
        lblLogo = new QLabel(frameSidebar);
        lblLogo->setObjectName("lblLogo");
        lblLogo->setMinimumSize(QSize(120, 120));
        lblLogo->setMaximumSize(QSize(120, 120));
        lblLogo->setPixmap(QPixmap(QString::fromUtf8(":/images/logo.png")));
        lblLogo->setScaledContents(true);

        layoutSidebar->addWidget(lblLogo, 0, Qt::AlignHCenter);

        lblNomApp = new QLabel(frameSidebar);
        lblNomApp->setObjectName("lblNomApp");

        layoutSidebar->addWidget(lblNomApp, 0, Qt::AlignHCenter);

        lblSlogan = new QLabel(frameSidebar);
        lblSlogan->setObjectName("lblSlogan");

        layoutSidebar->addWidget(lblSlogan, 0, Qt::AlignHCenter);

        spacerLogo = new QSpacerItem(20, 14, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        layoutSidebar->addItem(spacerLogo);

        btnNavDashboard = new QPushButton(frameSidebar);
        btnNavDashboard->setObjectName("btnNavDashboard");
        btnNavDashboard->setEnabled(false);

        layoutSidebar->addWidget(btnNavDashboard);

        btnNavEmployes = new QPushButton(frameSidebar);
        btnNavEmployes->setObjectName("btnNavEmployes");
        btnNavEmployes->setEnabled(false);

        layoutSidebar->addWidget(btnNavEmployes);

        btnNavVehicules = new QPushButton(frameSidebar);
        btnNavVehicules->setObjectName("btnNavVehicules");
        btnNavVehicules->setEnabled(false);

        layoutSidebar->addWidget(btnNavVehicules);

        btnNavInterventions = new QPushButton(frameSidebar);
        btnNavInterventions->setObjectName("btnNavInterventions");
        btnNavInterventions->setEnabled(false);

        layoutSidebar->addWidget(btnNavInterventions);

        btnNavEquipements = new QPushButton(frameSidebar);
        btnNavEquipements->setObjectName("btnNavEquipements");

        layoutSidebar->addWidget(btnNavEquipements);

        btnNavSensibilisation = new QPushButton(frameSidebar);
        btnNavSensibilisation->setObjectName("btnNavSensibilisation");
        btnNavSensibilisation->setEnabled(false);

        layoutSidebar->addWidget(btnNavSensibilisation);

        btnNavZones = new QPushButton(frameSidebar);
        btnNavZones->setObjectName("btnNavZones");
        btnNavZones->setEnabled(false);

        layoutSidebar->addWidget(btnNavZones);

        spacerNav = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        layoutSidebar->addItem(spacerNav);

        lblMotto = new QLabel(frameSidebar);
        lblMotto->setObjectName("lblMotto");

        layoutSidebar->addWidget(lblMotto);


        layoutCorps->addWidget(frameSidebar);

        widgetPrincipal = new QWidget(centralwidget);
        widgetPrincipal->setObjectName("widgetPrincipal");
        layoutPrincipal = new QVBoxLayout(widgetPrincipal);
        layoutPrincipal->setSpacing(0);
        layoutPrincipal->setObjectName("layoutPrincipal");
        layoutPrincipal->setContentsMargins(0, 0, 0, 0);
        frameHeader = new QFrame(widgetPrincipal);
        frameHeader->setObjectName("frameHeader");
        frameHeader->setMinimumSize(QSize(0, 68));
        frameHeader->setMaximumSize(QSize(16777215, 68));
        layoutHeader = new QHBoxLayout(frameHeader);
        layoutHeader->setSpacing(8);
        layoutHeader->setObjectName("layoutHeader");
        layoutHeader->setContentsMargins(20, 0, 20, 0);
        lblTitreApp = new QLabel(frameHeader);
        lblTitreApp->setObjectName("lblTitreApp");

        layoutHeader->addWidget(lblTitreApp);

        spacerHeader = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        layoutHeader->addItem(spacerHeader);

        lblUtilisateur = new QLabel(frameHeader);
        lblUtilisateur->setObjectName("lblUtilisateur");

        layoutHeader->addWidget(lblUtilisateur);


        layoutPrincipal->addWidget(frameHeader);

        widgetBody = new QWidget(widgetPrincipal);
        widgetBody->setObjectName("widgetBody");
        layoutBody = new QVBoxLayout(widgetBody);
        layoutBody->setSpacing(10);
        layoutBody->setObjectName("layoutBody");
        layoutBody->setContentsMargins(18, 14, 18, 14);
        lblAlertes = new QLabel(widgetBody);
        lblAlertes->setObjectName("lblAlertes");

        layoutBody->addWidget(lblAlertes);

        layoutCartes = new QHBoxLayout();
        layoutCartes->setSpacing(16);
        layoutCartes->setObjectName("layoutCartes");
        layoutCartes->setContentsMargins(0, 0, 0, 0);
        cardListe = new QFrame(widgetBody);
        cardListe->setObjectName("cardListe");
        layoutCardListe = new QVBoxLayout(cardListe);
        layoutCardListe->setSpacing(12);
        layoutCardListe->setObjectName("layoutCardListe");
        layoutCardListe->setContentsMargins(18, 16, 18, 16);
        layoutListeTitre = new QHBoxLayout();
        layoutListeTitre->setSpacing(8);
        layoutListeTitre->setObjectName("layoutListeTitre");
        layoutListeTitre->setContentsMargins(0, 0, 0, 0);
        lblListeTitre = new QLabel(cardListe);
        lblListeTitre->setObjectName("lblListeTitre");

        layoutListeTitre->addWidget(lblListeTitre);

        spacerListe = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        layoutListeTitre->addItem(spacerListe);

        btnNouveau = new QPushButton(cardListe);
        btnNouveau->setObjectName("btnNouveau");

        layoutListeTitre->addWidget(btnNouveau);


        layoutCardListe->addLayout(layoutListeTitre);

        layoutRecherche = new QHBoxLayout();
        layoutRecherche->setSpacing(8);
        layoutRecherche->setObjectName("layoutRecherche");
        layoutRecherche->setContentsMargins(0, 0, 0, 0);
        lblRechercher = new QLabel(cardListe);
        lblRechercher->setObjectName("lblRechercher");

        layoutRecherche->addWidget(lblRechercher);

        cbChampRecherche = new QComboBox(cardListe);
        cbChampRecherche->addItem(QString());
        cbChampRecherche->addItem(QString());
        cbChampRecherche->addItem(QString());
        cbChampRecherche->addItem(QString());
        cbChampRecherche->setObjectName("cbChampRecherche");

        layoutRecherche->addWidget(cbChampRecherche);

        leRecherche = new QLineEdit(cardListe);
        leRecherche->setObjectName("leRecherche");
        leRecherche->setMinimumSize(QSize(200, 0));
        leRecherche->setMaximumSize(QSize(16777215, 16777215));
        leRecherche->setClearButtonEnabled(true);

        layoutRecherche->addWidget(leRecherche);

        lblTrier = new QLabel(cardListe);
        lblTrier->setObjectName("lblTrier");

        layoutRecherche->addWidget(lblTrier);

        cbTri = new QComboBox(cardListe);
        cbTri->addItem(QString());
        cbTri->addItem(QString());
        cbTri->addItem(QString());
        cbTri->addItem(QString());
        cbTri->addItem(QString());
        cbTri->addItem(QString());
        cbTri->addItem(QString());
        cbTri->setObjectName("cbTri");

        layoutRecherche->addWidget(cbTri);

        cbOrdre = new QComboBox(cardListe);
        cbOrdre->addItem(QString());
        cbOrdre->addItem(QString());
        cbOrdre->setObjectName("cbOrdre");

        layoutRecherche->addWidget(cbOrdre);


        layoutCardListe->addLayout(layoutRecherche);

        tableView = new QTableView(cardListe);
        tableView->setObjectName("tableView");
        tableView->setAlternatingRowColors(true);
        tableView->setShowGrid(false);
        tableView->verticalHeader()->setVisible(false);

        layoutCardListe->addWidget(tableView);

        layoutExports = new QHBoxLayout();
        layoutExports->setSpacing(10);
        layoutExports->setObjectName("layoutExports");
        layoutExports->setContentsMargins(0, 0, 0, 0);
        btnExportPdf = new QPushButton(cardListe);
        btnExportPdf->setObjectName("btnExportPdf");

        layoutExports->addWidget(btnExportPdf);

        btnExportExcel = new QPushButton(cardListe);
        btnExportExcel->setObjectName("btnExportExcel");

        layoutExports->addWidget(btnExportExcel);

        btnStats = new QPushButton(cardListe);
        btnStats->setObjectName("btnStats");

        layoutExports->addWidget(btnStats);

        btnBonCommande = new QPushButton(cardListe);
        btnBonCommande->setObjectName("btnBonCommande");

        layoutExports->addWidget(btnBonCommande);


        layoutCardListe->addLayout(layoutExports);


        layoutCartes->addWidget(cardListe);

        cardForm = new QFrame(widgetBody);
        cardForm->setObjectName("cardForm");
        cardForm->setMinimumSize(QSize(400, 0));
        cardForm->setMaximumSize(QSize(400, 16777215));
        layoutCardForm = new QVBoxLayout(cardForm);
        layoutCardForm->setSpacing(14);
        layoutCardForm->setObjectName("layoutCardForm");
        layoutCardForm->setContentsMargins(18, 16, 18, 16);
        lblFormTitre = new QLabel(cardForm);
        lblFormTitre->setObjectName("lblFormTitre");

        layoutCardForm->addWidget(lblFormTitre);

        gridForm = new QGridLayout();
        gridForm->setObjectName("gridForm");
        gridForm->setVerticalSpacing(8);
        gridForm->setHorizontalSpacing(10);
        lblF0 = new QLabel(cardForm);
        lblF0->setObjectName("lblF0");

        gridForm->addWidget(lblF0, 0, 0, 1, 1);

        leId = new QLineEdit(cardForm);
        leId->setObjectName("leId");
        leId->setReadOnly(true);

        gridForm->addWidget(leId, 0, 1, 1, 1);

        lblF1 = new QLabel(cardForm);
        lblF1->setObjectName("lblF1");

        gridForm->addWidget(lblF1, 1, 0, 1, 1);

        leNom = new QLineEdit(cardForm);
        leNom->setObjectName("leNom");

        gridForm->addWidget(leNom, 1, 1, 1, 1);

        lblF2 = new QLabel(cardForm);
        lblF2->setObjectName("lblF2");

        gridForm->addWidget(lblF2, 2, 0, 1, 1);

        cbType = new QComboBox(cardForm);
        cbType->addItem(QString());
        cbType->addItem(QString());
        cbType->addItem(QString());
        cbType->addItem(QString());
        cbType->addItem(QString());
        cbType->setObjectName("cbType");

        gridForm->addWidget(cbType, 2, 1, 1, 1);

        lblF3 = new QLabel(cardForm);
        lblF3->setObjectName("lblF3");

        gridForm->addWidget(lblF3, 3, 0, 1, 1);

        sbQuantite = new QSpinBox(cardForm);
        sbQuantite->setObjectName("sbQuantite");
        sbQuantite->setMaximum(100000);
        sbQuantite->setValue(0);

        gridForm->addWidget(sbQuantite, 3, 1, 1, 1);

        lblF4 = new QLabel(cardForm);
        lblF4->setObjectName("lblF4");

        gridForm->addWidget(lblF4, 4, 0, 1, 1);

        sbSeuil = new QSpinBox(cardForm);
        sbSeuil->setObjectName("sbSeuil");
        sbSeuil->setMaximum(100000);
        sbSeuil->setValue(5);

        gridForm->addWidget(sbSeuil, 4, 1, 1, 1);

        lblF5 = new QLabel(cardForm);
        lblF5->setObjectName("lblF5");

        gridForm->addWidget(lblF5, 5, 0, 1, 1);

        deAcquisition = new QDateEdit(cardForm);
        deAcquisition->setObjectName("deAcquisition");
        deAcquisition->setCalendarPopup(true);

        gridForm->addWidget(deAcquisition, 5, 1, 1, 1);

        lblF6 = new QLabel(cardForm);
        lblF6->setObjectName("lblF6");

        gridForm->addWidget(lblF6, 6, 0, 1, 1);

        cbEtat = new QComboBox(cardForm);
        cbEtat->addItem(QString());
        cbEtat->addItem(QString());
        cbEtat->addItem(QString());
        cbEtat->setObjectName("cbEtat");

        gridForm->addWidget(cbEtat, 6, 1, 1, 1);

        lblF7 = new QLabel(cardForm);
        lblF7->setObjectName("lblF7");

        gridForm->addWidget(lblF7, 7, 0, 1, 1);

        deControle = new QDateEdit(cardForm);
        deControle->setObjectName("deControle");
        deControle->setCalendarPopup(true);

        gridForm->addWidget(deControle, 7, 1, 1, 1);

        lblF8 = new QLabel(cardForm);
        lblF8->setObjectName("lblF8");

        gridForm->addWidget(lblF8, 8, 0, 1, 1);

        leAffectation = new QLineEdit(cardForm);
        leAffectation->setObjectName("leAffectation");

        gridForm->addWidget(leAffectation, 8, 1, 1, 1);


        layoutCardForm->addLayout(gridForm);

        gridBtns = new QGridLayout();
        gridBtns->setSpacing(8);
        gridBtns->setObjectName("gridBtns");
        btnAjouter = new QPushButton(cardForm);
        btnAjouter->setObjectName("btnAjouter");

        gridBtns->addWidget(btnAjouter, 0, 0, 1, 1);

        btnModifier = new QPushButton(cardForm);
        btnModifier->setObjectName("btnModifier");

        gridBtns->addWidget(btnModifier, 0, 1, 1, 1);

        btnSupprimer = new QPushButton(cardForm);
        btnSupprimer->setObjectName("btnSupprimer");

        gridBtns->addWidget(btnSupprimer, 1, 0, 1, 1);

        btnVider = new QPushButton(cardForm);
        btnVider->setObjectName("btnVider");

        gridBtns->addWidget(btnVider, 1, 1, 1, 1);


        layoutCardForm->addLayout(gridBtns);

        spacerForm = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        layoutCardForm->addItem(spacerForm);


        layoutCartes->addWidget(cardForm);


        layoutBody->addLayout(layoutCartes);


        layoutPrincipal->addWidget(widgetBody);


        layoutCorps->addWidget(widgetPrincipal);


        layoutRoot->addLayout(layoutCorps);

        frameFooter = new QFrame(centralwidget);
        frameFooter->setObjectName("frameFooter");
        frameFooter->setMinimumSize(QSize(0, 34));
        frameFooter->setMaximumSize(QSize(16777215, 34));
        layoutFooter = new QHBoxLayout(frameFooter);
        layoutFooter->setSpacing(8);
        layoutFooter->setObjectName("layoutFooter");
        layoutFooter->setContentsMargins(18, 0, 18, 0);
        lblFooterG = new QLabel(frameFooter);
        lblFooterG->setObjectName("lblFooterG");

        layoutFooter->addWidget(lblFooterG);

        spacerFooter = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        layoutFooter->addItem(spacerFooter);

        lblFooterD = new QLabel(frameFooter);
        lblFooterD->setObjectName("lblFooterD");

        layoutFooter->addWidget(lblFooterD);


        layoutRoot->addWidget(frameFooter);

        EquipementsPage->setCentralWidget(centralwidget);

        retranslateUi(EquipementsPage);

        QMetaObject::connectSlotsByName(EquipementsPage);
    } // setupUi

    void retranslateUi(QMainWindow *EquipementsPage)
    {
        EquipementsPage->setWindowTitle(QCoreApplication::translate("EquipementsPage", "FireStation Manager - Gestion des \303\251quipements", nullptr));
        lblNomApp->setText(QCoreApplication::translate("EquipementsPage", "FIRE STATION", nullptr));
        lblSlogan->setText(QCoreApplication::translate("EquipementsPage", "PR\303\212TS \302\267 PROT\303\211GER \302\267 SAUVER", nullptr));
        btnNavDashboard->setText(QCoreApplication::translate("EquipementsPage", "\360\237\217\240  Tableau de bord", nullptr));
#if QT_CONFIG(tooltip)
        btnNavDashboard->setToolTip(QCoreApplication::translate("EquipementsPage", "Module g\303\251r\303\251 par un autre membre de l'\303\251quipe", nullptr));
#endif // QT_CONFIG(tooltip)
        btnNavEmployes->setText(QCoreApplication::translate("EquipementsPage", "\360\237\221\245  Employ\303\251s", nullptr));
#if QT_CONFIG(tooltip)
        btnNavEmployes->setToolTip(QCoreApplication::translate("EquipementsPage", "Module g\303\251r\303\251 par un autre membre de l'\303\251quipe", nullptr));
#endif // QT_CONFIG(tooltip)
        btnNavVehicules->setText(QCoreApplication::translate("EquipementsPage", "\360\237\232\222  V\303\251hicules", nullptr));
#if QT_CONFIG(tooltip)
        btnNavVehicules->setToolTip(QCoreApplication::translate("EquipementsPage", "Module g\303\251r\303\251 par un autre membre de l'\303\251quipe", nullptr));
#endif // QT_CONFIG(tooltip)
        btnNavInterventions->setText(QCoreApplication::translate("EquipementsPage", "\360\237\232\250  Interventions", nullptr));
#if QT_CONFIG(tooltip)
        btnNavInterventions->setToolTip(QCoreApplication::translate("EquipementsPage", "Module g\303\251r\303\251 par un autre membre de l'\303\251quipe", nullptr));
#endif // QT_CONFIG(tooltip)
        btnNavEquipements->setText(QCoreApplication::translate("EquipementsPage", "\360\237\247\260  \303\211quipements", nullptr));
        btnNavSensibilisation->setText(QCoreApplication::translate("EquipementsPage", "\360\237\223\243  Sensibilisation", nullptr));
#if QT_CONFIG(tooltip)
        btnNavSensibilisation->setToolTip(QCoreApplication::translate("EquipementsPage", "Module g\303\251r\303\251 par un autre membre de l'\303\251quipe", nullptr));
#endif // QT_CONFIG(tooltip)
        btnNavZones->setText(QCoreApplication::translate("EquipementsPage", "\360\237\227\272  Zones de couverture", nullptr));
#if QT_CONFIG(tooltip)
        btnNavZones->setToolTip(QCoreApplication::translate("EquipementsPage", "Module g\303\251r\303\251 par un autre membre de l'\303\251quipe", nullptr));
#endif // QT_CONFIG(tooltip)
        lblMotto->setText(QCoreApplication::translate("EquipementsPage", "Ensemble\n"
"pour sauver des vies", nullptr));
        lblTitreApp->setText(QCoreApplication::translate("EquipementsPage", "<span style=\" font-size:16pt; font-weight:700; color:#C62828;\">FIRE STATION</span><br/><span style=\" font-size:12pt; font-weight:600;\">Gestion des \303\211quipements</span>", nullptr));
        lblUtilisateur->setText(QCoreApplication::translate("EquipementsPage", "Responsable logistique / Magasinier", nullptr));
        lblAlertes->setText(QString());
        lblListeTitre->setText(QCoreApplication::translate("EquipementsPage", "Liste des \303\251quipements", nullptr));
        btnNouveau->setText(QCoreApplication::translate("EquipementsPage", "+  Ajouter un \303\251quipement", nullptr));
        lblRechercher->setText(QCoreApplication::translate("EquipementsPage", "Rechercher par", nullptr));
        cbChampRecherche->setItemText(0, QCoreApplication::translate("EquipementsPage", "Nom", nullptr));
        cbChampRecherche->setItemText(1, QCoreApplication::translate("EquipementsPage", "Type", nullptr));
        cbChampRecherche->setItemText(2, QCoreApplication::translate("EquipementsPage", "\303\211tat", nullptr));
        cbChampRecherche->setItemText(3, QCoreApplication::translate("EquipementsPage", "Affectation", nullptr));

        leRecherche->setPlaceholderText(QCoreApplication::translate("EquipementsPage", "Saisir un texte...", nullptr));
        lblTrier->setText(QCoreApplication::translate("EquipementsPage", "Trier par", nullptr));
        cbTri->setItemText(0, QCoreApplication::translate("EquipementsPage", "ID", nullptr));
        cbTri->setItemText(1, QCoreApplication::translate("EquipementsPage", "Nom", nullptr));
        cbTri->setItemText(2, QCoreApplication::translate("EquipementsPage", "Type", nullptr));
        cbTri->setItemText(3, QCoreApplication::translate("EquipementsPage", "Quantit\303\251", nullptr));
        cbTri->setItemText(4, QCoreApplication::translate("EquipementsPage", "Date d'acquisition", nullptr));
        cbTri->setItemText(5, QCoreApplication::translate("EquipementsPage", "\303\211tat", nullptr));
        cbTri->setItemText(6, QCoreApplication::translate("EquipementsPage", "Prochain contr\303\264le", nullptr));

        cbOrdre->setItemText(0, QCoreApplication::translate("EquipementsPage", "Croissant", nullptr));
        cbOrdre->setItemText(1, QCoreApplication::translate("EquipementsPage", "D\303\251croissant", nullptr));

        btnExportPdf->setText(QCoreApplication::translate("EquipementsPage", "\342\254\207  Exporter PDF", nullptr));
        btnExportExcel->setText(QCoreApplication::translate("EquipementsPage", "\342\254\207  Exporter Excel", nullptr));
        btnStats->setText(QCoreApplication::translate("EquipementsPage", "\360\237\223\212  Statistiques", nullptr));
        btnBonCommande->setText(QCoreApplication::translate("EquipementsPage", "\360\237\247\276  Bon de commande", nullptr));
        lblFormTitre->setText(QCoreApplication::translate("EquipementsPage", "Ajouter / Modifier un \303\251quipement", nullptr));
        lblF0->setText(QCoreApplication::translate("EquipementsPage", "ID", nullptr));
        leId->setPlaceholderText(QCoreApplication::translate("EquipementsPage", "auto", nullptr));
        lblF1->setText(QCoreApplication::translate("EquipementsPage", "Nom *", nullptr));
        lblF2->setText(QCoreApplication::translate("EquipementsPage", "Type", nullptr));
        cbType->setItemText(0, QCoreApplication::translate("EquipementsPage", "EPI", nullptr));
        cbType->setItemText(1, QCoreApplication::translate("EquipementsPage", "Mat\303\251riel de sauvetage", nullptr));
        cbType->setItemText(2, QCoreApplication::translate("EquipementsPage", "Extincteur", nullptr));
        cbType->setItemText(3, QCoreApplication::translate("EquipementsPage", "Tuyaux", nullptr));
        cbType->setItemText(4, QCoreApplication::translate("EquipementsPage", "Autre", nullptr));

        lblF3->setText(QCoreApplication::translate("EquipementsPage", "Quantit\303\251", nullptr));
        lblF4->setText(QCoreApplication::translate("EquipementsPage", "Seuil d'alerte", nullptr));
        lblF5->setText(QCoreApplication::translate("EquipementsPage", "Date d'acquisition", nullptr));
        deAcquisition->setDisplayFormat(QCoreApplication::translate("EquipementsPage", "dd/MM/yyyy", nullptr));
        lblF6->setText(QCoreApplication::translate("EquipementsPage", "\303\211tat", nullptr));
        cbEtat->setItemText(0, QCoreApplication::translate("EquipementsPage", "Fonctionnel", nullptr));
        cbEtat->setItemText(1, QCoreApplication::translate("EquipementsPage", "En panne", nullptr));
        cbEtat->setItemText(2, QCoreApplication::translate("EquipementsPage", "En r\303\251paration", nullptr));

        lblF7->setText(QCoreApplication::translate("EquipementsPage", "Prochain contr\303\264le", nullptr));
        deControle->setDisplayFormat(QCoreApplication::translate("EquipementsPage", "dd/MM/yyyy", nullptr));
        lblF8->setText(QCoreApplication::translate("EquipementsPage", "Caserne / V\303\251hicule", nullptr));
        btnAjouter->setText(QCoreApplication::translate("EquipementsPage", "Ajouter", nullptr));
        btnModifier->setText(QCoreApplication::translate("EquipementsPage", "Modifier", nullptr));
        btnSupprimer->setText(QCoreApplication::translate("EquipementsPage", "Supprimer", nullptr));
        btnVider->setText(QCoreApplication::translate("EquipementsPage", "R\303\251initialiser", nullptr));
        lblFooterG->setText(QCoreApplication::translate("EquipementsPage", "<b>Fire Station</b>   v1.0.0   |   Caserne Centrale - Tunis", nullptr));
        lblFooterD->setText(QCoreApplication::translate("EquipementsPage", "\302\253 Prot\303\251ger aujourd'hui, sauver demain \302\273", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EquipementsPage: public Ui_EquipementsPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EQUIPEMENTSPAGE_H
