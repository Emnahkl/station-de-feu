/********************************************************************************
** Form generated from reading UI file 'campagnespage.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMPAGNESPAGE_H
#define UI_CAMPAGNESPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CampagnesPage
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_central;
    QFrame *frame_sidebar;
    QVBoxLayout *verticalLayout_sidebar;
    QLabel *label_logoImage;
    QLabel *label_logo;
    QLabel *label_slogan;
    QSpacerItem *verticalSpacer_logo;
    QVBoxLayout *verticalLayout_nav;
    QPushButton *pushButton_navDashboard;
    QPushButton *pushButton_navInterventions;
    QPushButton *pushButton_navEmployes;
    QPushButton *pushButton_navVehicules;
    QPushButton *pushButton_navEquipements;
    QPushButton *pushButton_navCampagnes;
    QPushButton *pushButton_navZones;
    QSpacerItem *verticalSpacer_sidebar;
    QFrame *frame_hero;
    QVBoxLayout *verticalLayout_hero;
    QSpacerItem *verticalSpacer_hero;
    QLabel *label_heroTitre;
    QLabel *label_heroSousTitre;
    QSpacerItem *verticalSpacer_trait;
    QFrame *frame_heroTrait;
    QWidget *widget_main;
    QVBoxLayout *verticalLayout_main;
    QFrame *frame_header;
    QHBoxLayout *horizontalLayout_header;
    QLabel *label_pageTitle;
    QSpacerItem *horizontalSpacer_header;
    QLabel *label_user;
    QStackedWidget *stackedWidget;
    QWidget *page_dashboard;
    QVBoxLayout *verticalLayout_dashboard;
    QLabel *label_placeholder_dashboard;
    QWidget *page_employes;
    QVBoxLayout *verticalLayout_employes;
    QLabel *label_placeholder_employes;
    QWidget *page_vehicules;
    QVBoxLayout *verticalLayout_vehicules;
    QLabel *label_placeholder_vehicules;
    QWidget *page_equipements;
    QVBoxLayout *verticalLayout_equipements;
    QLabel *label_placeholder_equipements;
    QWidget *page_interventions;
    QVBoxLayout *verticalLayout_interventions;
    QLabel *label_placeholder_interventions;
    QWidget *page_campagnes;
    QVBoxLayout *verticalLayout_campagnes;
    QTabWidget *tabWidget_campagnes;
    QWidget *tab_gestion;
    QHBoxLayout *horizontalLayout_gestion;
    QGroupBox *groupBox_formulaire;
    QVBoxLayout *verticalLayout_formulaire;
    QFormLayout *formLayout_campagne;
    QLabel *label_id;
    QLineEdit *lineEdit_id;
    QLabel *label_titre;
    QLineEdit *lineEdit_titre;
    QLabel *label_theme;
    QComboBox *comboBox_theme;
    QLabel *label_date;
    QDateEdit *dateEdit_date;
    QLabel *label_lieu;
    QLineEdit *lineEdit_lieu;
    QLabel *label_public;
    QComboBox *comboBox_public;
    QLabel *label_participants;
    QSpinBox *spinBox_participants;
    QLabel *label_zone;
    QComboBox *comboBox_zone;
    QLabel *label_animateur;
    QComboBox *comboBox_animateur;
    QGridLayout *gridLayout_boutons;
    QPushButton *pushButton_ajouter;
    QPushButton *pushButton_modifier;
    QPushButton *pushButton_supprimer;
    QPushButton *pushButton_vider;
    QSpacerItem *verticalSpacer_formulaire;
    QGroupBox *groupBox_liste;
    QVBoxLayout *verticalLayout_liste;
    QHBoxLayout *horizontalLayout_outils;
    QLineEdit *lineEdit_recherche;
    QComboBox *comboBox_critere;
    QLabel *label_tri;
    QComboBox *comboBox_tri;
    QPushButton *pushButton_exportPdf;
    QPushButton *pushButton_exportExcel;
    QTableView *tableView_campagnes;
    QLabel *label_compteur;
    QWidget *tab_stats;
    QVBoxLayout *verticalLayout_stats;
    QHBoxLayout *horizontalLayout_stats;
    QLabel *label_statCritere;
    QComboBox *comboBox_statCritere;
    QPushButton *pushButton_actualiserStats;
    QSpacerItem *horizontalSpacer_stats;
    QLabel *label_statTotal;
    QGroupBox *groupBox_graphique;
    QVBoxLayout *verticalLayout_chart;
    QWidget *tab_ciblage;
    QVBoxLayout *verticalLayout_ciblage;
    QGroupBox *groupBox_ciblage;
    QVBoxLayout *verticalLayout_groupCiblage;
    QHBoxLayout *horizontalLayout_ciblage;
    QPushButton *pushButton_lancerCiblage;
    QLabel *label_zonePrioritaire;
    QSpacerItem *horizontalSpacer_ciblage;
    QPushButton *pushButton_planifierZone;
    QTableWidget *tableWidget_ciblage;
    QGroupBox *groupBox_impact;
    QGridLayout *gridLayout_impact;
    QLabel *label_campagneImpact;
    QComboBox *comboBox_campagneImpact;
    QLabel *label_fenetre;
    QSpinBox *spinBox_fenetre;
    QPushButton *pushButton_evaluerImpact;
    QLabel *label_impactAvant;
    QLabel *label_impactApres;
    QLabel *label_impactVariation;
    QLabel *label_impactVerdict;
    QWidget *tab_affiche;
    QHBoxLayout *horizontalLayout_affiche;
    QGroupBox *groupBox_afficheParams;
    QVBoxLayout *verticalLayout_afficheParams;
    QLabel *label_afficheCampagne;
    QComboBox *comboBox_afficheCampagne;
    QLabel *label_afficheSlogan;
    QLineEdit *lineEdit_slogan;
    QPushButton *pushButton_genererAffiche;
    QPushButton *pushButton_enregistrerAffiche;
    QSpacerItem *verticalSpacer_affiche;
    QLabel *label_affichePreview;
    QWidget *page_zones;
    QVBoxLayout *verticalLayout_zones;
    QLabel *label_placeholder_zones;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CampagnesPage)
    {
        if (CampagnesPage->objectName().isEmpty())
            CampagnesPage->setObjectName("CampagnesPage");
        CampagnesPage->resize(1320, 820);
        CampagnesPage->setMinimumSize(QSize(1100, 700));
        CampagnesPage->setStyleSheet(QString::fromUtf8("QMainWindow { background: #F4F4F4; }\n"
"QWidget { font-size: 13px; color: #2B2B2B; }\n"
"#frame_sidebar { background: qlineargradient(x1:0, y1:0, x2:0.3, y2:1, stop:0 #4A0503, stop:0.55 #380404, stop:1 #350404); border-right: 1px solid #1A0202; }\n"
"#frame_sidebar QLabel { color: #FFFFFF; background: transparent; }\n"
"#label_logo { font-size: 22px; font-weight: 900; color: #FFFFFF; letter-spacing: 1px; }\n"
"#label_slogan { color: #F2D7D7; font-size: 10px; font-weight: bold; letter-spacing: 1px; }\n"
"#frame_sidebar QPushButton { color: #FFFFFF; background: transparent; border: none; text-align: left; padding: 11px 14px; border-radius: 8px; font-size: 14px; font-weight: 600; }\n"
"#frame_sidebar QPushButton:hover { background: rgba(255, 255, 255, 0.08); }\n"
"#frame_sidebar QPushButton:checked { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #D3122A, stop:1 #A50E1F); border: 1px solid #E5394A; }\n"
"#frame_hero { border-image: url(:/images/campagnes/sidebar_hero.jpg) 0 0 0 0 stretch stretch; }\n"
""
                        "#label_heroTitre { font-size: 20px; font-weight: bold; color: #FFFFFF; }\n"
"#label_heroSousTitre { font-size: 15px; font-weight: bold; color: #FFFFFF; }\n"
"#frame_heroTrait { background: #E53935; border-radius: 1px; }\n"
"QStatusBar { background: #1E2329; color: #C9CED6; border-top: 1px solid #111; }\n"
"QStatusBar QLabel { color: #C9CED6; font-size: 12px; padding: 2px 8px; }\n"
"#frame_header { background: #FFFFFF; border-bottom: 3px solid #C62828; }\n"
"#label_pageTitle { font-size: 20px; font-weight: bold; color: #C62828; }\n"
"#label_user { color: #555555; }\n"
"QGroupBox { background: #FFFFFF; border: 1px solid #E0E0E0; border-radius: 8px; margin-top: 18px; padding: 10px; font-weight: bold; }\n"
"QGroupBox::title { subcontrol-origin: margin; left: 12px; padding: 0 6px; color: #C62828; }\n"
"QLineEdit, QComboBox, QDateEdit, QSpinBox { background: #FFFFFF; border: 1px solid #BDBDBD; border-radius: 5px; padding: 5px; min-height: 22px; }\n"
"QLineEdit:focus, QComboBox:focus, QDateEdit:focus, QSpinBox:focus "
                        "{ border: 1px solid #FF6F00; }\n"
"QPushButton { background: #FF6F00; color: #FFFFFF; border: none; border-radius: 6px; padding: 8px 14px; font-weight: bold; }\n"
"QPushButton:hover { background: #E65100; }\n"
"QPushButton:disabled { background: #BDBDBD; }\n"
"#pushButton_ajouter { background: #2E7D32; }\n"
"#pushButton_ajouter:hover { background: #1B5E20; }\n"
"#pushButton_supprimer { background: #C62828; }\n"
"#pushButton_supprimer:hover { background: #8E0000; }\n"
"#pushButton_vider { background: #757575; }\n"
"QTableView, QTableWidget { background: #FFFFFF; border: 1px solid #E0E0E0; border-radius: 6px; gridline-color: #EEEEEE; selection-background-color: #FFE0B2; selection-color: #2B2B2B; }\n"
"QHeaderView::section { background: #C62828; color: #FFFFFF; padding: 6px; border: none; font-weight: bold; }\n"
"QTabWidget::pane { border: none; }\n"
"QTabBar::tab { background: #E0E0E0; padding: 8px 18px; min-width: 130px; margin-right: 4px; border-top-left-radius: 6px; border-top-right-radius: 6px; }\n"
"QTabBar"
                        "::tab:selected { background: #C62828; color: #FFFFFF; font-weight: bold; }\n"
"#label_zonePrioritaire { color: #C62828; font-weight: bold; }\n"
"#label_impactVerdict { font-weight: bold; }\n"
"#label_affichePreview { background: #FFFFFF; border: 2px dashed #BDBDBD; border-radius: 8px; color: #757575; }\n"
"#label_placeholder_dashboard, #label_placeholder_employes, #label_placeholder_vehicules, #label_placeholder_equipements, #label_placeholder_interventions, #label_placeholder_zones { color: #757575; font-size: 16px; }"));
        centralwidget = new QWidget(CampagnesPage);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout_central = new QHBoxLayout(centralwidget);
        horizontalLayout_central->setSpacing(0);
        horizontalLayout_central->setObjectName("horizontalLayout_central");
        horizontalLayout_central->setContentsMargins(0, 0, 0, 0);
        frame_sidebar = new QFrame(centralwidget);
        frame_sidebar->setObjectName("frame_sidebar");
        frame_sidebar->setMinimumSize(QSize(240, 0));
        frame_sidebar->setMaximumSize(QSize(240, 16777215));
        verticalLayout_sidebar = new QVBoxLayout(frame_sidebar);
        verticalLayout_sidebar->setSpacing(6);
        verticalLayout_sidebar->setObjectName("verticalLayout_sidebar");
        verticalLayout_sidebar->setContentsMargins(0, 18, 0, 0);
        label_logoImage = new QLabel(frame_sidebar);
        label_logoImage->setObjectName("label_logoImage");
        label_logoImage->setMinimumSize(QSize(110, 110));
        label_logoImage->setMaximumSize(QSize(110, 110));
        label_logoImage->setPixmap(QPixmap(QString::fromUtf8(":/images/logo.png")));
        label_logoImage->setScaledContents(true);

        verticalLayout_sidebar->addWidget(label_logoImage, 0, Qt::AlignHCenter);

        label_logo = new QLabel(frame_sidebar);
        label_logo->setObjectName("label_logo");
        label_logo->setAlignment(Qt::AlignCenter);

        verticalLayout_sidebar->addWidget(label_logo);

        label_slogan = new QLabel(frame_sidebar);
        label_slogan->setObjectName("label_slogan");
        label_slogan->setAlignment(Qt::AlignCenter);

        verticalLayout_sidebar->addWidget(label_slogan);

        verticalSpacer_logo = new QSpacerItem(20, 14, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout_sidebar->addItem(verticalSpacer_logo);

        verticalLayout_nav = new QVBoxLayout();
        verticalLayout_nav->setSpacing(4);
        verticalLayout_nav->setObjectName("verticalLayout_nav");
        verticalLayout_nav->setContentsMargins(12, -1, 12, -1);
        pushButton_navDashboard = new QPushButton(frame_sidebar);
        pushButton_navDashboard->setObjectName("pushButton_navDashboard");
        pushButton_navDashboard->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/campagnes/ic_home.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_navDashboard->setIcon(icon);
        pushButton_navDashboard->setIconSize(QSize(22, 22));
        pushButton_navDashboard->setCheckable(true);
        pushButton_navDashboard->setAutoExclusive(true);

        verticalLayout_nav->addWidget(pushButton_navDashboard);

        pushButton_navInterventions = new QPushButton(frame_sidebar);
        pushButton_navInterventions->setObjectName("pushButton_navInterventions");
        pushButton_navInterventions->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/campagnes/ic_intervention.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_navInterventions->setIcon(icon1);
        pushButton_navInterventions->setIconSize(QSize(22, 22));
        pushButton_navInterventions->setCheckable(true);
        pushButton_navInterventions->setAutoExclusive(true);

        verticalLayout_nav->addWidget(pushButton_navInterventions);

        pushButton_navEmployes = new QPushButton(frame_sidebar);
        pushButton_navEmployes->setObjectName("pushButton_navEmployes");
        pushButton_navEmployes->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/campagnes/ic_equipe.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_navEmployes->setIcon(icon2);
        pushButton_navEmployes->setIconSize(QSize(22, 22));
        pushButton_navEmployes->setCheckable(true);
        pushButton_navEmployes->setAutoExclusive(true);

        verticalLayout_nav->addWidget(pushButton_navEmployes);

        pushButton_navVehicules = new QPushButton(frame_sidebar);
        pushButton_navVehicules->setObjectName("pushButton_navVehicules");
        pushButton_navVehicules->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/campagnes/ic_vehicule.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_navVehicules->setIcon(icon3);
        pushButton_navVehicules->setIconSize(QSize(22, 22));
        pushButton_navVehicules->setCheckable(true);
        pushButton_navVehicules->setAutoExclusive(true);

        verticalLayout_nav->addWidget(pushButton_navVehicules);

        pushButton_navEquipements = new QPushButton(frame_sidebar);
        pushButton_navEquipements->setObjectName("pushButton_navEquipements");
        pushButton_navEquipements->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/campagnes/ic_equipement.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_navEquipements->setIcon(icon4);
        pushButton_navEquipements->setIconSize(QSize(22, 22));
        pushButton_navEquipements->setCheckable(true);
        pushButton_navEquipements->setAutoExclusive(true);

        verticalLayout_nav->addWidget(pushButton_navEquipements);

        pushButton_navCampagnes = new QPushButton(frame_sidebar);
        pushButton_navCampagnes->setObjectName("pushButton_navCampagnes");
        pushButton_navCampagnes->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/campagnes/ic_campagne.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_navCampagnes->setIcon(icon5);
        pushButton_navCampagnes->setIconSize(QSize(22, 22));
        pushButton_navCampagnes->setCheckable(true);
        pushButton_navCampagnes->setChecked(true);
        pushButton_navCampagnes->setAutoExclusive(true);

        verticalLayout_nav->addWidget(pushButton_navCampagnes);

        pushButton_navZones = new QPushButton(frame_sidebar);
        pushButton_navZones->setObjectName("pushButton_navZones");
        pushButton_navZones->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/campagnes/ic_zone.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton_navZones->setIcon(icon6);
        pushButton_navZones->setIconSize(QSize(22, 22));
        pushButton_navZones->setCheckable(true);
        pushButton_navZones->setAutoExclusive(true);

        verticalLayout_nav->addWidget(pushButton_navZones);


        verticalLayout_sidebar->addLayout(verticalLayout_nav);

        verticalSpacer_sidebar = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_sidebar->addItem(verticalSpacer_sidebar);

        frame_hero = new QFrame(frame_sidebar);
        frame_hero->setObjectName("frame_hero");
        frame_hero->setMinimumSize(QSize(240, 202));
        frame_hero->setMaximumSize(QSize(240, 202));
        verticalLayout_hero = new QVBoxLayout(frame_hero);
        verticalLayout_hero->setSpacing(0);
        verticalLayout_hero->setObjectName("verticalLayout_hero");
        verticalLayout_hero->setContentsMargins(22, -1, -1, 18);
        verticalSpacer_hero = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_hero->addItem(verticalSpacer_hero);

        label_heroTitre = new QLabel(frame_hero);
        label_heroTitre->setObjectName("label_heroTitre");

        verticalLayout_hero->addWidget(label_heroTitre);

        label_heroSousTitre = new QLabel(frame_hero);
        label_heroSousTitre->setObjectName("label_heroSousTitre");

        verticalLayout_hero->addWidget(label_heroSousTitre);

        verticalSpacer_trait = new QSpacerItem(20, 8, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout_hero->addItem(verticalSpacer_trait);

        frame_heroTrait = new QFrame(frame_hero);
        frame_heroTrait->setObjectName("frame_heroTrait");
        frame_heroTrait->setMinimumSize(QSize(44, 3));
        frame_heroTrait->setMaximumSize(QSize(44, 3));

        verticalLayout_hero->addWidget(frame_heroTrait);


        verticalLayout_sidebar->addWidget(frame_hero);


        horizontalLayout_central->addWidget(frame_sidebar);

        widget_main = new QWidget(centralwidget);
        widget_main->setObjectName("widget_main");
        verticalLayout_main = new QVBoxLayout(widget_main);
        verticalLayout_main->setSpacing(0);
        verticalLayout_main->setObjectName("verticalLayout_main");
        verticalLayout_main->setContentsMargins(0, 0, 0, 0);
        frame_header = new QFrame(widget_main);
        frame_header->setObjectName("frame_header");
        frame_header->setMinimumSize(QSize(0, 64));
        horizontalLayout_header = new QHBoxLayout(frame_header);
        horizontalLayout_header->setObjectName("horizontalLayout_header");
        horizontalLayout_header->setContentsMargins(20, -1, 20, -1);
        label_pageTitle = new QLabel(frame_header);
        label_pageTitle->setObjectName("label_pageTitle");

        horizontalLayout_header->addWidget(label_pageTitle);

        horizontalSpacer_header = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_header->addItem(horizontalSpacer_header);

        label_user = new QLabel(frame_header);
        label_user->setObjectName("label_user");

        horizontalLayout_header->addWidget(label_user);


        verticalLayout_main->addWidget(frame_header);

        stackedWidget = new QStackedWidget(widget_main);
        stackedWidget->setObjectName("stackedWidget");
        page_dashboard = new QWidget();
        page_dashboard->setObjectName("page_dashboard");
        verticalLayout_dashboard = new QVBoxLayout(page_dashboard);
        verticalLayout_dashboard->setObjectName("verticalLayout_dashboard");
        label_placeholder_dashboard = new QLabel(page_dashboard);
        label_placeholder_dashboard->setObjectName("label_placeholder_dashboard");
        label_placeholder_dashboard->setAlignment(Qt::AlignCenter);

        verticalLayout_dashboard->addWidget(label_placeholder_dashboard);

        stackedWidget->addWidget(page_dashboard);
        page_employes = new QWidget();
        page_employes->setObjectName("page_employes");
        verticalLayout_employes = new QVBoxLayout(page_employes);
        verticalLayout_employes->setObjectName("verticalLayout_employes");
        label_placeholder_employes = new QLabel(page_employes);
        label_placeholder_employes->setObjectName("label_placeholder_employes");
        label_placeholder_employes->setAlignment(Qt::AlignCenter);

        verticalLayout_employes->addWidget(label_placeholder_employes);

        stackedWidget->addWidget(page_employes);
        page_vehicules = new QWidget();
        page_vehicules->setObjectName("page_vehicules");
        verticalLayout_vehicules = new QVBoxLayout(page_vehicules);
        verticalLayout_vehicules->setObjectName("verticalLayout_vehicules");
        label_placeholder_vehicules = new QLabel(page_vehicules);
        label_placeholder_vehicules->setObjectName("label_placeholder_vehicules");
        label_placeholder_vehicules->setAlignment(Qt::AlignCenter);

        verticalLayout_vehicules->addWidget(label_placeholder_vehicules);

        stackedWidget->addWidget(page_vehicules);
        page_equipements = new QWidget();
        page_equipements->setObjectName("page_equipements");
        verticalLayout_equipements = new QVBoxLayout(page_equipements);
        verticalLayout_equipements->setObjectName("verticalLayout_equipements");
        label_placeholder_equipements = new QLabel(page_equipements);
        label_placeholder_equipements->setObjectName("label_placeholder_equipements");
        label_placeholder_equipements->setAlignment(Qt::AlignCenter);

        verticalLayout_equipements->addWidget(label_placeholder_equipements);

        stackedWidget->addWidget(page_equipements);
        page_interventions = new QWidget();
        page_interventions->setObjectName("page_interventions");
        verticalLayout_interventions = new QVBoxLayout(page_interventions);
        verticalLayout_interventions->setObjectName("verticalLayout_interventions");
        label_placeholder_interventions = new QLabel(page_interventions);
        label_placeholder_interventions->setObjectName("label_placeholder_interventions");
        label_placeholder_interventions->setAlignment(Qt::AlignCenter);

        verticalLayout_interventions->addWidget(label_placeholder_interventions);

        stackedWidget->addWidget(page_interventions);
        page_campagnes = new QWidget();
        page_campagnes->setObjectName("page_campagnes");
        verticalLayout_campagnes = new QVBoxLayout(page_campagnes);
        verticalLayout_campagnes->setObjectName("verticalLayout_campagnes");
        verticalLayout_campagnes->setContentsMargins(16, 12, 16, 12);
        tabWidget_campagnes = new QTabWidget(page_campagnes);
        tabWidget_campagnes->setObjectName("tabWidget_campagnes");
        tab_gestion = new QWidget();
        tab_gestion->setObjectName("tab_gestion");
        horizontalLayout_gestion = new QHBoxLayout(tab_gestion);
        horizontalLayout_gestion->setObjectName("horizontalLayout_gestion");
        groupBox_formulaire = new QGroupBox(tab_gestion);
        groupBox_formulaire->setObjectName("groupBox_formulaire");
        groupBox_formulaire->setMinimumSize(QSize(340, 0));
        groupBox_formulaire->setMaximumSize(QSize(380, 16777215));
        verticalLayout_formulaire = new QVBoxLayout(groupBox_formulaire);
        verticalLayout_formulaire->setObjectName("verticalLayout_formulaire");
        formLayout_campagne = new QFormLayout();
        formLayout_campagne->setObjectName("formLayout_campagne");
        formLayout_campagne->setLabelAlignment(Qt::AlignRight|Qt::AlignVCenter);
        label_id = new QLabel(groupBox_formulaire);
        label_id->setObjectName("label_id");

        formLayout_campagne->setWidget(0, QFormLayout::ItemRole::LabelRole, label_id);

        lineEdit_id = new QLineEdit(groupBox_formulaire);
        lineEdit_id->setObjectName("lineEdit_id");

        formLayout_campagne->setWidget(0, QFormLayout::ItemRole::FieldRole, lineEdit_id);

        label_titre = new QLabel(groupBox_formulaire);
        label_titre->setObjectName("label_titre");

        formLayout_campagne->setWidget(1, QFormLayout::ItemRole::LabelRole, label_titre);

        lineEdit_titre = new QLineEdit(groupBox_formulaire);
        lineEdit_titre->setObjectName("lineEdit_titre");
        lineEdit_titre->setMaxLength(80);

        formLayout_campagne->setWidget(1, QFormLayout::ItemRole::FieldRole, lineEdit_titre);

        label_theme = new QLabel(groupBox_formulaire);
        label_theme->setObjectName("label_theme");

        formLayout_campagne->setWidget(2, QFormLayout::ItemRole::LabelRole, label_theme);

        comboBox_theme = new QComboBox(groupBox_formulaire);
        comboBox_theme->addItem(QString());
        comboBox_theme->addItem(QString());
        comboBox_theme->addItem(QString());
        comboBox_theme->addItem(QString());
        comboBox_theme->addItem(QString());
        comboBox_theme->addItem(QString());
        comboBox_theme->setObjectName("comboBox_theme");

        formLayout_campagne->setWidget(2, QFormLayout::ItemRole::FieldRole, comboBox_theme);

        label_date = new QLabel(groupBox_formulaire);
        label_date->setObjectName("label_date");

        formLayout_campagne->setWidget(3, QFormLayout::ItemRole::LabelRole, label_date);

        dateEdit_date = new QDateEdit(groupBox_formulaire);
        dateEdit_date->setObjectName("dateEdit_date");
        dateEdit_date->setCalendarPopup(true);

        formLayout_campagne->setWidget(3, QFormLayout::ItemRole::FieldRole, dateEdit_date);

        label_lieu = new QLabel(groupBox_formulaire);
        label_lieu->setObjectName("label_lieu");

        formLayout_campagne->setWidget(4, QFormLayout::ItemRole::LabelRole, label_lieu);

        lineEdit_lieu = new QLineEdit(groupBox_formulaire);
        lineEdit_lieu->setObjectName("lineEdit_lieu");
        lineEdit_lieu->setMaxLength(120);

        formLayout_campagne->setWidget(4, QFormLayout::ItemRole::FieldRole, lineEdit_lieu);

        label_public = new QLabel(groupBox_formulaire);
        label_public->setObjectName("label_public");

        formLayout_campagne->setWidget(5, QFormLayout::ItemRole::LabelRole, label_public);

        comboBox_public = new QComboBox(groupBox_formulaire);
        comboBox_public->addItem(QString());
        comboBox_public->addItem(QString());
        comboBox_public->addItem(QString());
        comboBox_public->addItem(QString());
        comboBox_public->addItem(QString());
        comboBox_public->setObjectName("comboBox_public");

        formLayout_campagne->setWidget(5, QFormLayout::ItemRole::FieldRole, comboBox_public);

        label_participants = new QLabel(groupBox_formulaire);
        label_participants->setObjectName("label_participants");

        formLayout_campagne->setWidget(6, QFormLayout::ItemRole::LabelRole, label_participants);

        spinBox_participants = new QSpinBox(groupBox_formulaire);
        spinBox_participants->setObjectName("spinBox_participants");
        spinBox_participants->setMaximum(100000);

        formLayout_campagne->setWidget(6, QFormLayout::ItemRole::FieldRole, spinBox_participants);

        label_zone = new QLabel(groupBox_formulaire);
        label_zone->setObjectName("label_zone");

        formLayout_campagne->setWidget(7, QFormLayout::ItemRole::LabelRole, label_zone);

        comboBox_zone = new QComboBox(groupBox_formulaire);
        comboBox_zone->setObjectName("comboBox_zone");

        formLayout_campagne->setWidget(7, QFormLayout::ItemRole::FieldRole, comboBox_zone);

        label_animateur = new QLabel(groupBox_formulaire);
        label_animateur->setObjectName("label_animateur");

        formLayout_campagne->setWidget(8, QFormLayout::ItemRole::LabelRole, label_animateur);

        comboBox_animateur = new QComboBox(groupBox_formulaire);
        comboBox_animateur->setObjectName("comboBox_animateur");

        formLayout_campagne->setWidget(8, QFormLayout::ItemRole::FieldRole, comboBox_animateur);


        verticalLayout_formulaire->addLayout(formLayout_campagne);

        gridLayout_boutons = new QGridLayout();
        gridLayout_boutons->setObjectName("gridLayout_boutons");
        pushButton_ajouter = new QPushButton(groupBox_formulaire);
        pushButton_ajouter->setObjectName("pushButton_ajouter");

        gridLayout_boutons->addWidget(pushButton_ajouter, 0, 0, 1, 1);

        pushButton_modifier = new QPushButton(groupBox_formulaire);
        pushButton_modifier->setObjectName("pushButton_modifier");

        gridLayout_boutons->addWidget(pushButton_modifier, 0, 1, 1, 1);

        pushButton_supprimer = new QPushButton(groupBox_formulaire);
        pushButton_supprimer->setObjectName("pushButton_supprimer");

        gridLayout_boutons->addWidget(pushButton_supprimer, 1, 0, 1, 1);

        pushButton_vider = new QPushButton(groupBox_formulaire);
        pushButton_vider->setObjectName("pushButton_vider");

        gridLayout_boutons->addWidget(pushButton_vider, 1, 1, 1, 1);


        verticalLayout_formulaire->addLayout(gridLayout_boutons);

        verticalSpacer_formulaire = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_formulaire->addItem(verticalSpacer_formulaire);


        horizontalLayout_gestion->addWidget(groupBox_formulaire);

        groupBox_liste = new QGroupBox(tab_gestion);
        groupBox_liste->setObjectName("groupBox_liste");
        verticalLayout_liste = new QVBoxLayout(groupBox_liste);
        verticalLayout_liste->setObjectName("verticalLayout_liste");
        horizontalLayout_outils = new QHBoxLayout();
        horizontalLayout_outils->setObjectName("horizontalLayout_outils");
        lineEdit_recherche = new QLineEdit(groupBox_liste);
        lineEdit_recherche->setObjectName("lineEdit_recherche");
        lineEdit_recherche->setClearButtonEnabled(true);

        horizontalLayout_outils->addWidget(lineEdit_recherche);

        comboBox_critere = new QComboBox(groupBox_liste);
        comboBox_critere->addItem(QString());
        comboBox_critere->addItem(QString());
        comboBox_critere->addItem(QString());
        comboBox_critere->addItem(QString());
        comboBox_critere->setObjectName("comboBox_critere");

        horizontalLayout_outils->addWidget(comboBox_critere);

        label_tri = new QLabel(groupBox_liste);
        label_tri->setObjectName("label_tri");

        horizontalLayout_outils->addWidget(label_tri);

        comboBox_tri = new QComboBox(groupBox_liste);
        comboBox_tri->addItem(QString());
        comboBox_tri->addItem(QString());
        comboBox_tri->addItem(QString());
        comboBox_tri->addItem(QString());
        comboBox_tri->setObjectName("comboBox_tri");

        horizontalLayout_outils->addWidget(comboBox_tri);

        pushButton_exportPdf = new QPushButton(groupBox_liste);
        pushButton_exportPdf->setObjectName("pushButton_exportPdf");

        horizontalLayout_outils->addWidget(pushButton_exportPdf);

        pushButton_exportExcel = new QPushButton(groupBox_liste);
        pushButton_exportExcel->setObjectName("pushButton_exportExcel");

        horizontalLayout_outils->addWidget(pushButton_exportExcel);


        verticalLayout_liste->addLayout(horizontalLayout_outils);

        tableView_campagnes = new QTableView(groupBox_liste);
        tableView_campagnes->setObjectName("tableView_campagnes");
        tableView_campagnes->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView_campagnes->setAlternatingRowColors(true);
        tableView_campagnes->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView_campagnes->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView_campagnes->horizontalHeader()->setStretchLastSection(true);
        tableView_campagnes->verticalHeader()->setVisible(false);

        verticalLayout_liste->addWidget(tableView_campagnes);

        label_compteur = new QLabel(groupBox_liste);
        label_compteur->setObjectName("label_compteur");

        verticalLayout_liste->addWidget(label_compteur);


        horizontalLayout_gestion->addWidget(groupBox_liste);

        tabWidget_campagnes->addTab(tab_gestion, QString());
        tab_stats = new QWidget();
        tab_stats->setObjectName("tab_stats");
        verticalLayout_stats = new QVBoxLayout(tab_stats);
        verticalLayout_stats->setObjectName("verticalLayout_stats");
        horizontalLayout_stats = new QHBoxLayout();
        horizontalLayout_stats->setObjectName("horizontalLayout_stats");
        label_statCritere = new QLabel(tab_stats);
        label_statCritere->setObjectName("label_statCritere");

        horizontalLayout_stats->addWidget(label_statCritere);

        comboBox_statCritere = new QComboBox(tab_stats);
        comboBox_statCritere->addItem(QString());
        comboBox_statCritere->addItem(QString());
        comboBox_statCritere->setObjectName("comboBox_statCritere");

        horizontalLayout_stats->addWidget(comboBox_statCritere);

        pushButton_actualiserStats = new QPushButton(tab_stats);
        pushButton_actualiserStats->setObjectName("pushButton_actualiserStats");

        horizontalLayout_stats->addWidget(pushButton_actualiserStats);

        horizontalSpacer_stats = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_stats->addItem(horizontalSpacer_stats);

        label_statTotal = new QLabel(tab_stats);
        label_statTotal->setObjectName("label_statTotal");

        horizontalLayout_stats->addWidget(label_statTotal);


        verticalLayout_stats->addLayout(horizontalLayout_stats);

        groupBox_graphique = new QGroupBox(tab_stats);
        groupBox_graphique->setObjectName("groupBox_graphique");
        verticalLayout_chart = new QVBoxLayout(groupBox_graphique);
        verticalLayout_chart->setObjectName("verticalLayout_chart");

        verticalLayout_stats->addWidget(groupBox_graphique);

        verticalLayout_stats->setStretch(1, 1);
        tabWidget_campagnes->addTab(tab_stats, QString());
        tab_ciblage = new QWidget();
        tab_ciblage->setObjectName("tab_ciblage");
        verticalLayout_ciblage = new QVBoxLayout(tab_ciblage);
        verticalLayout_ciblage->setObjectName("verticalLayout_ciblage");
        groupBox_ciblage = new QGroupBox(tab_ciblage);
        groupBox_ciblage->setObjectName("groupBox_ciblage");
        verticalLayout_groupCiblage = new QVBoxLayout(groupBox_ciblage);
        verticalLayout_groupCiblage->setObjectName("verticalLayout_groupCiblage");
        horizontalLayout_ciblage = new QHBoxLayout();
        horizontalLayout_ciblage->setObjectName("horizontalLayout_ciblage");
        pushButton_lancerCiblage = new QPushButton(groupBox_ciblage);
        pushButton_lancerCiblage->setObjectName("pushButton_lancerCiblage");

        horizontalLayout_ciblage->addWidget(pushButton_lancerCiblage);

        label_zonePrioritaire = new QLabel(groupBox_ciblage);
        label_zonePrioritaire->setObjectName("label_zonePrioritaire");

        horizontalLayout_ciblage->addWidget(label_zonePrioritaire);

        horizontalSpacer_ciblage = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_ciblage->addItem(horizontalSpacer_ciblage);

        pushButton_planifierZone = new QPushButton(groupBox_ciblage);
        pushButton_planifierZone->setObjectName("pushButton_planifierZone");
        pushButton_planifierZone->setEnabled(false);

        horizontalLayout_ciblage->addWidget(pushButton_planifierZone);


        verticalLayout_groupCiblage->addLayout(horizontalLayout_ciblage);

        tableWidget_ciblage = new QTableWidget(groupBox_ciblage);
        if (tableWidget_ciblage->columnCount() < 6)
            tableWidget_ciblage->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_ciblage->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_ciblage->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_ciblage->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_ciblage->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_ciblage->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_ciblage->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tableWidget_ciblage->setObjectName("tableWidget_ciblage");
        tableWidget_ciblage->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget_ciblage->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget_ciblage->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidget_ciblage->horizontalHeader()->setStretchLastSection(true);
        tableWidget_ciblage->verticalHeader()->setVisible(false);

        verticalLayout_groupCiblage->addWidget(tableWidget_ciblage);


        verticalLayout_ciblage->addWidget(groupBox_ciblage);

        groupBox_impact = new QGroupBox(tab_ciblage);
        groupBox_impact->setObjectName("groupBox_impact");
        gridLayout_impact = new QGridLayout(groupBox_impact);
        gridLayout_impact->setObjectName("gridLayout_impact");
        label_campagneImpact = new QLabel(groupBox_impact);
        label_campagneImpact->setObjectName("label_campagneImpact");

        gridLayout_impact->addWidget(label_campagneImpact, 0, 0, 1, 1);

        comboBox_campagneImpact = new QComboBox(groupBox_impact);
        comboBox_campagneImpact->setObjectName("comboBox_campagneImpact");
        comboBox_campagneImpact->setMinimumSize(QSize(280, 0));

        gridLayout_impact->addWidget(comboBox_campagneImpact, 0, 1, 1, 1);

        label_fenetre = new QLabel(groupBox_impact);
        label_fenetre->setObjectName("label_fenetre");

        gridLayout_impact->addWidget(label_fenetre, 0, 2, 1, 1);

        spinBox_fenetre = new QSpinBox(groupBox_impact);
        spinBox_fenetre->setObjectName("spinBox_fenetre");
        spinBox_fenetre->setMinimum(1);
        spinBox_fenetre->setMaximum(12);
        spinBox_fenetre->setValue(3);

        gridLayout_impact->addWidget(spinBox_fenetre, 0, 3, 1, 1);

        pushButton_evaluerImpact = new QPushButton(groupBox_impact);
        pushButton_evaluerImpact->setObjectName("pushButton_evaluerImpact");

        gridLayout_impact->addWidget(pushButton_evaluerImpact, 0, 4, 1, 1);

        label_impactAvant = new QLabel(groupBox_impact);
        label_impactAvant->setObjectName("label_impactAvant");

        gridLayout_impact->addWidget(label_impactAvant, 1, 0, 1, 2);

        label_impactApres = new QLabel(groupBox_impact);
        label_impactApres->setObjectName("label_impactApres");

        gridLayout_impact->addWidget(label_impactApres, 1, 2, 1, 2);

        label_impactVariation = new QLabel(groupBox_impact);
        label_impactVariation->setObjectName("label_impactVariation");

        gridLayout_impact->addWidget(label_impactVariation, 1, 4, 1, 1);

        label_impactVerdict = new QLabel(groupBox_impact);
        label_impactVerdict->setObjectName("label_impactVerdict");
        label_impactVerdict->setWordWrap(true);

        gridLayout_impact->addWidget(label_impactVerdict, 2, 0, 1, 5);


        verticalLayout_ciblage->addWidget(groupBox_impact);

        tabWidget_campagnes->addTab(tab_ciblage, QString());
        tab_affiche = new QWidget();
        tab_affiche->setObjectName("tab_affiche");
        horizontalLayout_affiche = new QHBoxLayout(tab_affiche);
        horizontalLayout_affiche->setObjectName("horizontalLayout_affiche");
        groupBox_afficheParams = new QGroupBox(tab_affiche);
        groupBox_afficheParams->setObjectName("groupBox_afficheParams");
        groupBox_afficheParams->setMaximumSize(QSize(380, 16777215));
        verticalLayout_afficheParams = new QVBoxLayout(groupBox_afficheParams);
        verticalLayout_afficheParams->setObjectName("verticalLayout_afficheParams");
        label_afficheCampagne = new QLabel(groupBox_afficheParams);
        label_afficheCampagne->setObjectName("label_afficheCampagne");

        verticalLayout_afficheParams->addWidget(label_afficheCampagne);

        comboBox_afficheCampagne = new QComboBox(groupBox_afficheParams);
        comboBox_afficheCampagne->setObjectName("comboBox_afficheCampagne");

        verticalLayout_afficheParams->addWidget(comboBox_afficheCampagne);

        label_afficheSlogan = new QLabel(groupBox_afficheParams);
        label_afficheSlogan->setObjectName("label_afficheSlogan");
        label_afficheSlogan->setWordWrap(true);

        verticalLayout_afficheParams->addWidget(label_afficheSlogan);

        lineEdit_slogan = new QLineEdit(groupBox_afficheParams);
        lineEdit_slogan->setObjectName("lineEdit_slogan");
        lineEdit_slogan->setMaxLength(70);

        verticalLayout_afficheParams->addWidget(lineEdit_slogan);

        pushButton_genererAffiche = new QPushButton(groupBox_afficheParams);
        pushButton_genererAffiche->setObjectName("pushButton_genererAffiche");

        verticalLayout_afficheParams->addWidget(pushButton_genererAffiche);

        pushButton_enregistrerAffiche = new QPushButton(groupBox_afficheParams);
        pushButton_enregistrerAffiche->setObjectName("pushButton_enregistrerAffiche");
        pushButton_enregistrerAffiche->setEnabled(false);

        verticalLayout_afficheParams->addWidget(pushButton_enregistrerAffiche);

        verticalSpacer_affiche = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_afficheParams->addItem(verticalSpacer_affiche);


        horizontalLayout_affiche->addWidget(groupBox_afficheParams);

        label_affichePreview = new QLabel(tab_affiche);
        label_affichePreview->setObjectName("label_affichePreview");
        label_affichePreview->setMinimumSize(QSize(360, 450));
        label_affichePreview->setAlignment(Qt::AlignCenter);

        horizontalLayout_affiche->addWidget(label_affichePreview);

        tabWidget_campagnes->addTab(tab_affiche, QString());

        verticalLayout_campagnes->addWidget(tabWidget_campagnes);

        stackedWidget->addWidget(page_campagnes);
        page_zones = new QWidget();
        page_zones->setObjectName("page_zones");
        verticalLayout_zones = new QVBoxLayout(page_zones);
        verticalLayout_zones->setObjectName("verticalLayout_zones");
        label_placeholder_zones = new QLabel(page_zones);
        label_placeholder_zones->setObjectName("label_placeholder_zones");
        label_placeholder_zones->setAlignment(Qt::AlignCenter);

        verticalLayout_zones->addWidget(label_placeholder_zones);

        stackedWidget->addWidget(page_zones);

        verticalLayout_main->addWidget(stackedWidget);


        horizontalLayout_central->addWidget(widget_main);

        CampagnesPage->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(CampagnesPage);
        statusbar->setObjectName("statusbar");
        statusbar->setSizeGripEnabled(false);
        CampagnesPage->setStatusBar(statusbar);

        retranslateUi(CampagnesPage);

        stackedWidget->setCurrentIndex(5);
        tabWidget_campagnes->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(CampagnesPage);
    } // setupUi

    void retranslateUi(QMainWindow *CampagnesPage)
    {
        CampagnesPage->setWindowTitle(QCoreApplication::translate("CampagnesPage", "FireStation Manager", nullptr));
        label_logo->setText(QCoreApplication::translate("CampagnesPage", "FIRE STATION", nullptr));
        label_slogan->setText(QCoreApplication::translate("CampagnesPage", "PR\303\212TS \302\267 PROT\303\211GER \302\267 SAUVER", nullptr));
        pushButton_navDashboard->setText(QCoreApplication::translate("CampagnesPage", "Tableau de bord", nullptr));
        pushButton_navInterventions->setText(QCoreApplication::translate("CampagnesPage", "Interventions", nullptr));
        pushButton_navEmployes->setText(QCoreApplication::translate("CampagnesPage", "\303\211quipe", nullptr));
        pushButton_navVehicules->setText(QCoreApplication::translate("CampagnesPage", "V\303\251hicules", nullptr));
        pushButton_navEquipements->setText(QCoreApplication::translate("CampagnesPage", "\303\211quipements", nullptr));
        pushButton_navCampagnes->setText(QCoreApplication::translate("CampagnesPage", "Sensibilisation", nullptr));
        pushButton_navZones->setText(QCoreApplication::translate("CampagnesPage", "Zones / Carte", nullptr));
        label_heroTitre->setText(QCoreApplication::translate("CampagnesPage", "Ensemble", nullptr));
        label_heroSousTitre->setText(QCoreApplication::translate("CampagnesPage", "pour sauver des vies", nullptr));
        label_pageTitle->setText(QCoreApplication::translate("CampagnesPage", "Gestion des campagnes de sensibilisation", nullptr));
        label_user->setText(QCoreApplication::translate("CampagnesPage", "\360\237\221\244 Officier pr\303\251vention", nullptr));
        label_placeholder_dashboard->setText(QCoreApplication::translate("CampagnesPage", "Tableau de bord \342\200\224 vue synth\303\251tique de l'activit\303\251 de la caserne", nullptr));
        label_placeholder_employes->setText(QCoreApplication::translate("CampagnesPage", "Module Gestion des employ\303\251s & authentification", nullptr));
        label_placeholder_vehicules->setText(QCoreApplication::translate("CampagnesPage", "Module Gestion des v\303\251hicules", nullptr));
        label_placeholder_equipements->setText(QCoreApplication::translate("CampagnesPage", "Module Gestion des \303\251quipements", nullptr));
        label_placeholder_interventions->setText(QCoreApplication::translate("CampagnesPage", "Module Gestion des interventions", nullptr));
        groupBox_formulaire->setTitle(QCoreApplication::translate("CampagnesPage", "Fiche campagne", nullptr));
        label_id->setText(QCoreApplication::translate("CampagnesPage", "ID :", nullptr));
        lineEdit_id->setPlaceholderText(QCoreApplication::translate("CampagnesPage", "Identifiant (entier)", nullptr));
        label_titre->setText(QCoreApplication::translate("CampagnesPage", "Titre :", nullptr));
        lineEdit_titre->setPlaceholderText(QCoreApplication::translate("CampagnesPage", "Ex. : \303\211coles sans risque", nullptr));
        label_theme->setText(QCoreApplication::translate("CampagnesPage", "Th\303\250me :", nullptr));
        comboBox_theme->setItemText(0, QCoreApplication::translate("CampagnesPage", "S\303\251curit\303\251 incendie", nullptr));
        comboBox_theme->setItemText(1, QCoreApplication::translate("CampagnesPage", "Premiers secours", nullptr));
        comboBox_theme->setItemText(2, QCoreApplication::translate("CampagnesPage", "Feux de for\303\252t", nullptr));
        comboBox_theme->setItemText(3, QCoreApplication::translate("CampagnesPage", "Accidents domestiques", nullptr));
        comboBox_theme->setItemText(4, QCoreApplication::translate("CampagnesPage", "S\303\251curit\303\251 routi\303\250re", nullptr));
        comboBox_theme->setItemText(5, QCoreApplication::translate("CampagnesPage", "Risques naturels", nullptr));

        label_date->setText(QCoreApplication::translate("CampagnesPage", "Date :", nullptr));
        dateEdit_date->setDisplayFormat(QCoreApplication::translate("CampagnesPage", "dd/MM/yyyy", nullptr));
        label_lieu->setText(QCoreApplication::translate("CampagnesPage", "Lieu :", nullptr));
        lineEdit_lieu->setPlaceholderText(QCoreApplication::translate("CampagnesPage", "\303\211tablissement, adresse\342\200\246", nullptr));
        label_public->setText(QCoreApplication::translate("CampagnesPage", "Public cible :", nullptr));
        comboBox_public->setItemText(0, QCoreApplication::translate("CampagnesPage", "\303\211coles", nullptr));
        comboBox_public->setItemText(1, QCoreApplication::translate("CampagnesPage", "Entreprises", nullptr));
        comboBox_public->setItemText(2, QCoreApplication::translate("CampagnesPage", "Habitants", nullptr));
        comboBox_public->setItemText(3, QCoreApplication::translate("CampagnesPage", "Associations", nullptr));
        comboBox_public->setItemText(4, QCoreApplication::translate("CampagnesPage", "Administrations", nullptr));

        label_participants->setText(QCoreApplication::translate("CampagnesPage", "Participants :", nullptr));
        label_zone->setText(QCoreApplication::translate("CampagnesPage", "Zone :", nullptr));
        label_animateur->setText(QCoreApplication::translate("CampagnesPage", "Animateur :", nullptr));
        pushButton_ajouter->setText(QCoreApplication::translate("CampagnesPage", "\342\236\225 Ajouter", nullptr));
        pushButton_modifier->setText(QCoreApplication::translate("CampagnesPage", "\342\234\217\357\270\217 Modifier", nullptr));
        pushButton_supprimer->setText(QCoreApplication::translate("CampagnesPage", "\360\237\227\221\357\270\217 Supprimer", nullptr));
        pushButton_vider->setText(QCoreApplication::translate("CampagnesPage", "\342\206\272 Vider", nullptr));
        groupBox_liste->setTitle(QCoreApplication::translate("CampagnesPage", "Liste des campagnes", nullptr));
        lineEdit_recherche->setPlaceholderText(QCoreApplication::translate("CampagnesPage", "\360\237\224\215 Rechercher\342\200\246", nullptr));
        comboBox_critere->setItemText(0, QCoreApplication::translate("CampagnesPage", "Th\303\250me", nullptr));
        comboBox_critere->setItemText(1, QCoreApplication::translate("CampagnesPage", "Public cible", nullptr));
        comboBox_critere->setItemText(2, QCoreApplication::translate("CampagnesPage", "Titre", nullptr));
        comboBox_critere->setItemText(3, QCoreApplication::translate("CampagnesPage", "Lieu", nullptr));

        label_tri->setText(QCoreApplication::translate("CampagnesPage", "Trier :", nullptr));
        comboBox_tri->setItemText(0, QCoreApplication::translate("CampagnesPage", "Date (r\303\251centes)", nullptr));
        comboBox_tri->setItemText(1, QCoreApplication::translate("CampagnesPage", "Date (anciennes)", nullptr));
        comboBox_tri->setItemText(2, QCoreApplication::translate("CampagnesPage", "Participants (d\303\251croissant)", nullptr));
        comboBox_tri->setItemText(3, QCoreApplication::translate("CampagnesPage", "Participants (croissant)", nullptr));

        pushButton_exportPdf->setText(QCoreApplication::translate("CampagnesPage", "\360\237\223\204 PDF", nullptr));
        pushButton_exportExcel->setText(QCoreApplication::translate("CampagnesPage", "\360\237\223\212 Excel", nullptr));
        label_compteur->setText(QCoreApplication::translate("CampagnesPage", "0 campagne(s)", nullptr));
        tabWidget_campagnes->setTabText(tabWidget_campagnes->indexOf(tab_gestion), QCoreApplication::translate("CampagnesPage", "\360\237\223\213 Gestion", nullptr));
        label_statCritere->setText(QCoreApplication::translate("CampagnesPage", "R\303\251partition des campagnes :", nullptr));
        comboBox_statCritere->setItemText(0, QCoreApplication::translate("CampagnesPage", "Par th\303\250me", nullptr));
        comboBox_statCritere->setItemText(1, QCoreApplication::translate("CampagnesPage", "Par zone", nullptr));

        pushButton_actualiserStats->setText(QCoreApplication::translate("CampagnesPage", "\360\237\224\204 Actualiser", nullptr));
        label_statTotal->setText(QCoreApplication::translate("CampagnesPage", "Total : 0 campagne(s) \342\200\224 0 participant(s)", nullptr));
        groupBox_graphique->setTitle(QCoreApplication::translate("CampagnesPage", "Graphique", nullptr));
        tabWidget_campagnes->setTabText(tabWidget_campagnes->indexOf(tab_stats), QCoreApplication::translate("CampagnesPage", "\360\237\223\210 Statistiques", nullptr));
        groupBox_ciblage->setTitle(QCoreApplication::translate("CampagnesPage", "Ciblage automatique des zones les plus touch\303\251es", nullptr));
        pushButton_lancerCiblage->setText(QCoreApplication::translate("CampagnesPage", "\360\237\216\257 Calculer le ciblage", nullptr));
        label_zonePrioritaire->setText(QCoreApplication::translate("CampagnesPage", "Zone prioritaire : \342\200\224", nullptr));
        pushButton_planifierZone->setText(QCoreApplication::translate("CampagnesPage", "\360\237\223\205 Planifier une campagne dans cette zone", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_ciblage->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("CampagnesPage", "Zone", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_ciblage->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("CampagnesPage", "Niveau de risque", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_ciblage->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("CampagnesPage", "Interventions (12 mois)", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_ciblage->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("CampagnesPage", "Campagnes (12 mois)", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_ciblage->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("CampagnesPage", "Score", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_ciblage->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("CampagnesPage", "Recommandation", nullptr));
        groupBox_impact->setTitle(QCoreApplication::translate("CampagnesPage", "\303\211valuation de l'impact d'une campagne (interventions avant / apr\303\250s)", nullptr));
        label_campagneImpact->setText(QCoreApplication::translate("CampagnesPage", "Campagne :", nullptr));
        label_fenetre->setText(QCoreApplication::translate("CampagnesPage", "P\303\251riode compar\303\251e :", nullptr));
        spinBox_fenetre->setSuffix(QCoreApplication::translate("CampagnesPage", " mois", nullptr));
        pushButton_evaluerImpact->setText(QCoreApplication::translate("CampagnesPage", "\360\237\223\211 \303\211valuer l'impact", nullptr));
        label_impactAvant->setText(QCoreApplication::translate("CampagnesPage", "Interventions avant : \342\200\224", nullptr));
        label_impactApres->setText(QCoreApplication::translate("CampagnesPage", "Interventions apr\303\250s : \342\200\224", nullptr));
        label_impactVariation->setText(QCoreApplication::translate("CampagnesPage", "Variation : \342\200\224", nullptr));
        label_impactVerdict->setText(QString());
        tabWidget_campagnes->setTabText(tabWidget_campagnes->indexOf(tab_ciblage), QCoreApplication::translate("CampagnesPage", "\360\237\216\257 Ciblage && impact", nullptr));
        groupBox_afficheParams->setTitle(QCoreApplication::translate("CampagnesPage", "G\303\251n\303\251ration d'affiche de sensibilisation", nullptr));
        label_afficheCampagne->setText(QCoreApplication::translate("CampagnesPage", "Campagne (th\303\250me, public, zone) :", nullptr));
        label_afficheSlogan->setText(QCoreApplication::translate("CampagnesPage", "Slogan (laisser vide = propos\303\251 automatiquement) :", nullptr));
        pushButton_genererAffiche->setText(QCoreApplication::translate("CampagnesPage", "\342\234\250 G\303\251n\303\251rer l'affiche", nullptr));
        pushButton_enregistrerAffiche->setText(QCoreApplication::translate("CampagnesPage", "\360\237\222\276 Enregistrer (PNG)", nullptr));
        label_affichePreview->setText(QCoreApplication::translate("CampagnesPage", "Aper\303\247u de l'affiche", nullptr));
        tabWidget_campagnes->setTabText(tabWidget_campagnes->indexOf(tab_affiche), QCoreApplication::translate("CampagnesPage", "\360\237\226\274\357\270\217 Affiche IA", nullptr));
        label_placeholder_zones->setText(QCoreApplication::translate("CampagnesPage", "Module Gestion des zones de couverture", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CampagnesPage: public Ui_CampagnesPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMPAGNESPAGE_H
