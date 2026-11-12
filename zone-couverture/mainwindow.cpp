#include "mainwindow.h"
#include "alerte.h"
#include "equipe.h"
#include "zone.h"

#include <QButtonGroup>
#include <QGridLayout>
#include <QIcon>
#include <QLinearGradient>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPainterPath>
#include <functional>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QVBoxLayout>

static const char *kStyle = R"CSS(
QMainWindow, #content { background:#f3f4f7; }
QWidget { font-family:"Segoe UI","DejaVu Sans",Arial; font-size:12px; color:#222; }
#sidebar QPushButton { color:#fbe9e9; background:transparent; border:none; text-align:left; padding:10px 14px; border-radius:6px; font-size:13px; }
#sidebar QPushButton:hover { background:rgba(255,255,255,0.14); }
#sidebar QPushButton:checked { background:#d32f2f; color:white; font-weight:bold; }
#topbar { background:white; border-bottom:1px solid #e4e5ea; }
#brand { color:#c62828; font-size:17px; font-weight:bold; }
#muted { color:#7a7d86; font-size:11px; }
#topbar QLineEdit { background:#f1f2f5; border:1px solid #e0e1e6; border-radius:5px; padding:7px 10px; }
#avatar { background:#d32f2f; color:white; border-radius:19px; font-weight:bold; }
#bell { font-size:16px; }
QFrame#card { background:white; border:1px solid #e6e7ec; border-radius:8px; }
QFrame#card QLabel, QFrame#panel QLabel { background:transparent; border:none; }
QFrame#panel { background:#fafafb; border:1px solid #e0e1e6; border-radius:6px; }
#cardTitle { font-size:14px; font-weight:bold; }
QPushButton#tool { background:white; border:1px solid #cfd1d8; border-radius:4px; padding:5px 9px; }
QPushButton#tool:hover { background:#f3f4f7; } QPushButton#tool:checked { background:#fde4e4; border-color:#d32f2f; color:#b71c1c; }
QPushButton#primary { background:#d32f2f; color:white; border:none; border-radius:4px; padding:6px 12px; font-weight:bold; }
QPushButton#primary:hover { background:#b71c1c; }
QPushButton#link { background:transparent; border:none; color:#c62828; font-weight:bold; }
QPushButton#row { background:transparent; border:none; text-align:left; border-radius:6px; }
QPushButton#row:hover { background:#fbf1f1; }
QPushButton#status { background:transparent; border:none; font-weight:bold; }
QPushButton#alert { background:#d32f2f; color:white; border:none; border-radius:5px; padding:9px 12px; font-weight:bold; }
QPushButton#quick { background:#fdecec; border:1px solid #f6caca; border-radius:6px; color:#b71c1c; font-weight:bold; padding:18px 8px; }
QPushButton#quick:hover { background:#fbdcdc; }
#hint { background:#eaf2ff; border:1px solid #b9d3ff; border-radius:5px; padding:5px 8px; color:#1a4fa0; }
#hint[warn="true"] { background:#fff4e0; border-color:#f3c77a; color:#8a5a00; }
QComboBox, QSpinBox, QDoubleSpinBox, QFrame#panel QLineEdit { border:1px solid #cfd1d8; border-radius:4px; padding:3px 6px; background:white; min-height:20px; }
QFrame#meteo { border-radius:8px; background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #2b8be8,stop:1 #7fb6ea); }
)CSS";

// panneau lateral : image de fond (pompier) calee en bas, largeur ajustee
class SideBar : public QFrame {
public:
    SideBar() : m_bg(":/images/sidebar.png") {}
protected:
    void paintEvent(QPaintEvent *) override {
        QPainter p(this); p.setRenderHint(QPainter::SmoothPixmapTransform);
        const QColor top = m_bg.toImage().pixelColor(m_bg.width() / 2, 0);
        p.fillRect(rect(), top);
        const QPixmap sc = m_bg.scaledToWidth(width(), Qt::SmoothTransformation);
        if (sc.height() >= height()) p.drawPixmap(0, height() - sc.height(), sc);          // fenetre basse : on rogne le haut
        else {
            p.drawPixmap(0, height() - sc.height(), sc);
            QLinearGradient g(0, height() - sc.height() - 40, 0, height() - sc.height() + 40);   // raccord doux avec le haut
            QColor t0 = top; t0.setAlpha(255); QColor t1 = top; t1.setAlpha(0);
            g.setColorAt(0, t0); g.setColorAt(1, t1);
            p.fillRect(QRect(0, height() - sc.height() - 40, width(), 80), g);
        }
        QLinearGradient shade(0, 0, 0, height() * 0.6);                                       // lisibilite du menu
        shade.setColorAt(0, QColor(0, 0, 0, 120)); shade.setColorAt(1, QColor(0, 0, 0, 0));
        p.fillRect(QRect(0, 0, width(), int(height() * 0.6)), shade);
    }
private:
    QPixmap m_bg;
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Fire Station — Carte des zones à risque");
    setWindowIcon(QIcon(":/images/logo.png"));
    setMinimumSize(1280, 780);
    setStyleSheet(kStyle);
    m_store = new ZoneStore(this);

    auto *central = new QWidget; auto *h = new QHBoxLayout(central);
    h->setContentsMargins(0, 0, 0, 0); h->setSpacing(0);
    h->addWidget(buildSidebar());

    auto *right = new QWidget; right->setObjectName("content");
    auto *rv = new QVBoxLayout(right); rv->setContentsMargins(0, 0, 0, 0); rv->setSpacing(0);
    rv->addWidget(buildTopbar());
    m_pages = new QStackedWidget; rv->addWidget(m_pages, 1);
    h->addWidget(right, 1);
    setCentralWidget(central);

    m_pages->addWidget(buildDashboard());                 // 0
    const char *autres[] = {"Interventions", "Équipe", "Véhicules", "Planning", "Rapports", "Paramètres"};
    for (const char *a : autres) m_pages->addWidget(buildPlaceholder(a));   // 1..6
    updateBell();
    connect(m_store, &ZoneStore::changed, this, &MainWindow::updateBell);
}

QFrame *MainWindow::buildSidebar()
{
    m_sidebar = new SideBar; m_sidebar->setObjectName("sidebar"); m_sidebar->setFixedWidth(220);
    auto *v = new QVBoxLayout(m_sidebar); v->setContentsMargins(12, 14, 12, 14); v->setSpacing(4);
    auto *logo = new QLabel; logo->setPixmap(QPixmap(":/images/logo.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logo->setStyleSheet("background:transparent;");
    v->addWidget(logo, 0, Qt::AlignHCenter); v->addSpacing(14);

    m_nav = new QButtonGroup(this); m_nav->setExclusive(true);
    // texte, page cible (0 = tableau de bord / carte)
    const struct { const char *txt; int page; } items[] = {
        {"▪  Tableau de bord", 0}, {"✱  Interventions", 1}, {"👥  Équipe", 2}, {"🚒  Véhicules", 3},
        {"🗺  Carte", 0}, {"▤  Planning", 4}, {"▮  Rapports", 5}, {"⚙  Paramètres", 6}};
    int id = 0;
    for (const auto &it : items) {
        auto *b = new QPushButton(it.txt); b->setCheckable(true); b->setCursor(Qt::PointingHandCursor);
        if (id == 4) b->setChecked(true);
        m_nav->addButton(b, id); v->addWidget(b);
        connect(b, &QPushButton::clicked, this, [this, p = it.page] { goTo(p); });
        ++id;
    }
    v->addStretch(1);
    return m_sidebar;
}

QWidget *MainWindow::buildTopbar()
{
    auto *bar = new QFrame; bar->setObjectName("topbar"); bar->setFixedHeight(62);
    auto *h = new QHBoxLayout(bar); h->setContentsMargins(18, 0, 18, 0); h->setSpacing(14);
    auto *burger = new QPushButton("☰"); burger->setFlat(true); burger->setCursor(Qt::PointingHandCursor);
    burger->setStyleSheet("border:none;font-size:18px;");
    connect(burger, &QPushButton::clicked, this, [this] { m_sidebar->setVisible(!m_sidebar->isVisible()); });
    auto *t = new QVBoxLayout; t->setSpacing(0);
    auto *brand = new QLabel("FIRE STATION"); brand->setObjectName("brand");
    auto *sub = new QLabel("Carte des zones à risque"); sub->setObjectName("muted");
    t->addStretch(1); t->addWidget(brand); t->addWidget(sub); t->addStretch(1);
    m_search = new QLineEdit; m_search->setPlaceholderText("🔍  Rechercher un lieu, une zone, une intervention...");
    m_search->setFixedWidth(450);
    connect(m_search, &QLineEdit::returnPressed, this, [this] { goTo(0); m_nav->button(4)->setChecked(true); m_zones->searchZone(m_search->text()); });
    m_bell = new QLabel("🔔"); m_bell->setObjectName("bell");
    auto *av = new QLabel("CC"); av->setObjectName("avatar"); av->setFixedSize(38, 38); av->setAlignment(Qt::AlignCenter);
    auto *u = new QLabel("<b>Chef de centre</b><br><span style='color:#7a7d86'>Centre de Tunis</span>");
    auto *dbStatus = new QLabel(m_store->oracleActive() ? "Oracle · connecté" : "Oracle · déconnecté");
    dbStatus->setStyleSheet(m_store->oracleActive()
        ? "color:#16834a;font-weight:bold;padding:0 8px;"
        : "color:#b42318;font-weight:bold;padding:0 8px;");
    dbStatus->setToolTip(m_store->oracleActive() ? "Les zones et leurs modifications sont enregistrées dans Oracle." : m_store->lastStorageError());
    h->addWidget(burger); h->addLayout(t); h->addSpacing(40); h->addWidget(m_search); h->addWidget(dbStatus); h->addStretch(1);
    h->addWidget(m_bell); h->addWidget(av); h->addWidget(u);
    return bar;
}

QWidget *MainWindow::buildDashboard()
{
    auto *page = new QWidget;
    auto *g = new QGridLayout(page); g->setContentsMargins(16, 14, 16, 14); g->setSpacing(14);

    m_zones = new ZoneCard(m_store);
    auto *prio = new ZonePrioCard(m_store);
    auto *equipes = new EquipesCard;
    auto *alertes = new AlerteCard(m_store);
    auto *meteo = new MeteoCard;
    auto *acces = buildAccesRapide();

    g->addWidget(m_zones, 0, 0, 2, 2);
    g->addWidget(alertes, 0, 2);
    g->addWidget(meteo, 1, 2);
    g->addWidget(prio, 2, 0);
    g->addWidget(equipes, 2, 1);
    g->addWidget(acces, 2, 2);
    g->setColumnStretch(0, 4); g->setColumnStretch(1, 4); g->setColumnStretch(2, 5);
    g->setRowStretch(0, 5); g->setRowStretch(1, 2); g->setRowStretch(2, 3);
    g->setRowMinimumHeight(2, 250);

    connect(prio, &ZonePrioCard::zoneClicked, m_zones, [this](const QString &id) { m_zones->selectZone(id); });
    connect(prio, &ZonePrioCard::voirTout, m_zones, &ZoneCard::resetFilters);
    connect(alertes, &AlerteCard::zoneClicked, m_zones, [this](const QString &id) { m_zones->selectZone(id); });

    auto *scroll = new QScrollArea; scroll->setWidgetResizable(true); scroll->setFrameShape(QFrame::NoFrame);
    page->setMinimumSize(1180, 820);
    scroll->setWidget(page);
    return scroll;
}

QFrame *MainWindow::buildAccesRapide()
{
    auto *c = new QFrame; c->setObjectName("card");
    auto *v = new QVBoxLayout(c); v->setContentsMargins(14, 12, 14, 12);
    auto *t = new QLabel("⚡  Accès rapide"); t->setObjectName("cardTitle"); v->addWidget(t);
    auto *g = new QGridLayout; g->setSpacing(10);
    auto mk = [&](const QString &txt, int r, int col, std::function<void()> fn) {
        auto *b = new QPushButton(txt); b->setObjectName("quick"); b->setCursor(Qt::PointingHandCursor);
        g->addWidget(b, r, col); connect(b, &QPushButton::clicked, this, fn); };
    mk("Nouveau rapport", 0, 0, [this] { m_nav->button(6)->setChecked(true); goTo(5); });
    mk("Planifier une intervention", 0, 1, [this] { m_nav->button(1)->setChecked(true); goTo(1); });
    mk("Gérer les zones", 1, 0, [this] { m_zones->startAdd(); });
    mk("Voir les rapports", 1, 1, [this] { m_nav->button(6)->setChecked(true); goTo(5); });
    v->addLayout(g); v->addStretch(1);
    return c;
}

QWidget *MainWindow::buildPlaceholder(const QString &titre)
{
    auto *w = new QWidget; auto *v = new QVBoxLayout(w);
    auto *c = new QFrame; c->setObjectName("card"); auto *cv = new QVBoxLayout(c);
    auto *t = new QLabel(titre); t->setObjectName("cardTitle");
    auto *m = new QLabel("Module géré par un autre membre de l'équipe — à intégrer ici.");
    m->setObjectName("muted");
    cv->addWidget(t); cv->addWidget(m); cv->addStretch(1);
    v->setContentsMargins(16, 14, 16, 14); v->addWidget(c);
    return w;
}

void MainWindow::goTo(int page) { m_pages->setCurrentIndex(page); }

void MainWindow::updateBell()
{
    int n = 0;
    for (const Zone &z : m_store->all()) if (z.sousCouverte(m_store->seuil())) ++n;
    m_bell->setText(n ? QString("🔔 <span style='background:#d32f2f;color:white;border-radius:8px;'>&nbsp;%1&nbsp;</span>").arg(n) : "🔔");
}
