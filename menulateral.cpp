#include "menulateral.h"

#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

MenuLateral::MenuLateral(Page courante, QWidget *parent)
    : QFrame(parent), m_courante(courante)
{
    setObjectName("menuLateral");
    setFixedWidth(215);
    setStyleSheet(
        "QFrame#menuLateral { background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #b71c1c, stop:1 #7f0000); }"
        "QFrame#menuLateral QLabel { color: white; background: transparent; }"
        "QFrame#menuLateral QPushButton { color: white; background: transparent; border: none;"
        "  text-align: left; padding: 12px 18px; font-size: 14px; }"
        "QFrame#menuLateral QPushButton:hover { background: rgba(0,0,0,0.22); }"
        "QFrame#menuLateral QPushButton:checked { background: rgba(0,0,0,0.45); font-weight: bold;"
        "  border-left: 4px solid white; }");

    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 20, 0, 20);
    lay->setSpacing(2);

    auto *logo = new QLabel;
    logo->setPixmap(QPixmap(":/images/logo.png").scaled(110, 110, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logo->setAlignment(Qt::AlignCenter);
    lay->addWidget(logo);
    auto *titre = new QLabel("FIRE STATION");
    titre->setAlignment(Qt::AlignCenter);
    titre->setStyleSheet("font-size: 16px; font-weight: bold; letter-spacing: 1px;");
    lay->addWidget(titre);
    lay->addSpacing(16);

    const struct { Page page; const char *texte; } items[] = {
        {Employes,      "☻  Équipe (employés)"},
        {Interventions, "⚠  Interventions"},
        {Zones,         "◈  Zones de couverture"},
        {Equipements,   "▣  Équipements"},
        {Campagnes,     "✉  Campagnes de\n     sensibilisation"},
    };
    for (const auto &it : items) {
        auto *b = new QPushButton(QString::fromUtf8(it.texte));
        b->setCheckable(true);
        b->setAutoExclusive(true);
        b->setCursor(Qt::PointingHandCursor);
        lay->addWidget(b);
        m_boutons.insert(it.page, b);
        const Page p = it.page;
        connect(b, &QPushButton::clicked, this, [this, p] {
            if (p == m_courante)
                return;
            marquer();          // la page change : ce menu reste coherent si on y revient
            emit naviguer(p);
        });
    }
    lay->addStretch();

    auto *btnQuitter = new QPushButton(QString::fromUtf8("⏻  Quitter"));
    btnQuitter->setCursor(Qt::PointingHandCursor);
    lay->addWidget(btnQuitter);
    connect(btnQuitter, &QPushButton::clicked, this, &MenuLateral::quitter);

    marquer();
}

void MenuLateral::marquer()
{
    if (QPushButton *b = m_boutons.value(m_courante))
        b->setChecked(true);
}
