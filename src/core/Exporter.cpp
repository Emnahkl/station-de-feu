#include "Exporter.h"
#include "CoverageAnalyzer.h"
#include <QDate>
#include <QFile>
#include <QLocale>
#include <QPageLayout>
#include <QPageSize>
#include <QPdfWriter>
#include <QTextDocument>

namespace {
QString csvEscape(QString s)
{
    s.replace('"', "\"\"");
    return '"' + s + '"';
}
}

namespace Exporter {

bool toCsv(const QString& path, const QVector<Zone>& zones)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;
    QLocale fr(QLocale::French);
    QString out;
    out += "ID zone;Nom;Région / quartier;Superficie (km²);Population;Niveau de risque;"
           "Temps d'intervention moyen (min);Interventions;Équipes;Véhicules;Latitude;Longitude\r\n";
    for (const Zone& z : zones) {
        out += z.idLabel() + ';' + csvEscape(z.nom) + ';' + csvEscape(z.region) + ';'
             + fr.toString(z.superficie, 'f', 1) + ';' + QString::number(z.population) + ';'
             + RiskUtil::label(z.risque) + ';' + fr.toString(z.tempsMoyen, 'f', 1) + ';'
             + QString::number(z.nbInterventions) + ';' + QString::number(z.nbEquipes) + ';'
             + QString::number(z.nbVehicules) + ';' + fr.toString(z.latitude, 'f', 5) + ';'
             + fr.toString(z.longitude, 'f', 5) + "\r\n";
    }
    f.write("\xEF\xBB\xBF");          // BOM UTF-8 : Excel détecte l'encodage
    f.write(out.toUtf8());
    return true;
}

bool toPdf(const QString& path, const QVector<Zone>& zones, double thresholdMin)
{
    QLocale fr(QLocale::French);
    QString html;
    html += "<h1 style='color:#B71C1C'>FIRE STATION — Zones de couverture</h1>";
    html += QString("<p>Rapport du %1 — %2 zone(s) — seuil d'alerte : %3 min</p>")
                .arg(QDate::currentDate().toString("dd/MM/yyyy")).arg(zones.size())
                .arg(fr.toString(thresholdMin, 'f', 1));
    html += QString("<p><b>Population couverte :</b> %1 &nbsp; <b>Temps moyen global :</b> %2 min &nbsp; "
                    "<b>Zones sous-couvertes :</b> %3</p>")
                .arg(fr.toString(CoverageAnalyzer::totalPopulation(zones)))
                .arg(fr.toString(CoverageAnalyzer::averageResponse(zones), 'f', 1))
                .arg(CoverageAnalyzer::underCoveredIds(zones, thresholdMin).size());
    html += "<table border='1' cellspacing='0' cellpadding='4' width='100%'>"
            "<tr bgcolor='#C62828' style='color:white'><th>ID</th><th>Nom</th><th>Région</th>"
            "<th>Superficie</th><th>Population</th><th>Risque</th><th>Temps moyen</th>"
            "<th>Interv.</th><th>Statut</th></tr>";
    for (const Zone& z : zones) {
        bool under = CoverageAnalyzer::isUnderCovered(z, thresholdMin);
        html += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td align='right'>%4 km²</td>"
                        "<td align='right'>%5</td><td style='color:%6'><b>%7</b></td>"
                        "<td align='right'>%8 min</td><td align='right'>%9</td>"
                        "<td style='color:%10'>%11</td></tr>")
                    .arg(z.idLabel(), z.nom.toHtmlEscaped(), z.region.toHtmlEscaped(),
                         fr.toString(z.superficie, 'f', 1), fr.toString(z.population),
                         RiskUtil::color(z.risque).name(), RiskUtil::label(z.risque),
                         fr.toString(z.tempsMoyen, 'f', 1), QString::number(z.nbInterventions),
                         under ? "#B91C1C" : "#2E7D32",
                         under ? "Sous-couverte" : "Couverte");
    }
    html += "</table>";

    QPdfWriter writer(path);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageOrientation(QPageLayout::Landscape);
    writer.setPageMargins(QMarginsF(12, 12, 12, 12), QPageLayout::Millimeter);
    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&writer);
    return QFile::exists(path);
}

}
