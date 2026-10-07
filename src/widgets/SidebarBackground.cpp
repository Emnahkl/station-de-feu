#include "SidebarBackground.h"
#include <QLinearGradient>
#include <QPainter>

SidebarBackground::SidebarBackground(const QString& resourcePath, QWidget* parent)
    : QWidget(parent), m_pix(resourcePath) {}

void SidebarBackground::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.fillRect(rect(), QColor("#1c0000"));
    if (!m_pix.isNull()) {
        QPixmap scaled = m_pix.scaledToWidth(width(), Qt::SmoothTransformation);   // largeur exacte : le slogan reste entier
        const int top = height() - scaled.height();
        p.drawPixmap(0, top, scaled);
        QLinearGradient fade(0, top, 0, top + 140);   // raccord invisible avec le fond uni
        fade.setColorAt(0.0, QColor("#1c0000"));
        fade.setColorAt(1.0, QColor(28, 0, 0, 0));
        p.fillRect(QRect(0, top, width(), 140), fade);
    }
    QLinearGradient g(0, 0, 0, height());   // voile pour la lisibilité du menu
    g.setColorAt(0.0, QColor(60, 0, 0, 170));
    g.setColorAt(0.55, QColor(60, 0, 0, 40));
    g.setColorAt(1.0, QColor(0, 0, 0, 0));
    p.fillRect(rect(), g);
}
