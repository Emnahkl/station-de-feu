#ifndef CAMPAGNEMETIERS_H
#define CAMPAGNEMETIERS_H

// Metiers avances du module « Campagnes de sensibilisation » (Faouzene)
// Repris de la branche gestion-campagnes-sensibilisation et adaptes a la base SAFESTATION :
//   1. Ciblage automatique des zones a sensibiliser en priorite
//   2. Evaluation d'impact d'une campagne (interventions avant / apres)
//   3. Generateur d'affiche (PNG) et d'affiche animee (GIF)

#include <QDate>
#include <QImage>
#include <QList>
#include <QString>

struct CiblageZone {
    int idZone = 0;
    QString nom;
    QString niveauRisque;
    int interventions12Mois = 0;
    int campagnes12Mois = 0;
    double score = 0.0;
    QString recommandation;
    int priorite = 0;          // 0 haute, 1 moyenne, 2 couverture suffisante
};

struct ImpactCampagne {
    bool valide = false;
    int avant = 0;
    int apres = 0;
    double variation = 0.0;    // en %
    bool periodeComplete = true;
    QString zone;
    QString verdict;
};

struct InfosAffiche {
    bool valide = false;
    int id = 0;
    QString titre, theme, lieu, publicCible, zone, animateurs;
    QDate date;
};

namespace CampagneMetiers {

// Score = interventions sur 12 mois x poids du risque / (1 + campagnes deja faites sur 12 mois)
QList<CiblageZone> ciblageAutomatique(const QDate &reference = QDate::currentDate());

ImpactCampagne evaluerImpact(int idCampagne, int fenetreMois = 3);

InfosAffiche chargerInfos(int idCampagne);
QString sloganPourTheme(const QString &theme);

// Dessine l'affiche. progression : 0 = debut de l'animation, 1 = affiche complete.
// phase : fait vaciller les flammes (animation en boucle).
QImage dessinerAffiche(const InfosAffiche &c, const QString &slogan,
                       double progression = 1.0, double phase = 0.0);

// Images de l'animation (apparition progressive puis flammes qui dansent)
QList<QImage> imagesAnimation(const InfosAffiche &c, const QString &slogan, int nbImages = 36);

// Enregistre l'affiche animee. largeur : taille du GIF (hauteur = largeur x 4/3)
bool exporterGif(const InfosAffiche &c, const QString &slogan, const QString &chemin,
                 int largeur = 450, int delaiCentiemes = 8, QString *erreur = nullptr);

} // namespace CampagneMetiers

#endif // CAMPAGNEMETIERS_H
