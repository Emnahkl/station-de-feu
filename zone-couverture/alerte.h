#pragma once
// alerte.h : alertes en temps reel + meteo (cartes de la colonne de droite)
#include <QFrame>
#include <QLabel>
class QNetworkAccessManager;
class QVBoxLayout;
class ZoneStore;

class AlerteCard : public QFrame {
    Q_OBJECT
public:
    AlerteCard(ZoneStore *store, QWidget *parent = nullptr);
signals:
    void zoneClicked(const QString &id);
private:
    void rebuild();
    ZoneStore *m_store;
    QVBoxLayout *m_list;
};

class MeteoCard : public QFrame {
    Q_OBJECT
public:
    explicit MeteoCard(QWidget *parent = nullptr);
private:
    void charger();
    QNetworkAccessManager *m_net;
    QLabel *m_temp, *m_desc;
};
