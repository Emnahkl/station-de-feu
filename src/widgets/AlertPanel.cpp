#include "AlertPanel.h"
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>

AlertPanel::AlertPanel(QWidget* parent) : QFrame(parent)
{
    setObjectName("card");
    auto* lay = new QVBoxLayout(this);
    lay->setContentsMargins(12, 10, 12, 10);

    auto* head = new QHBoxLayout;
    m_title = new QLabel;
    m_title->setObjectName("cardTitle");
    head->addWidget(m_title, 1);
    lay->addLayout(head);

    auto* row = new QHBoxLayout;
    row->addWidget(new QLabel(QString::fromUtf8("Seuil d'alerte :")));
    m_spin = new QDoubleSpinBox;
    m_spin->setRange(1, 120);
    m_spin->setDecimals(1);
    m_spin->setSingleStep(0.5);
    m_spin->setSuffix(" min");
    m_spin->setValue(12.0);
    row->addWidget(m_spin);
    row->addStretch();
    lay->addLayout(row);

    m_list = new QListWidget;
    m_list->setObjectName("alertList");
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    m_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_list->setWordWrap(true);
    lay->addWidget(m_list, 1);

    connect(m_spin, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &AlertPanel::thresholdChanged);
    connect(m_list, &QListWidget::itemClicked, this, [this](QListWidgetItem* it) {
        emit zoneClicked(it->data(Qt::UserRole).toInt());
    });
    setAlerts({}, 12.0);
}

double AlertPanel::threshold() const { return m_spin->value(); }

void AlertPanel::setThreshold(double minutes)
{
    QSignalBlocker b(m_spin);
    m_spin->setValue(minutes);
}

void AlertPanel::setAlerts(const QVector<Zone>& zones, double threshold)
{
    m_list->clear();
    m_title->setText(QString::fromUtf8("🔔  Alertes de zone sous-couverte (%1)").arg(zones.size()));
    if (zones.isEmpty()) {
        auto* it = new QListWidgetItem(QString::fromUtf8("✔  Toutes les zones respectent le seuil"));
        it->setFlags(Qt::NoItemFlags);
        it->setForeground(QColor("#2E7D32"));
        m_list->addItem(it);
        return;
    }
    for (const Zone& z : zones) {
        auto* it = new QListWidgetItem(
            QString::fromUtf8("⚠  %1 — %2 min  (+%3 min)")
                .arg(z.nom).arg(z.tempsMoyen, 0, 'f', 1).arg(z.tempsMoyen - threshold, 0, 'f', 1));
        it->setData(Qt::UserRole, z.id);
        it->setForeground(QColor("#B91C1C"));
        m_list->addItem(it);
    }
}
