#ifndef GRAPHIQUES_H
#define GRAPHIQUES_H

#include <QColor>
#include <QImage>
#include <QPainter>
#include <QRectF>
#include <QString>
#include <QVector>
#include <QWidget>

// Une barre / une part de camembert
struct DonneeGraphique {
    QString  etiquette;
    double   valeur;
    QColor   couleur;
};

// Dessin des graphiques avec QPainter (aucun module supplémentaire à installer).
// Les mêmes fonctions servent pour l'écran (fenêtre Statistiques) et pour le PDF.
namespace Graphiques {

void camembert(QPainter &p, const QRectF &zone, const QString &titre,
               const QVector<DonneeGraphique> &donnees);
void histogramme(QPainter &p, const QRectF &zone, const QString &titre,
                 const QVector<DonneeGraphique> &donnees);

// Rend un graphique dans une image (pour l'insérer dans le PDF). genre : 0 = camembert, 1 = histogramme
QImage versImage(int genre, const QSize &taille, const QString &titre,
                 const QVector<DonneeGraphique> &donnees);

} // namespace Graphiques

// Widget qui affiche un graphique à l'écran
class GraphiqueWidget : public QWidget
{
    Q_OBJECT
public:
    enum Genre { Camembert = 0, Histogramme = 1 };
    GraphiqueWidget(Genre genre, const QString &titre,
                    const QVector<DonneeGraphique> &donnees, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    Genre m_genre;
    QString m_titre;
    QVector<DonneeGraphique> m_donnees;
};

// Une étape de l'itinéraire (un « arrêt » du parcours)
struct EtapeItineraire {
    int     ordre;          // 1, 2, 3...
    int     idIntervention;
    QString titre;          // type d'intervention
    QString adresse;
    QString zone;
    QString gravite;
    QColor  couleur;        // couleur de la gravité
    QString arrivee;        // heure d'arrivée prévue
    QString fin;            // heure de fin prévue
    int     trajetMin;      // trajet depuis l'étape précédente (ou la caserne)
    int     surPlaceMin;
    bool    retard;
    QString texteRetard;    // « à l'heure » / « +12 min de retard »
};

// Itinéraire vertical façon Google Maps : départ, puis des repères reliés par un trait avec la durée
// de chaque trajet ; chaque arrêt est une épingle numérotée colorée selon la gravité.
class ItineraireWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ItineraireWidget(QWidget *parent = nullptr);
    void definir(const QVector<EtapeItineraire> &etapes, const QString &depart, const QString &fin);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    QVector<EtapeItineraire> m_etapes;
    QString m_depart, m_fin;
};

#endif // GRAPHIQUES_H
