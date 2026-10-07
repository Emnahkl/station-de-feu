#include "barchartwidget.h"

#include <QFontMetrics>
#include <QPainter>

BarChartWidget::BarChartWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumHeight(320);
}

void BarChartWidget::setData(const QList<QPair<QString, int>> &data, const QString &titre)
{
    m_data = data;
    m_titre = titre;
    update();
}

void BarChartWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), QColor("#FFFFFF"));

    QFont f = font();
    f.setBold(true);
    f.setPixelSize(18);
    p.setFont(f);
    p.setPen(QColor("#C62828"));
    p.drawText(QRect(0, 8, width(), 28), Qt::AlignCenter, m_titre);

    if (m_data.isEmpty()) {
        p.setPen(QColor("#555555"));
        p.setFont(font());
        p.drawText(rect(), Qt::AlignCenter, QString::fromUtf8("Aucune donnée à afficher"));
        return;
    }

    int max = 1;
    for (const auto &d : m_data) max = qMax(max, d.second);

    const int marginL = 40, marginR = 20, marginT = 50, marginB = 70;
    const QRect zone(marginL, marginT, width() - marginL - marginR, height() - marginT - marginB);

    // Axe et graduations
    p.setFont(font());
    p.setPen(QColor("#BDBDBD"));
    const int pas = qMax(1, (max + 4) / 5);
    for (int v = 0; v <= max; v += pas) {
        const int y = zone.bottom() - int(double(v) / max * zone.height());
        p.drawLine(zone.left(), y, zone.right(), y);
        p.setPen(QColor("#555555"));
        p.drawText(QRect(0, y - 8, marginL - 6, 16), Qt::AlignRight | Qt::AlignVCenter, QString::number(v));
        p.setPen(QColor("#E0E0E0"));
    }

    const int n = m_data.size();
    const double slot = double(zone.width()) / n;
    const int barW = int(slot * 0.6);
    const QColor couleurs[] = {QColor("#C62828"), QColor("#FF6F00")};

    for (int i = 0; i < n; ++i) {
        const int v = m_data.at(i).second;
        const int h = int(double(v) / max * zone.height());
        const int x = zone.left() + int(i * slot + (slot - barW) / 2);
        const QRect bar(x, zone.bottom() - h, barW, h);
        p.setPen(Qt::NoPen);
        p.setBrush(couleurs[i % 2]);
        p.drawRoundedRect(bar, 4, 4);

        p.setPen(QColor("#2B2B2B"));
        p.drawText(QRect(x - 10, bar.top() - 20, barW + 20, 18), Qt::AlignCenter, QString::number(v));

        const QRect lbl(int(zone.left() + i * slot), zone.bottom() + 6, int(slot), marginB - 10);
        p.drawText(lbl, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, m_data.at(i).first);
    }
}
