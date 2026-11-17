#ifndef CARTEZONESWIDGET_H
#define CARTEZONESWIDGET_H

#include <QList>
#include <QWidget>

// Carte schématique du Grand Tunis (dessinée avec QPainter, sans module web) :
// chaque zone est un disque placé selon ses coordonnées, de taille proportionnelle
// à sa population et coloré selon le risque calculé à partir de l'historique des
// interventions. Les zones sous-couvertes (temps d'intervention > seuil) sont
// entourées d'un cercle pointillé et marquées d'un « ! ».
class CarteZonesWidget : public QWidget
{
    Q_OBJECT
public:
    struct Point {
        int id = 0;
        QString nom;
        double latitude = 0;
        double longitude = 0;
        int population = 0;
        int interventions = 0;
        int classeRisque = 0;   // 0 faible, 1 moyen, 2 élevé (calculé)
        bool sousCouverte = false;
    };

    explicit CarteZonesWidget(QWidget *parent = nullptr);
    void setPoints(const QList<Point> &points);
    void setSelection(int idZone);

signals:
    void zoneCliquee(int idZone);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPointF versEcran(double latitude, double longitude) const;
    double rayon(const Point &p) const;

    QList<Point> m_points;
    int m_selection = -1;
};

#endif // CARTEZONESWIDGET_H
