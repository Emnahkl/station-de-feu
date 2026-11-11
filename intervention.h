#ifndef INTERVENTION_H
#define INTERVENTION_H

#include <QDate>
#include <QDateTime>
#include <QMap>
#include <QPair>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QVector>

// Entité « Intervention » (table INTERVENTIONS) avec ses CRUDs.
class Intervention
{
public:
    Intervention();
    Intervention(int id, const QString &type, const QString &adresse,
                 const QDate &date, const QTime &heure,
                 const QString &gravite, const QString &statut, int idZone);

    // --- Getters ---
    int     getId() const      { return m_id; }
    QString getType() const    { return m_type; }
    QString getAdresse() const { return m_adresse; }
    QDate   getDate() const    { return m_date; }
    QTime   getHeure() const   { return m_heure; }
    QString getGravite() const { return m_gravite; }
    QString getStatut() const  { return m_statut; }
    int     getIdZone() const  { return m_idZone; }
    QDateTime dateHeure() const { return QDateTime(m_date, m_heure); }

    // --- Setters ---
    void setId(int v)                { m_id = v; }
    void setType(const QString &v)   { m_type = v; }
    void setAdresse(const QString &v){ m_adresse = v; }
    void setDate(const QDate &v)     { m_date = v; }
    void setHeure(const QTime &v)    { m_heure = v; }
    void setGravite(const QString &v){ m_gravite = v; }
    void setStatut(const QString &v) { m_statut = v; }
    void setIdZone(int v)            { m_idZone = v; }

    // --- Valeurs autorisées (une seule source pour l'interface et le contrôle de saisie) ---
    static QStringList types();
    static QStringList gravites();
    static QStringList statuts();
    static QDate dateMinimale() { return QDate(2026, 1, 1); }   // aucune intervention avant cette date

    // --- Contrôle de saisie (renvoient "" si valide, sinon le message d'erreur) ---
    static QString erreurType(const QString &type);
    static QString erreurAdresse(const QString &adresse);
    static QString erreurDateHeure(const QDate &date, const QTime &heure);
    static QString erreurGravite(const QString &gravite);
    static QString erreurStatut(const QString &statut);
    // Une intervention dont le jour est passé est forcément terminée
    static bool estPassee(const QDate &date) { return date < QDate::currentDate(); }
    // Met à « Terminée » (en base) toutes les interventions dont la date est dépassée
    static bool cloturerPassees();
    QStringList erreurs() const;             // toutes les erreurs de l'objet (vide = valide)
    int idDoublon() const;                   // n° d'une AUTRE intervention identique (même type, adresse,
                                             // date et heure) ; 0 s'il n'y en a pas

    // --- CRUD ---
    bool ajouter();                          // Create
    bool modifier();                         // Update (selon getId())
    static bool supprimer(int id);           // Delete
    // Read : liste filtrée (gravité / statut / texte de recherche), vide = pas de filtre
    static QVector<Intervention> afficher(const QString &gravite = QString(),
                                          const QString &statut = QString(),
                                          const QString &recherche = QString());

    // Zones disponibles (clé étrangère ID_ZONE) : paires (id, nom)
    static QVector<QPair<int, QString> > zones();
    // Temps d'intervention (minutes) de chaque zone : paires (id zone -> minutes), utilisé par le parcours
    static QMap<int, int> tempsZones();

    // Dernier message d'erreur Oracle (pour l'afficher à l'utilisateur)
    static QString derniereErreur();

private:
    int     m_id;
    QString m_type;
    QString m_adresse;
    QDate   m_date;
    QTime   m_heure;
    QString m_gravite;
    QString m_statut;
    int     m_idZone;
};

#endif // INTERVENTION_H
