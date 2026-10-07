#include "MainWindow.h"
#include "core/ZoneRepository.h"
#include "ui/Theme.h"
#include "ui/ZoneManagementPage.h"
#include "widgets/LogoWidget.h"
#include "widgets/SidebarBackground.h"
#include "campagnewindow.h"

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle("FIRE STATION — Gestion des zones de couverture");
    resize(1500, 920);
    setStyleSheet(Theme::styleSheet());

    m_repo = new ZoneRepository(this);
    if (!m_repo->load(ZoneRepository::defaultPath()))
        m_repo->loadSamples();
    connect(m_repo, &ZoneRepository::changed, this, [this] { m_repo->save(ZoneRepository::defaultPath()); });

    auto* central = new QWidget;
    auto* h = new QHBoxLayout(central);
    h->setContentsMargins(0, 0, 0, 0);
    h->setSpacing(0);
    h->addWidget(buildSidebar());

    auto* content = new QWidget;
    content->setObjectName("content");
    auto* v = new QVBoxLayout(content);
    v->setContentsMargins(0, 0, 0, 0);
    v->setSpacing(0);
    v->addWidget(buildHeader());

    m_stack = new QStackedWidget;
    m_stack->addWidget(new ZoneManagementPage(m_repo));        // index 0 : Carte
    m_stack->addWidget(placeholderPage(QString()));            // index 1 : autres modules
    auto* campagnes = new CampagneWindow;                      // index 2 : Campagnes
    campagnes->setWindowFlags(Qt::Widget);
    m_stack->addWidget(campagnes);
    v->addWidget(m_stack, 1);
    h->addWidget(content, 1);
    setCentralWidget(central);
}

QWidget* MainWindow::buildSidebar()
{
    auto* side = new SidebarBackground(":/sidebar_bg.jpg");
    side->setObjectName("sidebarBg");
    side->setFixedWidth(220);
    auto* v = new QVBoxLayout(side);
    v->setContentsMargins(12, 16, 12, 16);
    v->setSpacing(4);

    v->addWidget(new LogoWidget(96), 0, Qt::AlignHCenter);
    v->addSpacing(14);

    struct Entry { const char* icon; const char* text; };
    const Entry entries[] = {
        {"🏠", "Tableau de bord"}, {"🚨", "Interventions"}, {"👥", "Équipe"}, {"🚒", "Véhicules"},
        {"🗺", "Carte"}, {"📢", "Campagnes"}, {"📅", "Planning"}, {"📄", "Rapports"}, {"⚙", "Paramètres"},
    };
    auto* group = new QButtonGroup(this);
    group->setExclusive(true);
    for (const Entry& e : entries) {
        QString text = QString::fromUtf8(e.text);
        auto* b = new QPushButton(QString::fromUtf8(e.icon) + "   " + text);
        b->setCheckable(true);
        b->setCursor(Qt::PointingHandCursor);
        group->addButton(b);
        v->addWidget(b);
        const bool isMap = (text == "Carte");
        const bool isCampagne = (text == "Campagnes");
        if (isMap) b->setChecked(true);
        connect(b, &QPushButton::clicked, this, [this, isMap, isCampagne, text] {
            if (isMap) {
                m_stack->setCurrentIndex(0);
                m_title->setText(QString::fromUtf8("Carte des zones à risque"));
            } else if (isCampagne) {
                m_stack->setCurrentIndex(2);
                m_title->setText(QString::fromUtf8("Campagnes de sensibilisation"));
            } else {
                auto* lbl = m_stack->widget(1)->findChild<QLabel*>("placeholderLabel");
                if (lbl) lbl->setText(QString::fromUtf8("Module « %1 »\n\nIntégré par un autre membre de l'équipe.").arg(text));
                m_stack->setCurrentIndex(1);
                m_title->setText(text);
            }
        });
    }
    v->addStretch();
    return side;
}

QWidget* MainWindow::buildHeader()
{
    auto* head = new QWidget;
    head->setObjectName("header");
    head->setFixedHeight(64);
    auto* h = new QHBoxLayout(head);
    h->setContentsMargins(20, 6, 20, 6);

    auto* pin = new QLabel(QString::fromUtf8("📍"));
    pin->setStyleSheet("font-size:22px;");
    h->addWidget(pin);

    auto* col = new QVBoxLayout;
    col->setSpacing(0);
    auto* brand = new QLabel("FIRE STATION");
    brand->setObjectName("pageTitle");
    m_title = new QLabel(QString::fromUtf8("Carte des zones à risque"));
    m_title->setObjectName("pageSubtitle");
    col->addWidget(brand);
    col->addWidget(m_title);
    h->addLayout(col);
    h->addStretch();

    auto* user = new QLabel(QString::fromUtf8("<b>Chef de centre</b><br><span style='color:#6b7280'>Centre de Tunis</span>"));
    user->setTextFormat(Qt::RichText);
    h->addWidget(user);
    return head;
}

QWidget* MainWindow::placeholderPage(const QString& title)
{
    auto* w = new QWidget;
    auto* l = new QVBoxLayout(w);
    auto* lbl = new QLabel(title);
    lbl->setObjectName("placeholderLabel");
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setStyleSheet("font-size:16px; color:#6b7280;");
    l->addWidget(lbl);
    return w;
}
