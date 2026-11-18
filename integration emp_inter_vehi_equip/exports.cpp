#include "exports.h"
#include <QPdfWriter>
#include <QTextDocument>
#include <QFont>
#include <QPageSize>
#include <QPageLayout>
#include <QMarginsF>
#include <QFile>
#include <QTextStream>
#include <QDate>

// Construit un tableau HTML à partir du modèle affiché
QString Exports::htmlDepuisModele(QAbstractItemModel *model, const QString &titre)
{
    QString h = "<h2 align='center'>" + titre.toHtmlEscaped() + "</h2>"
                "<p align='center'>SafeStation - édité le " +
                QDate::currentDate().toString("dd/MM/yyyy") + "</p>"
                "<table border='1' cellspacing='0' cellpadding='4' width='100%'>"
                "<tr bgcolor='#C62828' style='color:white'>";
    for (int c = 0; c < model->columnCount(); ++c)
        h += "<th>" + model->headerData(c, Qt::Horizontal).toString().toHtmlEscaped() + "</th>";
    h += "</tr>";
    for (int r = 0; r < model->rowCount(); ++r) {
        h += "<tr>";
        for (int c = 0; c < model->columnCount(); ++c)
            h += "<td>" + model->data(model->index(r, c)).toString().toHtmlEscaped() + "</td>";
        h += "</tr>";
    }
    h += "</table>";
    return h;
}

bool Exports::ecrirePdf(const QString &fichier, const QString &html)
{
    QPdfWriter writer(fichier);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageMargins(QMarginsF(12, 12, 12, 12), QPageLayout::Millimeter);
    writer.setResolution(96);

    QTextDocument doc;
    QFont police = doc.defaultFont();
    police.setPointSize(8);                 // police plus petite = tableau lisible sur A4
    doc.setDefaultFont(police);
    doc.setHtml(html);
    doc.setPageSize(QSizeF(writer.width(), writer.height()));
    doc.print(&writer);
    return QFile::exists(fichier);
}

// CSV avec ';' et BOM UTF-8 : Excel l'ouvre directement avec les accents corrects
bool Exports::ecrireCsv(const QString &fichier, QAbstractItemModel *model)
{
    QFile f(fichier);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    f.write("\xEF\xBB\xBF");
    QTextStream out(&f);
    auto champ = [](QString s) { s.replace('"', "\"\""); return "\"" + s + "\""; };

    QStringList ligne;
    for (int c = 0; c < model->columnCount(); ++c)
        ligne << champ(model->headerData(c, Qt::Horizontal).toString());
    out << ligne.join(';') << "\n";
    for (int r = 0; r < model->rowCount(); ++r) {
        ligne.clear();
        for (int c = 0; c < model->columnCount(); ++c)
            ligne << champ(model->data(model->index(r, c)).toString());
        out << ligne.join(';') << "\n";
    }
    return true;
}
