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


// ------------------------------------------------------------ Itinéraire (façon Google Maps)
namespace {
const int X_LIGNE = 34;        // axe vertical du trait
const int H_DEPART = 46;       // hauteur de la ligne « départ »
const int H_TRAJET = 54;       // longueur d'un trajet entre deux repères
const int H_ARRET  = 98;       // hauteur d'un arrêt (texte)
const int MARGE    = 14;
}

ItineraireWidget::ItineraireWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumWidth(300);
    definir(QVector<EtapeItineraire>(), QString(), QString());
}

void ItineraireWidget::definir(const QVector<EtapeItineraire> &etapes, const QString &depart, const QString &fin)
{
    m_etapes = etapes;
    m_depart = depart;
    m_fin = fin;
    setMinimumHeight(MARGE * 2 + H_DEPART + m_etapes.size() * (H_TRAJET + H_ARRET) + H_TRAJET + 50);
    update();
}

void ItineraireWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), Qt::white);
    if (m_etapes.isEmpty())
        return;

    const QColor bleu("#2563eb");
    const int xTexte = X_LIGNE + 30;
    const int largeurTexte = qMax(80, width() - xTexte - MARGE);

    // ---- positions verticales (centre de chaque repère) : départ, puis (trajet + arrêt) répétés
    QVector<int> yCentre;
    int ligne = MARGE;
    yCentre.push_back(ligne + 14);
    ligne += H_DEPART;
    for (int i = 0; i < m_etapes.size(); ++i) {
        ligne += H_TRAJET;
        yCentre.push_back(ligne + 14);
        ligne += H_ARRET;
    }

    // ---- trait de l'itinéraire (derrière les repères)
    p.setPen(QPen(bleu, 5, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(X_LIGNE, yCentre.first(), X_LIGNE, yCentre.last() + H_ARRET - 30);
    p.setPen(QPen(QColor("#9ca3af"), 4, Qt::DotLine, Qt::RoundCap));
    p.drawLine(X_LIGNE, yCentre.last() + H_ARRET - 30, X_LIGNE, yCentre.last() + H_ARRET - 30 + 22);

    // ---- départ : cercle creux
    p.setPen(QPen(bleu, 4));
    p.setBrush(Qt::white);
    p.drawEllipse(QPointF(X_LIGNE, yCentre[0]), 10, 10);
    p.setPen(COULEUR_TEXTE);
    p.setFont(police(14, true));
    p.drawText(QRectF(xTexte, yCentre[0] - 12, largeurTexte, 20), Qt::AlignLeft | Qt::AlignVCenter,
               QObject::tr("Caserne – départ"));
    p.setPen(COULEUR_AXE);
    p.setFont(police(12));
    p.drawText(QRectF(xTexte, yCentre[0] + 8, largeurTexte, 18), Qt::AlignLeft | Qt::AlignVCenter, m_depart);

    // ---- étapes
    for (int i = 0; i < m_etapes.size(); ++i) {
        const EtapeItineraire &e = m_etapes[i];
        const int cy = yCentre[i + 1];

        // durée du trajet, à côté du trait (pastille grise)
        const QString duree = QObject::tr("%1 min").arg(e.trajetMin);
        p.setFont(police(12, true));
        const int wDuree = QFontMetrics(p.font()).horizontalAdvance(duree) + 18;
        const QRectF pastille(xTexte, cy - 14 - H_TRAJET / 2 - 11, wDuree, 22);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor("#eef2ff"));
        p.drawRoundedRect(pastille, 11, 11);
        p.setPen(bleu);
        p.drawText(pastille, Qt::AlignCenter, duree);
        p.setPen(COULEUR_AXE);
        p.setFont(police(11));
        p.drawText(QRectF(pastille.right() + 8, pastille.top(), largeurTexte - wDuree - 8, 22),
                   Qt::AlignLeft | Qt::AlignVCenter, QObject::tr("de trajet"));

        // épingle numérotée
        p.setPen(QPen(Qt::white, 2));
        p.setBrush(e.couleur);
        p.drawEllipse(QPointF(X_LIGNE, cy), 15, 15);
        p.setPen(Qt::white);
        p.setFont(police(14, true));
        p.drawText(QRectF(X_LIGNE - 15, cy - 15, 30, 30), Qt::AlignCenter, QString::number(e.ordre));

        // texte de l'arrêt
        QFontMetrics fm(police(14, true));
        p.setPen(COULEUR_TEXTE);
        p.setFont(police(14, true));
        p.drawText(QRectF(xTexte, cy - 14, largeurTexte, 20), Qt::AlignLeft | Qt::AlignVCenter,
                   fm.elidedText(QString("#%1 · %2").arg(e.idIntervention).arg(e.titre), Qt::ElideRight, largeurTexte));
        p.setFont(police(12));
        QFontMetrics fm2(police(12));
        p.setPen(COULEUR_TEXTE);
        p.drawText(QRectF(xTexte, cy + 6, largeurTexte, 18), Qt::AlignLeft | Qt::AlignVCenter,
                   fm2.elidedText(e.adresse, Qt::ElideRight, largeurTexte));
        p.setPen(COULEUR_AXE);
        p.drawText(QRectF(xTexte, cy + 24, largeurTexte, 18), Qt::AlignLeft | Qt::AlignVCenter,
                   fm2.elidedText(e.zone + "  ·  " + QObject::tr("gravité %1").arg(e.gravite), Qt::ElideRight, largeurTexte));
        p.setFont(police(12, true));
        p.setPen(COULEUR_TEXTE);
        p.drawText(QRectF(xTexte, cy + 43, largeurTexte, 18), Qt::AlignLeft | Qt::AlignVCenter,
                   QObject::tr("%1 → %2  (%3 min sur place)").arg(e.arrivee, e.fin).arg(e.surPlaceMin));
        p.setPen(e.retard ? QColor("#b91c1c") : QColor("#15803d"));
        p.drawText(QRectF(xTexte, cy + 61, largeurTexte, 18), Qt::AlignLeft | Qt::AlignVCenter,
                   (e.retard ? QString::fromUtf8("\xE2\x9A\xA0 ") : QString::fromUtf8("\xE2\x9C\x94 ")) + e.texteRetard);
    }

    // ---- fin de mission : petit drapeau (cercle gris)
    const int yDrap = yCentre.last() + H_ARRET - 30 + 22 + 8;
    p.setPen(QPen(QColor("#6b7280"), 3));
    p.setBrush(Qt::white);
    p.drawEllipse(QPointF(X_LIGNE, yDrap), 8, 8);
    p.setPen(COULEUR_TEXTE);
    p.setFont(police(13, true));
    p.drawText(QRectF(xTexte, yDrap - 10, largeurTexte, 20), Qt::AlignLeft | Qt::AlignVCenter,
               QObject::tr("Fin du parcours – %1").arg(m_fin));
}
