#include "mainwindow.h"
#include "menupage.h"
#include "employespage.h"
#include "interventionspage.h"
#include "vehiculespage.h"
#include "equipementspage.h"
#include "zonespage.h"
#include "campagnespage.h"

#include <QAbstractButton>
#include <QButtonGroup>
#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    setObjectName("MainWindow");
    setWindowTitle(tr("FireStation Manager"));
    setWindowIcon(QIcon(":/images/logo.png"));
    setMinimumSize(1280, 740);
    resize(1500, 860);

    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);
    root->addWidget(buildSidebar());

    auto *center = new QFrame;
    center->setObjectName("centerFrame");
    auto *cl = new QVBoxLayout(center);
    cl->setContentsMargins(0, 0, 0, 0);
    cl->setSpacing(0);
    cl->addWidget(buildTopBar());

    // Modules proposés dans le menu (même ordre que l'enum Page)
    const QList<MenuPage::Module> modules = {
        {Interventions, tr("Interventions"), tr("Suivi, affectation et parcours"), ":/icons/nav/interventions.png"},
        {Equipe, tr("Équipe"), tr("Gestion des employés"), ":/icons/nav/equipe.png"},
        {Vehicules, tr("Véhicules"), tr("Flotte et maintenance"), ":/icons/nav/vehicules.png"},
        {Carte, tr("Carte"), tr("Zones de couverture de Tunis"), ":/icons/nav/carte.png"},
        {Equipements, tr("Équipements"), tr("Stocks, contrôles et alertes"), ":/icons/nav/equipements.png"},
        {Sensibilisation, tr("Campagne de sensibilisation"), tr("Prévention et ciblage des zones"),
         ":/icons/nav/sensibilisation.png"},
    };

    m_stack = new QStackedWidget;
    m_menu = new MenuPage(modules);
    m_interventions = new InterventionsPage;
    m_employes = new EmployesPage;
    m_vehicules = new VehiculesPage;
    m_equipements = new EquipementsPage;
    m_zones = new ZonesPage;
    m_campagnes = new CampagnesPage;
    // L'ordre doit suivre l'enum Page.
    m_stack->addWidget(m_menu);
    m_stack->addWidget(m_interventions);
    m_stack->addWidget(m_employes);
    m_stack->addWidget(m_vehicules);
    m_stack->addWidget(m_equipements);
    m_stack->addWidget(m_zones);
    m_stack->addWidget(m_campagnes);
    cl->addWidget(m_stack, 1);
    root->addWidget(center, 1);

    connect(m_menu, &MenuPage::moduleChoisi, this, &MainWindow::afficherPage);

    // La recherche de la barre du haut filtre les pages qui ont une liste
    connect(m_search, &QLineEdit::textChanged, m_employes, &EmployesPage::setSearch);
    connect(m_search, &QLineEdit::textChanged, m_interventions, &InterventionsPage::setSearch);
    connect(m_search, &QLineEdit::textChanged, m_zones, &ZonesPage::setSearch);

    // Après la connexion : le menu principal
    afficherPage(Menu);
}

void MainWindow::afficherPage(int page)
{
    m_stack->setCurrentIndex(page);
    if (QAbstractButton *b = m_navGroup->button(page))
        b->setChecked(true);
    // Sur le menu, la sidebar est inutile : elle apparaît dès qu'un module est ouvert
    m_sidebar->setVisible(page != Menu);
    m_search->setVisible(page != Menu);
}

QFrame *MainWindow::buildSidebar()
{
    m_sidebar = new QFrame;
    m_sidebar->setObjectName("sidebar");
    m_sidebar->setFixedWidth(230);
    auto *lay = new QVBoxLayout(m_sidebar);
    lay->setContentsMargins(12, 24, 12, 24);
    lay->setSpacing(4);

    auto *logo = new QLabel;
    logo->setObjectName("logoLabel");
    logo->setFixedSize(124, 124);
    logo->setPixmap(QPixmap(":/images/logo.png"));
    logo->setScaledContents(true);
    lay->addWidget(logo, 0, Qt::AlignHCenter);
    auto *nomApp = new QLabel(tr("FIRE STATION"));
    nomApp->setObjectName("sidebarNomApp");
    nomApp->setAlignment(Qt::AlignCenter);
    lay->addWidget(nomApp);
    lay->addSpacing(18);

    m_navGroup = new QButtonGroup(this);
    m_navGroup->setExclusive(true);

    struct Nav { Page page; const char *text; const char *icon; };
    const Nav items[] = {
        {Menu, "Menu", ":/icons/nav/home.png"},
        {Interventions, "Interventions", ":/icons/nav/interventions.png"},
        {Equipe, "Équipe", ":/icons/nav/equipe.png"},
        {Vehicules, "Véhicules", ":/icons/nav/vehicules.png"},
        {Carte, "Carte", ":/icons/nav/carte.png"},
        {Equipements, "Équipements", ":/icons/nav/equipements.png"},
        {Sensibilisation, "Campagne de\nSensibilisation", ":/icons/nav/sensibilisation.png"},
    };
    for (const Nav &n : items) {
        auto *b = new QPushButton("  " + QString::fromUtf8(n.text));
        b->setObjectName(QString("nav_%1").arg(int(n.page)));
        b->setIcon(QIcon(n.icon));
        b->setIconSize(QSize(22, 22));
        b->setCheckable(true);
        b->setCursor(Qt::PointingHandCursor);
        m_navGroup->addButton(b, n.page);
        lay->addWidget(b);
    }
    lay->addStretch(1);

    connect(m_navGroup, &QButtonGroup::idClicked, this, &MainWindow::afficherPage);
    return m_sidebar;
}

QFrame *MainWindow::buildTopBar()
{
    auto *bar = new QFrame;
    bar->setObjectName("topBar");
    bar->setFixedHeight(58);
    auto *lay = new QHBoxLayout(bar);
    lay->setContentsMargins(16, 8, 16, 8);
    lay->setSpacing(10);

    auto iconButton = [](const QString &name, const QString &icon) {
        auto *b = new QPushButton;
        b->setObjectName(name);
        b->setIcon(QIcon(icon));
        b->setIconSize(QSize(22, 22));
        b->setCursor(Qt::PointingHandCursor);
        return b;
    };

    auto *menu = iconButton("btnMenu", ":/icons/menu.png");
    lay->addWidget(menu);

    m_search = new QLineEdit;
    m_search->setObjectName("searchEdit");
    m_search->setPlaceholderText(tr("Rechercher"));
    m_search->setClearButtonEnabled(true);
    lay->addWidget(m_search);
    lay->addStretch(1);

    // Cloche + pastille de notifications
    auto *bellHost = new QWidget;
    bellHost->setFixedSize(42, 38);
    auto *bell = iconButton("btnNotif", ":/icons/bell.png");
    bell->setParent(bellHost);
    bell->setGeometry(0, 2, 36, 36);
    auto *badge = new QLabel("5", bellHost);
    badge->setObjectName("notifBadge");
    badge->setAlignment(Qt::AlignCenter);
    badge->setGeometry(22, 0, 18, 18);
    lay->addWidget(bellHost);

    auto *avatar = new QLabel;
    avatar->setFixedSize(38, 38);
    avatar->setPixmap(QPixmap(":/icons/avatar.png"));
    avatar->setScaledContents(true);
    lay->addWidget(avatar);

    m_labelUtilisateur = new QLabel(tr("CHEF DE CENTRE ·\nRAYEN"));
    m_labelUtilisateur->setObjectName("labelAdmin");
    lay->addWidget(m_labelUtilisateur);

    auto *power = iconButton("btnPower", ":/icons/power.png");
    lay->addWidget(power);

    connect(menu, &QPushButton::clicked, this, [this] {
        // Sur le menu principal, le bouton ne fait rien (pas de sidebar à afficher)
        if (m_stack->currentIndex() != Menu)
            m_sidebar->setVisible(!m_sidebar->isVisible());
    });
    connect(bell, &QPushButton::clicked, this, [this, badge] {
        QMessageBox::information(this, tr("Notifications"), tr("Vous avez %1 notifications.").arg(badge->text()));
    });
    connect(power, &QPushButton::clicked, this, [this] {
        if (QMessageBox::question(this, tr("Quitter"), tr("Fermer l'application ?")) == QMessageBox::Yes)
            close();
    });
    return bar;
}

// Affiche le nom saisi dans la fenêtre de connexion (barre du haut et menu).
void MainWindow::setUtilisateur(const QString &nom)
{
    m_menu->setUtilisateur(nom);
    if (!nom.isEmpty())
        m_labelUtilisateur->setText(tr("CONNECTÉ ·\n%1").arg(nom.toUpper()));
}
