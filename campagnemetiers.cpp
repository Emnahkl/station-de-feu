#include "campagnemetiers.h"
#include "gifwriter.h"

#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>
#include <QSqlQuery>
#include <QVariant>
#include <QtMath>
#include <algorithm>

namespace {

QString iso(const QDate &d)
{
    return d.toString("yyyy-MM-dd");
}

// "Élevé", "Eleve", "élevé"... -> 2.0 ; "Moyen" -> 1.5 ; sinon 1.0
double poidsRisque(const QString &risque)
{
    const QString r = risque.trimmed().toLower();
    if (r.startsWith(QString::fromUtf8("él")) || r.startsWith("el"))
        return 2.0;
    if (r.startsWith("moy"))
        return 1.5;
    return 1.0;
}

int compterInterventions(int idZone, const QDate &de, const QDate &a)
{
    QSqlQuery q;
    q.prepare("SELECT COUNT(*) FROM INTERVENTIONS WHERE ID_ZONE = :z "
              "AND DATE_INTERVENTION >= TO_DATE(:de,'YYYY-MM-DD') "
              "AND DATE_INTERVENTION < TO_DATE(:a,'YYYY-MM-DD')");
    q.bindValue(":z", idZone);
    q.bindValue(":de", iso(de));
    q.bindValue(":a", iso(a));
    return (q.exec() && q.next()) ? q.value(0).toInt() : 0;
}

// Avancement d'une etape de l'animation entre a et b (0..1, adouci)
double etape(double t, double a, double b)
{
    const double x = qBound(0.0, (t - a) / (b - a), 1.0);
    return x * x * (3 - 2 * x);
}

QColor avecAlpha(QColor c, double opacite)
{
    c.setAlphaF(qBound(0.0, opacite, 1.0) * c.alphaF());
    return c;
}

} // namespace

namespace CampagneMetiers {

// ---------------------------------------------------------------- Metier 1 : ciblage

QList<CiblageZone> ciblageAutomatique(const QDate &reference)
{
    const QString debut = iso(reference.addMonths(-12));
    const QString fin = iso(reference.addDays(1));

    QSqlQuery q;
    q.prepare("SELECT z.ID_ZONE, z.NOM, NVL(z.NIVEAU_RISQUE, 'Faible'), "
              " (SELECT COUNT(*) FROM INTERVENTIONS i WHERE i.ID_ZONE = z.ID_ZONE "
              "   AND i.DATE_INTERVENTION >= TO_DATE(:d1,'YYYY-MM-DD') "
              "   AND i.DATE_INTERVENTION < TO_DATE(:f1,'YYYY-MM-DD')), "
              " (SELECT COUNT(*) FROM CAMPAGNES_SENSIBILISATION c WHERE c.id_zone = z.ID_ZONE "
              "   AND c.date_campagne >= TO_DATE(:d2,'YYYY-MM-DD') "
              "   AND c.date_campagne < TO_DATE(:f2,'YYYY-MM-DD')) "
              "FROM ZONE_COUVERTURE z");
    q.bindValue(":d1", debut);
    q.bindValue(":f1", fin);
    q.bindValue(":d2", debut);
    q.bindValue(":f2", fin);

    QList<CiblageZone> res;
    if (!q.exec())
        return res;

    while (q.next()) {
        CiblageZone z;
        z.idZone = q.value(0).toInt();
        z.nom = q.value(1).toString();
        z.niveauRisque = q.value(2).toString();
        z.interventions12Mois = q.value(3).toInt();
        z.campagnes12Mois = q.value(4).toInt();
        z.score = z.interventions12Mois * poidsRisque(z.niveauRisque) / (1.0 + z.campagnes12Mois);
        res.append(z);
    }

    std::sort(res.begin(), res.end(), [](const CiblageZone &a, const CiblageZone &b) {
        if (a.score != b.score)
            return a.score > b.score;
        return poidsRisque(a.niveauRisque) > poidsRisque(b.niveauRisque);
    });

    const double max = res.isEmpty() ? 0.0 : res.first().score;
    for (CiblageZone &z : res) {
        if (max > 0 && z.score >= 0.66 * max) {
            z.priorite = 0;
            z.recommandation = QString::fromUtf8("Priorité haute : planifier une campagne");
        } else if (max > 0 && z.score >= 0.33 * max) {
            z.priorite = 1;
            z.recommandation = QString::fromUtf8("Priorité moyenne : à programmer");
        } else if (max <= 0 && z.campagnes12Mois == 0 && poidsRisque(z.niveauRisque) >= 2.0) {
            z.priorite = 1;
            z.recommandation = QString::fromUtf8("Risque élevé sans campagne récente : à programmer");
        } else {
            z.priorite = 2;
            z.recommandation = QString::fromUtf8("Couverture suffisante");
        }
    }
    return res;
}

// ---------------------------------------------------------------- Metier 2 : impact

ImpactCampagne evaluerImpact(int idCampagne, int fenetreMois)
{
    ImpactCampagne r;
    if (fenetreMois <= 0)
        return r;

    QSqlQuery q;
    q.prepare("SELECT TO_CHAR(c.date_campagne,'YYYY-MM-DD'), c.id_zone, z.NOM "
              "FROM CAMPAGNES_SENSIBILISATION c LEFT JOIN ZONE_COUVERTURE z ON z.ID_ZONE = c.id_zone "
              "WHERE c.id_campagne = :id");
    q.bindValue(":id", idCampagne);
    if (!q.exec() || !q.next())
        return r;

    const QDate d = QDate::fromString(q.value(0).toString(), "yyyy-MM-dd");
    const int idZone = q.value(1).toInt();
    r.zone = q.value(2).toString();
    if (!d.isValid() || idZone <= 0)
        return r;

    const QDate apresFin = d.addMonths(fenetreMois);
    r.periodeComplete = apresFin <= QDate::currentDate();
    r.avant = compterInterventions(idZone, d.addMonths(-fenetreMois), d);
    r.apres = compterInterventions(idZone, d.addDays(1), apresFin.addDays(1));
    r.valide = true;

    if (r.avant == 0) {
        r.variation = 0.0;
        r.verdict = r.apres == 0
            ? QString::fromUtf8("Aucune intervention avant ni après : impact non mesurable.")
            : QString::fromUtf8("Pas d'interventions avant la campagne : impact non mesurable.");
    } else {
        r.variation = 100.0 * (r.apres - r.avant) / r.avant;
        if (r.variation <= -20)
            r.verdict = QString::fromUtf8("Impact positif : baisse nette des interventions dans la zone.");
        else if (r.variation < 0)
            r.verdict = QString::fromUtf8("Impact modéré : légère baisse des interventions.");
        else if (r.variation == 0)
            r.verdict = QString::fromUtf8("Pas d'effet visible sur les interventions.");
        else
            r.verdict = QString::fromUtf8("Aucune baisse : renforcer ou recibler la campagne.");
    }
    if (!r.periodeComplete)
        r.verdict += QString::fromUtf8(" (période après campagne encore en cours)");
    return r;
}

// ---------------------------------------------------------------- Service : affiche

InfosAffiche chargerInfos(int idCampagne)
{
    InfosAffiche c;
    QSqlQuery q;
    q.prepare("SELECT c.titre, c.theme, TO_CHAR(c.date_campagne,'YYYY-MM-DD'), c.lieu, "
              "c.public_cible, z.NOM, "
              "(SELECT LISTAGG(e.nom || ' ' || e.prenom, ', ') WITHIN GROUP (ORDER BY e.nom) "
              "   FROM ANIMER a JOIN EMPLOYE e ON e.id_employe = a.id_employe "
              "  WHERE a.id_campagne = c.id_campagne) "
              "FROM CAMPAGNES_SENSIBILISATION c LEFT JOIN ZONE_COUVERTURE z ON z.ID_ZONE = c.id_zone "
              "WHERE c.id_campagne = :id");
    q.bindValue(":id", idCampagne);
    if (!q.exec() || !q.next())
        return c;
    c.valide = true;
    c.id = idCampagne;
    c.titre = q.value(0).toString();
    c.theme = q.value(1).toString();
    c.date = QDate::fromString(q.value(2).toString(), "yyyy-MM-dd");
    c.lieu = q.value(3).toString();
    c.publicCible = q.value(4).toString();
    c.zone = q.value(5).toString();
    c.animateurs = q.value(6).toString();
    return c;
}

QString sloganPourTheme(const QString &theme)
{
    const QString t = theme.toLower();
    if (t.contains("foret") || t.contains(QString::fromUtf8("forêt")))
        return QString::fromUtf8("Une étincelle suffit. Protégeons nos forêts !");
    if (t.contains("secours"))
        return QString::fromUtf8("Les bons gestes sauvent des vies.");
    if (t.contains("domestique"))
        return QString::fromUtf8("À la maison, la prudence commence par vous.");
    if (t.contains("evacuation") || t.contains(QString::fromUtf8("évacuation")))
        return QString::fromUtf8("Connaître la sortie, c'est sauver sa vie.");
    if (t.contains("electri") || t.contains(QString::fromUtf8("électri")))
        return QString::fromUtf8("Une prise surchargée, c'est un incendie annoncé.");
    if (t.contains("intemp"))
        return QString::fromUtf8("Préparés aujourd'hui, protégés demain.");
    return QString::fromUtf8("Le feu ne prévient pas. Vous, si !");
}

QImage dessinerAffiche(const InfosAffiche &c, const QString &sloganSaisi, double t, double phase)
{
    const QString slogan = sloganSaisi.trimmed().isEmpty() ? sloganPourTheme(c.theme)
                                                           : sloganSaisi.trimmed();
    const int W = 900, H = 1200;
    QImage img(W, H, QImage::Format_ARGB32_Premultiplied);
    QPainter p(&img);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);

    // Fond : degrade rouge pompier -> anthracite
    QLinearGradient g(0, 0, 0, H);
    g.setColorAt(0.0, QColor("#C62828"));
    g.setColorAt(0.65, QColor("#8E0000"));
    g.setColorAt(1.0, QColor("#2B2B2B"));
    p.fillRect(img.rect(), g);

    // Flammes : montent au debut puis vacillent (couleur selon le public)
    QColor accent("#FF8F00");
    const QString pub = c.publicCible.toLower();
    if (pub.contains("eleve") || pub.contains(QString::fromUtf8("élève")) || pub.contains("etudiant"))
        accent = QColor("#FFB300");
    else if (pub.contains("employe") || pub.contains("entreprise"))
        accent = QColor("#FF6F00");
    const double montee = etape(t, 0.0, 0.3);
    p.setPen(Qt::NoPen);
    for (int i = 0; i < 9; ++i) {
        const int x = 40 + i * 100, base = H - 220;
        const double vacille = 1.0 + 0.12 * qSin(phase * 2 * M_PI + i * 1.7);
        const double h = (120 + ((i * 37) % 90)) * montee * vacille;
        const double balance = 12 * qSin(phase * 2 * M_PI + i);
        QPainterPath flamme;
        flamme.moveTo(x, base);
        flamme.cubicTo(x - 30, base - h / 2, x + 20 + balance, base - h * 0.8, x + 10 + balance, base - h);
        flamme.cubicTo(x + 45 + balance, base - h * 0.7, x + 70, base - h / 3, x + 60, base);
        flamme.closeSubpath();
        QColor col = accent;
        col.setAlpha(110 + (i % 3) * 40);
        p.setBrush(col);
        p.drawPath(flamme);
    }

    // Logo circulaire (apparait en grossissant)
    const double logo = etape(t, 0.1, 0.3);
    if (logo > 0) {
        p.save();
        p.translate(W / 2, 150);
        p.scale(logo, logo);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor("#FFFFFF"));
        p.drawEllipse(QPoint(0, 0), 90, 90);
        p.setPen(QPen(QColor("#C62828"), 8));
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(QPoint(0, 0), 80, 80);
        QFont fl("Arial", 22, QFont::Bold);
        p.setFont(fl);
        p.setPen(QColor("#C62828"));
        p.drawText(QRect(-80, -40, 160, 80), Qt::AlignCenter, "FIRE\nSTATION");
        p.restore();
    }

    QFont f("Arial", 26, QFont::Bold);

    // Theme (fondu)
    p.setPen(avecAlpha(QColor("#FFFFFF"), etape(t, 0.25, 0.4)));
    p.setFont(f);
    p.drawText(QRect(40, 270, W - 80, 50), Qt::AlignCenter, c.theme.toUpper());

    // Titre (glisse du haut en apparaissant)
    const double titre = etape(t, 0.3, 0.5);
    f.setPointSize(54);
    p.setFont(f);
    p.setPen(avecAlpha(QColor("#FFFFFF"), titre));
    p.drawText(QRect(40, 330 - int(60 * (1 - titre)), W - 80, 220),
               Qt::AlignCenter | Qt::TextWordWrap, c.titre);

    // Slogan (machine a ecrire)
    const double ecrit = etape(t, 0.5, 0.75);
    const QString texteSlogan = QString::fromUtf8("« ") + slogan + QString::fromUtf8(" »");
    const int nbCar = int(std::round(texteSlogan.size() * ecrit));
    f.setPointSize(30);
    f.setItalic(true);
    p.setFont(f);
    p.setPen(QColor("#FFE0B2"));
    if (nbCar > 0)
        p.drawText(QRect(60, 560, W - 120, 140), Qt::AlignCenter | Qt::TextWordWrap,
                   texteSlogan.left(nbCar));

    // Bandeau d'informations (monte du bas)
    const double bandeau = etape(t, 0.7, 0.9);
    if (bandeau > 0) {
        const int dy = int(300 * (1 - bandeau));
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(255, 255, 255, int(235 * bandeau)));
        p.drawRoundedRect(QRect(70, 730 + dy, W - 140, 200), 18, 18);
        f.setItalic(false);
        f.setPointSize(c.animateurs.isEmpty() ? 24 : 21);
        p.setFont(f);
        p.setPen(avecAlpha(QColor("#2B2B2B"), bandeau));
        QString infos = QString::fromUtf8("Date : %1\nLieu : %2\nPublic : %3%4")
                            .arg(c.date.toString("dd/MM/yyyy"), c.lieu, c.publicCible,
                                 c.zone.isEmpty() ? QString() : QString::fromUtf8(" — Zone : ") + c.zone);
        if (!c.animateurs.isEmpty())
            infos += QString::fromUtf8("\nAnimé par : ") + c.animateurs;
        p.drawText(QRect(100, 745 + dy, W - 200, 170),
                   Qt::AlignVCenter | Qt::AlignLeft | Qt::TextWordWrap, infos);
    }

    // Pied de page (fondu, puis le numero d'urgence clignote)
    const double pied = etape(t, 0.85, 1.0);
    if (pied > 0) {
        f.setPointSize(20);
        p.setFont(f);
        p.setPen(avecAlpha(QColor("#FFFFFF"), pied));
        p.drawText(QRect(0, H - 90, W, 40), Qt::AlignCenter,
                   QString::fromUtf8("Protection civile — Ensemble pour sauver des vies"));
        const bool allume = t < 1.0 || qSin(phase * 2 * M_PI * 2) > -0.3;
        f.setPointSize(34);
        p.setFont(f);
        p.setPen(avecAlpha(QColor(allume ? "#FFEB3B" : "#FFB300"), pied));
        p.drawText(QRect(0, H - 150, W, 60), Qt::AlignCenter, QString::fromUtf8("URGENCE : 198"));
    }
    p.end();
    return img;
}

QList<QImage> imagesAnimation(const InfosAffiche &c, const QString &slogan, int nbImages)
{
    QList<QImage> images;
    const int apparition = qMax(2, nbImages * 2 / 3);   // 2/3 : construction de l'affiche
    const int boucle = qMax(1, nbImages - apparition);  // 1/3 : flammes qui dansent
    for (int i = 0; i < apparition; ++i)
        images.append(dessinerAffiche(c, slogan, double(i) / (apparition - 1), double(i) / boucle));
    for (int i = 0; i < boucle; ++i)
        images.append(dessinerAffiche(c, slogan, 1.0, double(i) / boucle));
    return images;
}

bool exporterGif(const InfosAffiche &c, const QString &slogan, const QString &chemin,
                 int largeur, int delaiCentiemes, QString *erreur)
{
    GifWriter gif;
    if (!gif.ouvrir(chemin, largeur, largeur * 4 / 3)) {
        if (erreur)
            *erreur = gif.erreur();
        return false;
    }
    const QList<QImage> images = imagesAnimation(c, slogan);
    for (int i = 0; i < images.size(); ++i) {
        // Pause plus longue sur l'affiche complete et a la fin de la boucle
        int delai = delaiCentiemes;
        if (i == images.size() * 2 / 3 - 1)
            delai = 150;
        if (!gif.ajouterImage(images.at(i), delai)) {
            if (erreur)
                *erreur = gif.erreur();
            return false;
        }
    }
    return gif.fermer();
}

} // namespace CampagneMetiers
