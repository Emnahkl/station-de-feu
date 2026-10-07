#include "ZoneDetailPanel.h"
#include <QLabel>
#include <QLocale>
#include <QVBoxLayout>

ZoneDetailPanel::ZoneDetailPanel(QWidget* parent) : QFrame(parent)
{
    setObjectName("card");
    auto* lay = new QVBoxLayout(this);
    lay->setContentsMargins(12, 10, 12, 10);
    auto* head = new QLabel(QString::fromUtf8("📍  Détails de la zone"));
    head->setObjectName("cardTitle");
    lay->addWidget(head);

    m_empty = new QLabel(QString::fromUtf8("Sélectionnez une zone sur la carte ou dans le tableau."));
    m_empty->setWordWrap(true);
    m_empty->setStyleSheet("color:#6b7280;");
    lay->addWidget(m_empty);

    m_title = new QLabel;
    m_title->setStyleSheet("font-size:15px; font-weight:700; color:#111827;");
    m_title->setWordWrap(true);
    m_badge = new QLabel;
    m_status = new QLabel;
    m_info = new QLabel;
    m_info->setTextFormat(Qt::RichText);
    m_info->setWordWrap(true);
    lay->addWidget(m_title);
    lay->addWidget(m_badge, 0, Qt::AlignLeft);
    lay->addWidget(m_status);
    lay->addWidget(m_info);
    lay->addStretch();
    setZone(nullptr, 12.0);
}

void ZoneDetailPanel::setZone(const Zone* z, double threshold)
{
    bool has = (z != nullptr);
    m_empty->setVisible(!has);
    m_title->setVisible(has);
    m_badge->setVisible(has);
    m_status->setVisible(has);
    m_info->setVisible(has);
    if (!has) return;

    QLocale fr(QLocale::French);
    m_title->setText(QString("%1  —  %2").arg(z->idLabel(), z->nom));
    QColor c = RiskUtil::color(z->risque);
    m_badge->setText(QString::fromUtf8("Risque %1").arg(RiskUtil::label(z->risque)));
    m_badge->setStyleSheet(QString("background:%1; color:white; border-radius:9px; padding:2px 10px; font-weight:600;")
                               .arg(c.name()));
    bool under = z->tempsMoyen > threshold;
    m_status->setText(under ? QString::fromUtf8("⚠ Sous-couverte (> %1 min)").arg(threshold, 0, 'f', 1)
                            : QString::fromUtf8("✔ Couverture conforme"));
    m_status->setStyleSheet(under ? "color:#B91C1C; font-weight:700;" : "color:#2E7D32; font-weight:700;");
    m_info->setText(QString::fromUtf8(
        "<table cellspacing='4'>"
        "<tr><td>Région / quartier</td><td><b>%1</b></td></tr>"
        "<tr><td>Superficie</td><td><b>%2 km²</b></td></tr>"
        "<tr><td>Population</td><td><b>%3</b></td></tr>"
        "<tr><td>Temps moyen</td><td><b>%4 min</b></td></tr>"
        "<tr><td>Interventions</td><td><b>%5</b></td></tr>"
        "<tr><td>Équipes affectées</td><td><b>%6</b></td></tr>"
        "<tr><td>Véhicules</td><td><b>%7</b></td></tr>"
        "<tr><td>Coordonnées</td><td><b>%8, %9</b></td></tr></table>")
            .arg(z->region.toHtmlEscaped(), fr.toString(z->superficie, 'f', 1), fr.toString(z->population),
                 fr.toString(z->tempsMoyen, 'f', 1), QString::number(z->nbInterventions),
                 QString::number(z->nbEquipes), QString::number(z->nbVehicules),
                 QString::number(z->latitude, 'f', 4), QString::number(z->longitude, 'f', 4)));
}
