#include "ZoneMapWidget.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QToolTip>
#include <QWheelEvent>
#include <QtMath>
#include <algorithm>
#include <functional>

namespace {
constexpr double kRefLat = 36.82;
constexpr double kRefLon = 10.22;
constexpr double kKmPerDegLat = 110.57;
constexpr double kKmPerDegLonEq = 111.32;
// Emprise de la carte (aussi utilisée pour caler une image de fond personnalisée)
constexpr double kLatMin = 36.70, kLatMax = 36.93, kLonMin = 10.07, kLonMax = 10.38;

struct LL { double lat, lon; };

const LL kCoast[] = {
    {37.00, 10.27}, {36.93, 10.30}, {36.90, 10.327}, {36.885, 10.337}, {36.868, 10.347},
    {36.850, 10.335}, {36.835, 10.322}, {36.822, 10.306}, {36.805, 10.300}, {36.790, 10.303},
    {36.770, 10.302}, {36.752, 10.314}, {36.735, 10.348}, {36.700, 10.378}, {36.660, 10.405},
    {36.55, 10.42}, {36.55, 11.00}, {37.05, 11.00}, {37.05, 10.26}
};

const QVector<QVector<LL>> kRoads = {
    { {36.80, 10.17}, {36.787, 10.215}, {36.768, 10.262}, {36.745, 10.292}, {36.715, 10.33} },
    { {36.80, 10.18}, {36.825, 10.21}, {36.842, 10.265}, {36.866, 10.31}, {36.880, 10.328} },
    { {36.80, 10.18}, {36.83, 10.185}, {36.862, 10.196}, {36.90, 10.20} },
    { {36.80, 10.17}, {36.77, 10.13}, {36.74, 10.10} },
};

struct Label { const char* text; double lat, lon; bool water; };
const Label kLabels[] = {
    {"Tunis", 36.8160, 10.1500, false}, {"La Goulette", 36.8160, 10.2800, false},
    {"Lac de Tunis", 36.8085, 10.2300, true}, {"Golfe de Tunis", 36.8300, 10.3600, true},
};

QPolygonF ellipseLL(double lat, double lon, double rLat, double rLon,
                    const std::function<QPointF(double, double)>& proj)
{
    QPolygonF poly;
    for (int i = 0; i < 48; ++i) {
        double a = 2 * M_PI * i / 48.0;
        poly << proj(lat + rLat * std::sin(a), lon + rLon * std::cos(a));
    }
    return poly;
}

QPointF evPos(QMouseEvent* e)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return e->position();
#else
    return e->localPos();
#endif
}

QPoint evGlobal(QMouseEvent* e)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return e->globalPosition().toPoint();
#else
    return e->globalPos();
#endif
}

void haloText(QPainter& p, const QPointF& pos, const QString& text, const QFont& font,
              const QColor& color, const QColor& halo = QColor(255, 255, 255, 230))
{
    QPainterPath path;
    path.addText(pos, font, text);
    p.setPen(QPen(halo, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    p.setBrush(Qt::NoBrush);
    p.drawPath(path);
    p.setPen(Qt::NoPen);
    p.setBrush(color);
    p.drawPath(path);
}
} // namespace

ZoneMapWidget::ZoneMapWidget(QWidget* parent) : QWidget(parent)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);
    setMinimumSize(420, 280);

    auto mkBtn = [this](const QString& t, const QString& tip) {
        auto* b = new QToolButton(this);
        b->setObjectName("mapBtn");
        b->setText(t);
        b->setToolTip(tip);
        b->setCursor(Qt::PointingHandCursor);
        b->setFixedSize(30, 30);
        return b;
    };
    m_btnIn = mkBtn("+", "Zoom avant");
    m_btnOut = mkBtn(QString::fromUtf8("−"), "Zoom arrière");
    m_btnReset = mkBtn(QString::fromUtf8("⌖"), "Recentrer la carte");
    connect(m_btnIn, &QToolButton::clicked, this, [this] { zoomBy(1.4); });
    connect(m_btnOut, &QToolButton::clicked, this, [this] { zoomBy(1 / 1.4); });
    connect(m_btnReset, &QToolButton::clicked, this, &ZoneMapWidget::resetView);

    m_timer.setInterval(50);
    connect(&m_timer, &QTimer::timeout, this, [this] {
        m_phase += 0.18;
        if (!m_alerts.isEmpty()) update();
    });
    m_timer.start();
    resetView();
}

// ---------- API ----------
void ZoneMapWidget::setZones(const QVector<Zone>& zones) { m_zones = zones; update(); }
void ZoneMapWidget::setVisibleIds(const QSet<int>& ids) { m_visible = ids; m_filterActive = true; update(); }
void ZoneMapWidget::setAlertIds(const QSet<int>& ids) { m_alerts = ids; update(); }
void ZoneMapWidget::setSelectedId(int id) { m_selected = id; update(); }

void ZoneMapWidget::setMode(Mode m)
{
    if (m == m_mode) return;
    m_mode = m;
    setCursor(m == Mode::AddZone ? Qt::CrossCursor : Qt::ArrowCursor);
    emit modeChanged(m);
    update();
}

void ZoneMapWidget::setBackgroundImage(const QImage& img) { m_bg = img; update(); }

void ZoneMapWidget::resetView()
{
    QPointF a = project(kLatMax, kLonMin), b = project(kLatMin, kLonMax);
    m_center = (a + b) / 2.0;
    double w = std::max(1.0, width() + 0.0), h = std::max(1.0, height() + 0.0);
    m_scale = std::min(w / (b.x() - a.x()), h / (b.y() - a.y())) * 0.97;
    update();
}

void ZoneMapWidget::centerLatLon(double& lat, double& lon) const { unproject(m_center, lat, lon); }

void ZoneMapWidget::zoomBy(double factor)
{
    m_scale = std::clamp(m_scale * factor, 4.0, 500.0);
    update();
}

// ---------- projection ----------
QPointF ZoneMapWidget::project(double lat, double lon) const
{
    const double kmLon = kKmPerDegLonEq * std::cos(qDegreesToRadians(kRefLat));
    return QPointF((lon - kRefLon) * kmLon, -(lat - kRefLat) * kKmPerDegLat);
}

void ZoneMapWidget::unproject(const QPointF& w, double& lat, double& lon) const
{
    const double kmLon = kKmPerDegLonEq * std::cos(qDegreesToRadians(kRefLat));
    lon = kRefLon + w.x() / kmLon;
    lat = kRefLat - w.y() / kKmPerDegLat;
}

QPointF ZoneMapWidget::toScreen(const QPointF& w) const
{
    return (w - m_center) * m_scale + QPointF(width() / 2.0, height() / 2.0);
}

QPointF ZoneMapWidget::toWorld(const QPointF& s) const
{
    return (s - QPointF(width() / 2.0, height() / 2.0)) / m_scale + m_center;
}

QPointF ZoneMapWidget::zoneScreenPos(const Zone& z) const
{
    return toScreen(project(z.latitude, z.longitude));
}

int ZoneMapWidget::indexOf(int id) const
{
    for (int i = 0; i < m_zones.size(); ++i)
        if (m_zones[i].id == id) return i;
    return -1;
}

bool ZoneMapWidget::isVisibleZone(int id) const
{
    return !m_filterActive || m_visible.contains(id);
}

int ZoneMapWidget::zoneAt(const QPointF& s) const
{
    int best = -1;
    double bestR = 1e18;
    // 1) marqueurs (priorité)
    for (const Zone& z : m_zones) {
        if (!isVisibleZone(z.id)) continue;
        QPointF c = zoneScreenPos(z);
        if (QLineF(c, s).length() <= 13) return z.id;
    }
    // 2) disque de la zone (le plus petit gagne)
    for (const Zone& z : m_zones) {
        if (!isVisibleZone(z.id)) continue;
        double r = std::max(14.0, z.rayonKm * m_scale);
        if (QLineF(zoneScreenPos(z), s).length() <= r && r < bestR) { best = z.id; bestR = r; }
    }
    return best;
}

// ---------- dessin ----------
void ZoneMapWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);
    p.fillRect(rect(), QColor("#f3efe6"));

    if (!m_bg.isNull()) {
        QPointF tl = toScreen(project(kLatMax, kLonMin)), br = toScreen(project(kLatMin, kLonMax));
        p.drawImage(QRectF(tl, br), m_bg);
    } else {
        drawBasemap(p);
    }
    drawZones(p);
    drawOverlays(p);
}

void ZoneMapWidget::drawBasemap(QPainter& p)
{
    auto proj = [this](double la, double lo) { return toScreen(project(la, lo)); };

    // mer
    QPolygonF sea;
    for (const LL& c : kCoast) sea << proj(c.lat, c.lon);
    p.setPen(QPen(QColor("#a9c9e6"), 2));
    p.setBrush(QColor("#cfe3f4"));
    p.drawPolygon(sea);

    // lacs / sebkhas
    p.setBrush(QColor("#cfe3f4"));
    p.setPen(QPen(QColor("#a9c9e6"), 1.5));
    p.drawPolygon(ellipseLL(36.808, 10.235, 0.017, 0.050, proj));
    p.setBrush(QColor("#dfe9ee"));
    p.drawPolygon(ellipseLL(36.775, 10.125, 0.020, 0.030, proj));
    p.drawPolygon(ellipseLL(36.870, 10.115, 0.014, 0.035, proj));

    // routes
    for (const auto& road : kRoads) {
        QPolygonF line;
        for (const LL& c : road) line << proj(c.lat, c.lon);
        p.setBrush(Qt::NoBrush);
        p.setPen(QPen(QColor("#d6cdb8"), 7, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        p.drawPolyline(line);
        p.setPen(QPen(QColor("#ffffff"), 4.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        p.drawPolyline(line);
    }

    // libellés
    for (const Label& l : kLabels) {
        QFont f("Sans", l.water ? 9 : 10);
        f.setItalic(l.water);
        f.setBold(!l.water);
        QPointF pos = proj(l.lat, l.lon);
        QString t = QString::fromUtf8(l.text);
        QFontMetricsF fm(f);
        pos.rx() -= fm.horizontalAdvance(t) / 2;
        haloText(p, pos, t, f, l.water ? QColor("#5d8fba") : QColor("#6b6557"),
                 l.water ? QColor(207, 227, 244, 220) : QColor(243, 239, 230, 220));
    }
}

void ZoneMapWidget::drawZones(QPainter& p)
{
    // ordre : zones normales, puis survol, puis sélection (au-dessus)
    QVector<int> order;
    for (int i = 0; i < m_zones.size(); ++i)
        if (isVisibleZone(m_zones[i].id)) order.append(i);
    std::stable_sort(order.begin(), order.end(), [this](int a, int b) {
        auto rank = [this](int i) { int id = m_zones[i].id; return id == m_selected ? 2 : (id == m_hover ? 1 : 0); };
        return rank(a) < rank(b);
    });

    QFont nameFont("Sans", 8);
    nameFont.setBold(true);
    QFont idFont("Sans", 8);
    idFont.setBold(true);

    for (int i : order) {
        const Zone& z = m_zones[i];
        QPointF c = zoneScreenPos(z);
        double r = std::max(14.0, z.rayonKm * m_scale);
        if (c.x() + r < 0 || c.y() + r < 0 || c.x() - r > width() || c.y() - r > height()) continue;

        QColor col = RiskUtil::color(z.risque);
        QColor fill = col;
        fill.setAlpha(z.id == m_hover ? 95 : 62);
        p.setBrush(fill);
        p.setPen(QPen(col, 1.5));
        p.drawEllipse(c, r, r);

        if (m_alerts.contains(z.id)) {   // alerte : anneau pointillé pulsant
            double pr = r + 6 + 4 * std::sin(m_phase);
            p.setBrush(Qt::NoBrush);
            p.setPen(QPen(QColor("#B91C1C"), 2, Qt::DashLine));
            p.drawEllipse(c, pr, pr);
        }
        if (z.id == m_selected) {
            p.setBrush(Qt::NoBrush);
            p.setPen(QPen(QColor("#1f2937"), 3));
            p.drawEllipse(c, r + 2, r + 2);
        }

        // marqueur
        p.setBrush(col);
        p.setPen(QPen(Qt::white, 2));
        p.drawEllipse(c, 11, 11);
        p.setPen(Qt::white);
        p.setFont(idFont);
        p.drawText(QRectF(c.x() - 11, c.y() - 11, 22, 22), Qt::AlignCenter, QString::number(z.id));

        // nom
        QFontMetricsF fm(nameFont);
        QPointF tp(c.x() - fm.horizontalAdvance(z.nom) / 2, c.y() + 26);
        haloText(p, tp, z.nom, nameFont, QColor("#1f2937"));
    }
}

void ZoneMapWidget::drawOverlays(QPainter& p)
{
    // légende
    QFont f("Sans", 8);
    p.setFont(f);
    QRectF box(12, height() - 112, 190, 100);
    p.setPen(QPen(QColor(0, 0, 0, 40)));
    p.setBrush(QColor(255, 255, 255, 235));
    p.drawRoundedRect(box, 8, 8);
    struct Item { QColor c; QString t; bool ring; };
    const Item items[] = {
        {RiskUtil::color(RiskLevel::Eleve), "Zone à risque élevé", false},
        {RiskUtil::color(RiskLevel::Moyen), "Zone à risque moyen", false},
        {RiskUtil::color(RiskLevel::Faible), "Zone à risque faible", false},
        {QColor("#B91C1C"), QString::fromUtf8("Zone sous-couverte (alerte)"), true},
    };
    double y = box.top() + 18;
    for (const Item& it : items) {
        if (it.ring) {
            p.setBrush(Qt::NoBrush);
            p.setPen(QPen(it.c, 2, Qt::DashLine));
            p.drawEllipse(QPointF(box.left() + 18, y - 3), 6, 6);
        } else {
            p.setBrush(it.c);
            p.setPen(Qt::NoPen);
            p.drawEllipse(QPointF(box.left() + 18, y - 3), 6, 6);
        }
        p.setPen(QColor("#374151"));
        p.drawText(QPointF(box.left() + 32, y), it.t);
        y += 22;
    }

    // échelle
    const double cand[] = {0.5, 1, 2, 5, 10, 20};
    double km = 1;
    for (double c : cand) if (c * m_scale <= 130) km = c;
    double len = km * m_scale;
    QPointF s(width() - 20 - len, height() - 18);
    p.setPen(QPen(QColor("#374151"), 2));
    p.drawLine(s, s + QPointF(len, 0));
    p.drawLine(s, s + QPointF(0, -5));
    p.drawLine(s + QPointF(len, 0), s + QPointF(len, -5));
    p.setPen(QColor("#374151"));
    p.drawText(QPointF(s.x(), s.y() - 8), QString("%1 km").arg(km));

    // bandeau de mode
    if (m_mode != Mode::Navigate) {
        QString msg = m_mode == Mode::AddZone
            ? QString::fromUtf8("Cliquez sur la carte pour placer la nouvelle zone  (Échap = annuler)")
            : QString::fromUtf8("Mode déplacement : glissez une zone pour la repositionner  (Échap = quitter)");
        QFont bf("Sans", 9);
        bf.setBold(true);
        p.setFont(bf);
        QFontMetricsF fm(bf);
        QRectF b(0, 0, fm.horizontalAdvance(msg) + 28, 30);
        b.moveCenter(QPointF(width() / 2.0, 26));
        p.setPen(Qt::NoPen);
        p.setBrush(QColor("#B71C1C"));
        p.drawRoundedRect(b, 15, 15);
        p.setPen(Qt::white);
        p.drawText(b, Qt::AlignCenter, msg);
    }
}

void ZoneMapWidget::resizeEvent(QResizeEvent*) { placeButtons(); }

void ZoneMapWidget::placeButtons()
{
    int x = width() - 42;
    m_btnIn->move(x, 12);
    m_btnOut->move(x, 46);
    m_btnReset->move(x, 80);
}

// ---------- interaction ----------
void ZoneMapWidget::mousePressEvent(QMouseEvent* e)
{
    setFocus();
    if (e->button() != Qt::LeftButton) return;
    QPointF pos = evPos(e);
    m_lastPos = pos;

    if (m_mode == Mode::AddZone) {
        double lat, lon;
        unproject(toWorld(pos), lat, lon);
        emit locationPicked(lat, lon);
        return;
    }
    int id = zoneAt(pos);
    if (id >= 0) {
        m_selected = id;
        emit zoneSelected(id);
        if (m_mode == Mode::MoveZone) m_dragId = id;
    } else {
        m_panning = true;
        setCursor(Qt::ClosedHandCursor);
    }
    update();
}

void ZoneMapWidget::mouseMoveEvent(QMouseEvent* e)
{
    QPointF pos = evPos(e);
    if (m_dragId >= 0) {
        int i = indexOf(m_dragId);
        if (i >= 0) unproject(toWorld(pos), m_zones[i].latitude, m_zones[i].longitude);
        update();
    } else if (m_panning) {
        m_center -= (pos - m_lastPos) / m_scale;
        m_lastPos = pos;
        update();
    } else {
        int id = zoneAt(pos);
        if (id != m_hover) {
            m_hover = id;
            update();
        }
        if (id >= 0) {
            const Zone& z = m_zones[indexOf(id)];
            QToolTip::showText(evGlobal(e),
                QString("<b>%1</b> (%2)<br>Risque : %3<br>Temps moyen : %4 min<br>Population : %5")
                    .arg(z.nom.toHtmlEscaped(), z.idLabel(), RiskUtil::label(z.risque))
                    .arg(z.tempsMoyen, 0, 'f', 1).arg(z.population), this);
            if (m_mode == Mode::MoveZone) setCursor(Qt::SizeAllCursor);
            else setCursor(Qt::PointingHandCursor);
        } else {
            QToolTip::hideText();
            setCursor(m_mode == Mode::AddZone ? Qt::CrossCursor : Qt::ArrowCursor);
        }
    }
}

void ZoneMapWidget::mouseReleaseEvent(QMouseEvent*)
{
    if (m_dragId >= 0) {
        int i = indexOf(m_dragId);
        if (i >= 0) emit zoneMoved(m_dragId, m_zones[i].latitude, m_zones[i].longitude);
        m_dragId = -1;
    }
    if (m_panning) {
        m_panning = false;
        setCursor(m_mode == Mode::AddZone ? Qt::CrossCursor : Qt::ArrowCursor);
    }
}

void ZoneMapWidget::mouseDoubleClickEvent(QMouseEvent* e)
{
    int id = zoneAt(evPos(e));
    if (id >= 0 && m_mode == Mode::Navigate) emit zoneActivated(id);
}

void ZoneMapWidget::wheelEvent(QWheelEvent* e)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QPointF pos = e->position();
#else
    QPointF pos = e->posF();
#endif
    QPointF anchor = toWorld(pos);
    m_scale = std::clamp(m_scale * std::pow(1.0015, e->angleDelta().y()), 4.0, 500.0);
    m_center = anchor - (pos - QPointF(width() / 2.0, height() / 2.0)) / m_scale;
    update();
}

void ZoneMapWidget::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Escape) setMode(Mode::Navigate);
    else QWidget::keyPressEvent(e);
}

void ZoneMapWidget::leaveEvent(QEvent*)
{
    if (m_hover != -1) { m_hover = -1; update(); }
}
