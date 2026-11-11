#include "graphiques.h"

#include <QFontMetrics>
#include <QPaintEvent>
#include <QtMath>
#include <cmath>

namespace {

const QColor COULEUR_TEXTE("#1f2937");
const QColor COULEUR_GRILLE("#e5e7eb");
const QColor COULEUR_AXE("#9ca3af");

QFont police(int px, bool gras = false)
{
    QFont f("Segoe UI");
    f.setPixelSize(px);
    f.setBold(gras);
    return f;
}

double somme(const QVector<DonneeGraphique> &d)
{
    double t = 0;
    for (int i = 0; i < d.size(); ++i) t += d[i].valeur;
    return t;
}

// Pas "joli" pour l'axe vertical (1, 2, 5, 10, 20...) pour avoir environ 5 graduations
int pasGraduation(double maximum)
{
    if (maximum <= 5) return 1;
    const double brut = maximum / 5.0;
    const double puiss = std::pow(10.0, std::floor(std::log10(brut)));
    const double r = brut / puiss;
    double pas = (r <= 1) ? 1 : (r <= 2) ? 2 : (r <= 5) ? 5 : 10;
    return qMax(1, int(pas * puiss));
}

void titreGraphique(QPainter &p, const QRectF &zone, const QString &titre)
{
    p.setPen(COULEUR_TEXTE);
    p.setFont(police(15, true));
    p.drawText(QRectF(zone.left(), zone.top(), zone.width(), 26), Qt::AlignLeft | Qt::AlignVCenter, titre);
}

void aucuneDonnee(QPainter &p, const QRectF &zone)
{
    p.setPen(COULEUR_AXE);
    p.setFont(police(13));
    p.drawText(zone, Qt::AlignCenter, QObject::tr("Aucune donnée"));
}

} // namespace

// ================================================================ CAMEMBERT
void Graphiques::camembert(QPainter &p, const QRectF &zone, const QString &titre,
                           const QVector<DonneeGraphique> &donnees)
{
    p.save();
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::TextAntialiasing, true);
    titreGraphique(p, zone, titre);

    const QRectF corps(zone.left(), zone.top() + 34, zone.width(), zone.height() - 34);
    const double total = somme(donnees);
    if (total <= 0) {
        aucuneDonnee(p, corps);
        p.restore();
        return;
    }

    const double diametre = qMin(corps.height() - 10, corps.width() * 0.55);
    const QRectF disque(corps.left() + 8, corps.top() + (corps.height() - diametre) / 2, diametre, diametre);

    int angle = 90 * 16;                       // on part du haut, sens horaire
    p.setPen(QPen(Qt::white, 2));
    for (int i = 0; i < donnees.size(); ++i) {
        if (donnees[i].valeur <= 0) continue;
        const int span = int(-donnees[i].valeur / total * 360 * 16);
        p.setBrush(donnees[i].couleur);
        p.drawPie(disque, angle, span);

        // pourcentage dans la part (si elle est assez grande)
        const double pct = donnees[i].valeur / total * 100.0;
        if (pct >= 8) {
            const double mid = qDegreesToRadians((angle + span / 2) / 16.0);
            const QPointF c = disque.center();
            const double r = diametre * 0.30;
            const QPointF pos(c.x() + r * std::cos(mid), c.y() - r * std::sin(mid));
            p.setPen(Qt::white);
            p.setFont(police(13, true));
            p.drawText(QRectF(pos.x() - 25, pos.y() - 10, 50, 20), Qt::AlignCenter,
                       QString::number(pct, 'f', 0) + "%");
            p.setPen(QPen(Qt::white, 2));
        }
        angle += span;
    }

    // légende à droite
    qreal y = corps.top() + qMax<qreal>(0, (corps.height() - donnees.size() * 26) / 2);
    const qreal x = disque.right() + 24;
    for (int i = 0; i < donnees.size(); ++i) {
        p.setPen(Qt::NoPen);
        p.setBrush(donnees[i].couleur);
        p.drawRoundedRect(QRectF(x, y + 4, 14, 14), 3, 3);
        p.setPen(COULEUR_TEXTE);
        p.setFont(police(13));
        p.drawText(QRectF(x + 22, y, zone.right() - x - 22, 22), Qt::AlignLeft | Qt::AlignVCenter,
                   QString("%1 : %2").arg(donnees[i].etiquette).arg(int(donnees[i].valeur)));
        y += 26;
    }
    p.restore();
}

// ============================================================== HISTOGRAMME
void Graphiques::histogramme(QPainter &p, const QRectF &zone, const QString &titre,
                             const QVector<DonneeGraphique> &donnees)
{
    p.save();
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::TextAntialiasing, true);
    titreGraphique(p, zone, titre);

    const QRectF corps(zone.left(), zone.top() + 34, zone.width(), zone.height() - 34);
    if (donnees.isEmpty() || somme(donnees) <= 0) {
        aucuneDonnee(p, corps);
        p.restore();
        return;
    }

    double maxi = 0;
    for (int i = 0; i < donnees.size(); ++i) maxi = qMax(maxi, donnees[i].valeur);
    const int pas = pasGraduation(maxi);
    const int sommet = (int(std::ceil(maxi / pas))) * pas;     // valeur max de l'axe

    const qreal margeGauche = 34, margeBas = 40, margeHaut = 18;
    const QRectF plan(corps.left() + margeGauche, corps.top() + margeHaut,
                      corps.width() - margeGauche - 6, corps.height() - margeHaut - margeBas);

    // grille + graduations
    p.setFont(police(11));
    for (int v = 0; v <= sommet; v += pas) {
        const qreal y = plan.bottom() - plan.height() * v / sommet;
        p.setPen(QPen(v == 0 ? COULEUR_AXE : COULEUR_GRILLE, 1));
        p.drawLine(QPointF(plan.left(), y), QPointF(plan.right(), y));
        p.setPen(COULEUR_AXE);
        p.drawText(QRectF(corps.left(), y - 9, margeGauche - 6, 18), Qt::AlignRight | Qt::AlignVCenter,
                   QString::number(v));
    }

    // barres
    const qreal tranche = plan.width() / donnees.size();
    const qreal largeur = qMin<qreal>(tranche * 0.62, 70);
    for (int i = 0; i < donnees.size(); ++i) {
        const qreal h = plan.height() * donnees[i].valeur / sommet;
        const qreal cx = plan.left() + tranche * (i + 0.5);
        const QRectF barre(cx - largeur / 2, plan.bottom() - h, largeur, h);
        p.setPen(Qt::NoPen);
        p.setBrush(donnees[i].couleur);
        if (h > 0) p.drawRoundedRect(barre, 4, 4);

        // valeur au-dessus
        p.setPen(COULEUR_TEXTE);
        p.setFont(police(13, true));
        p.drawText(QRectF(cx - tranche / 2, barre.top() - 20, tranche, 18), Qt::AlignCenter,
                   QString::number(int(donnees[i].valeur)));

        // étiquette sous la barre (retour à la ligne si besoin)
        p.setPen(COULEUR_TEXTE);
        p.setFont(police(11));
        p.drawText(QRectF(cx - tranche / 2 + 2, plan.bottom() + 6, tranche - 4, margeBas - 4),
                   Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, donnees[i].etiquette);
    }
    p.restore();
}

QImage Graphiques::versImage(int genre, const QSize &taille, const QString &titre,
                             const QVector<DonneeGraphique> &donnees)
{
    const int echelle = 2;                                  // image 2x pour rester nette dans le PDF
    QImage img(taille * echelle, QImage::Format_ARGB32);
    img.fill(Qt::white);
    QPainter p(&img);
    p.scale(echelle, echelle);
    const QRectF zone(8, 6, taille.width() - 16, taille.height() - 12);
    if (genre == GraphiqueWidget::Camembert)
        camembert(p, zone, titre, donnees);
    else
        histogramme(p, zone, titre, donnees);
    p.end();
    return img;
}

// ============================================================ GraphiqueWidget
GraphiqueWidget::GraphiqueWidget(Genre genre, const QString &titre,
                                 const QVector<DonneeGraphique> &donnees, QWidget *parent)
    : QWidget(parent), m_genre(genre), m_titre(titre), m_donnees(donnees)
{
    setMinimumSize(320, 190);   // taille réduite : les graphiques sont empilés dans le panneau de droite
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
}

void GraphiqueWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    const QRectF zone(12, 8, width() - 24, height() - 16);
    if (m_genre == Camembert)
        Graphiques::camembert(p, zone, m_titre, m_donnees);
    else
        Graphiques::histogramme(p, zone, m_titre, m_donnees);
}
