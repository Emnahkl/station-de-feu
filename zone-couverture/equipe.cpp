#include "equipe.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

EquipesCard::EquipesCard(QWidget *parent) : QFrame(parent)
{
    setObjectName("card");
    m_equipes = { {"Équipe 1 — Centre", "1 Chef · 4 Pompiers", true},
                  {"Équipe 2 — Nord",   "1 Chef · 3 Pompiers", true},
                  {"Équipe 3 — Sud",    "1 Chef · 4 Pompiers", false},
                  {"Équipe 4 — Ouest",  "1 Chef · 3 Pompiers", true} };
    auto *v = new QVBoxLayout(this); v->setContentsMargins(14, 12, 14, 12);
    auto *t = new QLabel("👥  Équipes disponibles"); t->setObjectName("cardTitle");
    v->addWidget(t);
    m_list = new QVBoxLayout; m_list->setSpacing(4); v->addLayout(m_list); v->addStretch(1);
    rebuild();
}

void EquipesCard::rebuild()
{
    while (QLayoutItem *it = m_list->takeAt(0)) { delete it->widget(); delete it; }
    for (int i = 0; i < m_equipes.size(); ++i) {
        const Equipe &e = m_equipes[i];
        auto *row = new QWidget; auto *h = new QHBoxLayout(row); h->setContentsMargins(6, 4, 6, 4);
        auto *ic = new QLabel("🚒"); ic->setFixedSize(34, 34); ic->setAlignment(Qt::AlignCenter);
        ic->setStyleSheet("background:#fbe3e3;border-radius:17px;");
        auto *tx = new QLabel(QString("<b>%1</b><br><span style='color:#777;font-size:11px'>%2</span>").arg(e.nom, e.composition));
        auto *st = new QPushButton(QString("● ") + (e.disponible ? "Disponible" : "En mission"));
        st->setObjectName("status"); st->setCursor(Qt::PointingHandCursor);
        st->setToolTip("Cliquer pour changer le statut");
        st->setStyleSheet(QString("color:%1;").arg(e.disponible ? "#1f9d55" : "#d32f2f"));
        connect(st, &QPushButton::clicked, this, [this, i] { m_equipes[i].disponible = !m_equipes[i].disponible; rebuild(); });
        h->addWidget(ic); h->addWidget(tx, 1); h->addWidget(st);
        m_list->addWidget(row);
    }
}
