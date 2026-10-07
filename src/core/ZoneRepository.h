#pragma once
#include "Zone.h"
#include <QObject>
#include <QVector>

// Couche données : CRUD + persistance JSON. Aucune dépendance vers l'interface.
class ZoneRepository : public QObject {
    Q_OBJECT
public:
    explicit ZoneRepository(QObject* parent = nullptr);

    const QVector<Zone>& zones() const { return m_zones; }
    const Zone* find(int id) const;

    int  add(Zone z);                  // Create  -> retourne le nouvel ID
    bool update(const Zone& z);        // Update
    bool remove(int id);               // Delete
    bool recordIntervention(int id, double minutes);  // met à jour moyenne + compteur

    bool load(const QString& path);
    bool save(const QString& path) const;
    void loadSamples();
    static QString defaultPath();

signals:
    void changed();

private:
    QVector<Zone> m_zones;
    int m_nextId = 1;
};
