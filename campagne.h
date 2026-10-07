#ifndef CAMPAGNE_H
#define CAMPAGNE_H

#include <QDate>
#include <QList>
#include <QPair>
#include <QSqlQueryModel>
#include <QString>

// Résultat du ciblage automatique pour une zone
struct CiblageZone {
    int idZone = 0;
    QString nom;
    QString niveauRisque;
    int interventions12Mois = 0;
    int campagnes12Mois = 0;
    double score = 0.0;
    QString recommandation;
};

// Résultat de l'évaluation d'impact d'une campagne
struct ImpactCampagne {
    bool valide = false;
    int avant = 0;
    int apres = 0;
    double variation = 0.0;   // en %
    bool periodeComplete = true;
    QString verdict;
};

class Campagne
{
public:
    Campagne();
    Campagne(int id, const QString &titre, const QString &theme, const QDate &date,
             const QString &lieu, const QString &publicCible, int nbParticipants,
             int idZone, int idEmploye);

    // Getters / setters
    int getId() const { return m_id; }
    QString getTitre() const { return m_titre; }
    QString getTheme() const { return m_theme; }
    QDate getDate() const { return m_date; }
    QString getLieu() const { return m_lieu; }
    QString getPublicCible() const { return m_publicCible; }
    int getNbParticipants() const { return m_nbParticipants; }
    int getIdZone() const { return m_idZone; }
    int getIdEmploye() const { return m_idEmploye; }

    void setId(int v) { m_id = v; }
    void setTitre(const QString &v) { m_titre = v; }
    void setTheme(const QString &v) { m_theme = v; }
    void setDate(const QDate &v) { m_date = v; }
    void setLieu(const QString &v) { m_lieu = v; }
    void setPublicCible(const QString &v) { m_publicCible = v; }
    void setNbParticipants(int v) { m_nbParticipants = v; }
    void setIdZone(int v) { m_idZone = v; }
    void setIdEmploye(int v) { m_idEmploye = v; }

    // Contrôle de saisie : renvoie un message vide si tout est valide
    QString valider() const;

    // CRUD
    bool ajouter(QString *erreur = nullptr);
    bool modifier(QString *erreur = nullptr);
    static bool supprimer(int id, QString *erreur = nullptr);
    static bool existe(int id);
    static bool charger(int id, Campagne &c);
    static int prochainId();

    // Affichage avec recherche et tri (métiers basiques 1 et 2)
    // critere : "TITRE", "THEME" ou "PUBLIC_CIBLE" ; tri : clause ORDER BY
    static QSqlQueryModel *afficher(const QString &critere = QString(),
                                    const QString &texte = QString(),
                                    const QString &tri = "DATE_CAMPAGNE DESC");

    // Statistiques (métier basique 4) : par thème ou par zone
    static QList<QPair<QString, int>> statistiquesParTheme();
    static QList<QPair<QString, int>> statistiquesParZone();

    // Métier innovant 1 : ciblage automatique des zones les plus touchées
    static QList<CiblageZone> ciblageAutomatique(const QDate &reference = QDate::currentDate());

    // Métier innovant 2 : impact (interventions avant / après la campagne)
    static ImpactCampagne evaluerImpact(int idCampagne, int fenetreMois = 3);

private:
    int m_id = 0;
    QString m_titre;
    QString m_theme;
    QDate m_date;
    QString m_lieu;
    QString m_publicCible;
    int m_nbParticipants = 0;
    int m_idZone = 0;
    int m_idEmploye = 0;
};

#endif // CAMPAGNE_H
