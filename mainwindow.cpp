#include "mainwindow.h"
#include "employespage.h"
#include "interventionspage.h"

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
    setWindowTitle(tr("Station de feu - Employés & Interventions"));
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

    m_stack = new QStackedWidget;
    m_employes = new EmployesPage;
    m_interventions = new InterventionsPage;
    // L'ordre doit suivre l'enum Page.
    m_stack->addWidget(placeholder(tr("Tableau de bord")));
    m_stack->addWidget(m_interventions);
    m_stack->addWidget(m_employes);
    m_stack->addWidget(placeholder(tr("Véhicules")));
    m_stack->addWidget(placeholder(tr("Carte")));
    m_stack->addWidget(placeholder(tr("Campagne de Sensibilisation")));
    m_stack->addWidget(placeholder(tr("Volontaires")));
    m_stack->addWidget(placeholder(tr("Paramètres")));
    cl->addWidget(m_stack, 1);
    root->addWidget(center, 1);

    // La recherche de la barre du haut filtre les deux pages.
    connect(m_search, &QLineEdit::textChanged, m_employes, &EmployesPage::setSearch);
    connect(m_search, &QLineEdit::textChanged, m_interventions, &InterventionsPage::setSearch);

    // Page affichée au démarrage : Équipe (gestion des employés).
    m_navGroup->button(Equipe)->setChecked(true);
    m_stack->setCurrentIndex(Equipe);
}

QFrame *MainWindow::buildSidebar()
{
    m_sidebar = new QFrame;
    m_sidebar->setObjectName("sidebar");
    m_sidebar->setFixedWidth(215);
    auto *lay = new QVBoxLayout(m_sidebar);
    lay->setContentsMargins(0, 24, 0, 24);
    lay->setSpacing(2);

    auto *logo = new QLabel;
    logo->setObjectName("logoLabel");
    logo->setFixedSize(124, 124);
    logo->setPixmap(QPixmap(":/images/logo.png"));
    logo->setScaledContents(true);
    lay->addWidget(logo, 0, Qt::AlignHCenter);
    lay->addSpacing(22);

    m_navGroup = new QButtonGroup(this);
    m_navGroup->setExclusive(true);

    struct Nav { Page page; const char *text; const char *icon; };
    const Nav items[] = {
        {Dashboard, "Tableau de bord", ":/icons/home.png"},
        {Interventions, "Interventions", ":/icons/interventions.png"},
        {Equipe, "Équipe", ":/icons/team.png"},
        {Vehicules, "Véhicules", ":/icons/vehicules.png"},
        {Carte, "Carte", ":/icons/zones.png"},
        {Sensibilisation, "Campagne de\nSensibilisation", ":/icons/sensibilisation.png"},
        {Volontaires, "Volontaires", ":/icons/employes.png"},
        {Parametres, "Paramètres", ":/icons/parametres.png"},
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

    connect(m_navGroup, &QButtonGroup::idClicked, this, [this](int id) { m_stack->setCurrentIndex(id); });
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

    auto *gear = iconButton("btnSettings", ":/icons/gear.png");
    lay->addWidget(gear);

    auto *avatar = new QLabel;
    avatar->setFixedSize(38, 38);
    avatar->setPixmap(QPixmap(":/icons/avatar.png"));
    avatar->setScaledContents(true);
    lay->addWidget(avatar);

    auto *name = new QLabel(tr("CHEF DE CENTRE ·\nRAYEN"));
    name->setObjectName("labelAdmin");
    lay->addWidget(name);

    auto *power = iconButton("btnPower", ":/icons/power.png");
    lay->addWidget(power);

    connect(menu, &QPushButton::clicked, this, [this] { m_sidebar->setVisible(!m_sidebar->isVisible()); });
    connect(bell, &QPushButton::clicked, this, [this, badge] {
        QMessageBox::information(this, tr("Notifications"), tr("Vous avez %1 notifications.").arg(badge->text()));
    });
    connect(gear, &QPushButton::clicked, this, [this] {
        m_navGroup->button(Parametres)->setChecked(true);
        m_stack->setCurrentIndex(Parametres);
    });
    connect(power, &QPushButton::clicked, this, [this] {
        if (QMessageBox::question(this, tr("Quitter"), tr("Fermer l'application ?")) == QMessageBox::Yes)
            close();
    });
    return bar;
}

QWidget *MainWindow::placeholder(const QString &title) const
{
    auto *w = new QWidget;
    auto *l = new QVBoxLayout(w);
    auto *t = new QLabel(title);
    t->setObjectName("labelTitle");
    t->setAlignment(Qt::AlignCenter);
    auto *s = new QLabel(tr("Module à intégrer ultérieurement."));
    s->setAlignment(Qt::AlignCenter);
    l->addStretch(1);
    l->addWidget(t);
    l->addWidget(s);
    l->addStretch(1);
    return w;
}
