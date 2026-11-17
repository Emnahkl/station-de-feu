#include "interventionspage.h"
#include "rightpanel.h"
#include "flowlayout.h"

#include <QComboBox>
#include <QDateTimeEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QMessageBox>
#include <QPageSize>
#include <QPdfWriter>
#include <QPushButton>
#include <QTableWidget>
#include <QTextDocument>
#include <QVBoxLayout>
#include <algorithm>

namespace {

const QStringList kTypes = {"Incendie", "Accident de la route", "Secours à personne", "Fuite de gaz", "Inondation"};
const QStringList kGravites = {"Faible", "Moyenne", "Élevée", "Critique"};
const QStringList kStatuts = {"En cours", "En attente", "Terminée"};

int graviteRank(const QString &g) { return kGravites.indexOf(g); }

QColor graviteColor(const QString &g)
{
    if (g == "Critique") return QColor("#b91c1c");
    if (g == "Élevée") return QColor("#ea580c");
    if (g == "Moyenne") return QColor("#b45309");
    return QColor("#15803d");
}

QColor statutColor(const QString &s)
{
    if (s == "En cours") return QColor("#b45309");
    if (s == "En attente") return QColor("#b91c1c");
    return QColor("#15803d");
}

QString idText(int id) { return QStringLiteral("INT-%1").arg(id, 3, 10, QLatin1Char('0')); }

} // namespace

InterventionsPage::InterventionsPage(QWidget *parent) : QWidget(parent)
{
    setObjectName("InterventionsPage");
    loadSampleData();
    buildUi();
    rebuildTable();
}

void InterventionsPage::loadSampleData()
{
    struct S { const char *t, *a; int d, h; const char *z, *g, *s; };
    const S rows[] = {
        {"Incendie", "12 Rue de la République", 0, 8, "Z1", "Critique", "En cours"},
        {"Accident de la route", "Avenue Habib Bourguiba", 0, 9, "Z2", "Élevée", "En cours"},
        {"Secours à personne", "5 Rue des Oliviers", 0, 10, "Z1", "Moyenne", "En attente"},
        {"Fuite de gaz", "Zone industrielle Nord", -1, 21, "Z3", "Élevée", "En attente"},
        {"Inondation", "Cité El Ghazala", -1, 16, "Z4", "Faible", "Terminée"},
        {"Incendie", "Marché central", -2, 13, "Z2", "Critique", "Terminée"},
    };
    const QDate today = QDate::currentDate();
    for (const S &r : rows)
        m_items.push_back({m_nextId++, r.t, r.a, QDateTime(today.addDays(r.d), QTime(r.h, 30)), r.z, r.g, r.s});
}

void InterventionsPage::buildUi()
{
    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(18, 6, 18, 18);
    root->setSpacing(16);

    auto *left = new QVBoxLayout;
    left->setSpacing(12);
    root->addLayout(left, 1);

    auto *title = new QLabel(tr("Gestion des Interventions"));
    title->setObjectName("labelTitle");
    left->addWidget(title);

    // --------------------------------------------------------------- Filtres
    auto *filters = new QFrame;
    filters->setObjectName("filtersFrame");
    auto *fl = new QHBoxLayout(filters);
    fl->setContentsMargins(14, 10, 14, 10);
    fl->setSpacing(10);
    fl->addWidget(new QLabel(tr("Filtrer par Gravité")));
    m_comboGravite = new QComboBox;
    m_comboGravite->addItem(tr("Toutes les gravités"));
    m_comboGravite->addItems(kGravites);
    fl->addWidget(m_comboGravite);
    fl->addWidget(new QLabel(tr("Filtrer par Statut")));
    m_comboStatut = new QComboBox;
    m_comboStatut->addItem(tr("Tous les statuts"));
    m_comboStatut->addItems(kStatuts);
    fl->addWidget(m_comboStatut);
    fl->addStretch(1);
    left->addWidget(filters);

    // --------------------------------------------------------------- Cartes
    auto *cards = new QHBoxLayout;
    cards->setSpacing(14);
    auto makeCard = [&](const QString &frameName, const QString &titleName, const QString &valueName,
                        const QString &text, QLabel *&valueLabel) {
        auto *card = new QFrame;
        card->setObjectName(frameName);
        auto *cl = new QVBoxLayout(card);
        cl->setContentsMargins(16, 10, 16, 10);
        cl->setSpacing(2);
        auto *t = new QLabel(text);
        t->setObjectName(titleName);
        valueLabel = new QLabel("0");
        valueLabel->setObjectName(valueName);
        cl->addWidget(t);
        cl->addWidget(valueLabel);
        cards->addWidget(card);
    };
    makeCard("cardEnCours", "lblEnCoursTitle", "lblEnCours", tr("En cours"), m_lblEnCours);
    makeCard("cardEnAttente", "lblEnAttenteTitle", "lblEnAttente", tr("En attente"), m_lblEnAttente);
    makeCard("cardTerminee", "lblTermineeTitle", "lblTerminee", tr("Terminée"), m_lblTerminee);
    cards->addStretch(1);
    left->addLayout(cards);

    // --------------------------------------------------------------- Actions
    auto *actions = new QFrame;
    actions->setObjectName("actionsFrame");
    auto *flow = new FlowLayout(actions, 12, 10, 10);
    auto addBtn = [&](const QString &obj, const QString &text, const QString &icon,
                      void (InterventionsPage::*slot)()) {
        auto *b = new QPushButton("  " + text);
        b->setObjectName(obj);
        b->setIcon(QIcon(icon));
        b->setIconSize(QSize(20, 20));
        b->setCursor(Qt::PointingHandCursor);
        flow->addWidget(b);
        connect(b, &QPushButton::clicked, this, slot);
    };
    addBtn("btnAjouter", tr("Ajouter"), ":/icons/add.png", &InterventionsPage::addIntervention);
    addBtn("btnModifier", tr("Modifier"), ":/icons/edit.png", &InterventionsPage::modifyCurrent);
    addBtn("btnSupprimer", tr("Supprimer"), ":/icons/delete.png", &InterventionsPage::removeCurrent);
    addBtn("btnConsulter", tr("Consulter"), ":/icons/view.png", &InterventionsPage::consultCurrent);
    addBtn("btnExportPdf", tr("Exporter PDF"), ":/icons/pdf.png", &InterventionsPage::exportPdf);
    addBtn("btnTriDate", tr("Trier selon la date"), ":/icons/calendar.png", &InterventionsPage::sortByDate);
    addBtn("btnTriGravite", tr("Trier selon la gravité"), ":/icons/interventions.png", &InterventionsPage::sortByGravite);
    addBtn("btnStats", tr("Statistiques"), ":/icons/stats.png", &InterventionsPage::showStats);
    addBtn("btnAffectation", tr("Affectation automatique"), ":/icons/team.png", &InterventionsPage::autoAssign);
    addBtn("btnParcours", tr("Parcours recommandé"), ":/icons/route.png", &InterventionsPage::showRoute);
    left->addWidget(actions);

    // --------------------------------------------------------------- Tableau
    auto *tableFrame = new QFrame;
    tableFrame->setObjectName("tableFrame");
    auto *tl = new QVBoxLayout(tableFrame);
    tl->setContentsMargins(0, 0, 0, 0);
    m_table = new QTableWidget;
    m_table->setObjectName("tableInterventions");
    m_table->setColumnCount(7);
    m_table->setHorizontalHeaderLabels({tr("ID"), tr("Type"), tr("Adresse"), tr("Date et heure"),
                                        tr("ID_zone"), tr("Gravité"), tr("Statut")});
    m_table->setFrameShape(QFrame::NoFrame);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setShowGrid(false);
    m_table->setAlternatingRowColors(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->verticalHeader()->setDefaultSectionSize(52);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->horizontalHeader()->setFixedHeight(44);
    tl->addWidget(m_table);
    left->addWidget(tableFrame, 1);

    // --------------------------------------------------------------- Panneau droit
    RightPanel::Config cfg;
    cfg.chatTitle = tr("IA Chatbot Interventions");
    cfg.greeting = tr("Bonjour ! Je suis l'assistant des interventions. Demandez-moi par exemple : « combien d'interventions en cours ? »");
    cfg.planTitle = tr("Smart Dispatch IA");
    cfg.btnNotify = tr("Notifier le Chef");
    cfg.btnAlert = tr("Alerte Critique");
    cfg.btnGenerate = tr("Générer Dispatch");
    m_panel = new RightPanel(cfg);
    m_panel->setResponder([this](const QString &q) { return answer(q); });
    root->addWidget(m_panel);

    connect(m_comboGravite, &QComboBox::currentIndexChanged, this, [this] { rebuildTable(); });
    connect(m_comboStatut, &QComboBox::currentIndexChanged, this, [this] { rebuildTable(); });
}

void InterventionsPage::setSearch(const QString &text)
{
    m_search = text.trimmed();
    rebuildTable();
}

int InterventionsPage::indexOfId(int id) const
{
    for (int i = 0; i < m_items.size(); ++i)
        if (m_items[i].id == id) return i;
    return -1;
}

int InterventionsPage::currentId() const
{
    const int row = m_table->currentRow();
    if (row < 0 || !m_table->item(row, 0)) return -1;
    return m_table->item(row, 0)->data(Qt::UserRole).toInt();
}

void InterventionsPage::sortByDate()
{
    std::stable_sort(m_items.begin(), m_items.end(),
                     [](const Intervention &x, const Intervention &y) { return x.date > y.date; });
    rebuildTable();
}

void InterventionsPage::sortByGravite()
{
    std::stable_sort(m_items.begin(), m_items.end(), [](const Intervention &x, const Intervention &y) {
        return graviteRank(x.gravite) > graviteRank(y.gravite);
    });
    rebuildTable();
}

void InterventionsPage::updateCards()
{
    int c = 0, a = 0, t = 0;
    for (const Intervention &i : std::as_const(m_items)) {
        if (i.statut == "En cours") ++c;
        else if (i.statut == "En attente") ++a;
        else ++t;
    }
    m_lblEnCours->setText(QString::number(c));
    m_lblEnAttente->setText(QString::number(a));
    m_lblTerminee->setText(QString::number(t));
}

void InterventionsPage::rebuildTable()
{
    const QString g = m_comboGravite->currentIndex() > 0 ? m_comboGravite->currentText() : QString();
    const QString s = m_comboStatut->currentIndex() > 0 ? m_comboStatut->currentText() : QString();

    QVector<const Intervention *> shown;
    for (const Intervention &i : std::as_const(m_items)) {
        if (!g.isEmpty() && i.gravite != g) continue;
        if (!s.isEmpty() && i.statut != s) continue;
        if (!m_search.isEmpty()
            && !idText(i.id).contains(m_search, Qt::CaseInsensitive)
            && !i.type.contains(m_search, Qt::CaseInsensitive)
            && !i.adresse.contains(m_search, Qt::CaseInsensitive)
            && !i.zone.contains(m_search, Qt::CaseInsensitive)) continue;
        shown.push_back(&i);
    }

    m_table->clearContents();
    m_table->setRowCount(shown.size());
    for (int r = 0; r < shown.size(); ++r) {
        const Intervention &i = *shown[r];
        const QStringList cells = {idText(i.id), i.type, i.adresse, i.date.toString("dd/MM/yyyy HH:mm"),
                                   i.zone, i.gravite, i.statut};
        for (int c = 0; c < cells.size(); ++c) {
            auto *it = new QTableWidgetItem(cells[c]);
            it->setData(Qt::UserRole, i.id);
            it->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            if (c == 5) { it->setForeground(graviteColor(i.gravite)); QFont f = it->font(); f.setBold(true); it->setFont(f); }
            if (c == 6) { it->setForeground(statutColor(i.statut)); QFont f = it->font(); f.setBold(true); it->setFont(f); }
            m_table->setItem(r, c, it);
        }
    }
    updateCards();
}

bool InterventionsPage::editIntervention(Intervention &it, const QString &title)
{
    QDialog dlg(this);
    dlg.setWindowTitle(title);
    auto *form = new QFormLayout(&dlg);
    auto *type = new QComboBox;
    type->addItems(kTypes);
    type->setEditable(true);
    type->setCurrentText(it.type);
    auto *adresse = new QLineEdit(it.adresse);
    auto *date = new QDateTimeEdit(it.date);
    date->setCalendarPopup(true);
    date->setDisplayFormat("dd/MM/yyyy HH:mm");
    auto *zone = new QLineEdit(it.zone);
    auto *grav = new QComboBox;
    grav->addItems(kGravites);
    grav->setCurrentText(it.gravite);
    auto *stat = new QComboBox;
    stat->addItems(kStatuts);
    stat->setCurrentText(it.statut);
    form->addRow(tr("Type"), type);
    form->addRow(tr("Adresse"), adresse);
    form->addRow(tr("Date et heure"), date);
    form->addRow(tr("ID_zone"), zone);
    form->addRow(tr("Gravité"), grav);
    form->addRow(tr("Statut"), stat);
    auto *bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form->addRow(bb);
    connect(bb, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(bb, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    if (dlg.exec() != QDialog::Accepted)
        return false;
    if (adresse->text().trimmed().isEmpty() || zone->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, title, tr("L'adresse et la zone sont obligatoires."));
        return false;
    }
    it.type = type->currentText().trimmed();
    it.adresse = adresse->text().trimmed();
    it.date = date->dateTime();
    it.zone = zone->text().trimmed();
    it.gravite = grav->currentText();
    it.statut = stat->currentText();
    return true;
}

void InterventionsPage::addIntervention()
{
    Intervention it{m_nextId, kTypes.first(), QString(), QDateTime::currentDateTime(), "Z1", kGravites.first(), kStatuts[1]};
    if (editIntervention(it, tr("Ajouter une intervention"))) {
        ++m_nextId;
        m_items.push_back(it);
        rebuildTable();
    }
}

void InterventionsPage::modifyCurrent()
{
    const int i = indexOfId(currentId());
    if (i < 0) {
        QMessageBox::information(this, tr("Modifier"), tr("Sélectionnez d'abord une intervention."));
        return;
    }
    Intervention copy = m_items[i];
    if (editIntervention(copy, tr("Modifier l'intervention"))) {
        m_items[i] = copy;
        rebuildTable();
    }
}

void InterventionsPage::removeCurrent()
{
    const int i = indexOfId(currentId());
    if (i < 0) {
        QMessageBox::information(this, tr("Supprimer"), tr("Sélectionnez d'abord une intervention."));
        return;
    }
    if (QMessageBox::question(this, tr("Supprimer"), tr("Supprimer l'intervention %1 ?").arg(idText(m_items[i].id))) == QMessageBox::Yes) {
        m_items.removeAt(i);
        rebuildTable();
    }
}

void InterventionsPage::consultCurrent()
{
    const int i = indexOfId(currentId());
    if (i < 0) {
        QMessageBox::information(this, tr("Consulter"), tr("Sélectionnez d'abord une intervention."));
        return;
    }
    const Intervention &it = m_items[i];
    QMessageBox::information(this, tr("Intervention %1").arg(idText(it.id)),
        tr("Type : %1\nAdresse : %2\nDate : %3\nZone : %4\nGravité : %5\nStatut : %6")
            .arg(it.type, it.adresse, it.date.toString("dd/MM/yyyy HH:mm"), it.zone, it.gravite, it.statut));
}

void InterventionsPage::exportPdf()
{
    const QString path = QFileDialog::getSaveFileName(this, tr("Exporter en PDF"), "interventions.pdf", "PDF (*.pdf)");
    if (path.isEmpty())
        return;
    QString html = "<h2>Liste des interventions</h2><table border='1' cellspacing='0' cellpadding='5' width='100%'>"
                   "<tr bgcolor='#1f2d3d'><th><font color='white'>ID</font></th><th><font color='white'>Type</font></th>"
                   "<th><font color='white'>Adresse</font></th><th><font color='white'>Date et heure</font></th>"
                   "<th><font color='white'>Zone</font></th><th><font color='white'>Gravité</font></th>"
                   "<th><font color='white'>Statut</font></th></tr>";
    for (const Intervention &i : std::as_const(m_items))
        html += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td><td>%6</td><td>%7</td></tr>")
                    .arg(idText(i.id), i.type.toHtmlEscaped(), i.adresse.toHtmlEscaped(),
                         i.date.toString("dd/MM/yyyy HH:mm"), i.zone.toHtmlEscaped(), i.gravite, i.statut);
    html += "</table>";
    QPdfWriter writer(path);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageOrientation(QPageLayout::Landscape);
    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&writer);
    QMessageBox::information(this, tr("Exporter PDF"), tr("Fichier enregistré :\n%1").arg(path));
}

void InterventionsPage::showStats()
{
    QMap<QString, int> g, s;
    for (const Intervention &i : std::as_const(m_items)) { ++g[i.gravite]; ++s[i.statut]; }
    QString txt = tr("Total : %1 intervention(s)\n\nPar gravité :\n").arg(m_items.size());
    for (const QString &k : kGravites) txt += QString("  • %1 : %2\n").arg(k).arg(g.value(k));
    txt += tr("\nPar statut :\n");
    for (const QString &k : kStatuts) txt += QString("  • %1 : %2\n").arg(k).arg(s.value(k));
    QMessageBox::information(this, tr("Statistiques"), txt);
}

void InterventionsPage::autoAssign()
{
    int n = 0;
    for (const Intervention &i : std::as_const(m_items)) n += i.statut == "En attente";
    if (n == 0) {
        QMessageBox::information(this, tr("Affectation automatique"), tr("Aucune intervention en attente."));
        return;
    }
    if (QMessageBox::question(this, tr("Affectation automatique"),
                              tr("Affecter automatiquement les équipes aux %1 intervention(s) en attente ?").arg(n)) != QMessageBox::Yes)
        return;
    for (Intervention &i : m_items)
        if (i.statut == "En attente") i.statut = "En cours";
    rebuildTable();
}

void InterventionsPage::showRoute()
{
    QVector<Intervention> open;
    for (const Intervention &i : std::as_const(m_items))
        if (i.statut != "Terminée") open.push_back(i);
    if (open.isEmpty()) {
        QMessageBox::information(this, tr("Parcours recommandé"), tr("Aucune intervention à traiter."));
        return;
    }
    std::stable_sort(open.begin(), open.end(), [](const Intervention &a, const Intervention &b) {
        return graviteRank(a.gravite) > graviteRank(b.gravite);
    });
    QString txt = tr("Ordre de passage recommandé (gravité décroissante) :\n\n");
    for (int k = 0; k < open.size(); ++k)
        txt += QString("%1. %2 — %3 (%4)\n").arg(k + 1).arg(idText(open[k].id), open[k].adresse, open[k].gravite);
    QMessageBox::information(this, tr("Parcours recommandé"), txt);
}

QString InterventionsPage::answer(const QString &question) const
{
    const QString q = question.toLower();
    auto count = [this](const QString &s) { int n = 0; for (const Intervention &i : m_items) n += i.statut == s; return n; };
    if (q.contains("cours"))
        return tr("%1 intervention(s) sont en cours.").arg(count("En cours"));
    if (q.contains("attente"))
        return tr("%1 intervention(s) sont en attente.").arg(count("En attente"));
    if (q.contains("termin"))
        return tr("%1 intervention(s) sont terminées.").arg(count("Terminée"));
    if (q.contains("critique")) {
        int n = 0;
        for (const Intervention &i : m_items) n += i.gravite == "Critique" && i.statut != "Terminée";
        return tr("%1 intervention(s) critiques non terminées.").arg(n);
    }
    return tr("Essayez : « combien d'interventions en cours ? », « en attente ? » ou « critiques ? ».");
}
