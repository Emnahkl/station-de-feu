#include "cartezoneswidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

// Emprise de la carte (Grand Tunis)
static const double LAT_MIN = 36.70, LAT_MAX = 36.93;
static const double LON_MIN = 10.04, LON_MAX = 10.40;

CarteZonesWidget::CarteZonesWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(520, 380);
    setCursor(Qt::PointingHandCursor);
}

void CarteZonesWidget::setPoints(const QList<Point> &points)
{
    m_points = points;
    update();
}

void CarteZonesWidget::setSelection(int idZone)
{
    m_selection = idZone;
    update();
}

QPointF CarteZonesWidget::versEcran(double latitude, double longitude) const
{
    const QRectF r = rect().adjusted(10, 10, -10, -40);
    const double x = r.left() + (longitude - LON_MIN) / (LON_MAX - LON_MIN) * r.width();
    const double y = r.top() + (LAT_MAX - latitude) / (LAT_MAX - LAT_MIN) * r.height();
    return QPointF(x, y);
}

double CarteZonesWidget::rayon(const Point &p) const
{
    return 9.0 + qSqrt(qMax(0, p.population)) / 22.0;
}

void CarteZonesWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    const QRectF carte = rect().adjusted(10, 10, -10, -40);

    // Terre
    p.setPen(QPen(QColor("#d5d9df"), 1));
    p.setBrush(QColor("#f3efe6"));
    p.drawRoundedRect(carte, 8, 8);
    p.setClipRect(carte);

    // Golfe de Tunis (mer au nord-est)
    QPainterPath mer;
    mer.moveTo(versEcran(LAT_MAX + 0.02, 10.20));
    mer.cubicTo(versEcran(36.90, 10.26), versEcran(36.86, 10.29), versEcran(36.84, 10.30));
    mer.cubicTo(versEcran(36.82, 10.31), versEcran(36.79, 10.30), versEcran(36.76, 10.33));
    mer.cubicTo(versEcran(36.74, 10.35), versEcran(36.72, 10.38), versEcran(LAT_MIN - 0.02, 10.40));
    mer.lineTo(versEcran(LAT_MIN - 0.02, LON_MAX + 0.02));
    mer.lineTo(versEcran(LAT_MAX + 0.02, LON_MAX + 0.02));
    mer.closeSubpath();
    p.setPen(Qt::NoPen);
    p.setBrush(QColor("#cfe3f3"));
    p.drawPath(mer);

    // Lac de Tunis et Sebkha Séjoumi
    p.drawEllipse(QRectF(versEcran(36.835, 10.205), versEcran(36.790, 10.290)));
    p.drawEllipse(QRectF(versEcran(36.775, 10.135), versEcran(36.745, 10.190)));

    p.setPen(QColor("#6b8fb3"));
    QFont f = font();
    f.setItalic(true);
    f.setPixelSize(12);
    p.setFont(f);
    p.drawText(versEcran(36.905, 10.31), "Golfe de Tunis");
    p.drawText(versEcran(36.815, 10.222), "Lac de Tunis");
    p.drawText(versEcran(36.762, 10.140), "Sebkha Séjoumi");
    p.setClipping(false);

    // Zones
    const QColor couleurs[3] = {QColor("#2E7D32"), QColor("#FF6F00"), QColor("#C62828")};
    f.setItalic(false);
    f.setBold(true);
    f.setPixelSize(12);
    p.setFont(f);
    for (const Point &z : qAsConst(m_points)) {
        const QPointF c = versEcran(z.latitude, z.longitude);
        const double r = rayon(z);
        QColor col = couleurs[qBound(0, z.classeRisque, 2)];

        if (z.sousCouverte) {
            p.setPen(QPen(QColor("#C62828"), 2, Qt::DashLine));
            p.setBrush(Qt::NoBrush);
            p.drawEllipse(c, r + 7, r + 7);
        }
        col.setAlpha(190);
        p.setPen(QPen(z.id == m_selection ? QColor("#1f2937") : Qt::white, z.id == m_selection ? 3 : 2));
        p.setBrush(col);
        p.drawEllipse(c, r, r);

        p.setPen(Qt::white);
        p.drawText(QRectF(c.x() - r, c.y() - r, 2 * r, 2 * r), Qt::AlignCenter, QString::number(z.interventions));
        p.setPen(QColor("#1f2937"));
        p.drawText(QRectF(c.x() - 80, c.y() + r + 2, 160, 16), Qt::AlignHCenter | Qt::AlignTop,
                   z.sousCouverte ? "! " + z.nom : z.nom);
    }

    // Légende
    f.setBold(false);
    f.setPixelSize(12);
    p.setFont(f);
    int x = int(carte.left()) + 4;
    const int y = height() - 22;
    const QStringList lib = {"Risque faible", "Risque moyen", "Risque élevé"};
    for (int i = 0; i < 3; ++i) {
        p.setPen(Qt::NoPen);
        p.setBrush(couleurs[i]);
        p.drawEllipse(QPointF(x + 6, y), 6, 6);
        p.setPen(QColor("#374151"));
        p.drawText(QPointF(x + 16, y + 4), lib.at(i));
        x += 120;
    }
    p.setPen(QPen(QColor("#C62828"), 2, Qt::DashLine));
    p.setBrush(Qt::NoBrush);
    p.drawEllipse(QPointF(x + 6, y), 6, 6);
    p.setPen(QColor("#374151"));
    p.drawText(QPointF(x + 16, y + 4), "Zone sous-couverte   ·   chiffre = interventions (12 mois)");
}

void CarteZonesWidget::mousePressEvent(QMouseEvent *event)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QPointF pos = event->position();
#else
    const QPointF pos = event->localPos();
#endif
    for (const Point &z : qAsConst(m_points)) {
        const QPointF c = versEcran(z.latitude, z.longitude);
        if (QLineF(c, pos).length() <= rayon(z) + 4) {
            m_selection = z.id;
            update();
            emit zoneCliquee(z.id);
            return;
        }
    }
}
