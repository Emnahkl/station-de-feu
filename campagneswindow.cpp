#include "campagneswindow.h"
#include "campagnemetiers.h"

#include <QtCharts>
#include <QtWidgets>
#include <QPdfWriter>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTextDocument>

namespace {
// Colonnes renvoyees par Campagne::selectColonnes()
enum Col { ColId = 0, ColTitre, ColTheme, ColDate, ColLieu, ColPublic, ColNb, ColZone,
           ColAnimateurs, ColIdZone };
}

CampagnesWindow::CampagnesWindow(QWidget *parent)
    : QWidget(parent)
{
    construireInterface();
    recharger();
}

void CampagnesWindow::recharger()
{
    chargerListes();
    rafraichir();
    on_stats_refresh();
}

void CampagnesWindow::marquerMenu()
{
    m_menu->marquer();
}

// ---------------------------------------------------------------- Interface

void CampagnesWindow::construireInterface()
{
    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);
    m_menu = new MenuLateral(MenuLateral::Campagnes);
    connect(m_menu, &MenuLateral::quitter, this, &QWidget::close);   // AppShell ferme l'application
    root->addWidget(m_menu);

    auto *contenu = new QWidget;
    contenu->setStyleSheet("QGroupBox { font-weight: bold; }"
                           "QPushButton#btnPrincipal { background:#c62828; color:white; border:none;"
                           "  border-radius:5px; padding:7px 12px; font-weight:bold; }"
                           "QPushButton#btnPrincipal:hover { background:#a31515; }"
                           "QPushButton#btnPrincipal:disabled { background:#5d4037; color:#bcaaa4; }");
    auto *contenuLay = new QVBoxLayout(contenu);
    contenuLay->setContentsMargins(18, 12, 18, 18);

    auto *titrePage = new QLabel("Gestion des campagnes de sensibilisation");
    titrePage->setStyleSheet("font-size: 22px; font-weight: bold; color: #e53935;");
    contenuLay->addWidget(titrePage);

    tabs = new QTabWidget;
    contenuLay->addWidget(tabs, 1);
    root->addWidget(contenu, 1);

    // ================= Onglet Gestion =================
    auto *gestion = new QWidget;
    auto *mainLayout = new QHBoxLayout(gestion);

    auto *formBox = new QGroupBox("Campagne de sensibilisation");
    auto *form = new QFormLayout(formBox);

    idSpin = new QSpinBox;
    idSpin->setRange(0, 999999999);
    idSpin->setSpecialValueText("Auto (nouvelle)");
    idSpin->setReadOnly(true);
    idSpin->setButtonSymbols(QAbstractSpinBox::NoButtons);
    idSpin->setToolTip("Attribué automatiquement (séquence seq_campagne). "
                       "Cliquez une ligne du tableau pour la modifier.");

    titreEdit = new QLineEdit;
    titreEdit->setMaxLength(100);
    titreEdit->setPlaceholderText("Ex : Prévention des feux de forêt");
    titreEdit->setValidator(new QRegularExpressionValidator(
        QRegularExpression("^[A-Za-z\\x{00C0}-\\x{00FF}0-9 '\\-]{0,100}$"), this));

    themeCombo = new QComboBox;
    themeCombo->addItems({"-- Choisir --", "Incendie domestique", "Feux de foret",
                          "Premiers secours", "Evacuation", "Risques electriques", "Intemperies"});

    dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    dateEdit->setMinimumDate(QDate(2000, 1, 1));

    lieuEdit = new QLineEdit;
    lieuEdit->setMaxLength(100);
    lieuEdit->setPlaceholderText("Ex : Lycée Bourguiba, Tunis");

    publicCombo = new QComboBox;
    publicCombo->addItems({"-- Choisir --", "Eleves", "Etudiants", "Employes",
                           "Grand public", "Personnes agees"});

    nbSpin = new QSpinBox;
    nbSpin->setRange(0, 999999);   // NUMBER(6)

    zoneCombo = new QComboBox;

    animateursList = new QListWidget;
    animateursList->setFixedHeight(110);
    animateursList->setToolTip("Cochez au moins un employé animateur (association ANIMER)");

    form->addRow("ID :", idSpin);
    form->addRow("Titre :", titreEdit);
    form->addRow("Thème :", themeCombo);
    form->addRow("Date :", dateEdit);
    form->addRow("Lieu :", lieuEdit);
    form->addRow("Public cible :", publicCombo);
    form->addRow("Nb participants :", nbSpin);
    form->addRow("Zone de couverture :", zoneCombo);
    form->addRow("Animateurs :", animateursList);

    auto *btnAjouter = new QPushButton("Ajouter");
    auto *btnModifier = new QPushButton("Modifier");
    auto *btnSupprimer = new QPushButton("Supprimer");
    auto *btnVider = new QPushButton("Vider");
    btnAjouter->setObjectName("btnPrincipal");
    btnModifier->setObjectName("btnPrincipal");
    auto *btns = new QGridLayout;
    btns->addWidget(btnAjouter, 0, 0);
    btns->addWidget(btnModifier, 0, 1);
    btns->addWidget(btnSupprimer, 1, 0);
    btns->addWidget(btnVider, 1, 1);
    form->addRow(btns);
    formBox->setFixedWidth(400);

    // Partie droite : recherche, tri, tableau, PDF
    auto *right = new QVBoxLayout;
    auto *outils = new QHBoxLayout;
    critereCombo = new QComboBox;
    critereCombo->addItems({"Titre", "ID", "Theme", "Lieu", "Zone", "Animateur"});
    rechercheEdit = new QLineEdit;
    rechercheEdit->setPlaceholderText("Rechercher...");
    rechercheEdit->setClearButtonEnabled(true);
    rechercheEdit->setMaxLength(50);

    triCombo = new QComboBox;
    triCombo->addItem("ID", "c.id_campagne");
    triCombo->addItem("Titre", "c.titre");
    triCombo->addItem("Date", "c.date_campagne");
    triCombo->addItem("Participants", "c.nb_participants");
    ordreCombo = new QComboBox;
    ordreCombo->addItem("Croissant", "ASC");
    ordreCombo->addItem("Décroissant", "DESC");

    auto *btnPdf = new QPushButton("Exporter PDF");
    btnPdf->setObjectName("btnPrincipal");
    auto *btnExcel = new QPushButton("Exporter Excel");
    btnExcel->setObjectName("btnPrincipal");
    btnExcel->setToolTip("Fichier CSV qui s'ouvre directement dans Excel (accents conservés)");

    outils->addWidget(new QLabel("Recherche par :"));
    outils->addWidget(critereCombo);
    outils->addWidget(rechercheEdit, 1);
    outils->addSpacing(15);
    outils->addWidget(new QLabel("Trier par :"));
    outils->addWidget(triCombo);
    outils->addWidget(ordreCombo);
    outils->addSpacing(15);
    outils->addWidget(btnPdf);
    outils->addWidget(btnExcel);

    table = new QTableView;
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);

    compteLabel = new QLabel;
    compteLabel->setStyleSheet("color:#455a64;");

    right->addLayout(outils);
    right->addWidget(table, 1);
    right->addWidget(compteLabel);

    mainLayout->addWidget(formBox);
    mainLayout->addLayout(right, 1);
    tabs->addTab(gestion, "Gestion des campagnes");

    // ================= Onglet Statistiques =================
    auto *stats = new QWidget;
    auto *statsLayout = new QVBoxLayout(stats);
    auto *statsTop = new QHBoxLayout;
    statsCombo = new QComboBox;
    statsCombo->addItem("Par thème", "THEME");
    statsCombo->addItem("Par public cible", "PUBLIC_CIBLE");
    statsCombo->addItem("Par zone de couverture", "ZONE");
    statsCombo->addItem("Par mois", "MOIS");
    auto *btnStats = new QPushButton("Actualiser");
    statsTop->addWidget(new QLabel("Répartition des campagnes :"));
    statsTop->addWidget(statsCombo);
    statsTop->addWidget(btnStats);
    statsTop->addStretch();
    chartView = new QChartView;
    chartView->setRenderHint(QPainter::Antialiasing);
    statsLayout->addLayout(statsTop);
    statsLayout->addWidget(chartView, 1);
    tabs->addTab(stats, "Statistiques");

    // ================= Metiers avances =================
    tabs->addTab(construireOngletCiblage(), "Ciblage des zones");
    tabs->addTab(construireOngletImpact(), "Évaluation d'impact");
    tabs->addTab(construireOngletAffiche(), "Affiche && GIF");

    // ================= Connexions =================
    connect(btnAjouter, &QPushButton::clicked, this, &CampagnesWindow::on_ajouter_clicked);
    connect(btnModifier, &QPushButton::clicked, this, &CampagnesWindow::on_modifier_clicked);
    connect(btnSupprimer, &QPushButton::clicked, this, &CampagnesWindow::on_supprimer_clicked);
    connect(btnVider, &QPushButton::clicked, this, &CampagnesWindow::on_vider_clicked);
    connect(btnPdf, &QPushButton::clicked, this, &CampagnesWindow::on_exportPdf_clicked);
    connect(btnExcel, &QPushButton::clicked, this, &CampagnesWindow::on_exportExcel_clicked);
    connect(table, &QTableView::clicked, this, &CampagnesWindow::on_table_clicked);
    connect(rechercheEdit, &QLineEdit::textChanged, this, &CampagnesWindow::rafraichir);
    connect(critereCombo, &QComboBox::currentIndexChanged, this, &CampagnesWindow::rafraichir);
    connect(triCombo, &QComboBox::currentIndexChanged, this, &CampagnesWindow::rafraichir);
    connect(ordreCombo, &QComboBox::currentIndexChanged, this, &CampagnesWindow::rafraichir);
    connect(btnStats, &QPushButton::clicked, this, &CampagnesWindow::on_stats_refresh);
    connect(statsCombo, &QComboBox::currentIndexChanged, this, &CampagnesWindow::on_stats_refresh);
    connect(tabs, &QTabWidget::currentChanged, this, [this](int i) {
        if (i == 1) on_stats_refresh();
        if (i == 2 && ciblageTable->rowCount() == 0) on_lancerCiblage_clicked();
        if (i != 4) arreterAnimation();
    });
}

// ---------------------------------------------------------------- Donnees

QString CampagnesWindow::ordreCourant() const
{
    return triCombo->currentData().toString() + " " + ordreCombo->currentData().toString();
}

void CampagnesWindow::rafraichir()
{
    QAbstractItemModel *ancien = table->model();
    if (rechercheEdit->text().trimmed().isEmpty())
        table->setModel(c.afficher(ordreCourant()));
    else
        table->setModel(c.rechercher(critereCombo->currentText(), rechercheEdit->text(),
                                     ordreCourant()));
    delete ancien;
    table->setColumnHidden(ColIdZone, true);
    table->resizeColumnsToContents();
    compteLabel->setText(QString("%1 campagne(s) affichée(s)").arg(table->model()->rowCount()));
    chargerCampagnesCombos();
}

void CampagnesWindow::chargerListes()
{
    const int zoneCourante = zoneCombo->currentData().toInt();
    const QList<int> coches = animateursCoches();

    zoneCombo->clear();
    zoneCombo->addItem("-- Choisir --", 0);
    for (const auto &z : Campagne::listeZones())
        zoneCombo->addItem(z.second, z.first);
    zoneCombo->setCurrentIndex(qMax(0, zoneCombo->findData(zoneCourante)));

    animateursList->clear();
    for (const auto &e : Campagne::listeEmployes()) {
        auto *item = new QListWidgetItem(e.second, animateursList);
        item->setData(Qt::UserRole, e.first);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(coches.contains(e.first) ? Qt::Checked : Qt::Unchecked);
    }
}

QList<int> CampagnesWindow::animateursCoches() const
{
    QList<int> ids;
    for (int i = 0; i < animateursList->count(); ++i)
        if (animateursList->item(i)->checkState() == Qt::Checked)
            ids.append(animateursList->item(i)->data(Qt::UserRole).toInt());
    return ids;
}

Campagne CampagnesWindow::campagneDuFormulaire() const
{
    return Campagne(idSpin->value(), titreEdit->text().trimmed(), themeCombo->currentText(),
                    dateEdit->date(), lieuEdit->text().trimmed(), publicCombo->currentText(),
                    nbSpin->value(), zoneCombo->currentData().toInt(), animateursCoches());
}

bool CampagnesWindow::controleSaisie(bool ajout)
{
    QStringList erreurs;

    if (!ajout && (idSpin->value() <= 0 || !c.existe(idSpin->value())))
        erreurs << "Sélectionnez d'abord une campagne existante dans le tableau.";

    if (titreEdit->text().trimmed().length() < 3)
        erreurs << "Le titre doit contenir au moins 3 caractères.";
    if (themeCombo->currentIndex() == 0)
        erreurs << "Choisissez un thème.";
    if (lieuEdit->text().trimmed().length() < 2)
        erreurs << "Le lieu doit contenir au moins 2 caractères.";
    if (publicCombo->currentIndex() == 0)
        erreurs << "Choisissez un public cible.";
    if (nbSpin->value() <= 0)
        erreurs << "Le nombre de participants doit être supérieur à 0.";
    if (zoneCombo->currentData().toInt() == 0)
        erreurs << "Choisissez la zone de couverture ciblée.";
    if (animateursCoches().isEmpty())
        erreurs << "Cochez au moins un employé animateur (une campagne est animée par 1 à N employés).";
    if (dateEdit->date() > QDate::currentDate().addYears(2))
        erreurs << "La date ne peut pas dépasser deux ans à partir d'aujourd'hui.";

    if (!erreurs.isEmpty()) {
        QMessageBox::warning(this, "Contrôle de saisie", "- " + erreurs.join("\n- "));
        return false;
    }
    return true;
}

// ---------------------------------------------------------------- CRUD

void CampagnesWindow::on_ajouter_clicked()
{
    if (!controleSaisie(true))
        return;
    Campagne nouvelle = campagneDuFormulaire();
    if (nouvelle.ajouter()) {
        QMessageBox::information(this, "Ajout",
                                 QString("Campagne n° %1 ajoutée avec succès.").arg(nouvelle.getId()));
        rafraichir();
        on_vider_clicked();
    } else {
        QMessageBox::critical(this, "Ajout", "Échec de l'ajout.\n" + Campagne::derniereErreur());
    }
}

void CampagnesWindow::on_modifier_clicked()
{
    if (!controleSaisie(false))
        return;
    Campagne modif = campagneDuFormulaire();
    if (modif.modifier()) {
        QMessageBox::information(this, "Modification", "Campagne modifiée avec succès.");
        rafraichir();
    } else {
        QMessageBox::critical(this, "Modification",
                              "Échec de la modification.\n" + Campagne::derniereErreur());
    }
}

void CampagnesWindow::on_supprimer_clicked()
{
    const int id = idSpin->value();
    if (id <= 0 || !c.existe(id)) {
        QMessageBox::warning(this, "Suppression", "Sélectionnez une campagne existante dans le tableau.");
        return;
    }
    if (QMessageBox::question(this, "Suppression",
                              QString("Supprimer la campagne n° %1 « %2 » ?")
                                  .arg(id).arg(titreEdit->text())) != QMessageBox::Yes)
        return;

    if (c.supprimer(id)) {
        QMessageBox::information(this, "Suppression", "Campagne supprimée.");
        rafraichir();
        on_vider_clicked();
    } else {
        QMessageBox::critical(this, "Suppression",
                              "Échec de la suppression.\n" + Campagne::derniereErreur());
    }
}

void CampagnesWindow::on_vider_clicked()
{
    idSpin->setValue(0);
    titreEdit->clear();
    themeCombo->setCurrentIndex(0);
    dateEdit->setDate(QDate::currentDate());
    lieuEdit->clear();
    publicCombo->setCurrentIndex(0);
    nbSpin->setValue(0);
    zoneCombo->setCurrentIndex(0);
    for (int i = 0; i < animateursList->count(); ++i)
        animateursList->item(i)->setCheckState(Qt::Unchecked);
    table->clearSelection();
}

void CampagnesWindow::on_table_clicked(const QModelIndex &index)
{
    const QAbstractItemModel *m = table->model();
    const int r = index.row();
    const int id = m->index(r, ColId).data().toInt();

    idSpin->setValue(id);
    titreEdit->setText(m->index(r, ColTitre).data().toString());
    themeCombo->setCurrentText(m->index(r, ColTheme).data().toString());
    dateEdit->setDate(QDate::fromString(m->index(r, ColDate).data().toString(), "yyyy-MM-dd"));
    lieuEdit->setText(m->index(r, ColLieu).data().toString());
    publicCombo->setCurrentText(m->index(r, ColPublic).data().toString());
    nbSpin->setValue(m->index(r, ColNb).data().toInt());
    zoneCombo->setCurrentIndex(qMax(0, zoneCombo->findData(m->index(r, ColIdZone).data().toInt())));

    const QList<int> anim = Campagne::animateursDe(id);
    for (int i = 0; i < animateursList->count(); ++i) {
        QListWidgetItem *it = animateursList->item(i);
        it->setCheckState(anim.contains(it->data(Qt::UserRole).toInt()) ? Qt::Checked : Qt::Unchecked);
    }
}

// ---------------------------------------------------------------- Export PDF

void CampagnesWindow::on_exportPdf_clicked()
{
    QString fichier = QFileDialog::getSaveFileName(this, "Exporter en PDF", "campagnes.pdf", "PDF (*.pdf)");
    if (fichier.isEmpty())
        return;
    if (!fichier.endsWith(".pdf", Qt::CaseInsensitive))
        fichier += ".pdf";

    const QAbstractItemModel *m = table->model();
    QString html;
    html += "<h2 style='color:#c0392b;text-align:center'>Liste des campagnes de sensibilisation</h2>";
    html += "<p style='text-align:center'>Fire Station Manager - généré le "
            + QDate::currentDate().toString("dd/MM/yyyy") + "</p>";
    html += "<table border='1' cellspacing='0' cellpadding='4' width='100%'>"
            "<tr style='background:#c0392b;color:white'>";
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

    QMessageBox::information(this, "Export PDF", "PDF généré :\n" + fichier);
    QDesktopServices::openUrl(QUrl::fromLocalFile(fichier));
}

// ---------------------------------------------------------------- Statistiques

void CampagnesWindow::on_stats_refresh()
{
    const QString critere = statsCombo->currentData().toString();
    const QMap<QString, int> data = c.statistiques(critere);

    int total = 0;
    for (int v : data)
        total += v;

    auto *series = new QPieSeries;
    for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
        QPieSlice *slice = series->append(it.key(), it.value());
        const double pct = total ? 100.0 * it.value() / total : 0;
        slice->setLabel(QString("%1 : %2 (%3%)").arg(it.key()).arg(it.value()).arg(pct, 0, 'f', 1));
        slice->setLabelVisible(true);
    }

    auto *chart = new QChart;
    chart->addSeries(series);
    chart->setTitle(total ? QString("Campagnes %1 - total : %2")
                                .arg(statsCombo->currentText().toLower()).arg(total)
                          : QString("Aucune campagne enregistrée"));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setAlignment(Qt::AlignRight);

    QChart *ancien = chartView->chart();
    chartView->setChart(chart);
    delete ancien;
}

// ---------------------------------------------------------------- Export Excel

void CampagnesWindow::on_exportExcel_clicked()
{
    QString fichier = QFileDialog::getSaveFileName(
        this, "Exporter pour Excel",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/campagnes_sensibilisation.csv",
        "CSV Excel (*.csv)");
    if (fichier.isEmpty())
        return;
    if (!fichier.endsWith(".csv", Qt::CaseInsensitive))
        fichier += ".csv";

    QFile f(fichier);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Export Excel", "Impossible d'écrire le fichier.");
        return;
    }
    QTextStream out(&f);
    out << QChar(0xFEFF);   // BOM : Excel reconnait les accents

    const QAbstractItemModel *m = table->model();
    auto cellule = [](QString s) { s.replace('"', "\"\""); return "\"" + s + "\""; };
    QStringList ligne;
    for (int col = 0; col < m->columnCount(); ++col)
        if (!table->isColumnHidden(col))
            ligne << cellule(m->headerData(col, Qt::Horizontal).toString());
    out << ligne.join(';') << "\n";
    for (int row = 0; row < m->rowCount(); ++row) {
        ligne.clear();
        for (int col = 0; col < m->columnCount(); ++col)
            if (!table->isColumnHidden(col))
                ligne << cellule(m->index(row, col).data().toString());
        out << ligne.join(';') << "\n";
    }
    f.close();
    QMessageBox::information(this, "Export Excel", "Fichier enregistré :\n" + fichier);
    QDesktopServices::openUrl(QUrl::fromLocalFile(fichier));
}

// ---------------------------------------------------------------- Listes des campagnes (onglets metiers)

void CampagnesWindow::chargerCampagnesCombos()
{
    if (!impactCombo || !afficheCombo)
        return;
    const int idImpact = impactCombo->currentData().toInt();
    const int idAffiche = afficheCombo->currentData().toInt();

    QList<QPair<int, QString>> campagnes;
    QSqlQuery q("SELECT id_campagne, titre, TO_CHAR(date_campagne,'DD/MM/YYYY') "
                "FROM CAMPAGNES_SENSIBILISATION ORDER BY date_campagne DESC, id_campagne DESC");
    while (q.next())
        campagnes.append(qMakePair(q.value(0).toInt(), QString("n° %1 - %2 (%3)")
                                                          .arg(q.value(0).toInt())
                                                          .arg(q.value(1).toString(), q.value(2).toString())));

    for (QComboBox *combo : { impactCombo, afficheCombo }) {
        const QSignalBlocker bloque(combo);
        combo->clear();
        if (campagnes.isEmpty())
            combo->addItem("-- Aucune campagne : ajoutez-en une dans l'onglet Gestion --", 0);
        for (const auto &cp : campagnes)
            combo->addItem(cp.second, cp.first);
    }
    impactCombo->setCurrentIndex(qMax(0, impactCombo->findData(idImpact)));
    afficheCombo->setCurrentIndex(qMax(0, afficheCombo->findData(idAffiche)));
}

// ---------------------------------------------------------------- Metier 1 : ciblage des zones

QWidget *CampagnesWindow::construireOngletCiblage()
{
    auto *w = new QWidget;
    auto *lay = new QVBoxLayout(w);

    auto *explication = new QLabel(
        "Classe les zones de couverture à sensibiliser en priorité, à partir des interventions des "
        "12 derniers mois, du niveau de risque de la zone et des campagnes déjà réalisées.\n"
        "Score = interventions × poids du risque (Faible 1 · Moyen 1,5 · Élevé 2) ÷ (1 + campagnes sur 12 mois)");
    explication->setWordWrap(true);
    explication->setStyleSheet("color:#90a4ae;");

    auto *haut = new QHBoxLayout;
    auto *btnLancer = new QPushButton("Lancer le ciblage");
    btnLancer->setObjectName("btnPrincipal");
    zonePrioritaireLabel = new QLabel("Cliquez sur « Lancer le ciblage ».");
    zonePrioritaireLabel->setStyleSheet("font-weight:bold; font-size:15px; color:#e53935;");
    haut->addWidget(btnLancer);
    haut->addSpacing(15);
    haut->addWidget(zonePrioritaireLabel, 1);

    ciblageTable = new QTableWidget(0, 6);
    ciblageTable->setHorizontalHeaderLabels({ "Zone", "Niveau de risque", "Interventions (12 mois)",
                                              "Campagnes (12 mois)", "Score", "Recommandation" });
    ciblageTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ciblageTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ciblageTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ciblageTable->setAlternatingRowColors(true);
    ciblageTable->verticalHeader()->setVisible(false);
    ciblageTable->horizontalHeader()->setStretchLastSection(true);

    btnPlanifier = new QPushButton("Planifier une campagne dans la zone sélectionnée");
    btnPlanifier->setObjectName("btnPrincipal");
    btnPlanifier->setEnabled(false);

    lay->addWidget(explication);
    lay->addLayout(haut);
    lay->addWidget(ciblageTable, 1);
    lay->addWidget(btnPlanifier, 0, Qt::AlignRight);

    connect(btnLancer, &QPushButton::clicked, this, &CampagnesWindow::on_lancerCiblage_clicked);
    connect(btnPlanifier, &QPushButton::clicked, this, &CampagnesWindow::on_planifierZone_clicked);
    connect(ciblageTable, &QTableWidget::cellDoubleClicked, this, &CampagnesWindow::on_planifierZone_clicked);
    return w;
}

void CampagnesWindow::on_lancerCiblage_clicked()
{
    const QList<CiblageZone> res = CampagneMetiers::ciblageAutomatique();
    ciblageTable->setRowCount(0);

    const QColor couleurs[3] = { QColor("#e53935"), QColor("#fb8c00"), QColor("#43a047") };
    for (int r = 0; r < res.size(); ++r) {
        const CiblageZone &z = res.at(r);
        ciblageTable->insertRow(r);
        auto *zone = new QTableWidgetItem(z.nom);
        zone->setData(Qt::UserRole, z.idZone);
        ciblageTable->setItem(r, 0, zone);
        ciblageTable->setItem(r, 1, new QTableWidgetItem(z.niveauRisque));
        ciblageTable->setItem(r, 2, new QTableWidgetItem(QString::number(z.interventions12Mois)));
        ciblageTable->setItem(r, 3, new QTableWidgetItem(QString::number(z.campagnes12Mois)));
        ciblageTable->setItem(r, 4, new QTableWidgetItem(QString::number(z.score, 'f', 1)));
        auto *reco = new QTableWidgetItem(z.recommandation);
        reco->setForeground(couleurs[qBound(0, z.priorite, 2)]);
        QFont gras = reco->font();
        gras.setBold(z.priorite == 0);
        reco->setFont(gras);
        ciblageTable->setItem(r, 5, reco);
    }
    ciblageTable->resizeColumnsToContents();

    if (res.isEmpty()) {
        zonePrioritaireLabel->setText("Aucune zone de couverture enregistrée.");
        btnPlanifier->setEnabled(false);
        return;
    }
    zonePrioritaireLabel->setText(QString("Zone prioritaire : %1 (score %2)")
                                      .arg(res.first().nom)
                                      .arg(res.first().score, 0, 'f', 1));
    ciblageTable->selectRow(0);
    btnPlanifier->setEnabled(true);
}

void CampagnesWindow::on_planifierZone_clicked()
{
    const int ligne = qMax(0, ciblageTable->currentRow());
    QTableWidgetItem *item = ciblageTable->item(ligne, 0);
    if (!item)
        return;

    on_vider_clicked();
    zoneCombo->setCurrentIndex(qMax(0, zoneCombo->findData(item->data(Qt::UserRole))));
    tabs->setCurrentIndex(0);
    titreEdit->setFocus();
    QMessageBox::information(this, "Planification",
                             QString("Zone « %1 » présélectionnée.\n"
                                     "Complétez le formulaire puis cliquez sur « Ajouter ».")
                                 .arg(item->text()));
}

// ---------------------------------------------------------------- Metier 2 : evaluation d'impact

QWidget *CampagnesWindow::construireOngletImpact()
{
    auto *w = new QWidget;
    auto *lay = new QVBoxLayout(w);

    auto *explication = new QLabel(
        "Compare le nombre d'interventions dans la zone de la campagne pendant les N mois qui précèdent "
        "et les N mois qui suivent la date de la campagne.");
    explication->setWordWrap(true);
    explication->setStyleSheet("color:#90a4ae;");

    auto *haut = new QHBoxLayout;
    impactCombo = new QComboBox;
    impactCombo->setMinimumWidth(380);
    fenetreSpin = new QSpinBox;
    fenetreSpin->setRange(1, 24);
    fenetreSpin->setValue(3);
    fenetreSpin->setSuffix(" mois");
    auto *btnEvaluer = new QPushButton("Évaluer l'impact");
    btnEvaluer->setObjectName("btnPrincipal");
    haut->addWidget(new QLabel("Campagne :"));
    haut->addWidget(impactCombo, 1);
    haut->addSpacing(10);
    haut->addWidget(new QLabel("Fenêtre :"));
    haut->addWidget(fenetreSpin);
    haut->addWidget(btnEvaluer);

    auto *resultats = new QGroupBox("Résultat");
    auto *res = new QVBoxLayout(resultats);
    impactAvantLabel = new QLabel("Interventions avant : -");
    impactApresLabel = new QLabel("Interventions après : -");
    impactVariationLabel = new QLabel("Variation : -");
    impactVerdictLabel = new QLabel;
    impactVerdictLabel->setWordWrap(true);
    for (QLabel *l : { impactAvantLabel, impactApresLabel, impactVariationLabel })
        l->setStyleSheet("font-size:15px;");
    impactVerdictLabel->setStyleSheet("font-size:15px; font-weight:bold;");
    res->addWidget(impactAvantLabel);
    res->addWidget(impactApresLabel);
    res->addWidget(impactVariationLabel);
    res->addWidget(impactVerdictLabel);

    impactChart = new QChartView;
    impactChart->setRenderHint(QPainter::Antialiasing);
    impactChart->setMinimumHeight(260);

    lay->addWidget(explication);
    lay->addLayout(haut);
    lay->addWidget(resultats);
    lay->addWidget(impactChart, 1);

    connect(btnEvaluer, &QPushButton::clicked, this, &CampagnesWindow::on_evaluerImpact_clicked);
    return w;
}

void CampagnesWindow::on_evaluerImpact_clicked()
{
    const int id = impactCombo->currentData().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Impact", "Aucune campagne à évaluer.");
        return;
    }
    const int n = fenetreSpin->value();
    const ImpactCampagne r = CampagneMetiers::evaluerImpact(id, n);
    if (!r.valide) {
        QMessageBox::warning(this, "Impact",
                             "Impossible d'évaluer cette campagne (date ou zone de couverture manquante).");
        return;
    }

    impactAvantLabel->setText(QString("Interventions %1 mois avant (zone %2) : %3").arg(n).arg(r.zone).arg(r.avant));
    impactApresLabel->setText(QString("Interventions %1 mois après : %2").arg(n).arg(r.apres));
    impactVariationLabel->setText(QString("Variation : %1%2 %")
                                      .arg(r.variation > 0 ? "+" : "")
                                      .arg(r.variation, 0, 'f', 1));
    const QString couleur = r.variation < 0 ? "#43a047" : (r.variation > 0 ? "#e53935" : "#fb8c00");
    impactVariationLabel->setStyleSheet("font-size:15px; font-weight:bold; color:" + couleur + ";");
    impactVerdictLabel->setStyleSheet("font-size:15px; font-weight:bold; color:" + couleur + ";");
    impactVerdictLabel->setText(r.verdict);

    // Graphique avant / apres
    auto *set = new QBarSet("Interventions");
    *set << r.avant << r.apres;
    set->setColor(QColor("#c62828"));
    auto *series = new QBarSeries;
    series->append(set);
    series->setLabelsVisible(true);
    auto *chart = new QChart;
    chart->addSeries(series);
    chart->setTitle(QString("Interventions dans la zone : %1 mois avant / après la campagne").arg(n));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    auto *axeX = new QBarCategoryAxis;
    axeX->append({ "Avant", "Après" });
    chart->addAxis(axeX, Qt::AlignBottom);
    series->attachAxis(axeX);
    auto *axeY = new QValueAxis;
    axeY->setRange(0, qMax(1, qMax(r.avant, r.apres)) * 1.2);
    axeY->setLabelFormat("%d");
    chart->addAxis(axeY, Qt::AlignLeft);
    series->attachAxis(axeY);
    chart->legend()->setVisible(false);

    QChart *ancien = impactChart->chart();
    impactChart->setChart(chart);
    delete ancien;
}

// ---------------------------------------------------------------- Service : affiche PNG et GIF

QWidget *CampagnesWindow::construireOngletAffiche()
{
    auto *w = new QWidget;
    auto *lay = new QHBoxLayout(w);

    auto *gauche = new QGroupBox("Affiche de la campagne");
    gauche->setFixedWidth(380);
    auto *form = new QVBoxLayout(gauche);
    afficheCombo = new QComboBox;
    sloganEdit = new QLineEdit;
    sloganEdit->setMaxLength(90);
    sloganEdit->setPlaceholderText("Vide = slogan automatique selon le thème");
    sloganEdit->setClearButtonEnabled(true);

    auto *btnGenerer = new QPushButton("Générer l'affiche");
    btnGenerer->setObjectName("btnPrincipal");
    btnEnregistrerPng = new QPushButton("Enregistrer l'affiche (PNG)");
    btnApercuAnime = new QPushButton("▶  Aperçu animé");
    btnEnregistrerGif = new QPushButton("Enregistrer l'affiche animée (GIF)");
    btnEnregistrerGif->setObjectName("btnPrincipal");
    for (QPushButton *b : { btnEnregistrerPng, btnApercuAnime, btnEnregistrerGif })
        b->setEnabled(false);

    auto *aide = new QLabel("Le GIF anime l'affiche : les flammes montent, le titre apparaît, "
                            "le slogan s'écrit lettre par lettre, puis les informations s'affichent "
                            "et le numéro d'urgence clignote. Il tourne en boucle (idéal pour les "
                            "réseaux sociaux et les écrans d'accueil).");
    aide->setWordWrap(true);
    aide->setStyleSheet("color:#90a4ae;");

    form->addWidget(new QLabel("Campagne :"));
    form->addWidget(afficheCombo);
    form->addSpacing(6);
    form->addWidget(new QLabel("Slogan (facultatif) :"));
    form->addWidget(sloganEdit);
    form->addSpacing(10);
    form->addWidget(btnGenerer);
    form->addWidget(btnEnregistrerPng);
    form->addSpacing(14);
    form->addWidget(btnApercuAnime);
    form->addWidget(btnEnregistrerGif);
    form->addSpacing(10);
    form->addWidget(aide);
    form->addStretch();

    apercuLabel = new QLabel("Choisissez une campagne puis cliquez sur « Générer l'affiche ».");
    apercuLabel->setAlignment(Qt::AlignCenter);
    apercuLabel->setMinimumSize(360, 480);
    apercuLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    apercuLabel->setStyleSheet("background:#1e1e1e; color:#90a4ae; border:1px solid #444; border-radius:6px;");

    lay->addWidget(gauche);
    lay->addWidget(apercuLabel, 1);

    m_timer = new QTimer(this);
    m_timer->setInterval(80);
    connect(m_timer, &QTimer::timeout, this, &CampagnesWindow::imageSuivante);
    connect(btnGenerer, &QPushButton::clicked, this, &CampagnesWindow::on_genererAffiche_clicked);
    connect(btnEnregistrerPng, &QPushButton::clicked, this, &CampagnesWindow::on_enregistrerAffiche_clicked);
    connect(btnApercuAnime, &QPushButton::clicked, this, &CampagnesWindow::on_apercuAnime_clicked);
    connect(btnEnregistrerGif, &QPushButton::clicked, this, &CampagnesWindow::on_enregistrerGif_clicked);
    // Changer de campagne ou de slogan invalide l'affiche affichee
    auto invalider = [this] {
        arreterAnimation();
        m_images.clear();
        for (QPushButton *b : { btnEnregistrerPng, btnApercuAnime, btnEnregistrerGif })
            b->setEnabled(false);
    };
    connect(afficheCombo, &QComboBox::currentIndexChanged, this, invalider);
    connect(sloganEdit, &QLineEdit::textChanged, this, invalider);
    return w;
}

void CampagnesWindow::afficherApercu(const QImage &image)
{
    apercuLabel->setPixmap(QPixmap::fromImage(image).scaled(apercuLabel->size() - QSize(16, 16),
                                                            Qt::KeepAspectRatio,
                                                            Qt::SmoothTransformation));
}

void CampagnesWindow::arreterAnimation()
{
    if (m_timer && m_timer->isActive()) {
        m_timer->stop();
        btnApercuAnime->setText("▶  Aperçu animé");
        if (!m_affiche.isNull())
            afficherApercu(m_affiche);
    }
}

void CampagnesWindow::on_genererAffiche_clicked()
{
    const int id = afficheCombo->currentData().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Affiche", "Choisissez une campagne.");
        return;
    }
    const InfosAffiche infos = CampagneMetiers::chargerInfos(id);
    if (!infos.valide) {
        QMessageBox::warning(this, "Affiche", "Campagne introuvable.");
        return;
    }
    arreterAnimation();
    m_images.clear();
    m_affiche = CampagneMetiers::dessinerAffiche(infos, sloganEdit->text());
    afficherApercu(m_affiche);
    for (QPushButton *b : { btnEnregistrerPng, btnApercuAnime, btnEnregistrerGif })
        b->setEnabled(true);
}

void CampagnesWindow::on_enregistrerAffiche_clicked()
{
    if (m_affiche.isNull())
        return;
    QString chemin = QFileDialog::getSaveFileName(
        this, "Enregistrer l'affiche",
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
            + QString("/affiche_campagne_%1.png").arg(afficheCombo->currentData().toInt()),
        "Image PNG (*.png)");
    if (chemin.isEmpty())
        return;
    if (!chemin.endsWith(".png", Qt::CaseInsensitive))
        chemin += ".png";
    if (m_affiche.save(chemin, "PNG"))
        QMessageBox::information(this, "Affiche", "Affiche enregistrée :\n" + chemin);
    else
        QMessageBox::critical(this, "Affiche", "Impossible d'enregistrer l'image.");
}

void CampagnesWindow::on_apercuAnime_clicked()
{
    if (m_timer->isActive()) {
        arreterAnimation();
        return;
    }
    if (m_images.isEmpty()) {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        const InfosAffiche infos = CampagneMetiers::chargerInfos(afficheCombo->currentData().toInt());
        m_images = CampagneMetiers::imagesAnimation(infos, sloganEdit->text());
        QApplication::restoreOverrideCursor();
    }
    if (m_images.isEmpty())
        return;
    m_imageCourante = 0;
    btnApercuAnime->setText("■  Arrêter l'aperçu");
    m_timer->start();
}

void CampagnesWindow::imageSuivante()
{
    if (m_images.isEmpty()) {
        m_timer->stop();
        return;
    }
    afficherApercu(m_images.at(m_imageCourante));
    // Petite pause sur l'affiche complete, comme dans le GIF
    m_timer->setInterval(m_imageCourante == m_images.size() * 2 / 3 - 1 ? 1500 : 80);
    m_imageCourante = (m_imageCourante + 1) % m_images.size();
}

void CampagnesWindow::on_enregistrerGif_clicked()
{
    const int id = afficheCombo->currentData().toInt();
    const InfosAffiche infos = CampagneMetiers::chargerInfos(id);
    if (!infos.valide) {
        QMessageBox::warning(this, "Affiche animée", "Choisissez une campagne.");
        return;
    }
    QString chemin = QFileDialog::getSaveFileName(
        this, "Enregistrer l'affiche animée",
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
            + QString("/affiche_campagne_%1.gif").arg(id),
        "Image GIF animée (*.gif)");
    if (chemin.isEmpty())
        return;
    if (!chemin.endsWith(".gif", Qt::CaseInsensitive))
        chemin += ".gif";

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString erreur;
    const bool ok = CampagneMetiers::exporterGif(infos, sloganEdit->text(), chemin, 450, 8, &erreur);
    QApplication::restoreOverrideCursor();

    if (ok) {
        QMessageBox::information(this, "Affiche animée", "GIF enregistré :\n" + chemin);
        QDesktopServices::openUrl(QUrl::fromLocalFile(chemin));
    } else {
        QMessageBox::critical(this, "Affiche animée", "Impossible d'enregistrer le GIF.\n" + erreur);
    }
}
