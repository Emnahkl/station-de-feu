#include "zone.h"
#include "connection.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPainter>
#include <QPainterPath>
#include <QWheelEvent>
#include <cmath>
#include <algorithm>
#include <QStyle>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QSaveFile>
#include <QTableWidget>
#include <QHeaderView>
#include <QTabWidget>
#include <QFileDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QProgressBar>
#include <QSignalBlocker>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFileInfo>
#include <QTextStream>
#include <QDialog>
#include <QTableWidgetItem>

// ============================================================
//  ZoneStore
// ============================================================
ZoneStore::ZoneStore(QObject *parent) : QObject(parent)
{
    Connection &connection = Connection::createInstance();
    if (!connection.isConfigured()) {
        m_storageError = QStringLiteral("Les identifiants Oracle sont absents. Vérifiez database/connection.ini.");
        return;
    }
    if (!connection.createConnection()) {
        m_storageError = connection.lastError();
        return;
    }
    if (loadOracle())
        persist();
}

bool ZoneStore::connectOracle()
{
    Connection &connection=Connection::createInstance();
    if(!connection.createConnection()) { emit storageError(connection.lastError()); return false; }
    if(!loadOracle()) { emit storageError(m_storageError); return false; }
    m_oracleActive=true; persist(); emit changed(); return true;
}

bool ZoneStore::loadOracle()
{
    QSqlQuery q(Connection::createInstance().database());
    // Read zones directly from the table. Keeping the base query independent
    // from the optional intervention/crew tables prevents a mismatch in those
    // tables from hiding every coverage zone in the UI.
    const QString sql=QStringLiteral("SELECT ID_ZONE,NOM,NVL(REGION,'-'),NVL(SUPERFICIE,0),NVL(POPULATION,0),NVL(NIVEAU_RISQUE,'Faible'),NVL(TEMPS_INTERVENTION,0),NVL(LATITUDE,36.8),NVL(LONGITUDE,10.18) FROM ZONE_COUVERTURE ORDER BY ID_ZONE");
    if(!q.exec(sql)){m_storageError=q.lastError().text(); qWarning().noquote()<<"Lecture Oracle impossible:"<<m_storageError; return false;}
    QList<Zone> loaded;
    while(q.next()){
        Zone z; const int id=q.value(0).toInt(); z.id=QString("Z-%1").arg(id,3,10,QChar('0')); z.nom=q.value(1).toString(); z.region=q.value(2).toString();
        z.superficie=q.value(3).toDouble(); z.population=q.value(4).toInt(); z.risque=q.value(5).toString();
        z.temps=q.value(6).toDouble(); z.lat=q.value(7).toDouble(); z.lon=q.value(8).toDouble();
        loaded.append(z);
    }
    m_zones=loaded; m_oracleActive=true; return true;
}

bool ZoneStore::saveOracle(const Zone &z, bool insert)
{
    QSqlQuery q(Connection::createInstance().database());
    const QString sql=insert
      ? QStringLiteral("INSERT INTO ZONE_COUVERTURE (ID_ZONE,NOM,REGION,SUPERFICIE,POPULATION,NIVEAU_RISQUE,TEMPS_INTERVENTION,LATITUDE,LONGITUDE) VALUES (:id,:nom,:region,:superficie,:population,:risque,:temps,:lat,:lon)")
      : QStringLiteral("UPDATE ZONE_COUVERTURE SET NOM=:nom,REGION=:region,SUPERFICIE=:superficie,POPULATION=:population,NIVEAU_RISQUE=:risque,TEMPS_INTERVENTION=:temps,LATITUDE=:lat,LONGITUDE=:lon WHERE ID_ZONE=:id");
    q.prepare(sql); q.bindValue(":id",z.id.mid(2).toInt()); q.bindValue(":nom",z.nom); q.bindValue(":region",z.region); q.bindValue(":superficie",z.superficie); q.bindValue(":population",z.population); q.bindValue(":risque",z.risque); q.bindValue(":temps",z.temps); q.bindValue(":lat",z.lat); q.bindValue(":lon",z.lon);
    if (q.exec() && q.numRowsAffected() != 0)
        return true;

    m_storageError = q.lastError().text();
    if (m_storageError.isEmpty())
        m_storageError = QStringLiteral("La zone à modifier n'existe plus dans Oracle.");
    emit storageError(m_storageError);
    return false;
}

bool ZoneStore::deleteOracle(const QString &id)
{
    QSqlQuery q(Connection::createInstance().database()); q.prepare("DELETE FROM ZONE_COUVERTURE WHERE ID_ZONE=:id"); q.bindValue(":id",id.mid(2).toInt());
    if (q.exec() && q.numRowsAffected() != 0)
        return true;

    m_storageError = q.lastError().text();
    if (m_storageError.isEmpty())
        m_storageError = QStringLiteral("La zone à supprimer n'existe plus dans Oracle.");
    emit storageError(m_storageError);
    return false;
}
void ZoneStore::setSeuil(double v) { m_seuil=qBound(1.0,v,180.0); persist(); emit changed(); }
void ZoneStore::load() {
 const QString path=QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/zones.json"; QFile f(path); if(!f.open(QIODevice::ReadOnly)) return;
 const QJsonObject root=QJsonDocument::fromJson(f.readAll()).object(); m_seuil=root.value("seuil").toDouble(12.0); QJsonArray a=root.value("zones").toArray(); if(!root.contains("zones")) return; QList<Zone> loaded;
 for(const QJsonValue &v:a){QJsonObject o=v.toObject(); Zone z; z.id=o["id"].toString();z.nom=o["nom"].toString();z.region=o["region"].toString();z.risque=o["risque"].toString();z.temps=o["temps"].toDouble();z.superficie=o["superficie"].toDouble();z.population=o["population"].toInt();z.interventions=o["interventions"].toInt();z.agents=o["agents"].toInt();z.equipes=o["equipes"].toInt();z.vehicules=o["vehicules"].toInt();z.lat=o["lat"].toDouble();z.lon=o["lon"].toDouble(); loaded<<z;} m_zones=loaded;
}
void ZoneStore::persist() const {
 QJsonArray a; for(const Zone &z:m_zones){QJsonObject o;o["id"]=z.id;o["nom"]=z.nom;o["region"]=z.region;o["risque"]=z.risque;o["temps"]=z.temps;o["superficie"]=z.superficie;o["population"]=z.population;o["interventions"]=z.interventions;o["agents"]=z.agents;o["equipes"]=z.equipes;o["vehicules"]=z.vehicules;o["lat"]=z.lat;o["lon"]=z.lon;a.append(o);} QJsonObject root;root["seuil"]=m_seuil;root["zones"]=a;QString path=QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/zones.json";QDir().mkpath(QFileInfo(path).path());QSaveFile f(path);if(f.open(QIODevice::WriteOnly)){f.write(QJsonDocument(root).toJson());f.commit();}
}
const Zone *ZoneStore::find(const QString &id) const
{
    for (const Zone &z : m_zones) if (z.id == id) return &z;
    return nullptr;
}
QString ZoneStore::nextId() const
{
    int mx = 0;
    for (const Zone &z : m_zones) mx = std::max(mx, z.id.mid(2).toInt());
    return QString("Z-%1").arg(mx + 1, 3, 10, QChar('0'));
}
bool ZoneStore::ajouter(Zone &z)
{
    if (!m_oracleActive) {
        m_storageError = QStringLiteral("Ajout refusé : Oracle n’est pas connecté. Aucune zone n’a été ajoutée localement.");
        emit storageError(m_storageError);
        return false;
    }
    {
        QSqlDatabase db = Connection::createInstance().database();
        QSqlQuery sequenceExists(db);
        if (!sequenceExists.exec(QStringLiteral("SELECT COUNT(*) FROM USER_SEQUENCES WHERE SEQUENCE_NAME='SEQ_ZONE'"))
            || !sequenceExists.next()) {
            m_storageError = sequenceExists.lastError().text();
            emit storageError(m_storageError);
            return false;
        }
        if (sequenceExists.value(0).toInt() == 0) {
            QSqlQuery maxId(db);
            if (!maxId.exec(QStringLiteral("SELECT NVL(MAX(ID_ZONE),0)+1 FROM ZONE_COUVERTURE")) || !maxId.next()) {
                m_storageError = maxId.lastError().text();
                emit storageError(m_storageError);
                return false;
            }
            const int firstId = qMax(1, maxId.value(0).toInt());
            QSqlQuery createSequence(db);
            if (!createSequence.exec(QStringLiteral("CREATE SEQUENCE SEQ_ZONE START WITH %1 INCREMENT BY 1 NOCACHE").arg(firstId))) {
                m_storageError = createSequence.lastError().text();
                emit storageError(m_storageError);
                return false;
            }
        }
        if (!db.transaction()) {
            m_storageError = db.lastError().text();
            emit storageError(m_storageError);
            return false;
        }

        QSqlQuery sequence(db);
        if (!sequence.exec(QStringLiteral("SELECT SEQ_ZONE.NEXTVAL FROM DUAL")) || !sequence.next()) {
            m_storageError = sequence.lastError().text();
            db.rollback();
            emit storageError(m_storageError);
            return false;
        }

        z.id = QStringLiteral("Z-%1").arg(sequence.value(0).toInt(), 3, 10, QLatin1Char('0'));
        if (!saveOracle(z, true)) {
            db.rollback();
            return false;
        }
        if (!db.commit()) {
            m_storageError = db.lastError().text();
            db.rollback();
            emit storageError(m_storageError);
            return false;
        }
    }

    m_zones.append(z);
    persist();
    emit changed();
    return true;
}
bool ZoneStore::modifier(const Zone &z)
{
    if (!m_oracleActive) {
        m_storageError = QStringLiteral("Modification refusée : Oracle n’est pas connecté. Aucune donnée locale n’a été modifiée.");
        emit storageError(m_storageError);
        return false;
    }
    for (Zone &x : m_zones) {
        if (x.id != z.id)
            continue;

        if (m_oracleActive) {
            QSqlDatabase db = Connection::createInstance().database();
            if (!db.transaction()) {
                m_storageError = db.lastError().text();
                emit storageError(m_storageError);
                return false;
            }
            if (!saveOracle(z, false)) {
                db.rollback();
                return false;
            }
            if (!db.commit()) {
                m_storageError = db.lastError().text();
                db.rollback();
                emit storageError(m_storageError);
                return false;
            }
        }

        x = z;
        persist();
        emit changed();
        return true;
    }
    m_storageError = QStringLiteral("La zone à modifier est introuvable.");
    return false;
}
bool ZoneStore::supprimer(const QString &id)
{
    if (!m_oracleActive) {
        m_storageError = QStringLiteral("Suppression refusée : Oracle n’est pas connecté. Aucune zone locale n’a été supprimée.");
        emit storageError(m_storageError);
        return false;
    }
    for (int i = 0; i < m_zones.size(); ++i) {
        if (m_zones[i].id != id)
            continue;

        if (m_oracleActive) {
            QSqlDatabase db = Connection::createInstance().database();
            if (!db.transaction()) {
                m_storageError = db.lastError().text();
                emit storageError(m_storageError);
                return false;
            }
            if (!deleteOracle(id)) {
                db.rollback();
                return false;
            }
            if (!db.commit()) {
                m_storageError = db.lastError().text();
                db.rollback();
                emit storageError(m_storageError);
                return false;
            }
        }

        m_zones.removeAt(i);
        persist();
        emit changed();
        return true;
    }
    m_storageError = QStringLiteral("La zone à supprimer est introuvable.");
    return false;
}

// ============================================================
//  MapWidget : tuiles OpenStreetMap telechargees + dessin des zones
// ============================================================
static const double kPi = 3.14159265358979323846;
static const double kCaserneLat = 36.8000, kCaserneLon = 10.1800;
static QColor riskColor(const QString &r) { return QColor(r == "Élevé" ? "#d32f2f" : r == "Moyen" ? "#f59e0b" : "#2e9e5b"); }

MapWidget::MapWidget(QWidget *parent) : QWidget(parent), m_net(new QNetworkAccessManager(this))
{
    setMouseTracking(false);
    setCursor(Qt::OpenHandCursor);
    setMinimumSize(420, 300);
    auto mk = [this](const QString &t, int dz, int y) {
        auto *b = new QPushButton(t, this); b->setFixedSize(30, 30); b->move(0, y);
        b->setStyleSheet("background:white;border:1px solid #c9ccd3;border-radius:4px;font-size:16px;font-weight:bold;");
        connect(b, &QPushButton::clicked, this, [this, dz] { zoomAt(rect().center(), dz); });
        return b; };
    auto *p = mk("+", 1, 10); auto *m = mk("−", -1, 46);
    p->setProperty("zbtn", true); m->setProperty("zbtn", true);
}

QPointF MapWidget::toPx(double lat, double lon) const
{
    const double n = 256.0 * std::pow(2.0, m_zoom);
    auto wx = [&](double lo) { return (lo + 180.0) / 360.0 * n; };
    auto wy = [&](double la) { double r = la * kPi / 180.0; return (1.0 - std::asinh(std::tan(r)) / kPi) / 2.0 * n; };
    return QPointF(width() / 2.0 + wx(lon) - wx(m_lon), height() / 2.0 + wy(lat) - wy(m_lat));
}
QPointF MapWidget::toLL(const QPointF &px) const
{
    const double n = 256.0 * std::pow(2.0, m_zoom);
    const double cx = (m_lon + 180.0) / 360.0 * n;
    const double cy = (1.0 - std::asinh(std::tan(m_lat * kPi / 180.0)) / kPi) / 2.0 * n;
    const double x = cx + px.x() - width() / 2.0, y = cy + px.y() - height() / 2.0;
    const double lon = x / n * 360.0 - 180.0;
    const double lat = std::atan(std::sinh(kPi * (1.0 - 2.0 * y / n))) * 180.0 / kPi;
    return QPointF(lat, lon);                              // x = lat, y = lon
}

void MapWidget::setZones(const QList<Zone> &zones, const QString &sel) { m_zones = zones; m_sel = sel; update(); }

void MapWidget::flyTo(double lat, double lon, int zoom) { m_lat = lat; m_lon = lon; m_zoom = qBound(3, zoom, 18); update(); }

QPixmap MapWidget::tile(int z, int x, int y)
{
    const int n = 1 << z;
    x = ((x % n) + n) % n;
    if (y < 0 || y >= n) return QPixmap();
    const QString key = QString("%1/%2/%3/%4").arg(m_fond).arg(z).arg(x).arg(y);
    auto it = m_cache.constFind(key);
    if (it != m_cache.constEnd()) return it.value();
    if (!m_pending.contains(key)) {
        m_pending.insert(key);
        QString url;
        if (m_fond == 1) url = QString("https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/%1/%2/%3").arg(z).arg(y).arg(x);
        else if (m_fond == 2) url = QString("https://a.tile.opentopomap.org/%1/%2/%3.png").arg(z).arg(x).arg(y);
        else url = QString("https://tile.openstreetmap.org/%1/%2/%3.png").arg(z).arg(x).arg(y);
        QNetworkRequest rq{QUrl(url)};
        rq.setRawHeader("User-Agent", "FireStationApp/1.0 (projet etudiant)");
        rq.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
        QNetworkReply *r = m_net->get(rq);
        connect(r, &QNetworkReply::finished, this, [this, r, key] {
            r->deleteLater(); m_pending.remove(key);
            QPixmap pm;
            if (r->error() == QNetworkReply::NoError && pm.loadFromData(r->readAll())) {
                if (m_cache.size() > 600) m_cache.clear();
                m_cache.insert(key, pm); update();
            }
        });
    }
    return QPixmap();
}

int MapWidget::hit(const QPoint &p) const
{
    for (int i = m_zones.size() - 1; i >= 0; --i) {
        const QPointF c = toPx(m_zones[i].lat, m_zones[i].lon);
        if (std::hypot(c.x() - p.x(), c.y() - p.y()) <= 19) return i;
    }
    return -1;
}

void MapWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this); p.setRenderHint(QPainter::Antialiasing); p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.fillRect(rect(), QColor("#e9e5dc"));

    // --- tuiles
    const double n = 256.0 * std::pow(2.0, m_zoom);
    const double cx = (m_lon + 180.0) / 360.0 * n;
    const double cy = (1.0 - std::asinh(std::tan(m_lat * kPi / 180.0)) / kPi) / 2.0 * n;
    const double x0 = cx - width() / 2.0, y0 = cy - height() / 2.0;
    for (int ty = int(std::floor(y0 / 256)); ty <= int(std::floor((y0 + height()) / 256)); ++ty)
        for (int tx = int(std::floor(x0 / 256)); tx <= int(std::floor((x0 + width()) / 256)); ++tx) {
            const QPixmap pm = tile(m_zoom, tx, ty);
            const QPointF pos(tx * 256 - x0, ty * 256 - y0);
            if (!pm.isNull()) p.drawPixmap(pos, pm);
        }

    // --- zones
    const double mpp = 156543.03392 * std::cos(m_lat * kPi / 180.0) / std::pow(2.0, m_zoom);   // metres / pixel
    for (const Zone &z : m_zones) {
        const QPointF c = toPx(z.lat, z.lon);
        const QColor col = riskColor(z.risque);
        QColor fill = col; fill.setAlpha(55);
        QPen pen(col, z.sousCouverte(m_threshold) ? 3 : 1);
        if (z.sousCouverte(m_threshold)) pen.setStyle(Qt::DashLine);
        p.setPen(pen); p.setBrush(fill);
        const double r = 1800.0 / mpp; p.drawEllipse(c, r, r);
    }
    { // caserne
        const QPointF c = toPx(kCaserneLat, kCaserneLon);
        p.setPen(QPen(Qt::white, 3)); p.setBrush(QColor("#1d6fe8")); p.drawEllipse(c, 13, 13);
        p.setPen(Qt::white); p.setFont(QFont(font().family(), 11, QFont::Bold)); p.drawText(QRectF(c.x() - 13, c.y() - 13, 26, 26), Qt::AlignCenter, "⌂");
    }
    for (const Zone &z : m_zones) {
        const QPointF c = toPx(z.lat, z.lon);
        if (z.id == m_sel) { p.setPen(QPen(QColor("#1d4ed8"), 3)); p.setBrush(Qt::NoBrush); p.drawEllipse(c, 21, 21); }
        p.setPen(QPen(Qt::white, 3)); p.setBrush(riskColor(z.risque)); p.drawEllipse(c, 17, 17);
        p.setPen(Qt::white); p.setFont(QFont(font().family(), 11, QFont::Bold));
        p.drawText(QRectF(c.x() - 17, c.y() - 17, 34, 34), Qt::AlignCenter, z.risque == "Faible" ? "✔" : z.risque == "Moyen" ? "⚠" : "🔥");
        QFont f(font().family(), 8, QFont::Bold); p.setFont(f);
        const QRectF lr(c.x() - 24, c.y() + 18, 48, 15);
        p.setPen(Qt::NoPen); p.setBrush(QColor("#333")); p.drawRoundedRect(lr, 7, 7);
        p.setPen(Qt::white); p.drawText(lr, Qt::AlignCenter, z.id);
    }
    if (m_tempOn) {                                         // repere de la nouvelle position
        const QPointF c = toPx(m_tempLat, m_tempLon);
        p.setPen(QPen(Qt::white, 2)); p.setBrush(QColor("#1d4ed8")); p.drawEllipse(c, 9, 9);
        p.setPen(QPen(QColor("#1d4ed8"), 2)); p.drawLine(c + QPointF(-16, 0), c + QPointF(16, 0)); p.drawLine(c + QPointF(0, -16), c + QPointF(0, 16));
    }

    // --- bulle d'information de la zone selectionnee
    for (const Zone &z : m_zones) {
        if (z.id != m_sel) continue;
        const QPointF c = toPx(z.lat, z.lon);
        QFont fb(font().family(), 9, QFont::Bold), fn(font().family(), 8);
        const QStringList lines = {z.id + " · " + z.region,
            QString("Temps moyen : %1 min  ·  %2 interv.").arg(QString::number(z.temps, 'f', 1).replace('.', ',')).arg(z.interventions),
            QString("Agents liés : %1  ·  Véhicules : %2").arg(z.agents).arg(z.vehicules),
            QString("Population : %1").arg(QLocale(QLocale::French).toString(z.population)),
            QString("Superficie : %1 km²").arg(z.superficie)};
        QRectF box(c.x() - 120, c.y() - 36 - 112, 240, 112);
        box.translate(qMax(0.0, 6 - box.left()), qMax(0.0, 6 - box.top()));
        p.setPen(QPen(QColor("#cfd1d8"), 1)); p.setBrush(Qt::white); p.drawRoundedRect(box, 6, 6);
        p.setPen(QColor("#222")); p.setFont(fb); p.drawText(QPointF(box.left() + 10, box.top() + 20), z.nom);
        const QRectF bd(box.right() - 62, box.top() + 7, 54, 18);
        p.setPen(Qt::NoPen); p.setBrush(riskColor(z.risque)); p.drawRoundedRect(bd, 9, 9);
        p.setPen(Qt::white); p.setFont(fn); p.drawText(bd, Qt::AlignCenter, z.risque);
        p.setPen(QColor("#555")); for (int i = 0; i < lines.size(); ++i) p.drawText(QPointF(box.left() + 10, box.top() + 38 + i * 15), lines[i]);
    }

    // --- echelle + attribution
    const double nice[] = {50000, 20000, 10000, 5000, 2000, 1000, 500, 200, 100, 50};
    double len = 100; for (double v : nice) if (v / mpp < 160) { len = v; break; }
    const double px = len / mpp, bx = 14, by = height() - 18;
    p.setPen(QPen(QColor("#222"), 2)); p.drawLine(QPointF(bx, by), QPointF(bx + px, by));
    p.drawLine(QPointF(bx, by - 4), QPointF(bx, by)); p.drawLine(QPointF(bx + px, by - 4), QPointF(bx + px, by));
    p.setFont(QFont(font().family(), 8)); p.drawText(QPointF(bx, by - 6), len >= 1000 ? QString::number(len / 1000) + " km" : QString::number(len) + " m");
    const QString att = m_fond == 1 ? "Imagery © Esri" : m_fond == 2 ? "© OpenStreetMap, SRTM | © OpenTopoMap" : "© OpenStreetMap contributors";
    p.setPen(QColor("#333")); p.setFont(QFont(font().family(), 7));
    const QRectF ar(width() - 230, height() - 16, 226, 14);
    p.fillRect(ar.adjusted(60, 0, 4, 2), QColor(255, 255, 255, 190));
    p.drawText(ar, Qt::AlignRight | Qt::AlignVCenter, att);
}

void MapWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton) return;
    m_press = m_last = e->pos(); m_moved = false;
    const int i = hit(e->pos());
    m_dragIdx = (m_drag && i >= 0) ? i : -1;
    m_panning = (m_dragIdx < 0);
    if (m_panning) setCursor(Qt::ClosedHandCursor);
}
void MapWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (!(e->buttons() & Qt::LeftButton)) return;
    if ((e->pos() - m_press).manhattanLength() > 4) m_moved = true;
    if (m_dragIdx >= 0 && m_moved) {
        const QPointF ll = toLL(e->pos()); m_zones[m_dragIdx].lat = ll.x(); m_zones[m_dragIdx].lon = ll.y(); update();
    } else if (m_panning) {
        const QPointF a = toLL(m_last), b = toLL(e->pos());
        m_lat -= b.x() - a.x(); m_lon -= b.y() - a.y(); update();
    }
    m_last = e->pos();
}
void MapWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton) return;
    setCursor(Qt::OpenHandCursor);
    if (m_dragIdx >= 0 && m_moved) {
        emit zoneMoved(m_zones[m_dragIdx].id, m_zones[m_dragIdx].lat, m_zones[m_dragIdx].lon);
    } else if (!m_moved) {
        const int i = hit(e->pos());
        if (i >= 0) emit zoneClicked(m_zones[i].id);
        else { const QPointF ll = toLL(e->pos()); emit mapClicked(ll.x(), ll.y()); }
    }
    m_dragIdx = -1; m_panning = false;
}
void MapWidget::wheelEvent(QWheelEvent *e) { zoomAt(e->position().toPoint(), e->angleDelta().y() > 0 ? 1 : -1); }

void MapWidget::zoomAt(const QPoint &p, int dz)
{
    const int nz = qBound(3, m_zoom + dz, 18);
    if (nz == m_zoom) return;
    const QPointF before = toLL(p);
    m_zoom = nz;
    const QPointF after = toLL(p);
    m_lat += before.x() - after.x(); m_lon += before.y() - after.y();
    update();
}

// ============================================================
//  ZoneCard
// ============================================================
static QString badgeColor(const QString &r) { return r == "Élevé" ? "#d32f2f" : r == "Moyen" ? "#f59e0b" : "#2e9e5b"; }

ZoneCard::ZoneCard(ZoneStore *store, QWidget *parent) : QFrame(parent), m_store(store)
{
    setObjectName("card");
    buildUi();
    connect(m_store, &ZoneStore::changed, this, [this] { refreshFilters(); refreshMap(); });
    connect(m_store, &ZoneStore::storageError, this, [this](const QString &e) { hint("Oracle : " + e, true); }, Qt::QueuedConnection);
    refreshFilters();
    refreshMap();
}

void ZoneCard::buildUi()
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(14, 12, 14, 12);
    root->setSpacing(8);

    // --- en-tete + filtres
    auto *head = new QHBoxLayout;
    auto *tt = new QVBoxLayout;
    auto *title = new QLabel("Carte des zones à risque"); title->setObjectName("cardTitle");
    auto *sub = new QLabel("Visualisez les zones sensibles et planifiez les interventions"); sub->setObjectName("muted");
    tt->addWidget(title); tt->addWidget(sub);
    head->addLayout(tt, 1);
    m_region = new QComboBox; m_niveau = new QComboBox; m_niveau->addItems({"Tous", "Élevé", "Moyen", "Faible"});
    auto mk = [](const QString &t, QWidget *w) { auto *v = new QVBoxLayout; v->setSpacing(1);
        auto *l = new QLabel(t); l->setObjectName("muted"); v->addWidget(l); v->addWidget(w); return v; };
    head->addLayout(mk("Région / quartier", m_region));
    head->addLayout(mk("Niveau de risque", m_niveau));
    auto *pos = new QPushButton("◉ Ma position"); pos->setObjectName("tool");
    head->addWidget(pos, 0, Qt::AlignBottom);
    root->addLayout(head);

    // --- barre CRUD
    auto *bar = new QHBoxLayout; bar->setSpacing(6);
    auto tool = [&](const QString &t) { auto *b = new QPushButton(t); b->setObjectName("tool"); b->setCursor(Qt::PointingHandCursor); bar->addWidget(b); return b; };
    m_bAdd = tool("＋ Nouvelle zone");
    m_bPlace = tool("➚ Placer sur la carte");
    m_bMove = tool("✥ Déplacer");
    m_bEdit = tool("✎ Modifier");
    m_bDel = tool("🗑 Supprimer");
    m_bMove->setCheckable(true); m_bPlace->setCheckable(true);
    bar->addStretch(1);
    m_fond = new QComboBox; m_fond->addItems({"🗺 Plan", "🛰 Satellite", "⛰ Relief"});
    bar->addWidget(m_fond);
    auto *tableBtn = new QPushButton("▤ Tableau"); tableBtn->setObjectName("tool"); bar->addWidget(tableBtn);
    auto *csvBtn = new QPushButton("CSV"); csvBtn->setObjectName("tool"); bar->addWidget(csvBtn);
    auto *pdfBtn = new QPushButton("PDF"); pdfBtn->setObjectName("tool"); bar->addWidget(pdfBtn);
    auto *statsBtn = new QPushButton("Statistiques"); statsBtn->setObjectName("tool"); bar->addWidget(statsBtn);
    m_seuil = new QDoubleSpinBox; m_seuil->setRange(1,180); m_seuil->setDecimals(1); m_seuil->setSuffix(" min"); m_seuil->setPrefix("Seuil "); m_seuil->setValue(m_store->seuil()); m_seuil->setMaximumWidth(110); bar->addWidget(m_seuil);
    root->addLayout(bar);

    m_hint = new QLabel; m_hint->setObjectName("hint"); m_hint->hide(); m_hint->setWordWrap(true);
    root->addWidget(m_hint);

    // --- carte + panneau integre
    auto *body = new QHBoxLayout; body->setSpacing(10);
    m_map = new MapWidget;
    body->addWidget(m_map, 1);

    m_panel = new QFrame; m_panel->setObjectName("panel"); m_panel->setFixedWidth(280); m_panel->hide();
    auto *pv = new QVBoxLayout(m_panel); pv->setContentsMargins(12, 10, 12, 10);
    m_pages = new QStackedWidget; pv->addWidget(m_pages);

    // page 0 : formulaire ajouter / modifier
    auto *fp = new QWidget; auto *fv = new QVBoxLayout(fp); fv->setContentsMargins(0, 0, 0, 0);
    m_formTitle = new QLabel; m_formTitle->setObjectName("cardTitle"); fv->addWidget(m_formTitle);
    auto *fl = new QFormLayout; fl->setLabelAlignment(Qt::AlignLeft); fl->setVerticalSpacing(5);
    m_nom = new QLineEdit; m_nom->setPlaceholderText("Nom de la zone");
    m_fRegion = new QComboBox; m_fRegion->setEditable(true);
    m_fRegion->addItems({"Tunis", "Ben Arous", "Ariana", "Manouba", "La Marsa", "Radès / Ben Arous"});
    m_fRisque = new QComboBox; m_fRisque->addItems({"Élevé", "Moyen", "Faible"}); m_fRisque->setEnabled(false); m_fRisque->setToolTip("Calculé selon le nombre d interventions déclaré : moins de 15 faible, 15 à 29 moyen, 30 ou plus élevé.");
    m_fTemps = new QDoubleSpinBox; m_fTemps->setRange(0, 120); m_fTemps->setDecimals(1); m_fTemps->setSuffix(" min");
    m_fInterv = new QSpinBox; m_fInterv->setRange(0, 9999);
    m_fEquipes = new QSpinBox; m_fEquipes->setRange(0, 99);
    m_fVeh = new QSpinBox; m_fVeh->setRange(0, 99);
    m_fPop = new QSpinBox; m_fPop->setRange(0, 5000000); m_fPop->setSingleStep(1000);
    m_fSuperficie = new QDoubleSpinBox; m_fSuperficie->setRange(0,100000); m_fSuperficie->setDecimals(2); m_fSuperficie->setSuffix(" km²");
    m_fLat = new QDoubleSpinBox; m_fLat->setRange(-90, 90); m_fLat->setDecimals(5);
    m_fLon = new QDoubleSpinBox; m_fLon->setRange(-180, 180); m_fLon->setDecimals(5);
    fl->addRow("Nom", m_nom); fl->addRow("Région", m_fRegion); fl->addRow("Risque calculé", m_fRisque);
    fl->addRow("Temps moyen", m_fTemps); fl->addRow("Interventions", m_fInterv);
    fl->addRow("Équipes", m_fEquipes); fl->addRow("Véhicules", m_fVeh); fl->addRow("Population", m_fPop); fl->addRow("Superficie", m_fSuperficie);
    fl->addRow("Latitude", m_fLat); fl->addRow("Longitude", m_fLon);
    fv->addLayout(fl);
    auto *tip = new QLabel("Astuce : cliquez sur la carte pour remplir latitude / longitude."); tip->setObjectName("muted"); tip->setWordWrap(true);
    fv->addWidget(tip); fv->addStretch(1);
    auto *fb = new QHBoxLayout;
    auto *ok = new QPushButton("Enregistrer"); ok->setObjectName("primary");
    auto *ko = new QPushButton("Annuler"); ko->setObjectName("tool");
    fb->addWidget(ok); fb->addWidget(ko); fv->addLayout(fb);
    m_pages->addWidget(fp);

    // page 1 : confirmation suppression
    auto *dp = new QWidget; auto *dv = new QVBoxLayout(dp); dv->setContentsMargins(0, 0, 0, 0);
    auto *dt = new QLabel("Supprimer la zone"); dt->setObjectName("cardTitle");
    m_confirmText = new QLabel; m_confirmText->setWordWrap(true);
    dv->addWidget(dt); dv->addWidget(m_confirmText); dv->addStretch(1);
    auto *db = new QHBoxLayout;
    auto *dok = new QPushButton("Supprimer"); dok->setObjectName("primary");
    auto *dko = new QPushButton("Annuler"); dko->setObjectName("tool");
    db->addWidget(dok); db->addWidget(dko); dv->addLayout(db);
    m_pages->addWidget(dp);

    body->addWidget(m_panel);
    root->addLayout(body, 1);

    // --- connexions
    connect(m_map, &MapWidget::zoneClicked, this, [this](const QString &id) {
        m_sel = id; emit zoneSelected(id);
        if (m_form == Edit) fillForm(*m_store->find(m_sel));
        if (m_form == Delete) openForm(Delete);
        refreshMap(); });
    connect(m_map, &MapWidget::zoneMoved, this, [this](const QString &id, double la, double lo) {
        if (const Zone *p = m_store->find(id)) { Zone z = *p; z.lat = la; z.lon = lo; m_store->modifier(z); hint(QString("Zone %1 déplacée.").arg(z.id)); } });
    connect(m_map, &MapWidget::mapClicked, this, [this](double la, double lo) {
        if (m_form == Add || m_form == Edit) { m_fLat->setValue(la); m_fLon->setValue(lo); m_map->setTemp(true, la, lo); }
        else if (m_mode == Place) {
            if (const Zone *p = m_store->find(m_sel)) { Zone z = *p; z.lat = la; z.lon = lo; m_store->modifier(z);
                setMode(Idle); hint(QString("Zone %1 placée sur la carte.").arg(z.id)); } } });
    connect(m_region, &QComboBox::currentTextChanged, this, [this] { refreshMap(); });
    connect(m_niveau, &QComboBox::currentTextChanged, this, [this] { refreshMap(); });
    connect(m_fond, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int i) { m_map->setFond(i); });
    connect(tableBtn, &QPushButton::clicked, this, &ZoneCard::showTable);
    connect(csvBtn, &QPushButton::clicked, this, &ZoneCard::exportCsv);
    connect(pdfBtn, &QPushButton::clicked, this, &ZoneCard::exportPdf);
    connect(statsBtn, &QPushButton::clicked, this, &ZoneCard::showStats);
    connect(m_seuil, QOverload<double>::of(&QDoubleSpinBox::valueChanged), m_store, &ZoneStore::setSeuil);
    connect(pos, &QPushButton::clicked, this, [this] { m_map->locate(); });

    connect(m_bAdd, &QPushButton::clicked, this, [this] { startAdd(); });
    connect(m_bEdit, &QPushButton::clicked, this, [this] { if (needSelection()) openForm(Edit); });
    connect(m_bDel, &QPushButton::clicked, this, [this] { if (needSelection()) openForm(Delete); });
    connect(m_bPlace, &QPushButton::clicked, this, [this](bool on) {
        if (on && m_form == None && !needSelection()) { m_bPlace->setChecked(false); return; }
        setMode(on ? Place : Idle); });
    connect(m_bMove, &QPushButton::clicked, this, [this](bool on) {
        if (on && !needSelection()) { m_bMove->setChecked(false); return; }
        setMode(on ? Move : Idle); });
    connect(ok, &QPushButton::clicked, this, &ZoneCard::saveForm);
    connect(ko, &QPushButton::clicked, this, &ZoneCard::closeForm);
    connect(dko, &QPushButton::clicked, this, &ZoneCard::closeForm);
    connect(dok, &QPushButton::clicked, this, [this] {
        const QString id = m_sel;
        if (!m_store->supprimer(id)) {
            hint(m_store->lastStorageError().isEmpty()
                     ? QStringLiteral("La suppression a échoué.")
                     : QStringLiteral("Suppression impossible : %1").arg(m_store->lastStorageError()), true);
            return;
        }
        m_sel.clear();
        closeForm();
        hint("Zone " + id + " supprimée.");
    });
}

void ZoneCard::hint(const QString &text, bool warn)
{
    m_hint->setProperty("warn", warn);
    m_hint->style()->unpolish(m_hint); m_hint->style()->polish(m_hint);
    m_hint->setText(text); m_hint->setVisible(!text.isEmpty());
}

bool ZoneCard::needSelection()
{
    if (m_store->find(m_sel)) return true;
    hint("Sélectionnez d'abord une zone en cliquant sur son marqueur sur la carte.", true);
    return false;
}

void ZoneCard::refreshFilters()
{
    const QString cur = m_region->currentText();
    QSignalBlocker b(m_region);
    m_region->clear(); m_region->addItem("Toutes");
    QStringList regs;
    for (const Zone &z : m_store->all()) if (!regs.contains(z.region)) regs << z.region;
    regs.sort(); m_region->addItems(regs);
    int i = m_region->findText(cur); m_region->setCurrentIndex(i < 0 ? 0 : i);
}

void ZoneCard::resetFilters() { m_region->setCurrentIndex(0); m_niveau->setCurrentIndex(0); }

void ZoneCard::refreshMap()
{
    QList<Zone> vis;
    for (const Zone &z : m_store->all()) {
        if (m_region->currentIndex() > 0 && z.region != m_region->currentText()) continue;
        if (m_niveau->currentIndex() > 0 && z.risque != m_niveau->currentText()) continue;
        vis.append(z);
    }
    m_map->setDraggable(m_mode == Move);
    m_map->setThreshold(m_store->seuil());
    m_map->setZones(vis, m_sel);
}

void ZoneCard::selectZone(const QString &id, bool fly)
{
    m_sel = id;
    emit zoneSelected(id);
    if (const Zone *z = m_store->find(id)) {
        if (m_form == Edit) fillForm(*z);
        if (fly) m_map->flyTo(z->lat, z->lon, 14);
        refreshMap();
    }
}

bool ZoneCard::searchZone(const QString &text)
{
    const QString t = text.trimmed();
    if (t.isEmpty()) return false;
    for (const Zone &z : m_store->all())
        if (z.id.compare(t, Qt::CaseInsensitive) == 0 || z.nom.contains(t, Qt::CaseInsensitive) || z.region.contains(t, Qt::CaseInsensitive)) {
            resetFilters(); selectZone(z.id); return true;
        }
    hint("Aucune zone ne correspond à « " + t + " ».", true);
    return false;
}

void ZoneCard::setMode(Mode m)
{
    m_mode = m;
    m_bMove->setChecked(m == Move); m_bPlace->setChecked(m == Place);
    m_map->setCrosshair(m == Place);
    if (m == Move) hint("Mode déplacement : faites glisser le marqueur de la zone vers sa nouvelle position.");
    else if (m == Place) hint("Mode placement : cliquez sur la carte pour positionner la zone.");
    else hint("");
    refreshMap();
}

void ZoneCard::fillForm(const Zone &z)
{
    m_nom->setText(z.nom); m_fRegion->setCurrentText(z.region); m_fRisque->setCurrentText(z.risque);
    m_fTemps->setValue(z.temps); m_fInterv->setValue(z.interventions); m_fEquipes->setValue(z.equipes);
    const bool oracle=m_store->oracleActive(); m_fInterv->setEnabled(!oracle); m_fVeh->setEnabled(!oracle); m_fEquipes->setEnabled(!oracle);
    m_fVeh->setValue(z.vehicules); m_fPop->setValue(z.population); m_fSuperficie->setValue(z.superficie); m_fLat->setValue(z.lat); m_fLon->setValue(z.lon);
}

void ZoneCard::startAdd() { openForm(Add); }

void ZoneCard::openForm(Form f)
{
    m_form = f;
    if (f == Add) {
        Zone z; z.id = m_store->nextId(); z.risque = "Moyen"; z.region = "Tunis"; z.temps = 10; z.equipes = 1; z.vehicules = 1;
        fillForm(z); m_nom->clear();
        m_formTitle->setText("Nouvelle zone " + z.id); m_pages->setCurrentIndex(0);
    } else if (f == Edit) {
        fillForm(*m_store->find(m_sel));
        m_formTitle->setText("Modifier " + m_sel); m_pages->setCurrentIndex(0);
    } else if (f == Delete) {
        const Zone *z = m_store->find(m_sel);
        m_confirmText->setText(QString("Voulez-vous vraiment supprimer %1 — %2 ?\nCette action est définitive.").arg(z->id, z->nom));
        m_pages->setCurrentIndex(1);
    }
    m_panel->show();
    if (f == Add) hint("Cliquez sur la carte pour choisir l'emplacement de la nouvelle zone.");
    else hint("");
    if (f == Add || f == Edit) m_map->setCrosshair(true);
}

void ZoneCard::closeForm()
{
    m_form = None; m_panel->hide(); m_map->setTemp(false); m_map->setCrosshair(m_mode == Place);
    if (m_mode == Idle) hint("");
}

void ZoneCard::saveForm()
{
    if (m_nom->text().trimmed().isEmpty()) { hint("Le nom de la zone est obligatoire.", true); return; }
    Zone z;
    if (m_form == Edit) z = *m_store->find(m_sel); else z.id = m_store->nextId();
    z.nom = m_nom->text().trimmed(); z.region = m_fRegion->currentText().trimmed(); z.risque = m_fRisque->currentText();
    z.temps = m_fTemps->value(); z.interventions = m_fInterv->value(); z.equipes = m_fEquipes->value();
    z.vehicules = m_fVeh->value(); z.population = m_fPop->value(); z.superficie = m_fSuperficie->value(); z.risque=z.interventions>=30?"Élevé":z.interventions>=15?"Moyen":"Faible"; m_fRisque->setCurrentText(z.risque); z.lat = m_fLat->value(); z.lon = m_fLon->value();
    const bool add = (m_form == Add);
    const bool saved = add ? m_store->ajouter(z) : m_store->modifier(z);
    if (!saved) {
        hint(m_store->lastStorageError().isEmpty()
                 ? QStringLiteral("L'enregistrement a échoué.")
                 : QStringLiteral("Enregistrement impossible : %1").arg(m_store->lastStorageError()), true);
        return;
    }
    if (add)
        m_sel = z.id;
    closeForm();
    hint(QString("Zone %1 %2.").arg(z.id, add ? "ajoutée" : "modifiée"));
    emit zoneSelected(z.id);
    m_map->flyTo(z.lat, z.lon, 14);
}

// ============================================================
//  ZonePrioCard
// ============================================================
void ZoneCard::refreshTable() {}
void ZoneCard::showTable()
{
    QDialog dialog(this); dialog.setWindowTitle("Tableau des zones"); dialog.resize(1100, 560);
    auto *layout = new QVBoxLayout(&dialog);
    auto *search = new QLineEdit; search->setPlaceholderText("Rechercher par nom ou région"); layout->addWidget(search);
    auto *table = new QTableWidget(0, 10);
    table->setHorizontalHeaderLabels({"ID", "Nom", "Région", "Superficie km²", "Population", "Risque", "Temps min", "Interventions (12 mois)", "Agents liés", "Véhicules liés"});
    table->setSortingEnabled(true); table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); layout->addWidget(table);
    for (const Zone &z : m_store->all()) {
        const int row=table->rowCount(); table->insertRow(row);
        const QStringList values={z.id,z.nom,z.region,QString::number(z.superficie,'f',2),QString::number(z.population),z.risque,QString::number(z.temps,'f',1),QString::number(z.interventions),QString::number(z.agents),QString::number(z.vehicules)};
        for(int col=0;col<values.size();++col)table->setItem(row,col,new QTableWidgetItem(values[col]));
    }
    connect(search,&QLineEdit::textChanged,table,[table](const QString &value){for(int row=0;row<table->rowCount();++row)table->setRowHidden(row,!value.isEmpty()&&!table->item(row,1)->text().contains(value,Qt::CaseInsensitive)&&!table->item(row,2)->text().contains(value,Qt::CaseInsensitive));});
    connect(table,&QTableWidget::cellDoubleClicked,this,[this,table](int row,int){selectZone(table->item(row,0)->text());});
    dialog.exec();
}

void ZoneCard::exportCsv()
{
    const QString path=QFileDialog::getSaveFileName(this,"Exporter CSV","zones.csv","CSV (*.csv)"); if(path.isEmpty())return;
    QSaveFile file(path); if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){hint("Impossible de créer le CSV.",true);return;}
    QTextStream out(&file); out<<"ID;Nom;Région;Superficie_km2;Population;Risque;Temps_min;Interventions_12_mois;Agents_lies;Vehicules_lies"<<Qt::endl;
    for(const Zone &z:m_store->all())out<<z.id<<";\""<<z.nom<<"\";\""<<z.region<<"\";"<<z.superficie<<";"<<z.population<<";"<<z.risque<<";"<<z.temps<<";"<<z.interventions<<";"<<z.agents<<";"<<z.vehicules<<Qt::endl;
    if(file.commit())hint("Export CSV terminé."); else hint("Échec de l’écriture du CSV.",true);
}

void ZoneCard::exportPdf()
{
    const QString path=QFileDialog::getSaveFileName(this,"Exporter PDF","zones.pdf","PDF (*.pdf)"); if(path.isEmpty())return;
    QPrinter printer(QPrinter::HighResolution); printer.setOutputFormat(QPrinter::PdfFormat); printer.setOutputFileName(path); QTextDocument doc;
    QString html="<h2>Zones de couverture</h2><table border='1' cellspacing='0' cellpadding='4'><tr><th>ID</th><th>Nom</th><th>Région</th><th>Superficie km²</th><th>Population</th><th>Risque</th><th>Temps min</th><th>Interventions 12 mois</th><th>Agents</th><th>Véhicules</th></tr>";
    for(const Zone &z:m_store->all()) html+=QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td><td>%6</td><td>%7</td><td>%8</td><td>%9</td><td>%10</td></tr>").arg(z.id).arg(z.nom.toHtmlEscaped()).arg(z.region.toHtmlEscaped()).arg(QString::number(z.superficie,'f',2)).arg(QString::number(z.population)).arg(z.risque).arg(QString::number(z.temps,'f',1)).arg(QString::number(z.interventions)).arg(QString::number(z.agents)).arg(QString::number(z.vehicules));
    doc.setHtml(html+"</table>"); doc.print(&printer); hint("Export PDF terminé.");
}

void ZoneCard::showStats()
{
    QDialog dialog(this); dialog.setWindowTitle("Statistiques des zones"); auto *layout=new QVBoxLayout(&dialog); const int total=m_store->all().size();
    for(const QString &risk:{QStringLiteral("Élevé"),QStringLiteral("Moyen"),QStringLiteral("Faible")}){int count=0;for(const Zone &z:m_store->all())if(z.risque==risk)++count;layout->addWidget(new QLabel(QString("%1 : %2 zone(s)").arg(risk).arg(count)));auto *bar=new QProgressBar;bar->setRange(0,qMax(1,total));bar->setValue(count);layout->addWidget(bar);}
    for(const Zone &z:m_store->all()){layout->addWidget(new QLabel(QString("%1 — %2 interventions sur 12 mois").arg(z.nom).arg(z.interventions)));auto *bar=new QProgressBar;bar->setRange(0,100);bar->setValue(qMin(100,z.interventions));layout->addWidget(bar);}
    layout->addWidget(new QLabel(QString("Seuil d’alerte : %1 min").arg(m_store->seuil()))); dialog.exec();
}
ZonePrioCard::ZonePrioCard(ZoneStore *store, QWidget *parent) : QFrame(parent), m_store(store)
{
    setObjectName("card");
    auto *v = new QVBoxLayout(this); v->setContentsMargins(14, 12, 14, 12);
    auto *h = new QHBoxLayout;
    auto *t = new QLabel("🔥  Zones à risque prioritaires"); t->setObjectName("cardTitle");
    auto *all = new QPushButton("Voir tout ›"); all->setObjectName("link");
    all->setCursor(Qt::PointingHandCursor);
    h->addWidget(t); h->addStretch(1); h->addWidget(all);
    v->addLayout(h);
    m_list = new QVBoxLayout; m_list->setSpacing(4); v->addLayout(m_list); v->addStretch(1);
    connect(all, &QPushButton::clicked, this, &ZonePrioCard::voirTout);
    connect(m_store, &ZoneStore::changed, this, &ZonePrioCard::rebuild);
    rebuild();
}

void ZonePrioCard::rebuild()
{
    while (QLayoutItem *it = m_list->takeAt(0)) { delete it->widget(); delete it; }
    QList<Zone> zs = m_store->all();
    auto rank = [](const QString &r) { return r == "Élevé" ? 0 : r == "Moyen" ? 1 : 2; };
    std::sort(zs.begin(), zs.end(), [&](const Zone &a, const Zone &b) {
        return rank(a.risque) != rank(b.risque) ? rank(a.risque) < rank(b.risque) : a.temps > b.temps; });
    for (int i = 0; i < qMin(4, int(zs.size())); ++i) {
        const Zone &z = zs[i];
        auto *row = new QPushButton; row->setObjectName("row"); row->setCursor(Qt::PointingHandCursor); row->setMinimumHeight(48);
        auto *h = new QHBoxLayout(row); h->setContentsMargins(6, 4, 6, 4);
        auto *ic = new QLabel(z.risque == "Faible" ? "✔" : z.risque == "Moyen" ? "⚠" : "🔥");
        ic->setFixedSize(34, 34); ic->setAlignment(Qt::AlignCenter);
        ic->setStyleSheet(QString("background:%1;border-radius:17px;color:%2;").arg(z.risque == "Moyen" ? "#fdf0d5" : "#fbe3e3", badgeColor(z.risque)));
        auto *tx = new QLabel(QString("<b>%1</b><br><span style='color:#777;font-size:11px'>%2 • %3 min%4</span>")
                                  .arg(z.nom, z.region, QString::number(z.temps, 'f', 1).replace('.', ','),
                                       z.sousCouverte(m_store->seuil()) ? " ⚠" : ""));
        auto *b = new QLabel(z.risque); b->setAlignment(Qt::AlignCenter); b->setFixedSize(54, 30);
        b->setStyleSheet(QString("background:%1;color:white;border-radius:5px;font-weight:bold;").arg(badgeColor(z.risque)));
        for (QLabel *l : {ic, tx, b}) l->setAttribute(Qt::WA_TransparentForMouseEvents);
        h->addWidget(ic); h->addWidget(tx, 1); h->addWidget(b);
        connect(row, &QPushButton::clicked, this, [this, id = z.id] { emit zoneClicked(id); });
        m_list->addWidget(row);
    }
}
