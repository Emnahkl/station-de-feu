#include "KpiCard.h"
#include <QLabel>
#include <QVBoxLayout>

KpiCard::KpiCard(const QString& title, const QColor& accent, QWidget* parent) : QFrame(parent)
{
    setObjectName("kpiCard");
    setStyleSheet(QString("#kpiCard { background:white; border:1px solid #e5e7eb; "
                          "border-left:5px solid %1; border-radius:8px; }").arg(accent.name()));
    auto* lay = new QVBoxLayout(this);
    lay->setContentsMargins(12, 8, 12, 8);
    lay->setSpacing(0);
    auto* t = new QLabel(title);
    t->setStyleSheet("color:#6b7280; font-size:11px; font-weight:600; border:none;");
    m_value = new QLabel("—");
    m_value->setStyleSheet("color:#111827; font-size:22px; font-weight:700; border:none;");
    m_hint = new QLabel;
    m_hint->setStyleSheet("color:#9ca3af; font-size:10px; border:none;");
    lay->addWidget(t);
    lay->addWidget(m_value);
    lay->addWidget(m_hint);
}

void KpiCard::setValue(const QString& value, const QString& hint)
{
    m_value->setText(value);
    m_hint->setText(hint);
    m_hint->setVisible(!hint.isEmpty());
}
