#include "integratedwindow.h"

#include "../mainwindow.h"
#include "emp_interv/employeswindow.h"
#include "emp_interv/gestioninterventions.h"

#include <QEvent>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QVBoxLayout>

IntegratedWindow::IntegratedWindow(QWidget *parent)
    : QMainWindow(parent), pages(new QStackedWidget(this))
{
    auto *central = new QWidget(this);
    auto *layout = new QHBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Une seule barre de navigation pour toutes les pages intégrées.
    auto *sidebar = new QFrame(central);
    sidebar->setObjectName("sharedSidebar");
    sidebar->setFixedWidth(215);
    auto *menu = new QVBoxLayout(sidebar);
    menu->setContentsMargins(12, 18, 12, 18);
    menu->setSpacing(8);
    auto *brand = new QLabel(QStringLiteral("FIRE STATION"), sidebar);
    brand->setObjectName("sharedBrand");
    brand->setAlignment(Qt::AlignCenter);
    menu->addWidget(brand);
    menu->addSpacing(22);
    const QStringList labels = {QStringLiteral("▦   Tableau de bord"), QStringLiteral("☻   Équipe"),
                                QStringLiteral("▣   Véhicules"), QStringLiteral("⚠   Interventions")};
    for (int i = 0; i < labels.size(); ++i) {
        auto *button = new QPushButton(labels.at(i), sidebar);
        button->setCheckable(true);
        button->setAutoExclusive(true);
        button->setMinimumHeight(46);
        menu->addWidget(button);
        menuButtons.append(button);
        connect(button, &QPushButton::clicked, this, [this, i]() { selectionnerMenu(i); });
    }
    menu->addStretch();
    sidebar->setStyleSheet(QStringLiteral(
        "#sharedSidebar { border-image: url(:/images/sidebar_bg.png) 0 0 0 0 stretch stretch; }"
        "#sharedBrand { color:white; font-size:18px; font-weight:bold; padding:18px 0; }"
        "#sharedSidebar QPushButton { color:white; text-align:left; background:rgba(76,5,8,190);"
        "border:0; border-radius:8px; padding:10px 12px; }"
        "#sharedSidebar QPushButton:hover, #sharedSidebar QPushButton:checked { background:#d71920; }"));
    layout->addWidget(sidebar);
    layout->addWidget(pages, 1);

    employes = new EmployesWindow(pages);
    employes->setWindowFlags(Qt::Widget);
    vehicules = new VehicleWindow(pages);
    vehicules->setWindowFlags(Qt::Widget);
    interventions = new GestionInterventions(pages);
    interventions->setWindowFlags(Qt::Widget);
    if (auto *internal = employes->findChild<QWidget *>("sidebar")) internal->hide();
    if (auto *internal = vehicules->findChild<QWidget *>("sidebar")) internal->hide();
    if (auto *internal = interventions->findChild<QWidget *>("sidebar")) internal->hide();
    pages->addWidget(employes);
    pages->addWidget(vehicules);
    pages->addWidget(interventions);

    connect(employes, &EmployesWindow::ouvrirVehicules, this, [this]() { afficherPage(1); });
    connect(employes, &EmployesWindow::ouvrirInterventions, this, [this]() { afficherPage(2); });
    connect(employes, &EmployesWindow::ouvrirEquipe, this, [this]() {
        pages->setCurrentIndex(0);
        employes->afficherEquipe();
        setWindowTitle(QStringLiteral("Fire Station — Gestion des employés"));
        menuButtons.at(1)->setChecked(true);
    });
    connect(vehicules, &VehicleWindow::ouvrirTableauDeBord, this, [this]() { afficherPage(0); });
    connect(vehicules, &VehicleWindow::ouvrirInterventions, this, [this]() { afficherPage(2); });
    connect(interventions, &GestionInterventions::ouvrirEmployes, this, [this]() { afficherPage(0); });

    employes->installEventFilter(this);
    vehicules->installEventFilter(this);
    interventions->installEventFilter(this);

    setCentralWidget(central);
    setMinimumSize(1280, 720);
    afficherPage(0);
}

void IntegratedWindow::afficherPage(int index)
{
    pages->setCurrentIndex(index);
    if (index == 0) {
        employes->marquerMenu();
        setWindowTitle(QStringLiteral("Fire Station — Tableau de bord"));
    } else if (index == 1) {
        vehicules->afficherVehicules();
        setWindowTitle(QStringLiteral("Fire Station — Gestion des véhicules"));
    } else {
        interventions->marquerMenu();
        setWindowTitle(QStringLiteral("Fire Station — Gestion des interventions"));
    }
    const int menuIndex = index == 0 ? 0 : (index == 1 ? 2 : 3);
    if (menuIndex < menuButtons.size()) menuButtons.at(menuIndex)->setChecked(true);
}

void IntegratedWindow::selectionnerMenu(int index)
{
    if (index == 0) afficherPage(0);
    else if (index == 1) {
        pages->setCurrentIndex(0);
        employes->afficherEquipe();
        setWindowTitle(QStringLiteral("Fire Station — Gestion des employés"));
        menuButtons.at(1)->setChecked(true);
    } else if (index == 2) afficherPage(1);
    else if (index == 3) afficherPage(2);
}

bool IntegratedWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Close && (watched == employes || watched == vehicules || watched == interventions)) {
        QMetaObject::invokeMethod(this, [this]() { close(); }, Qt::QueuedConnection);
        return true;
    }
    return QMainWindow::eventFilter(watched, event);
}
