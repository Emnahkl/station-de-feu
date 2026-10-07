#include "employespage.h"
#include "rightpanel.h"
#include "flowlayout.h"

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QMessageBox>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

namespace {

const QStringList kGrades = {"Sapeur", "Caporal", "Sergent", "Lieutenant", "Capitaine"};
const QStringList kSpecs = {"Fire", "Medic", "Logistics"};
const QStringList kStatuts = {"En service", "En pause", "En formation"};

int starsFor(const QString &grade)
{
    const int i = kGrades.indexOf(grade);
    return i < 0 ? 1 : qMin(3, i / 2 + 1);   // Sapeur/Caporal 1, Sergent/Lieutenant 2, Capitaine 3
}

QPixmap roundAvatar(int size)
{
    QPixmap src(":/icons/agent.png");
    QPixmap out(size, size);
    out.fill(Qt::transparent);
    QPainter p(&out);
    p.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addEllipse(0, 0, size, size);
    p.setClipPath(path);
    p.drawPixmap(0, 0, src.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    p.setClipping(false);
    p.setPen(QPen(QColor("#d1d5db"), 1.5));
    p.drawEllipse(QRectF(0.75, 0.75, size - 1.5, size - 1.5));
    return out;
}

QPixmap gradeBadge(const QString &grade)
{
    QPixmap badge(QString(":/icons/grade_%1.png").arg(starsFor(grade)));
    QPixmap out(badge.width(), badge.height() + 16);
    out.fill(Qt::transparent);
    QPainter p(&out);
    p.setRenderHint(QPainter::TextAntialiasing);
    p.drawPixmap(0, 0, badge);
    QFont f = p.font();
    f.setPixelSize(10);
    p.setFont(f);
    p.setPen(QColor("#6b7280"));
    p.drawText(QRect(0, badge.height(), out.width(), 16), Qt::AlignCenter, grade);
    return out.scaledToWidth(100, Qt::SmoothTransformation);
}

QWidget *centered(QWidget *w)
{
    auto *host = new QWidget;
    auto *lay = new QHBoxLayout(host);
    lay->setContentsMargins(4, 2, 4, 2);
    lay->setAlignment(Qt::AlignCenter);
    lay->addWidget(w);
    return host;
}

QPushButton *rowButton(const QString &text)
{
    auto *b = new QPushButton(text);
    b->setProperty("role", "rowbtn");
    b->setCursor(Qt::PointingHandCursor);
    return b;
}

} // namespace

EmployesPage::EmployesPage(QWidget *parent) : QWidget(parent)
{
    setObjectName("EmployesPage");
    loadSampleData();
    buildUi();
    rebuildTable();
}

void EmployesPage::loadSampleData()
{
    const QStringList grades = {"Capitaine", "Capitaine", "Lieutenant", "Sergent", "Caporal", "Sapeur",
                                "Sergent", "Capitaine", "Lieutenant", "Caporal", "Sapeur", "Sergent"};
    const QStringList specs = {"Fire", "Medic", "Medic", "Logistics", "Medic", "Logistics",
                               "Logistics", "Fire", "Logistics", "Fire", "Medic", "Fire"};
    const QStringList statuts = {"En service", "En pause", "En pause", "En formation", "En pause", "En formation",
                                 "En formation", "En service", "En pause", "En service", "En formation", "En pause"};
    for (int i = 0; i < 12; ++i)
        m_agents.push_back({m_nextId++, QString("Agent %1").arg(i + 1), grades[i], specs[i], statuts[i]});
}

void EmployesPage::buildUi()
{
    auto *root = new QHBoxLayout(this);
    root->setContentsMargins(18, 14, 18, 18);
    root->setSpacing(16);

    auto *left = new QVBoxLayout;
    left->setSpacing(12);
    root->addLayout(left, 1);

    // --------------------------------------------------------------- Filtres
    auto *filters = new QFrame;
    filters->setObjectName("filtersFrame");
    auto *fl = new QHBoxLayout(filters);
    fl->setContentsMargins(14, 10, 14, 10);
    fl->setSpacing(10);
    fl->addWidget(new QLabel(tr("Filtrer par Grade")));
    m_comboGrade = new QComboBox;
    m_comboGrade->addItem(tr("Tous les grades"));
    m_comboGrade->addItems(kGrades);
    fl->addWidget(m_comboGrade);
    fl->addWidget(new QLabel(tr("Filtrer par Spécialité")));
    m_comboSpec = new QComboBox;
    m_comboSpec->addItem(tr("Toutes les spécialités"));
    m_comboSpec->addItems(kSpecs);
    fl->addWidget(m_comboSpec);
    fl->addStretch(1);
    left->addWidget(filters);

    // --------------------------------------------------------------- Actions
    auto *actions = new QFrame;
    actions->setObjectName("actionsFrame");
    auto *al = new FlowLayout(actions, 10, 10, 8);
    struct Act { const char *obj; const char *text; const char *icon; };
    const Act acts[] = {
        {"btnScan", "Scanner QRCode\n(Check-in)", ":/icons/btn_scan.png"},
        {"btnFace", "Face ID\nLogin", ":/icons/btn_face.png"},
        {"btnAddVol", "Ajouter un\nVolontaire", ":/icons/btn_addvol.png"},
        {"btnAddEmp", "Ajouter un\nEmployé", ":/icons/btn_addemp.png"},
        {"btnDelEmp", "Supprimer\nun Employé", ":/icons/btn_delemp.png"},
    };
    QMap<QString, QPushButton *> btn;
    for (const Act &a : acts) {
        auto *b = new QPushButton(QString::fromUtf8(a.text));
        b->setObjectName(a.obj);
        b->setIcon(QIcon(a.icon));
        b->setIconSize(QSize(40, 40));
        b->setCursor(Qt::PointingHandCursor);
        al->addWidget(b);
        btn[a.obj] = b;
    }
    left->addWidget(actions);

    // --------------------------------------------------------------- Tableau
    auto *tableFrame = new QFrame;
    tableFrame->setObjectName("tableFrame");
    auto *tl = new QVBoxLayout(tableFrame);
    tl->setContentsMargins(0, 0, 0, 0);
    m_table = new QTableWidget;
    m_table->setObjectName("tableEmployes");
    m_table->setColumnCount(7);
    m_table->setHorizontalHeaderLabels({tr("Avatar"), tr("Nom"), tr("Grade"), tr("Spécialité"),
                                        tr("Statut de Présence"), tr("Code QR"), tr("Emploi du temps")});
    m_table->setFrameShape(QFrame::NoFrame);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setShowGrid(false);
    m_table->setAlternatingRowColors(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->verticalHeader()->setDefaultSectionSize(63);
    auto *h = m_table->horizontalHeader();
    h->setSectionResizeMode(QHeaderView::Fixed);
    h->setSectionResizeMode(1, QHeaderView::Stretch);
    h->setDefaultAlignment(Qt::AlignCenter);
    h->setFixedHeight(44);
    const int widths[] = {60, 0, 120, 115, 128, 66, 216};
    for (int c = 0; c < 7; ++c)
        if (widths[c]) m_table->setColumnWidth(c, widths[c]);
    tl->addWidget(m_table);
    left->addWidget(tableFrame, 1);

    // --------------------------------------------------------------- Panneau droit
    RightPanel::Config cfg;
    cfg.chatTitle = tr("IA Chatbot RH");
    cfg.greeting = tr("Bonjour ! Je suis l'assistant RH. Demandez-moi par exemple : « combien d'agents en service ? »");
    cfg.planTitle = tr("Smart Planning IA");
    cfg.btnNotify = tr("Notifier Lt. Médecin");
    cfg.btnAlert = tr("Alerte Grade S/L");
    cfg.btnGenerate = tr("Générer Planning");
    m_panel = new RightPanel(cfg);
    m_panel->setResponder([this](const QString &q) { return answer(q); });
    root->addWidget(m_panel);

    // --------------------------------------------------------------- Connexions
    connect(m_comboGrade, &QComboBox::currentIndexChanged, this, [this] { rebuildTable(); });
    connect(m_comboSpec, &QComboBox::currentIndexChanged, this, [this] { rebuildTable(); });
    connect(btn["btnAddEmp"], &QPushButton::clicked, this, [this] { addAgent(tr("Ajouter un employé")); });
    connect(btn["btnAddVol"], &QPushButton::clicked, this, [this] { addAgent(tr("Ajouter un volontaire")); });
    connect(btn["btnDelEmp"], &QPushButton::clicked, this, [this] { removeCurrent(); });
    connect(btn["btnScan"], &QPushButton::clicked, this, [this] {
        QMessageBox::information(this, tr("Scanner QRCode"), tr("Présentez le QR code de l'agent devant la caméra (check-in)."));
    });
    connect(btn["btnFace"], &QPushButton::clicked, this, [this] {
        QMessageBox::information(this, tr("Face ID"), tr("Reconnaissance faciale : module à brancher."));
    });
}

void EmployesPage::setSearch(const QString &text)
{
    m_search = text.trimmed();
    rebuildTable();
}

int EmployesPage::indexOfId(int id) const
{
    for (int i = 0; i < m_agents.size(); ++i)
        if (m_agents[i].id == id) return i;
    return -1;
}

int EmployesPage::currentAgentId() const
{
    const int row = m_table->currentRow();
    if (row < 0) return -1;
    auto *it = m_table->item(row, 1);
    return it ? it->data(Qt::UserRole).toInt() : -1;
}

void EmployesPage::rebuildTable()
{
    const QString grade = m_comboGrade->currentIndex() > 0 ? m_comboGrade->currentText() : QString();
    const QString spec = m_comboSpec->currentIndex() > 0 ? m_comboSpec->currentText() : QString();

    QVector<const Agent *> shown;
    for (const Agent &a : std::as_const(m_agents)) {
        if (!grade.isEmpty() && a.grade != grade) continue;
        if (!spec.isEmpty() && a.specialite != spec) continue;
        if (!m_search.isEmpty()
            && !a.nom.contains(m_search, Qt::CaseInsensitive)
            && !a.grade.contains(m_search, Qt::CaseInsensitive)
            && !a.specialite.contains(m_search, Qt::CaseInsensitive)
            && !a.statut.contains(m_search, Qt::CaseInsensitive)) continue;
        shown.push_back(&a);
    }

    m_table->clearContents();
    m_table->setRowCount(shown.size());
    for (int r = 0; r < shown.size(); ++r) {
        const Agent &a = *shown[r];
        const int id = a.id;

        auto *avatar = new QLabel;
        avatar->setPixmap(roundAvatar(40));
        m_table->setCellWidget(r, 0, centered(avatar));

        auto *name = new QTableWidgetItem(a.nom);
        name->setData(Qt::UserRole, id);
        m_table->setItem(r, 1, name);
        for (int c : {0, 2, 3, 4, 5, 6}) {
            auto *filler = new QTableWidgetItem;
            filler->setData(Qt::UserRole, id);
            m_table->setItem(r, c, filler);
        }

        auto *grd = new QLabel;
        grd->setPixmap(gradeBadge(a.grade));
        m_table->setCellWidget(r, 2, centered(grd));

        auto *specW = new QWidget;
        auto *sl = new QHBoxLayout(specW);
        sl->setContentsMargins(4, 0, 0, 0);
        sl->setSpacing(6);
        auto *si = new QLabel;
        si->setPixmap(QPixmap(QString(":/icons/spec_%1.png").arg(a.specialite.toLower())).scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        auto *st = new QLabel(a.specialite);
        st->setObjectName("specText");
        sl->addWidget(si);
        sl->addWidget(st);
        sl->addStretch(1);
        m_table->setCellWidget(r, 3, specW);

        auto *chip = new QLabel(a.statut);
        chip->setAlignment(Qt::AlignCenter);
        chip->setObjectName(a.statut == "En service" ? "chipService" : a.statut == "En pause" ? "chipPause" : "chipFormation");
        chip->setFixedSize(88, 24);
        m_table->setCellWidget(r, 4, centered(chip));

        auto *qr = new QLabel;
        qr->setPixmap(QPixmap(QString(":/icons/qr_%1.png").arg((id - 1) % 12 + 1)).scaled(46, 46, Qt::KeepAspectRatio, Qt::FastTransformation));
        m_table->setCellWidget(r, 5, centered(qr));

        auto *act = new QWidget;
        auto *ll = new QHBoxLayout(act);
        ll->setContentsMargins(4, 0, 4, 0);
        ll->setSpacing(4);
        auto *bV = rowButton(tr("Visualiser"));
        auto *bM = rowButton(tr("Modifier"));
        auto *bA = rowButton(tr("Assigner"));
        ll->addWidget(bV);
        ll->addWidget(bM);
        ll->addWidget(bA);
        m_table->setCellWidget(r, 6, act);
        connect(bV, &QPushButton::clicked, this, [this, id] { showAgent(id); });
        connect(bM, &QPushButton::clicked, this, [this, id] {
            const int i = indexOfId(id);
            if (i >= 0) {
                Agent copy = m_agents[i];
                if (editAgent(copy, tr("Modifier l'employé"))) {
                    m_agents[i] = copy;
                    rebuildTable();
                }
            }
        });
        connect(bA, &QPushButton::clicked, this, [this, id] { assignAgent(id); });
    }
}

bool EmployesPage::editAgent(Agent &agent, const QString &title)
{
    QDialog dlg(this);
    dlg.setWindowTitle(title);
    auto *form = new QFormLayout(&dlg);
    auto *nom = new QLineEdit(agent.nom);
    auto *grade = new QComboBox;
    grade->addItems(kGrades);
    grade->setCurrentText(agent.grade);
    auto *spec = new QComboBox;
    spec->addItems(kSpecs);
    spec->setCurrentText(agent.specialite);
    auto *statut = new QComboBox;
    statut->addItems(kStatuts);
    statut->setCurrentText(agent.statut);
    form->addRow(tr("Nom"), nom);
    form->addRow(tr("Grade"), grade);
    form->addRow(tr("Spécialité"), spec);
    form->addRow(tr("Statut"), statut);
    auto *bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form->addRow(bb);
    connect(bb, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(bb, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    if (dlg.exec() != QDialog::Accepted)
        return false;
    if (nom->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, title, tr("Le nom est obligatoire."));
        return false;
    }
    agent.nom = nom->text().trimmed();
    agent.grade = grade->currentText();
    agent.specialite = spec->currentText();
    agent.statut = statut->currentText();
    return true;
}

void EmployesPage::addAgent(const QString &title)
{
    Agent a{m_nextId, QString("Agent %1").arg(m_nextId), kGrades.first(), kSpecs.first(), kStatuts.first()};
    if (editAgent(a, title)) {
        ++m_nextId;
        m_agents.push_back(a);
        rebuildTable();
        m_table->scrollToBottom();
    }
}

void EmployesPage::removeCurrent()
{
    const int id = currentAgentId();
    const int i = indexOfId(id);
    if (i < 0) {
        QMessageBox::information(this, tr("Supprimer"), tr("Sélectionnez d'abord un employé dans le tableau."));
        return;
    }
    if (QMessageBox::question(this, tr("Supprimer"), tr("Supprimer « %1 » ?").arg(m_agents[i].nom)) == QMessageBox::Yes) {
        m_agents.removeAt(i);
        rebuildTable();
    }
}

void EmployesPage::showAgent(int id)
{
    const int i = indexOfId(id);
    if (i < 0) return;
    const Agent &a = m_agents[i];
    QMessageBox::information(this, tr("Fiche employé"),
        tr("Nom : %1\nGrade : %2\nSpécialité : %3\nStatut : %4\nCode QR : AGENT-%5")
            .arg(a.nom, a.grade, a.specialite, a.statut).arg(a.id, 3, 10, QLatin1Char('0')));
}

void EmployesPage::assignAgent(int id)
{
    const int i = indexOfId(id);
    if (i < 0) return;
    QMessageBox::information(this, tr("Assigner"),
        tr("%1 est assigné(e) au prochain créneau du planning (simulation).").arg(m_agents[i].nom));
}

QString EmployesPage::answer(const QString &question) const
{
    const QString q = question.toLower();
    auto count = [this](const QString &s) { int n = 0; for (const Agent &a : m_agents) n += a.statut == s; return n; };
    if (q.contains("service"))
        return tr("%1 agent(s) sont actuellement en service.").arg(count("En service"));
    if (q.contains("pause"))
        return tr("%1 agent(s) sont en pause.").arg(count("En pause"));
    if (q.contains("formation"))
        return tr("%1 agent(s) sont en formation.").arg(count("En formation"));
    if (q.contains("combien") || q.contains("total") || q.contains("agents"))
        return tr("L'effectif total est de %1 agents.").arg(m_agents.size());
    return tr("Essayez : « combien d'agents en service ? », « en pause ? » ou « en formation ? ».");
}
