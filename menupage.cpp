#include "menupage.h"

#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QToolButton>
#include <QVBoxLayout>

// Icône blanche posée sur une pastille rouge (même charte que la sidebar)
static QPixmap pastille(const QString &icone, int taille)
{
    QPixmap pm(taille, taille);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    QLinearGradient g(0, 0, taille, taille);
    g.setColorAt(0, QColor("#d3122a"));
    g.setColorAt(1, QColor("#a50e1f"));
    p.setPen(Qt::NoPen);
    p.setBrush(g);
    p.drawEllipse(0, 0, taille, taille);
    const int m = taille / 4;
    p.drawPixmap(m, m, QPixmap(icone).scaled(taille - 2 * m, taille - 2 * m, Qt::KeepAspectRatio,
                                            Qt::SmoothTransformation));
    return pm;
}

MenuPage::MenuPage(const QList<Module> &modules, QWidget *parent) : QWidget(parent)
{
    setObjectName("MenuPage");
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(60, 40, 60, 40);
    root->setSpacing(6);

    m_bienvenue = new QLabel(tr("Bienvenue"));
    m_bienvenue->setObjectName("menuBienvenue");
    root->addWidget(m_bienvenue);
    auto *sousTitre = new QLabel(tr("Choisissez le module dans lequel vous souhaitez travailler."));
    sousTitre->setObjectName("menuSousTitre");
    root->addWidget(sousTitre);
    root->addSpacing(26);

    auto *grille = new QGridLayout;
    grille->setHorizontalSpacing(24);
    grille->setVerticalSpacing(24);
    for (int i = 0; i < modules.size(); ++i) {
        const Module &m = modules.at(i);
        auto *tuile = new QToolButton;
        tuile->setObjectName("menuTuile");
        tuile->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        tuile->setIcon(QIcon(pastille(m.icone, 128)));
        tuile->setIconSize(QSize(64, 64));
        tuile->setText(m.titre + "\n" + m.description);
        tuile->setCursor(Qt::PointingHandCursor);
        tuile->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        tuile->setMinimumSize(240, 170);
        tuile->setMaximumHeight(220);
        const int page = m.page;
        connect(tuile, &QToolButton::clicked, this, [this, page] { emit moduleChoisi(page); });
        grille->addWidget(tuile, i / 3, i % 3);
        m_tuiles << tuile;
        m_pages << page;
    }
    root->addLayout(grille);
    root->addStretch(1);
}

void MenuPage::setUtilisateur(const QString &nom)
{
    m_bienvenue->setText(nom.isEmpty() ? tr("Bienvenue") : tr("Bienvenue, %1").arg(nom));
}

void MenuPage::setModulesAutorises(const QList<int> &pages)
{
    for (int i = 0; i < m_tuiles.size(); ++i) {
        const bool ok = pages.contains(m_pages.at(i));
        m_tuiles.at(i)->setEnabled(ok);
        m_tuiles.at(i)->setToolTip(ok ? QString() : tr("Module non autorisé pour votre poste"));
    }
}
