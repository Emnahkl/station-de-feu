#include "LogoWidget.h"
#include <QCoreApplication>
#include <QFile>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>

LogoWidget::LogoWidget(int size, QWidget* parent) : QWidget(parent)
{
    setFixedSize(size, size);
    m_pix.load(":/logo.png");                       // logo embarqué (resources.qrc)
    QString path = QCoreApplication::applicationDirPath() + "/logo.png";
    if (m_pix.isNull() && QFile::exists(path)) m_pix.load(path);
}

void LogoWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    if (!m_pix.isNull()) {
        p.drawPixmap(rect(), m_pix);
        return;
    }
    QRectF r = QRectF(rect()).adjusted(2, 2, -2, -2);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawEllipse(r);
    p.setBrush(QColor("#E00000"));
    p.drawEllipse(r.adjusted(3, 3, -3, -3));
    p.setBrush(Qt::white);
    p.drawEllipse(r.adjusted(7, 7, -7, -7));

    // flamme stylisée
    QPainterPath flame;
    QPointF c = r.center() + QPointF(0, r.height() * 0.17);
    double s = r.width() / 140.0;
    flame.moveTo(c.x(), c.y() + 38 * s);
    flame.cubicTo(c.x() - 34 * s, c.y() + 30 * s, c.x() - 30 * s, c.y() - 5 * s, c.x() - 8 * s, c.y() - 20 * s);
    flame.cubicTo(c.x() - 8 * s, c.y() - 5 * s, c.x() + 2 * s, c.y() - 5 * s, c.x() + 6 * s, c.y() - 28 * s);
    flame.cubicTo(c.x() + 34 * s, c.y() - 5 * s, c.x() + 38 * s, c.y() + 28 * s, c.x(), c.y() + 38 * s);
    p.setBrush(QColor("#F97316"));
    p.drawPath(flame);

    QFont f("Serif", int(r.width() / 7.5));
    f.setBold(true);
    p.setFont(f);
    p.setPen(QColor("#9B1C1C"));
    p.drawText(QRectF(r.left(), r.top() + r.height() * 0.15, r.width(), r.height() * 0.2), Qt::AlignCenter, "FIRE");
    QFont f2("Serif", int(r.width() / 11));
    f2.setBold(true);
    p.setFont(f2);
    p.drawText(QRectF(r.left(), r.top() + r.height() * 0.34, r.width(), r.height() * 0.14), Qt::AlignCenter, "STATION");
}
