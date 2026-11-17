/********************************************************************************
** Form generated from reading UI file 'zonespage.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ZONESPAGE_H
#define UI_ZONESPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ZonesPage
{
public:
    QVBoxLayout *verticalLayout_root;
    QTabWidget *tabWidget_zones;
    QWidget *tab_gestion;
    QHBoxLayout *horizontalLayout_gestion;
    QGroupBox *groupBox_formulaire;
    QVBoxLayout *verticalLayout_formulaire;
    QFormLayout *formLayout_zone;
    QLabel *label_id;
    QLineEdit *lineEdit_id;
    QLabel *label_nom;
    QLineEdit *lineEdit_nom;
    QLabel *label_region;
    QLineEdit *lineEdit_region;
    QLabel *label_superficie;
    QDoubleSpinBox *doubleSpinBox_superficie;
    QLabel *label_population;
    QSpinBox *spinBox_population;
    QLabel *label_risque;
    QComboBox *comboBox_risque;
    QLabel *label_temps;
    QSpinBox *spinBox_temps;
    QLabel *label_latitude;
    QDoubleSpinBox *doubleSpinBox_latitude;
    QLabel *label_longitude;
    QDoubleSpinBox *doubleSpinBox_longitude;
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
    QTableView *tableView_zones;
    QLabel *label_compteur;
    QWidget *tab_carte;
    QHBoxLayout *horizontalLayout_carte;
    QGroupBox *groupBox_carte;
    QVBoxLayout *verticalLayout_carte;
    QGroupBox *groupBox_alertes;
    QVBoxLayout *verticalLayout_alertes;
    QHBoxLayout *horizontalLayout_seuil;
    QLabel *label_seuil;
    QSpinBox *spinBox_seuil;
    QSpacerItem *horizontalSpacer_seuil;
    QLabel *label_alerteResume;
    QTableWidget *tableWidget_alertes;
    QLabel *label_zoneSelection;
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

    void setupUi(QWidget *ZonesPage)
    {
        if (ZonesPage->objectName().isEmpty())
            ZonesPage->setObjectName("ZonesPage");
        ZonesPage->resize(1200, 760);
        ZonesPage->setStyleSheet(QString::fromUtf8("QWidget#ZonesPage { background: #eef0f4; }\n"
"QWidget { font-size: 13px; color: #2B2B2B; }\n"
"QGroupBox { background: #FFFFFF; border: 1px solid #E0E0E0; border-radius: 8px; margin-top: 18px; padding: 10px; font-weight: bold; }\n"
"QGroupBox::title { subcontrol-origin: margin; left: 12px; padding: 0 6px; color: #C62828; }\n"
"QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox { background: #FFFFFF; border: 1px solid #BDBDBD; border-radius: 5px; padding: 5px; min-height: 22px; }\n"
"QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus { border: 1px solid #FF6F00; }\n"
"QPushButton { background: #FF6F00; color: #FFFFFF; border: none; border-radius: 6px; padding: 8px 14px; font-weight: bold; }\n"
"QPushButton:hover { background: #E65100; }\n"
"#pushButton_ajouter { background: #2E7D32; }\n"
"#pushButton_ajouter:hover { background: #1B5E20; }\n"
"#pushButton_supprimer { background: #C62828; }\n"
"#pushButton_supprimer:hover { background: #8E0000; }\n"
"#pushButton_vider { background: #757575; }\n"
""
                        "QTableView, QTableWidget { background: #FFFFFF; border: 1px solid #E0E0E0; border-radius: 6px; gridline-color: #EEEEEE; selection-background-color: #FFE0B2; selection-color: #2B2B2B; alternate-background-color: #FFF3E0; }\n"
"QHeaderView::section { background: #C62828; color: #FFFFFF; padding: 6px; border: none; font-weight: bold; }\n"
"QTabWidget::pane { border: none; }\n"
"QTabBar::tab { background: #E0E0E0; padding: 8px 18px; min-width: 150px; margin-right: 4px; border-top-left-radius: 6px; border-top-right-radius: 6px; }\n"
"QTabBar::tab:selected { background: #C62828; color: #FFFFFF; font-weight: bold; }\n"
"#label_alerteResume { font-weight: bold; padding: 8px 10px; border-radius: 6px; }\n"
"#label_zoneSelection { color: #374151; }"));
        verticalLayout_root = new QVBoxLayout(ZonesPage);
        verticalLayout_root->setObjectName("verticalLayout_root");
        verticalLayout_root->setContentsMargins(16, 12, 16, 12);
        tabWidget_zones = new QTabWidget(ZonesPage);
        tabWidget_zones->setObjectName("tabWidget_zones");
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
        formLayout_zone = new QFormLayout();
        formLayout_zone->setObjectName("formLayout_zone");
        formLayout_zone->setLabelAlignment(Qt::AlignRight|Qt::AlignVCenter);
        label_id = new QLabel(groupBox_formulaire);
        label_id->setObjectName("label_id");

        formLayout_zone->setWidget(0, QFormLayout::ItemRole::LabelRole, label_id);

        lineEdit_id = new QLineEdit(groupBox_formulaire);
        lineEdit_id->setObjectName("lineEdit_id");

        formLayout_zone->setWidget(0, QFormLayout::ItemRole::FieldRole, lineEdit_id);

        label_nom = new QLabel(groupBox_formulaire);
        label_nom->setObjectName("label_nom");

        formLayout_zone->setWidget(1, QFormLayout::ItemRole::LabelRole, label_nom);

        lineEdit_nom = new QLineEdit(groupBox_formulaire);
        lineEdit_nom->setObjectName("lineEdit_nom");
        lineEdit_nom->setMaxLength(60);

        formLayout_zone->setWidget(1, QFormLayout::ItemRole::FieldRole, lineEdit_nom);

        label_region = new QLabel(groupBox_formulaire);
        label_region->setObjectName("label_region");

        formLayout_zone->setWidget(2, QFormLayout::ItemRole::LabelRole, label_region);

        lineEdit_region = new QLineEdit(groupBox_formulaire);
        lineEdit_region->setObjectName("lineEdit_region");
        lineEdit_region->setMaxLength(60);

        formLayout_zone->setWidget(2, QFormLayout::ItemRole::FieldRole, lineEdit_region);

        label_superficie = new QLabel(groupBox_formulaire);
        label_superficie->setObjectName("label_superficie");

        formLayout_zone->setWidget(3, QFormLayout::ItemRole::LabelRole, label_superficie);

        doubleSpinBox_superficie = new QDoubleSpinBox(groupBox_formulaire);
        doubleSpinBox_superficie->setObjectName("doubleSpinBox_superficie");
        doubleSpinBox_superficie->setDecimals(1);
        doubleSpinBox_superficie->setMinimum(0.000000000000000);
        doubleSpinBox_superficie->setMaximum(5000.000000000000000);
        doubleSpinBox_superficie->setValue(1.000000000000000);

        formLayout_zone->setWidget(3, QFormLayout::ItemRole::FieldRole, doubleSpinBox_superficie);

        label_population = new QLabel(groupBox_formulaire);
        label_population->setObjectName("label_population");

        formLayout_zone->setWidget(4, QFormLayout::ItemRole::LabelRole, label_population);

        spinBox_population = new QSpinBox(groupBox_formulaire);
        spinBox_population->setObjectName("spinBox_population");
        spinBox_population->setMinimum(0);
        spinBox_population->setMaximum(5000000);
        spinBox_population->setValue(1000);

        formLayout_zone->setWidget(4, QFormLayout::ItemRole::FieldRole, spinBox_population);

        label_risque = new QLabel(groupBox_formulaire);
        label_risque->setObjectName("label_risque");

        formLayout_zone->setWidget(5, QFormLayout::ItemRole::LabelRole, label_risque);

        comboBox_risque = new QComboBox(groupBox_formulaire);
        comboBox_risque->addItem(QString());
        comboBox_risque->addItem(QString());
        comboBox_risque->addItem(QString());
        comboBox_risque->setObjectName("comboBox_risque");

        formLayout_zone->setWidget(5, QFormLayout::ItemRole::FieldRole, comboBox_risque);

        label_temps = new QLabel(groupBox_formulaire);
        label_temps->setObjectName("label_temps");

        formLayout_zone->setWidget(6, QFormLayout::ItemRole::LabelRole, label_temps);

        spinBox_temps = new QSpinBox(groupBox_formulaire);
        spinBox_temps->setObjectName("spinBox_temps");
        spinBox_temps->setMinimum(1);
        spinBox_temps->setMaximum(180);
        spinBox_temps->setValue(10);

        formLayout_zone->setWidget(6, QFormLayout::ItemRole::FieldRole, spinBox_temps);

        label_latitude = new QLabel(groupBox_formulaire);
        label_latitude->setObjectName("label_latitude");

        formLayout_zone->setWidget(7, QFormLayout::ItemRole::LabelRole, label_latitude);

        doubleSpinBox_latitude = new QDoubleSpinBox(groupBox_formulaire);
        doubleSpinBox_latitude->setObjectName("doubleSpinBox_latitude");
        doubleSpinBox_latitude->setDecimals(4);
        doubleSpinBox_latitude->setMinimum(36.500000000000000);
        doubleSpinBox_latitude->setMaximum(37.200000000000003);
        doubleSpinBox_latitude->setSingleStep(0.005000000000000);
        doubleSpinBox_latitude->setValue(36.799999999999997);

        formLayout_zone->setWidget(7, QFormLayout::ItemRole::FieldRole, doubleSpinBox_latitude);

        label_longitude = new QLabel(groupBox_formulaire);
        label_longitude->setObjectName("label_longitude");

        formLayout_zone->setWidget(8, QFormLayout::ItemRole::LabelRole, label_longitude);

        doubleSpinBox_longitude = new QDoubleSpinBox(groupBox_formulaire);
        doubleSpinBox_longitude->setObjectName("doubleSpinBox_longitude");
        doubleSpinBox_longitude->setDecimals(4);
        doubleSpinBox_longitude->setMinimum(9.800000000000001);
        doubleSpinBox_longitude->setMaximum(10.600000000000000);
        doubleSpinBox_longitude->setSingleStep(0.005000000000000);
        doubleSpinBox_longitude->setValue(10.180000000000000);

        formLayout_zone->setWidget(8, QFormLayout::ItemRole::FieldRole, doubleSpinBox_longitude);


        verticalLayout_formulaire->addLayout(formLayout_zone);

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

        tableView_zones = new QTableView(groupBox_liste);
        tableView_zones->setObjectName("tableView_zones");
        tableView_zones->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView_zones->setAlternatingRowColors(true);
        tableView_zones->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView_zones->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView_zones->horizontalHeader()->setStretchLastSection(true);
        tableView_zones->verticalHeader()->setVisible(false);

        verticalLayout_liste->addWidget(tableView_zones);

        label_compteur = new QLabel(groupBox_liste);
        label_compteur->setObjectName("label_compteur");

        verticalLayout_liste->addWidget(label_compteur);


        horizontalLayout_gestion->addWidget(groupBox_liste);

        tabWidget_zones->addTab(tab_gestion, QString());
        tab_carte = new QWidget();
        tab_carte->setObjectName("tab_carte");
        horizontalLayout_carte = new QHBoxLayout(tab_carte);
        horizontalLayout_carte->setObjectName("horizontalLayout_carte");
        groupBox_carte = new QGroupBox(tab_carte);
        groupBox_carte->setObjectName("groupBox_carte");
        verticalLayout_carte = new QVBoxLayout(groupBox_carte);
        verticalLayout_carte->setObjectName("verticalLayout_carte");

        horizontalLayout_carte->addWidget(groupBox_carte);

        groupBox_alertes = new QGroupBox(tab_carte);
        groupBox_alertes->setObjectName("groupBox_alertes");
        groupBox_alertes->setMinimumSize(QSize(380, 0));
        groupBox_alertes->setMaximumSize(QSize(460, 16777215));
        verticalLayout_alertes = new QVBoxLayout(groupBox_alertes);
        verticalLayout_alertes->setObjectName("verticalLayout_alertes");
        horizontalLayout_seuil = new QHBoxLayout();
        horizontalLayout_seuil->setObjectName("horizontalLayout_seuil");
        label_seuil = new QLabel(groupBox_alertes);
        label_seuil->setObjectName("label_seuil");

        horizontalLayout_seuil->addWidget(label_seuil);

        spinBox_seuil = new QSpinBox(groupBox_alertes);
        spinBox_seuil->setObjectName("spinBox_seuil");
        spinBox_seuil->setMinimum(1);
        spinBox_seuil->setMaximum(120);
        spinBox_seuil->setValue(12);

        horizontalLayout_seuil->addWidget(spinBox_seuil);

        horizontalSpacer_seuil = new QSpacerItem(10, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_seuil->addItem(horizontalSpacer_seuil);


        verticalLayout_alertes->addLayout(horizontalLayout_seuil);

        label_alerteResume = new QLabel(groupBox_alertes);
        label_alerteResume->setObjectName("label_alerteResume");
        label_alerteResume->setWordWrap(true);

        verticalLayout_alertes->addWidget(label_alerteResume);

        tableWidget_alertes = new QTableWidget(groupBox_alertes);
        if (tableWidget_alertes->columnCount() < 4)
            tableWidget_alertes->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_alertes->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_alertes->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_alertes->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_alertes->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableWidget_alertes->setObjectName("tableWidget_alertes");
        tableWidget_alertes->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget_alertes->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget_alertes->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidget_alertes->horizontalHeader()->setStretchLastSection(true);
        tableWidget_alertes->verticalHeader()->setVisible(false);

        verticalLayout_alertes->addWidget(tableWidget_alertes);

        label_zoneSelection = new QLabel(groupBox_alertes);
        label_zoneSelection->setObjectName("label_zoneSelection");
        label_zoneSelection->setWordWrap(true);

        verticalLayout_alertes->addWidget(label_zoneSelection);


        horizontalLayout_carte->addWidget(groupBox_alertes);

        tabWidget_zones->addTab(tab_carte, QString());
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
        tabWidget_zones->addTab(tab_stats, QString());

        verticalLayout_root->addWidget(tabWidget_zones);


        retranslateUi(ZonesPage);

        tabWidget_zones->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ZonesPage);
    } // setupUi

    void retranslateUi(QWidget *ZonesPage)
    {
        groupBox_formulaire->setTitle(QCoreApplication::translate("ZonesPage", "Fiche zone", nullptr));
        label_id->setText(QCoreApplication::translate("ZonesPage", "ID :", nullptr));
        lineEdit_id->setPlaceholderText(QCoreApplication::translate("ZonesPage", "Identifiant (entier)", nullptr));
        label_nom->setText(QCoreApplication::translate("ZonesPage", "Nom :", nullptr));
        lineEdit_nom->setPlaceholderText(QCoreApplication::translate("ZonesPage", "Ex. : La Marsa", nullptr));
        label_region->setText(QCoreApplication::translate("ZonesPage", "R\303\251gion / quartier :", nullptr));
        lineEdit_region->setPlaceholderText(QCoreApplication::translate("ZonesPage", "Ex. : Tunis Nord", nullptr));
        label_superficie->setText(QCoreApplication::translate("ZonesPage", "Superficie :", nullptr));
        doubleSpinBox_superficie->setSuffix(QCoreApplication::translate("ZonesPage", " km\302\262", nullptr));
        label_population->setText(QCoreApplication::translate("ZonesPage", "Population :", nullptr));
        spinBox_population->setSuffix(QCoreApplication::translate("ZonesPage", " hab.", nullptr));
        label_risque->setText(QCoreApplication::translate("ZonesPage", "Niveau de risque :", nullptr));
        comboBox_risque->setItemText(0, QCoreApplication::translate("ZonesPage", "Faible", nullptr));
        comboBox_risque->setItemText(1, QCoreApplication::translate("ZonesPage", "Moyen", nullptr));
        comboBox_risque->setItemText(2, QCoreApplication::translate("ZonesPage", "\303\211lev\303\251", nullptr));

        label_temps->setText(QCoreApplication::translate("ZonesPage", "Temps d'intervention :", nullptr));
        spinBox_temps->setSuffix(QCoreApplication::translate("ZonesPage", " min", nullptr));
        label_latitude->setText(QCoreApplication::translate("ZonesPage", "Latitude :", nullptr));
        label_longitude->setText(QCoreApplication::translate("ZonesPage", "Longitude :", nullptr));
        pushButton_ajouter->setText(QCoreApplication::translate("ZonesPage", "\342\236\225 Ajouter", nullptr));
        pushButton_modifier->setText(QCoreApplication::translate("ZonesPage", "\342\234\217\357\270\217 Modifier", nullptr));
        pushButton_supprimer->setText(QCoreApplication::translate("ZonesPage", "\360\237\227\221\357\270\217 Supprimer", nullptr));
        pushButton_vider->setText(QCoreApplication::translate("ZonesPage", "\342\206\272 Vider", nullptr));
        groupBox_liste->setTitle(QCoreApplication::translate("ZonesPage", "Liste des zones de couverture", nullptr));
        lineEdit_recherche->setPlaceholderText(QCoreApplication::translate("ZonesPage", "\360\237\224\215 Rechercher\342\200\246", nullptr));
        comboBox_critere->setItemText(0, QCoreApplication::translate("ZonesPage", "Nom", nullptr));
        comboBox_critere->setItemText(1, QCoreApplication::translate("ZonesPage", "R\303\251gion", nullptr));

        label_tri->setText(QCoreApplication::translate("ZonesPage", "Trier :", nullptr));
        comboBox_tri->setItemText(0, QCoreApplication::translate("ZonesPage", "Risque (\303\251lev\303\251 d'abord)", nullptr));
        comboBox_tri->setItemText(1, QCoreApplication::translate("ZonesPage", "Risque (faible d'abord)", nullptr));
        comboBox_tri->setItemText(2, QCoreApplication::translate("ZonesPage", "Temps d'intervention (d\303\251croissant)", nullptr));
        comboBox_tri->setItemText(3, QCoreApplication::translate("ZonesPage", "Temps d'intervention (croissant)", nullptr));
        comboBox_tri->setItemText(4, QCoreApplication::translate("ZonesPage", "Nom", nullptr));

        pushButton_exportPdf->setText(QCoreApplication::translate("ZonesPage", "\360\237\223\204 PDF", nullptr));
        pushButton_exportExcel->setText(QCoreApplication::translate("ZonesPage", "\360\237\223\212 Excel", nullptr));
        label_compteur->setText(QCoreApplication::translate("ZonesPage", "0 zone(s)", nullptr));
        tabWidget_zones->setTabText(tabWidget_zones->indexOf(tab_gestion), QCoreApplication::translate("ZonesPage", "\360\237\223\213 Gestion", nullptr));
        groupBox_carte->setTitle(QCoreApplication::translate("ZonesPage", "Carte de Tunis \342\200\224 zones \303\240 risque selon l'historique des interventions", nullptr));
        groupBox_alertes->setTitle(QCoreApplication::translate("ZonesPage", "Alertes de zones sous-couvertes", nullptr));
        label_seuil->setText(QCoreApplication::translate("ZonesPage", "Seuil d'alerte :", nullptr));
        spinBox_seuil->setSuffix(QCoreApplication::translate("ZonesPage", " min", nullptr));
        label_alerteResume->setText(QString());
        QTableWidgetItem *___qtablewidgetitem = tableWidget_alertes->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("ZonesPage", "Zone", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_alertes->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("ZonesPage", "Temps moyen", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_alertes->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("ZonesPage", "D\303\251passement", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_alertes->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("ZonesPage", "Interventions (12 mois)", nullptr));
        label_zoneSelection->setText(QCoreApplication::translate("ZonesPage", "Cliquez sur une zone de la carte pour voir sa fiche.", nullptr));
        tabWidget_zones->setTabText(tabWidget_zones->indexOf(tab_carte), QCoreApplication::translate("ZonesPage", "\360\237\227\272\357\270\217 Carte && alertes", nullptr));
        label_statCritere->setText(QCoreApplication::translate("ZonesPage", "Statistiques (12 derniers mois) :", nullptr));
        comboBox_statCritere->setItemText(0, QCoreApplication::translate("ZonesPage", "Interventions par zone", nullptr));
        comboBox_statCritere->setItemText(1, QCoreApplication::translate("ZonesPage", "Interventions par niveau de risque", nullptr));

        pushButton_actualiserStats->setText(QCoreApplication::translate("ZonesPage", "\360\237\224\204 Actualiser", nullptr));
        label_statTotal->setText(QString());
        groupBox_graphique->setTitle(QCoreApplication::translate("ZonesPage", "Graphique", nullptr));
        tabWidget_zones->setTabText(tabWidget_zones->indexOf(tab_stats), QCoreApplication::translate("ZonesPage", "\360\237\223\210 Statistiques", nullptr));
        (void)ZonesPage;
    } // retranslateUi

};

namespace Ui {
    class ZonesPage: public Ui_ZonesPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZONESPAGE_H
