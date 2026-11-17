#ifndef ZONE_H
#define ZONE_H

#include <QList>
#include <QPair>
#include <QSqlQueryModel>
#include <QString>

// Zone de couverture de la caserne (table ZONE_COUVERTURE)
class Zone
{
public:
    Zone() = default;
    Zone(int id, const QString &nom, const QString &region, double superficie, int population,
         const QString &niveauRisque, int tempsIntervention, double latitude, double longitude);

    int getId() const { return m_id; }
    QString getNom() const { return m_nom; }
    QString getRegion() const { return m_region; }
    double getSuperficie() const { return m_superficie; }
    int getPopulation() const { return m_population; }
    QString getNiveauRisque() const { return m_niveauRisque; }
    int getTempsIntervention() const { return m_tempsIntervention; }
    double getLatitude() const { return m_latitude; }
    double getLongitude() const { return m_longitude; }

    void setId(int v) { m_id = v; }
    void setNom(const QString &v) { m_nom = v; }
    void setRegion(const QString &v) { m_region = v; }
    void setSuperficie(double v) { m_superficie = v; }
    void setPopulation(int v) { m_population = v; }
    void setNiveauRisque(const QString &v) { m_niveauRisque = v; }
    void setTempsIntervention(int v) { m_tempsIntervention = v; }
    void setLatitude(double v) { m_latitude = v; }
    void setLongitude(double v) { m_longitude = v; }

    // Contrôle de saisie : message vide si tout est valide
    QString valider() const;

    // CRUD
    bool ajouter(QString *erreur = nullptr);
    bool modifier(QString *erreur = nullptr);
    static bool supprimer(int id, QString *erreur = nullptr);
    static bool existe(int id);
    static bool charger(int id, Zone &z);
    static int prochainId();
    static QList<Zone> toutes();

    // Affichage avec recherche (NOM, REGION) et tri (risque, temps d'intervention...)
    static QSqlQueryModel *afficher(const QString &critere, const QString &texte, const QString &tri);

    // Statistiques
    static QList<QPair<QString, int>> interventionsParZone(int mois = 12);
    static QList<QPair<QString, int>> interventionsParNiveauRisque(int mois = 12);
    static int interventionsRecentes(int idZone, int mois = 12);

private:
    int m_id = 0;
    QString m_nom;
    QString m_region;
    double m_superficie = 0.0;
    int m_population = 0;
    QString m_niveauRisque = "Moyen";
    int m_tempsIntervention = 0;
    double m_latitude = 36.80;
    double m_longitude = 10.18;
};

#endif // ZONE_H
