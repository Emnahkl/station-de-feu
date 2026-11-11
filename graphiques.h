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

#endif // GRAPHIQUES_H
