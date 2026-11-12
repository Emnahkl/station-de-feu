#include "mainwindow.h"

#include <QtWidgets>
#include <QtCharts>
#include <QPdfWriter>
#include <QTextDocument>
#include <QSqlQueryModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    construireInterface();
    chargerListes();
    rafraichir();
    on_stats_refresh();
}

void MainWindow::construireInterface()
{
    setWindowTitle("Fire Station Manager - Campagnes de sensibilisation");
    resize(1250, 720);

    QTabWidget *tabs = new QTabWidget(this);
    setCentralWidget(tabs);

    // ================= Onglet Gestion =================
    QWidget *gestion = new QWidget;
    QHBoxLayout *mainLayout = new QHBoxLayout(gestion);

    QGroupBox *formBox = new QGroupBox("Campagne de sensibilisation");
    QFormLayout *form = new QFormLayout(formBox);

    idSpin = new QSpinBox;
    idSpin->setRange(0, 999999);

    titreEdit = new QLineEdit;
    titreEdit->setMaxLength(100);
    titreEdit->setPlaceholderText("Ex : Prevention incendies de foret");
    titreEdit->setValidator(new QRegularExpressionValidator(
        QRegularExpression("^[A-Za-z\\x{00C0}-\\x{00FF}0-9 '\\-]{0,100}$"), this));

    themeCombo = new QComboBox;
    themeCombo->addItems({"-- Choisir --", "Incendie domestique", "Feux de foret",
                          "Premiers secours", "Evacuation", "Risques electriques",
                          "Intemperies"});

    descriptionEdit = new QPlainTextEdit;
    descriptionEdit->setPlaceholderText("Objectif et contenu de la campagne...");
    descriptionEdit->setFixedHeight(60);

    employeCombo = new QComboBox;
    zoneCombo = new QComboBox;

    dateDebutEdit = new QDateEdit(QDate::currentDate());
    dateDebutEdit->setCalendarPopup(true);
    dateDebutEdit->setDisplayFormat("dd/MM/yyyy");
    dateFinEdit = new QDateEdit(QDate::currentDate().addDays(1));
    dateFinEdit->setCalendarPopup(true);
    dateFinEdit->setDisplayFormat("dd/MM/yyyy");

    publicCombo = new QComboBox;
    publicCombo->addItems({"-- Choisir --", "Eleves", "Etudiants", "Employes",
                           "Grand public", "Personnes agees"});

    nbSpin = new QSpinBox;
    nbSpin->setRange(0, 100000);

    budgetSpin = new QDoubleSpinBox;
    budgetSpin->setRange(0, 1000000);
    budgetSpin->setDecimals(2);
    budgetSpin->setSuffix(" DT");

    scoreSpin = new QDoubleSpinBox;
    scoreSpin->setRange(0, 100);
    scoreSpin->setDecimals(1);
    scoreSpin->setSuffix(" %");

    statutCombo = new QComboBox;
    statutCombo->addItems({"-- Choisir --", "Planifiee", "En cours", "Terminee", "Annulee"});

    form->addRow("ID :", idSpin);
    form->addRow("Titre :", titreEdit);
    form->addRow("Theme :", themeCombo);
    form->addRow("Public cible :", publicCombo);
    form->addRow("Description :", descriptionEdit);
    form->addRow("Date debut :", dateDebutEdit);
    form->addRow("Date fin :", dateFinEdit);
    form->addRow("Nb participants :", nbSpin);
    form->addRow("Budget :", budgetSpin);
    form->addRow("Statut :", statutCombo);
    form->addRow("Score impact :", scoreSpin);
    form->addRow("Organisateur :", employeCombo);
    form->addRow("Zone ciblee :", zoneCombo);

    QPushButton *btnAjouter = new QPushButton("Ajouter");
    QPushButton *btnModifier = new QPushButton("Modifier");
    QPushButton *btnSupprimer = new QPushButton("Supprimer");
    QPushButton *btnVider = new QPushButton("Vider");
    QGridLayout *btns = new QGridLayout;
    btns->addWidget(btnAjouter, 0, 0);
    btns->addWidget(btnModifier, 0, 1);
    btns->addWidget(btnSupprimer, 1, 0);
    btns->addWidget(btnVider, 1, 1);
    form->addRow(btns);
    formBox->setFixedWidth(380);

    // Partie droite : recherche, tri, tableau, PDF
    QVBoxLayout *right = new QVBoxLayout;

    QHBoxLayout *outils = new QHBoxLayout;
    critereCombo = new QComboBox;
    critereCombo->addItems({"Titre", "ID", "Theme", "Zone", "Organisateur", "Statut"});
    rechercheEdit = new QLineEdit;
    rechercheEdit->setPlaceholderText("Rechercher...");
    rechercheEdit->setClearButtonEnabled(true);

    triCombo = new QComboBox;
    triCombo->addItem("ID", "c.ID_CAMPAGNE");
    triCombo->addItem("Titre", "c.TITRE");
    triCombo->addItem("Date debut", "c.DATE_DEBUT");
    triCombo->addItem("Budget", "c.BUDGET");
    triCombo->addItem("Participants", "c.NB_PARTICIPANTS");
    triCombo->addItem("Score impact", "c.SCORE_IMPACT");
    ordreCombo = new QComboBox;
    ordreCombo->addItem("Croissant", "ASC");
    ordreCombo->addItem("Decroissant", "DESC");

    QPushButton *btnPdf = new QPushButton("Exporter PDF");

    outils->addWidget(new QLabel("Recherche par :"));
    outils->addWidget(critereCombo);
    outils->addWidget(rechercheEdit, 1);
    outils->addSpacing(15);
    outils->addWidget(new QLabel("Trier par :"));
    outils->addWidget(triCombo);
    outils->addWidget(ordreCombo);
    outils->addSpacing(15);
    outils->addWidget(btnPdf);

    table = new QTableView;
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);

    right->addLayout(outils);
    right->addWidget(table);

    mainLayout->addWidget(formBox);
    mainLayout->addLayout(right, 1);
    tabs->addTab(gestion, "Gestion des campagnes");

    // ================= Onglet Statistiques =================
    QWidget *stats = new QWidget;
    QVBoxLayout *statsLayout = new QVBoxLayout(stats);
    QHBoxLayout *statsTop = new QHBoxLayout;
    statsCombo = new QComboBox;
    statsCombo->addItem("Par theme", "THEME");
    statsCombo->addItem("Par statut", "STATUT");
    statsCombo->addItem("Par public cible", "PUBLIC_CIBLE");
    statsCombo->addItem("Par zone ciblee", "ZONE");
    QPushButton *btnStats = new QPushButton("Actualiser");
    statsTop->addWidget(new QLabel("Repartition des campagnes :"));
    statsTop->addWidget(statsCombo);
    statsTop->addWidget(btnStats);
    statsTop->addStretch();
    chartView = new QChartView;
    chartView->setRenderHint(QPainter::Antialiasing);
    statsLayout->addLayout(statsTop);
    statsLayout->addWidget(chartView, 1);
    tabs->addTab(stats, "Statistiques");

    // ================= Connexions =================
    connect(btnAjouter, &QPushButton::clicked, this, &MainWindow::on_ajouter_clicked);
    connect(btnModifier, &QPushButton::clicked, this, &MainWindow::on_modifier_clicked);
    connect(btnSupprimer, &QPushButton::clicked, this, &MainWindow::on_supprimer_clicked);
    connect(btnVider, &QPushButton::clicked, this, &MainWindow::on_vider_clicked);
    connect(btnPdf, &QPushButton::clicked, this, &MainWindow::on_exportPdf_clicked);
    connect(table, &QTableView::clicked, this, &MainWindow::on_table_clicked);
    connect(rechercheEdit, &QLineEdit::textChanged, this, &MainWindow::on_recherche_changed);
    connect(critereCombo, &QComboBox::currentIndexChanged, this, &MainWindow::on_recherche_changed);
    connect(triCombo, &QComboBox::currentIndexChanged, this, &MainWindow::on_recherche_changed);
    connect(ordreCombo, &QComboBox::currentIndexChanged, this, &MainWindow::on_recherche_changed);
    connect(btnStats, &QPushButton::clicked, this, &MainWindow::on_stats_refresh);
    connect(statsCombo, &QComboBox::currentIndexChanged, this, &MainWindow::on_stats_refresh);
    connect(tabs, &QTabWidget::currentChanged, this, [this](int i) {
        if (i == 1) on_stats_refresh();
    });
}

// ---------------------------------------------------------------------------

QString MainWindow::ordreCourant() const
{
    return triCombo->currentData().toString() + " " + ordreCombo->currentData().toString();
}

void MainWindow::rafraichir()
{
    QAbstractItemModel *ancien = table->model();
    if (rechercheEdit->text().trimmed().isEmpty())
        table->setModel(c.afficher(ordreCourant()));
    else
        table->setModel(c.rechercher(critereCombo->currentText(), rechercheEdit->text(),
                                     ordreCourant()));
    delete ancien;
    table->setColumnHidden(13, true);
    table->setColumnHidden(14, true);
    table->resizeColumnsToContents();
    table->setColumnWidth(4, 220);
}

void MainWindow::chargerListes()
{
    employeCombo->clear();
    employeCombo->addItem("-- Choisir --", 0);
    for (const auto &e : Campagne::listeEmployes())
        employeCombo->addItem(e.second, e.first);

    zoneCombo->clear();
    zoneCombo->addItem("-- Choisir --", 0);
    for (const auto &z : Campagne::listeZones())
        zoneCombo->addItem(z.second, z.first);
}

Campagne MainWindow::campagneDuFormulaire() const
{
    return Campagne(idSpin->value(), titreEdit->text().trimmed(), themeCombo->currentText(),
                    publicCombo->currentText(), descriptionEdit->toPlainText().trimmed(),
                    dateDebutEdit->date(), dateFinEdit->date(), nbSpin->value(),
                    budgetSpin->value(), statutCombo->currentText(), scoreSpin->value(),
                    employeCombo->currentData().toInt(), zoneCombo->currentData().toInt());
}

bool MainWindow::controleSaisie(bool ajout)
{
    QStringList erreurs;

    if (idSpin->value() <= 0)
        erreurs << "L'ID doit etre un entier strictement positif.";
    else if (ajout && c.existe(idSpin->value()))
        erreurs << "Cet ID existe deja.";
    else if (!ajout && !c.existe(idSpin->value()))
        erreurs << "Aucune campagne avec cet ID.";

    QString titre = titreEdit->text().trimmed();
    if (titre.length() < 3)
        erreurs << "Le titre doit contenir au moins 3 caracteres.";
    if (themeCombo->currentIndex() == 0)
        erreurs << "Choisissez un theme.";
    if (descriptionEdit->toPlainText().trimmed().length() < 10)
        erreurs << "La description doit contenir au moins 10 caracteres.";
    if (descriptionEdit->toPlainText().length() > 500)
        erreurs << "La description ne doit pas depasser 500 caracteres.";
    if (employeCombo->currentData().toInt() == 0)
        erreurs << "Choisissez l'employe organisateur.";
    if (zoneCombo->currentData().toInt() == 0)
        erreurs << "Choisissez la zone ciblee.";
    if (statutCombo->currentText() == "Planifiee" && scoreSpin->value() > 0)
        erreurs << "Une campagne planifiee ne peut pas encore avoir de score d'impact.";
    if (dateFinEdit->date() < dateDebutEdit->date())
        erreurs << "La date de fin doit etre apres la date de debut.";
    if (publicCombo->currentIndex() == 0)
        erreurs << "Choisissez un public cible.";
    if (nbSpin->value() <= 0)
        erreurs << "Le nombre de participants doit etre superieur a 0.";
    if (statutCombo->currentIndex() == 0)
        erreurs << "Choisissez un statut.";
    if (statutCombo->currentText() == "Planifiee" && dateDebutEdit->date() < QDate::currentDate())
        erreurs << "Une campagne planifiee ne peut pas commencer dans le passe.";

    if (!erreurs.isEmpty()) {
        QMessageBox::warning(this, "Controle de saisie", "- " + erreurs.join("\n- "));
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------

void MainWindow::on_ajouter_clicked()
{
    if (!controleSaisie(true))
        return;
    Campagne nouvelle = campagneDuFormulaire();
    if (nouvelle.ajouter()) {
        QMessageBox::information(this, "Ajout", "Campagne ajoutee avec succes.");
        rafraichir();
        on_vider_clicked();
    } else {
        QMessageBox::critical(this, "Ajout", "Echec de l'ajout.");
    }
}

void MainWindow::on_modifier_clicked()
{
    if (!controleSaisie(false))
        return;
    Campagne modif = campagneDuFormulaire();
    if (modif.modifier()) {
        QMessageBox::information(this, "Modification", "Campagne modifiee avec succes.");
        rafraichir();
    } else {
        QMessageBox::critical(this, "Modification", "Echec de la modification.");
    }
}

void MainWindow::on_supprimer_clicked()
{
    int id = idSpin->value();
    if (id <= 0 || !c.existe(id)) {
        QMessageBox::warning(this, "Suppression",
                             "Selectionnez une campagne existante dans le tableau.");
        return;
    }
    if (QMessageBox::question(this, "Suppression",
                              QString("Supprimer la campagne %1 ?").arg(id))
        != QMessageBox::Yes)
        return;

    if (c.supprimer(id)) {
        QMessageBox::information(this, "Suppression", "Campagne supprimee.");
        rafraichir();
        on_vider_clicked();
    } else {
        QMessageBox::critical(this, "Suppression", "Echec de la suppression.");
    }
}

void MainWindow::on_vider_clicked()
{
    idSpin->setValue(0);
    idSpin->setEnabled(true);
    titreEdit->clear();
    themeCombo->setCurrentIndex(0);
    descriptionEdit->clear();
    scoreSpin->setValue(0);
    employeCombo->setCurrentIndex(0);
    zoneCombo->setCurrentIndex(0);
    dateDebutEdit->setDate(QDate::currentDate());
    dateFinEdit->setDate(QDate::currentDate().addDays(1));
    publicCombo->setCurrentIndex(0);
    nbSpin->setValue(0);
    budgetSpin->setValue(0);
    statutCombo->setCurrentIndex(0);
    table->clearSelection();
}

void MainWindow::on_table_clicked(const QModelIndex &index)
{
    const QAbstractItemModel *m = table->model();
    int r = index.row();
    idSpin->setValue(m->index(r, 0).data().toInt());
    titreEdit->setText(m->index(r, 1).data().toString());
    themeCombo->setCurrentText(m->index(r, 2).data().toString());
    publicCombo->setCurrentText(m->index(r, 3).data().toString());
    descriptionEdit->setPlainText(m->index(r, 4).data().toString());
    dateDebutEdit->setDate(QDate::fromString(m->index(r, 5).data().toString(), "yyyy-MM-dd"));
    dateFinEdit->setDate(QDate::fromString(m->index(r, 6).data().toString(), "yyyy-MM-dd"));
    nbSpin->setValue(m->index(r, 7).data().toInt());
    budgetSpin->setValue(m->index(r, 8).data().toDouble());
    statutCombo->setCurrentText(m->index(r, 9).data().toString());
    scoreSpin->setValue(m->index(r, 10).data().toDouble());
    employeCombo->setCurrentIndex(employeCombo->findData(m->index(r, 13).data().toInt()));
    zoneCombo->setCurrentIndex(zoneCombo->findData(m->index(r, 14).data().toInt()));
}

void MainWindow::on_recherche_changed()
{
    rafraichir();
}

void MainWindow::on_exportPdf_clicked()
{
    QString fichier = QFileDialog::getSaveFileName(this, "Exporter en PDF",
                                                   "campagnes.pdf", "PDF (*.pdf)");
    if (fichier.isEmpty())
        return;
    if (!fichier.endsWith(".pdf", Qt::CaseInsensitive))
        fichier += ".pdf";

    const QAbstractItemModel *m = table->model();
    QString html;
    html += "<h2 style='color:#c0392b;text-align:center'>Liste des campagnes de sensibilisation</h2>";
    html += "<p style='text-align:center'>Fire Station Manager - genere le "
            + QDate::currentDate().toString("dd/MM/yyyy") + "</p>";
    html += "<table border='1' cellspacing='0' cellpadding='4' width='100%'><tr style='background:#c0392b;color:white'>";
    for (int col = 0; col < m->columnCount(); ++col)
        if (!table->isColumnHidden(col))
            html += "<th>" + m->headerData(col, Qt::Horizontal).toString().toHtmlEscaped() + "</th>";
    html += "</tr>";
    for (int row = 0; row < m->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < m->columnCount(); ++col)
            if (!table->isColumnHidden(col))
                html += "<td>" + m->index(row, col).data().toString().toHtmlEscaped() + "</td>";
        html += "</tr>";
    }
    html += "</table>";
    html += QString("<p>Nombre total de campagnes : <b>%1</b></p>").arg(m->rowCount());

    QPdfWriter writer(fichier);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageOrientation(QPageLayout::Landscape);
    writer.setResolution(96);
    writer.setPageMargins(QMarginsF(10, 10, 10, 10), QPageLayout::Millimeter);

    QTextDocument doc;
    doc.setHtml(html);
    doc.setPageSize(QSizeF(writer.width(), writer.height()));
    doc.print(&writer);

    QMessageBox::information(this, "Export PDF", "PDF genere :\n" + fichier);
    QDesktopServices::openUrl(QUrl::fromLocalFile(fichier));
}

void MainWindow::on_stats_refresh()
{
    QString colonne = statsCombo->currentData().toString();
    QMap<QString, int> data = c.statistiques(colonne);

    int total = 0;
    for (int v : data)
        total += v;

    QPieSeries *series = new QPieSeries;
    for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
        QPieSlice *slice = series->append(it.key(), it.value());
        double pct = total ? 100.0 * it.value() / total : 0;
        slice->setLabel(QString("%1 : %2 (%3%)").arg(it.key()).arg(it.value())
                            .arg(pct, 0, 'f', 1));
        slice->setLabelVisible(true);
    }

    QChart *chart = new QChart;
    chart->addSeries(series);
    chart->setTitle(QString("Campagnes %1 - total : %2")
                        .arg(statsCombo->currentText().toLower()).arg(total));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setAlignment(Qt::AlignRight);

    QChart *ancien = chartView->chart();
    chartView->setChart(chart);
    delete ancien;
}
