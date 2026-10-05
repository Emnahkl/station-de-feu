/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *mainLayout;
    QWidget *sidebar;
    QVBoxLayout *sidebarLayout;
    QLabel *labelLogo;
    QPushButton *btnMenuDashboard;
    QPushButton *btnMenuInterventions;
    QPushButton *btnMenuEquipe;
    QPushButton *btnMenuVehicules;
    QPushButton *btnMenuCarte;
    QPushButton *btnMenuCampagne;
    QPushButton *btnMenuVolontaires;
    QPushButton *btnMenuParametres;
    QSpacerItem *sidebarSpacer;
    QWidget *contentWidget;
    QVBoxLayout *contentLayout;
    QFrame *topBar;
    QHBoxLayout *topBarLayout;
    QPushButton *btnHamburger;
    QLineEdit *lineEditSearch;
    QSpacerItem *topSpacer;
    QPushButton *btnAlerte;
    QPushButton *btnTopParam;
    QLabel *labelAvatarTop;
    QLabel *labelUserTop;
    QPushButton *btnPower;
    QWidget *bodyWidget;
    QHBoxLayout *bodyLayout;
    QWidget *leftColumn;
    QVBoxLayout *leftLayout;
    QFrame *filterFrame;
    QHBoxLayout *filterLayout;
    QLabel *labelFiltreGrade;
    QComboBox *comboGrade;
    QLabel *labelFiltreSpec;
    QComboBox *comboSpec;
    QSpacerItem *filterSpacer;
    QFrame *actionFrame;
    QHBoxLayout *actionLayout;
    QPushButton *btnQr;
    QPushButton *btnFaceId;
    QPushButton *btnAjouterVolontaire;
    QPushButton *btnAjouterEmploye;
    QPushButton *btnSupprimerEmploye;
    QSpacerItem *actionSpacer;
    QFrame *formPanel;
    QVBoxLayout *formLayout;
    QLabel *labelFormTitle;
    QHBoxLayout *formRow1;
    QLabel *labelNom;
    QLineEdit *lineEditNom;
    QLabel *labelFormGrade;
    QComboBox *comboFormGrade;
    QLabel *labelFormSpec;
    QComboBox *comboFormSpec;
    QLabel *labelFormStatut;
    QComboBox *comboFormStatut;
    QSpacerItem *formRow1Spacer;
    QHBoxLayout *formRow2;
    QLabel *labelFormTarget;
    QComboBox *comboFormTarget;
    QLabel *labelFormGarde;
    QComboBox *comboFormGarde;
    QLabel *labelFormCode;
    QLineEdit *lineEditCode;
    QSpacerItem *formRow2Spacer;
    QHBoxLayout *formInfoRow;
    QLabel *labelFormQr;
    QLabel *labelFormInfo;
    QHBoxLayout *formBtnRow;
    QSpacerItem *formBtnSpacer;
    QPushButton *btnFormAnnuler;
    QPushButton *btnFormValider;
    QTableWidget *tableEmployees;
    QWidget *rightColumn;
    QVBoxLayout *rightLayout;
    QFrame *chatFrame;
    QVBoxLayout *chatFrameLayout;
    QLabel *labelChatTitle;
    QScrollArea *chatScroll;
    QWidget *chatContent;
    QVBoxLayout *chatLayout;
    QSpacerItem *chatSpacer;
    QHBoxLayout *chatInputRow;
    QLineEdit *lineEditChat;
    QPushButton *btnSendChat;
    QFrame *planFrame;
    QVBoxLayout *planLayout;
    QLabel *labelPlanTitle;
    QHBoxLayout *planHeader;
    QLabel *labelUnite;
    QSpacerItem *planHS;
    QLabel *labelP1;
    QLabel *labelP2;
    QLabel *labelP3;
    QProgressBar *barPlan1;
    QProgressBar *barPlan2;
    QProgressBar *barPlan3;
    QHBoxLayout *dispoRow;
    QLabel *labelDispo;
    QProgressBar *barDispo;
    QHBoxLayout *progRow;
    QLabel *labelProgress;
    QProgressBar *barProgress;
    QHBoxLayout *planBtnRow;
    QPushButton *btnNotifier;
    QPushButton *btnAlerteGrade;
    QPushButton *btnGenererPlanning;
    QFrame *commFrame;
    QVBoxLayout *commLayout;
    QLabel *labelCommTitle;
    QLabel *labelEmail;
    QHBoxLayout *emailRow;
    QLineEdit *lineEditEmail;
    QPushButton *btnEnvoyerEmail;
    QLabel *labelSms;
    QLineEdit *lineEditSms;
    QHBoxLayout *smsRow;
    QPushButton *btnEnvoyerSms;
    QPushButton *btnEnvoyerWhats;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1366, 768);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QHBoxLayout(centralwidget);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        sidebar = new QWidget(centralwidget);
        sidebar->setObjectName("sidebar");
        sidebar->setMinimumSize(QSize(200, 0));
        sidebar->setMaximumSize(QSize(200, 16777215));
        sidebarLayout = new QVBoxLayout(sidebar);
        sidebarLayout->setSpacing(2);
        sidebarLayout->setObjectName("sidebarLayout");
        sidebarLayout->setContentsMargins(0, 10, 0, 0);
        labelLogo = new QLabel(sidebar);
        labelLogo->setObjectName("labelLogo");
        labelLogo->setAlignment(Qt::AlignCenter);
        labelLogo->setMinimumSize(QSize(0, 150));

        sidebarLayout->addWidget(labelLogo);

        btnMenuDashboard = new QPushButton(sidebar);
        btnMenuDashboard->setObjectName("btnMenuDashboard");
        btnMenuDashboard->setCheckable(true);
        btnMenuDashboard->setAutoExclusive(true);
        btnMenuDashboard->setFlat(true);

        sidebarLayout->addWidget(btnMenuDashboard);

        btnMenuInterventions = new QPushButton(sidebar);
        btnMenuInterventions->setObjectName("btnMenuInterventions");
        btnMenuInterventions->setCheckable(true);
        btnMenuInterventions->setAutoExclusive(true);
        btnMenuInterventions->setFlat(true);

        sidebarLayout->addWidget(btnMenuInterventions);

        btnMenuEquipe = new QPushButton(sidebar);
        btnMenuEquipe->setObjectName("btnMenuEquipe");
        btnMenuEquipe->setCheckable(true);
        btnMenuEquipe->setAutoExclusive(true);
        btnMenuEquipe->setFlat(true);

        sidebarLayout->addWidget(btnMenuEquipe);

        btnMenuVehicules = new QPushButton(sidebar);
        btnMenuVehicules->setObjectName("btnMenuVehicules");
        btnMenuVehicules->setCheckable(true);
        btnMenuVehicules->setAutoExclusive(true);
        btnMenuVehicules->setFlat(true);

        sidebarLayout->addWidget(btnMenuVehicules);

        btnMenuCarte = new QPushButton(sidebar);
        btnMenuCarte->setObjectName("btnMenuCarte");
        btnMenuCarte->setCheckable(true);
        btnMenuCarte->setAutoExclusive(true);
        btnMenuCarte->setFlat(true);

        sidebarLayout->addWidget(btnMenuCarte);

        btnMenuCampagne = new QPushButton(sidebar);
        btnMenuCampagne->setObjectName("btnMenuCampagne");
        btnMenuCampagne->setCheckable(true);
        btnMenuCampagne->setAutoExclusive(true);
        btnMenuCampagne->setFlat(true);

        sidebarLayout->addWidget(btnMenuCampagne);

        btnMenuVolontaires = new QPushButton(sidebar);
        btnMenuVolontaires->setObjectName("btnMenuVolontaires");
        btnMenuVolontaires->setCheckable(true);
        btnMenuVolontaires->setAutoExclusive(true);
        btnMenuVolontaires->setFlat(true);

        sidebarLayout->addWidget(btnMenuVolontaires);

        btnMenuParametres = new QPushButton(sidebar);
        btnMenuParametres->setObjectName("btnMenuParametres");
        btnMenuParametres->setCheckable(true);
        btnMenuParametres->setAutoExclusive(true);
        btnMenuParametres->setFlat(true);

        sidebarLayout->addWidget(btnMenuParametres);

        sidebarSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        sidebarLayout->addItem(sidebarSpacer);


        mainLayout->addWidget(sidebar);

        contentWidget = new QWidget(centralwidget);
        contentWidget->setObjectName("contentWidget");
        contentLayout = new QVBoxLayout(contentWidget);
        contentLayout->setSpacing(0);
        contentLayout->setObjectName("contentLayout");
        contentLayout->setContentsMargins(0, 0, 0, 0);
        topBar = new QFrame(contentWidget);
        topBar->setObjectName("topBar");
        topBar->setMinimumSize(QSize(0, 58));
        topBar->setMaximumSize(QSize(16777215, 58));
        topBarLayout = new QHBoxLayout(topBar);
        topBarLayout->setSpacing(10);
        topBarLayout->setObjectName("topBarLayout");
        topBarLayout->setContentsMargins(14, 0, 14, 0);
        btnHamburger = new QPushButton(topBar);
        btnHamburger->setObjectName("btnHamburger");

        topBarLayout->addWidget(btnHamburger);

        lineEditSearch = new QLineEdit(topBar);
        lineEditSearch->setObjectName("lineEditSearch");
        lineEditSearch->setClearButtonEnabled(true);
        lineEditSearch->setMinimumSize(QSize(260, 32));

        topBarLayout->addWidget(lineEditSearch);

        topSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        topBarLayout->addItem(topSpacer);

        btnAlerte = new QPushButton(topBar);
        btnAlerte->setObjectName("btnAlerte");
        btnAlerte->setFlat(true);

        topBarLayout->addWidget(btnAlerte);

        btnTopParam = new QPushButton(topBar);
        btnTopParam->setObjectName("btnTopParam");
        btnTopParam->setFlat(true);

        topBarLayout->addWidget(btnTopParam);

        labelAvatarTop = new QLabel(topBar);
        labelAvatarTop->setObjectName("labelAvatarTop");

        topBarLayout->addWidget(labelAvatarTop);

        labelUserTop = new QLabel(topBar);
        labelUserTop->setObjectName("labelUserTop");

        topBarLayout->addWidget(labelUserTop);

        btnPower = new QPushButton(topBar);
        btnPower->setObjectName("btnPower");
        btnPower->setFlat(true);

        topBarLayout->addWidget(btnPower);


        contentLayout->addWidget(topBar);

        bodyWidget = new QWidget(contentWidget);
        bodyWidget->setObjectName("bodyWidget");
        bodyLayout = new QHBoxLayout(bodyWidget);
        bodyLayout->setSpacing(12);
        bodyLayout->setObjectName("bodyLayout");
        bodyLayout->setContentsMargins(12, 12, 12, 12);
        leftColumn = new QWidget(bodyWidget);
        leftColumn->setObjectName("leftColumn");
        leftLayout = new QVBoxLayout(leftColumn);
        leftLayout->setSpacing(10);
        leftLayout->setObjectName("leftLayout");
        leftLayout->setContentsMargins(0, 0, 0, 0);
        filterFrame = new QFrame(leftColumn);
        filterFrame->setObjectName("filterFrame");
        filterLayout = new QHBoxLayout(filterFrame);
        filterLayout->setSpacing(10);
        filterLayout->setObjectName("filterLayout");
        filterLayout->setContentsMargins(12, 8, 12, 8);
        labelFiltreGrade = new QLabel(filterFrame);
        labelFiltreGrade->setObjectName("labelFiltreGrade");

        filterLayout->addWidget(labelFiltreGrade);

        comboGrade = new QComboBox(filterFrame);
        comboGrade->setObjectName("comboGrade");
        comboGrade->setMinimumSize(QSize(190, 34));

        filterLayout->addWidget(comboGrade);

        labelFiltreSpec = new QLabel(filterFrame);
        labelFiltreSpec->setObjectName("labelFiltreSpec");

        filterLayout->addWidget(labelFiltreSpec);

        comboSpec = new QComboBox(filterFrame);
        comboSpec->setObjectName("comboSpec");
        comboSpec->setMinimumSize(QSize(190, 34));

        filterLayout->addWidget(comboSpec);

        filterSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        filterLayout->addItem(filterSpacer);


        leftLayout->addWidget(filterFrame);

        actionFrame = new QFrame(leftColumn);
        actionFrame->setObjectName("actionFrame");
        actionLayout = new QHBoxLayout(actionFrame);
        actionLayout->setSpacing(8);
        actionLayout->setObjectName("actionLayout");
        actionLayout->setContentsMargins(8, 6, 8, 6);
        btnQr = new QPushButton(actionFrame);
        btnQr->setObjectName("btnQr");
        btnQr->setFlat(true);

        actionLayout->addWidget(btnQr);

        btnFaceId = new QPushButton(actionFrame);
        btnFaceId->setObjectName("btnFaceId");
        btnFaceId->setFlat(true);

        actionLayout->addWidget(btnFaceId);

        btnAjouterVolontaire = new QPushButton(actionFrame);
        btnAjouterVolontaire->setObjectName("btnAjouterVolontaire");
        btnAjouterVolontaire->setFlat(true);

        actionLayout->addWidget(btnAjouterVolontaire);

        btnAjouterEmploye = new QPushButton(actionFrame);
        btnAjouterEmploye->setObjectName("btnAjouterEmploye");
        btnAjouterEmploye->setFlat(true);

        actionLayout->addWidget(btnAjouterEmploye);

        btnSupprimerEmploye = new QPushButton(actionFrame);
        btnSupprimerEmploye->setObjectName("btnSupprimerEmploye");
        btnSupprimerEmploye->setFlat(true);

        actionLayout->addWidget(btnSupprimerEmploye);

        actionSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        actionLayout->addItem(actionSpacer);


        leftLayout->addWidget(actionFrame);

        formPanel = new QFrame(leftColumn);
        formPanel->setObjectName("formPanel");
        formPanel->setVisible(false);
        formLayout = new QVBoxLayout(formPanel);
        formLayout->setSpacing(8);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(14, 10, 14, 10);
        labelFormTitle = new QLabel(formPanel);
        labelFormTitle->setObjectName("labelFormTitle");

        formLayout->addWidget(labelFormTitle);

        formRow1 = new QHBoxLayout();
        formRow1->setSpacing(8);
        formRow1->setObjectName("formRow1");
        formRow1->setContentsMargins(0, 0, 0, 0);
        labelNom = new QLabel(formPanel);
        labelNom->setObjectName("labelNom");

        formRow1->addWidget(labelNom);

        lineEditNom = new QLineEdit(formPanel);
        lineEditNom->setObjectName("lineEditNom");
        lineEditNom->setMinimumSize(QSize(170, 30));

        formRow1->addWidget(lineEditNom);

        labelFormGrade = new QLabel(formPanel);
        labelFormGrade->setObjectName("labelFormGrade");

        formRow1->addWidget(labelFormGrade);

        comboFormGrade = new QComboBox(formPanel);
        comboFormGrade->setObjectName("comboFormGrade");
        comboFormGrade->setMinimumSize(QSize(110, 30));

        formRow1->addWidget(comboFormGrade);

        labelFormSpec = new QLabel(formPanel);
        labelFormSpec->setObjectName("labelFormSpec");

        formRow1->addWidget(labelFormSpec);

        comboFormSpec = new QComboBox(formPanel);
        comboFormSpec->setObjectName("comboFormSpec");
        comboFormSpec->setMinimumSize(QSize(120, 30));

        formRow1->addWidget(comboFormSpec);

        labelFormStatut = new QLabel(formPanel);
        labelFormStatut->setObjectName("labelFormStatut");

        formRow1->addWidget(labelFormStatut);

        comboFormStatut = new QComboBox(formPanel);
        comboFormStatut->setObjectName("comboFormStatut");
        comboFormStatut->setMinimumSize(QSize(120, 30));

        formRow1->addWidget(comboFormStatut);

        formRow1Spacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        formRow1->addItem(formRow1Spacer);


        formLayout->addLayout(formRow1);

        formRow2 = new QHBoxLayout();
        formRow2->setSpacing(8);
        formRow2->setObjectName("formRow2");
        formRow2->setContentsMargins(0, 0, 0, 0);
        labelFormTarget = new QLabel(formPanel);
        labelFormTarget->setObjectName("labelFormTarget");

        formRow2->addWidget(labelFormTarget);

        comboFormTarget = new QComboBox(formPanel);
        comboFormTarget->setObjectName("comboFormTarget");
        comboFormTarget->setMinimumSize(QSize(200, 30));

        formRow2->addWidget(comboFormTarget);

        labelFormGarde = new QLabel(formPanel);
        labelFormGarde->setObjectName("labelFormGarde");

        formRow2->addWidget(labelFormGarde);

        comboFormGarde = new QComboBox(formPanel);
        comboFormGarde->setObjectName("comboFormGarde");
        comboFormGarde->setMinimumSize(QSize(230, 30));

        formRow2->addWidget(comboFormGarde);

        labelFormCode = new QLabel(formPanel);
        labelFormCode->setObjectName("labelFormCode");

        formRow2->addWidget(labelFormCode);

        lineEditCode = new QLineEdit(formPanel);
        lineEditCode->setObjectName("lineEditCode");
        lineEditCode->setMinimumSize(QSize(200, 30));

        formRow2->addWidget(lineEditCode);

        formRow2Spacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        formRow2->addItem(formRow2Spacer);


        formLayout->addLayout(formRow2);

        formInfoRow = new QHBoxLayout();
        formInfoRow->setSpacing(12);
        formInfoRow->setObjectName("formInfoRow");
        formInfoRow->setContentsMargins(0, 0, 0, 0);
        labelFormQr = new QLabel(formPanel);
        labelFormQr->setObjectName("labelFormQr");
        labelFormQr->setMinimumSize(QSize(96, 96));
        labelFormQr->setMaximumSize(QSize(96, 96));

        formInfoRow->addWidget(labelFormQr);

        labelFormInfo = new QLabel(formPanel);
        labelFormInfo->setObjectName("labelFormInfo");
        labelFormInfo->setWordWrap(true);
        labelFormInfo->setTextInteractionFlags(Qt::NoTextInteraction);

        formInfoRow->addWidget(labelFormInfo);


        formLayout->addLayout(formInfoRow);

        formBtnRow = new QHBoxLayout();
        formBtnRow->setSpacing(8);
        formBtnRow->setObjectName("formBtnRow");
        formBtnRow->setContentsMargins(0, 0, 0, 0);
        formBtnSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        formBtnRow->addItem(formBtnSpacer);

        btnFormAnnuler = new QPushButton(formPanel);
        btnFormAnnuler->setObjectName("btnFormAnnuler");

        formBtnRow->addWidget(btnFormAnnuler);

        btnFormValider = new QPushButton(formPanel);
        btnFormValider->setObjectName("btnFormValider");

        formBtnRow->addWidget(btnFormValider);


        formLayout->addLayout(formBtnRow);


        leftLayout->addWidget(formPanel);

        tableEmployees = new QTableWidget(leftColumn);
        tableEmployees->setObjectName("tableEmployees");

        leftLayout->addWidget(tableEmployees);


        bodyLayout->addWidget(leftColumn);

        rightColumn = new QWidget(bodyWidget);
        rightColumn->setObjectName("rightColumn");
        rightColumn->setMinimumSize(QSize(330, 0));
        rightColumn->setMaximumSize(QSize(330, 16777215));
        rightLayout = new QVBoxLayout(rightColumn);
        rightLayout->setSpacing(10);
        rightLayout->setObjectName("rightLayout");
        rightLayout->setContentsMargins(0, 0, 0, 0);
        chatFrame = new QFrame(rightColumn);
        chatFrame->setObjectName("chatFrame");
        chatFrameLayout = new QVBoxLayout(chatFrame);
        chatFrameLayout->setSpacing(6);
        chatFrameLayout->setObjectName("chatFrameLayout");
        chatFrameLayout->setContentsMargins(10, 10, 10, 10);
        labelChatTitle = new QLabel(chatFrame);
        labelChatTitle->setObjectName("labelChatTitle");

        chatFrameLayout->addWidget(labelChatTitle);

        chatScroll = new QScrollArea(chatFrame);
        chatScroll->setObjectName("chatScroll");
        chatScroll->setWidgetResizable(true);
        chatScroll->setFrameShape(QFrame::NoFrame);
        chatContent = new QWidget();
        chatContent->setObjectName("chatContent");
        chatLayout = new QVBoxLayout(chatContent);
        chatLayout->setSpacing(8);
        chatLayout->setObjectName("chatLayout");
        chatLayout->setContentsMargins(6, 6, 6, 6);
        chatSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        chatLayout->addItem(chatSpacer);

        chatScroll->setWidget(chatContent);

        chatFrameLayout->addWidget(chatScroll);

        chatInputRow = new QHBoxLayout();
        chatInputRow->setSpacing(6);
        chatInputRow->setObjectName("chatInputRow");
        chatInputRow->setContentsMargins(0, 0, 0, 0);
        lineEditChat = new QLineEdit(chatFrame);
        lineEditChat->setObjectName("lineEditChat");
        lineEditChat->setMinimumSize(QSize(0, 30));

        chatInputRow->addWidget(lineEditChat);

        btnSendChat = new QPushButton(chatFrame);
        btnSendChat->setObjectName("btnSendChat");

        chatInputRow->addWidget(btnSendChat);


        chatFrameLayout->addLayout(chatInputRow);


        rightLayout->addWidget(chatFrame);

        planFrame = new QFrame(rightColumn);
        planFrame->setObjectName("planFrame");
        planLayout = new QVBoxLayout(planFrame);
        planLayout->setSpacing(6);
        planLayout->setObjectName("planLayout");
        planLayout->setContentsMargins(10, 10, 10, 10);
        labelPlanTitle = new QLabel(planFrame);
        labelPlanTitle->setObjectName("labelPlanTitle");

        planLayout->addWidget(labelPlanTitle);

        planHeader = new QHBoxLayout();
        planHeader->setSpacing(14);
        planHeader->setObjectName("planHeader");
        planHeader->setContentsMargins(0, 0, 0, 0);
        labelUnite = new QLabel(planFrame);
        labelUnite->setObjectName("labelUnite");

        planHeader->addWidget(labelUnite);

        planHS = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        planHeader->addItem(planHS);

        labelP1 = new QLabel(planFrame);
        labelP1->setObjectName("labelP1");

        planHeader->addWidget(labelP1);

        labelP2 = new QLabel(planFrame);
        labelP2->setObjectName("labelP2");

        planHeader->addWidget(labelP2);

        labelP3 = new QLabel(planFrame);
        labelP3->setObjectName("labelP3");

        planHeader->addWidget(labelP3);


        planLayout->addLayout(planHeader);

        barPlan1 = new QProgressBar(planFrame);
        barPlan1->setObjectName("barPlan1");
        barPlan1->setTextVisible(false);
        barPlan1->setValue(0);
        barPlan1->setMaximumSize(QSize(16777215, 10));

        planLayout->addWidget(barPlan1);

        barPlan2 = new QProgressBar(planFrame);
        barPlan2->setObjectName("barPlan2");
        barPlan2->setTextVisible(false);
        barPlan2->setValue(0);
        barPlan2->setMaximumSize(QSize(16777215, 10));

        planLayout->addWidget(barPlan2);

        barPlan3 = new QProgressBar(planFrame);
        barPlan3->setObjectName("barPlan3");
        barPlan3->setTextVisible(false);
        barPlan3->setValue(0);
        barPlan3->setMaximumSize(QSize(16777215, 10));

        planLayout->addWidget(barPlan3);

        dispoRow = new QHBoxLayout();
        dispoRow->setSpacing(8);
        dispoRow->setObjectName("dispoRow");
        dispoRow->setContentsMargins(0, 0, 0, 0);
        labelDispo = new QLabel(planFrame);
        labelDispo->setObjectName("labelDispo");

        dispoRow->addWidget(labelDispo);

        barDispo = new QProgressBar(planFrame);
        barDispo->setObjectName("barDispo");
        barDispo->setTextVisible(false);
        barDispo->setMaximumSize(QSize(16777215, 8));

        dispoRow->addWidget(barDispo);


        planLayout->addLayout(dispoRow);

        progRow = new QHBoxLayout();
        progRow->setSpacing(8);
        progRow->setObjectName("progRow");
        progRow->setContentsMargins(0, 0, 0, 0);
        labelProgress = new QLabel(planFrame);
        labelProgress->setObjectName("labelProgress");

        progRow->addWidget(labelProgress);

        barProgress = new QProgressBar(planFrame);
        barProgress->setObjectName("barProgress");
        barProgress->setTextVisible(false);
        barProgress->setMaximumSize(QSize(16777215, 8));

        progRow->addWidget(barProgress);


        planLayout->addLayout(progRow);

        planBtnRow = new QHBoxLayout();
        planBtnRow->setSpacing(6);
        planBtnRow->setObjectName("planBtnRow");
        planBtnRow->setContentsMargins(0, 0, 0, 0);
        btnNotifier = new QPushButton(planFrame);
        btnNotifier->setObjectName("btnNotifier");

        planBtnRow->addWidget(btnNotifier);

        btnAlerteGrade = new QPushButton(planFrame);
        btnAlerteGrade->setObjectName("btnAlerteGrade");

        planBtnRow->addWidget(btnAlerteGrade);


        planLayout->addLayout(planBtnRow);

        btnGenererPlanning = new QPushButton(planFrame);
        btnGenererPlanning->setObjectName("btnGenererPlanning");

        planLayout->addWidget(btnGenererPlanning);


        rightLayout->addWidget(planFrame);

        commFrame = new QFrame(rightColumn);
        commFrame->setObjectName("commFrame");
        commLayout = new QVBoxLayout(commFrame);
        commLayout->setSpacing(6);
        commLayout->setObjectName("commLayout");
        commLayout->setContentsMargins(10, 10, 10, 10);
        labelCommTitle = new QLabel(commFrame);
        labelCommTitle->setObjectName("labelCommTitle");

        commLayout->addWidget(labelCommTitle);

        labelEmail = new QLabel(commFrame);
        labelEmail->setObjectName("labelEmail");

        commLayout->addWidget(labelEmail);

        emailRow = new QHBoxLayout();
        emailRow->setSpacing(6);
        emailRow->setObjectName("emailRow");
        emailRow->setContentsMargins(0, 0, 0, 0);
        lineEditEmail = new QLineEdit(commFrame);
        lineEditEmail->setObjectName("lineEditEmail");
        lineEditEmail->setMinimumSize(QSize(0, 30));

        emailRow->addWidget(lineEditEmail);

        btnEnvoyerEmail = new QPushButton(commFrame);
        btnEnvoyerEmail->setObjectName("btnEnvoyerEmail");

        emailRow->addWidget(btnEnvoyerEmail);


        commLayout->addLayout(emailRow);

        labelSms = new QLabel(commFrame);
        labelSms->setObjectName("labelSms");

        commLayout->addWidget(labelSms);

        lineEditSms = new QLineEdit(commFrame);
        lineEditSms->setObjectName("lineEditSms");
        lineEditSms->setMinimumSize(QSize(0, 30));

        commLayout->addWidget(lineEditSms);

        smsRow = new QHBoxLayout();
        smsRow->setSpacing(6);
        smsRow->setObjectName("smsRow");
        smsRow->setContentsMargins(0, 0, 0, 0);
        btnEnvoyerSms = new QPushButton(commFrame);
        btnEnvoyerSms->setObjectName("btnEnvoyerSms");

        smsRow->addWidget(btnEnvoyerSms);

        btnEnvoyerWhats = new QPushButton(commFrame);
        btnEnvoyerWhats->setObjectName("btnEnvoyerWhats");

        smsRow->addWidget(btnEnvoyerWhats);


        commLayout->addLayout(smsRow);


        rightLayout->addWidget(commFrame);


        bodyLayout->addWidget(rightColumn);


        contentLayout->addWidget(bodyWidget);


        mainLayout->addWidget(contentWidget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Fire Station \342\200\223 Gestion des employ\303\251s", nullptr));
        labelLogo->setText(QString());
        btnMenuDashboard->setText(QCoreApplication::translate("MainWindow", "Tableau de bord", nullptr));
        btnMenuInterventions->setText(QCoreApplication::translate("MainWindow", "Interventions", nullptr));
        btnMenuEquipe->setText(QCoreApplication::translate("MainWindow", "\303\211quipe", nullptr));
        btnMenuVehicules->setText(QCoreApplication::translate("MainWindow", "V\303\251hicules", nullptr));
        btnMenuCarte->setText(QCoreApplication::translate("MainWindow", "Carte", nullptr));
        btnMenuCampagne->setText(QCoreApplication::translate("MainWindow", "Campagne de Sensibilisation", nullptr));
        btnMenuVolontaires->setText(QCoreApplication::translate("MainWindow", "Volontaires", nullptr));
        btnMenuParametres->setText(QCoreApplication::translate("MainWindow", "Param\303\250tres", nullptr));
        btnHamburger->setText(QCoreApplication::translate("MainWindow", "\342\230\260", nullptr));
        lineEditSearch->setPlaceholderText(QCoreApplication::translate("MainWindow", "Rechercher", nullptr));
        btnAlerte->setText(QString());
        btnTopParam->setText(QString());
        labelAvatarTop->setText(QString());
        labelUserTop->setText(QCoreApplication::translate("MainWindow", "CHEF DE CENTRE \302\267\n"
"RAYEN", nullptr));
        btnPower->setText(QCoreApplication::translate("MainWindow", "\342\217\273", nullptr));
        labelFiltreGrade->setText(QCoreApplication::translate("MainWindow", "Filtrer par Grade", nullptr));
        labelFiltreSpec->setText(QCoreApplication::translate("MainWindow", "Filtrer par Sp\303\251cialit\303\251", nullptr));
        btnQr->setText(QString());
        btnFaceId->setText(QString());
        btnAjouterVolontaire->setText(QString());
        btnAjouterEmploye->setText(QString());
        btnSupprimerEmploye->setText(QString());
        labelFormTitle->setText(QCoreApplication::translate("MainWindow", "Formulaire", nullptr));
        labelNom->setText(QCoreApplication::translate("MainWindow", "Nom :", nullptr));
        lineEditNom->setPlaceholderText(QCoreApplication::translate("MainWindow", "Nom et pr\303\251nom", nullptr));
        labelFormGrade->setText(QCoreApplication::translate("MainWindow", "Grade :", nullptr));
        labelFormSpec->setText(QCoreApplication::translate("MainWindow", "Sp\303\251cialit\303\251 :", nullptr));
        labelFormStatut->setText(QCoreApplication::translate("MainWindow", "Statut :", nullptr));
        labelFormTarget->setText(QCoreApplication::translate("MainWindow", "Employ\303\251 :", nullptr));
        labelFormGarde->setText(QCoreApplication::translate("MainWindow", "Garde :", nullptr));
        labelFormCode->setText(QCoreApplication::translate("MainWindow", "Code QR :", nullptr));
        lineEditCode->setPlaceholderText(QCoreApplication::translate("MainWindow", "ex. FS-AGT-001", nullptr));
        labelFormQr->setText(QString());
        labelFormInfo->setText(QString());
        btnFormAnnuler->setText(QCoreApplication::translate("MainWindow", "Annuler", nullptr));
        btnFormValider->setText(QCoreApplication::translate("MainWindow", "Valider", nullptr));
        labelChatTitle->setText(QCoreApplication::translate("MainWindow", "IA Chatbot RH", nullptr));
        lineEditChat->setPlaceholderText(QCoreApplication::translate("MainWindow", "Poser une question\342\200\246", nullptr));
        btnSendChat->setText(QCoreApplication::translate("MainWindow", "Envoyer", nullptr));
        labelPlanTitle->setText(QCoreApplication::translate("MainWindow", "Smart Planning IA", nullptr));
        labelUnite->setText(QCoreApplication::translate("MainWindow", "Unit\303\251 Chef", nullptr));
        labelP1->setText(QCoreApplication::translate("MainWindow", "Jour", nullptr));
        labelP2->setText(QCoreApplication::translate("MainWindow", "Nuit", nullptr));
        labelP3->setText(QCoreApplication::translate("MainWindow", "Week-end", nullptr));
        labelDispo->setText(QCoreApplication::translate("MainWindow", "Disponibilit\303\251", nullptr));
        labelProgress->setText(QCoreApplication::translate("MainWindow", "Progress", nullptr));
        btnNotifier->setText(QCoreApplication::translate("MainWindow", "Notifier Lt. M\303\251decin", nullptr));
        btnAlerteGrade->setText(QCoreApplication::translate("MainWindow", "Alerte Grade S/L", nullptr));
        btnGenererPlanning->setText(QCoreApplication::translate("MainWindow", "G\303\251n\303\251rer Planning", nullptr));
        labelCommTitle->setText(QCoreApplication::translate("MainWindow", "Communication Directe", nullptr));
        labelEmail->setText(QCoreApplication::translate("MainWindow", "Email (Sujet, Corps, Liste)", nullptr));
        lineEditEmail->setPlaceholderText(QCoreApplication::translate("MainWindow", "Email", nullptr));
        btnEnvoyerEmail->setText(QCoreApplication::translate("MainWindow", "Envoyer", nullptr));
        labelSms->setText(QCoreApplication::translate("MainWindow", "SMS/WhatsApp (Message, Num\303\251ros)", nullptr));
        lineEditSms->setPlaceholderText(QCoreApplication::translate("MainWindow", "Num\303\251ro ; Message", nullptr));
        btnEnvoyerSms->setText(QCoreApplication::translate("MainWindow", "Envoyer", nullptr));
        btnEnvoyerWhats->setText(QCoreApplication::translate("MainWindow", "Envoyer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
