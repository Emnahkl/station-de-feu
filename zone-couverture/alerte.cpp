#include "alerte.h"
#include "zone.h"
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

static QLabel *circle(const QString &txt, const QString &bg, const QString &fg)
{
    auto *l = new QLabel(txt); l->setFixedSize(34, 34); l->setAlignment(Qt::AlignCenter);
    l->setStyleSheet(QString("background:%1;color:%2;border-radius:17px;").arg(bg, fg));
    return l;
}

AlerteCard::AlerteCard(ZoneStore *store, QWidget *parent) : QFrame(parent), m_store(store)
{
    setObjectName("card");
    auto *v = new QVBoxLayout(this); v->setContentsMargins(14, 12, 14, 12);
    auto *h = new QHBoxLayout;
    auto *t = new QLabel("🔔  Alertes en temps réel"); t->setObjectName("cardTitle");
    auto *all = new QLabel("<a style='color:#c62828;text-decoration:none'><b>Voir tout ›</b></a>");
    h->addWidget(t); h->addStretch(1); h->addWidget(all);
    v->addLayout(h);
    m_list = new QVBoxLayout; m_list->setSpacing(6); v->addLayout(m_list); v->addStretch(1);
    connect(m_store, &ZoneStore::changed, this, &AlerteCard::rebuild);
    rebuild();
}

void AlerteCard::rebuild()
{
    while (QLayoutItem *it = m_list->takeAt(0)) { delete it->widget(); delete it; }
    // alertes "sous-couverture" calculees depuis les zones (temps moyen > seuil)
    QList<Zone> zs = m_store->all();
    std::sort(zs.begin(), zs.end(), [](const Zone &a, const Zone &b) { return a.temps > b.temps; });
    int n = 0;
    for (const Zone &z : zs) {
        if (!z.sousCouverte(m_store->seuil()) || n >= 3) continue;
        ++n;
        auto *row = new QWidget; auto *h = new QHBoxLayout(row); h->setContentsMargins(0, 0, 0, 0);
        auto *tx = new QLabel(QString("<b>Zone sous-couverte — %1</b><br><span style='color:#777;font-size:11px'>Temps moyen %2 min (seuil %3 min)</span>")
                                  .arg(z.nom, QString::number(z.temps, 'f', 1).replace('.', ','),
                                       QString::number(m_store->seuil(), 'f', 1).replace('.', ',')));
        tx->setWordWrap(true);
        auto *b = new QPushButton("Alerte"); b->setObjectName("alert"); b->setCursor(Qt::PointingHandCursor);
        connect(b, &QPushButton::clicked, this, [this, id = z.id] { emit zoneClicked(id); });
        h->addWidget(circle("⏱", "#fbe3e3", "#d32f2f")); h->addWidget(tx, 1); h->addWidget(b);
        m_list->addWidget(row);
    }
    struct Inc { const char *ico, *bg, *fg, *titre, *sous, *heure, *etat; bool cours; };
    const Inc incs[] = {
        {"🔥", "#fbe3e3", "#d32f2f", "Incendie — Zone industrielle de Radès", "Zone industrielle, Radès", "10:02", "En cours", true},
        {"⚠", "#fdf0d5", "#f59e0b", "Accident de la route — La Marsa", "Route de La Marsa", "07:59", "Terminé", false},
        {"🌲", "#dff3e6", "#2e9e5b", "Feu de forêt — Ben Arous", "Forêt de Chouach", "04:34", "Terminé", false}};
    for (const Inc &i : incs) {
        auto *row = new QWidget; auto *h = new QHBoxLayout(row); h->setContentsMargins(0, 0, 0, 0);
        auto *tx = new QLabel(QString("<b>%1</b><br><span style='color:#777;font-size:11px'>%2</span>").arg(i.titre, i.sous));
        tx->setWordWrap(true);
        auto *r = new QLabel(QString("<span style='color:#777;font-size:11px'>%1</span><br><b style='color:%2;font-size:11px'>%3</b>")
                                 .arg(i.heure, i.cours ? "#d32f2f" : "#555", i.etat));
        r->setAlignment(Qt::AlignRight);
        h->addWidget(circle(i.ico, i.bg, i.fg)); h->addWidget(tx, 1); h->addWidget(r);
        m_list->addWidget(row);
    }
}

// ---------------- Meteo (Open-Meteo, gratuit, sans cle) ----------------
MeteoCard::MeteoCard(QWidget *parent) : QFrame(parent), m_net(new QNetworkAccessManager(this))
{
    setObjectName("meteo");
    auto *v = new QVBoxLayout(this); v->setContentsMargins(16, 12, 16, 12);
    auto *city = new QLabel("Tunis"); city->setStyleSheet("color:white;font-size:16px;font-weight:bold;");
    m_temp = new QLabel("—"); m_temp->setStyleSheet("color:white;font-size:34px;font-weight:bold;");
    m_desc = new QLabel("Météo indisponible"); m_desc->setStyleSheet("color:#e8f1ff;");
    v->addWidget(city); v->addWidget(m_temp); v->addWidget(m_desc); v->addStretch(1);
    charger();
    auto *tm = new QTimer(this); connect(tm, &QTimer::timeout, this, &MeteoCard::charger); tm->start(15 * 60 * 1000);
}

void MeteoCard::charger()
{
    QNetworkReply *r = m_net->get(QNetworkRequest(QUrl(
        "https://api.open-meteo.com/v1/forecast?latitude=36.8&longitude=10.18&current=temperature_2m,weather_code,wind_speed_10m")));
    connect(r, &QNetworkReply::finished, this, [this, r] {
        r->deleteLater();
        if (r->error() != QNetworkReply::NoError) { m_temp->setText("—"); m_desc->setText("Météo indisponible"); return; }
        const QJsonObject c = QJsonDocument::fromJson(r->readAll()).object()["current"].toObject();
        const int code = c["weather_code"].toInt();
        QString d = code == 0 ? "☀ Ciel dégagé" : code < 4 ? "⛅ Nuageux" : code < 50 ? "🌫 Brouillard" : code < 70 ? "🌧 Pluie"
                  : code < 80 ? "❄ Neige" : code < 90 ? "🌦 Averses" : "⛈ Orage";
        m_temp->setText(QString::number(c["temperature_2m"].toDouble(), 'f', 0) + " °C");
        m_desc->setText(QString("%1 · vent %2 km/h").arg(d).arg(c["wind_speed_10m"].toDouble(), 0, 'f', 0));
    });
}
