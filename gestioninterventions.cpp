#include "gestioninterventions.h"
#include "ui_gestioninterventions.h"
#include "graphiques.h"

#include <QAbstractTextDocumentLayout>
#include <QBrush>
#include <QColor>
#include <QComboBox>
#include <QDateEdit>
#include <QDateTime>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFont>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QKeySequence>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPageLayout>
#include <QPageSize>
#include <QPainter>
#include <QPdfWriter>
#include <QPixmap>
#include <QPolygonF>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QShortcut>
#include <QSignalBlocker>
#include <QStyle>
#include <QTableWidgetItem>
#include <QTextDocument>
#include <QTimeEdit>
#include <QTimer>
#include <QUrl>
#include <algorithm>

namespace {

QStringList listeTypes()    { return Intervention::types(); }
QStringList listeGravites() { return Intervention::gravites(); }
QStringList listeStatuts()  { return Intervention::statuts(); }

QColor couleurGravite(const QString &g)
{
    if (g == "Critique") return QColor("#b91c1c");
    if (g == "Élevée")   return QColor("#ea580c");
    if (g == "Moyenne")  return QColor("#b45309");
    return QColor("#15803d");
}

QColor couleurStatut(const QString &s)
{
    if (s == "En cours")   return QColor("#b45309");
    if (s == "En attente") return QColor("#b91c1c");
    return QColor("#15803d");
}

// ---- Données des graphiques (écran + PDF) ----
QVector<DonneeGraphique> donneesStatut(const QVector<Intervention> &l)
{
    QVector<DonneeGraphique> d;
    const QStringList s = listeStatuts();
    for (int i = 0; i < s.size(); ++i) {
        int n = 0;
        for (int k = 0; k < l.size(); ++k) if (l[k].getStatut() == s[i]) ++n;
        QColor c = (s[i] == "En cours") ? QColor("#f59e0b")
                 : (s[i] == "En attente") ? QColor("#ef4444") : QColor("#22c55e");
        DonneeGraphique g = { s[i], double(n), c };
        d.push_back(g);
    }
    return d;
}

QVector<DonneeGraphique> donneesGravite(const QVector<Intervention> &l)
{
    QVector<DonneeGraphique> d;
    const QStringList s = listeGravites();
    const QColor couleurs[] = { QColor("#22c55e"), QColor("#f59e0b"), QColor("#ea580c"), QColor("#b91c1c") };
    for (int i = 0; i < s.size(); ++i) {
        int n = 0;
        for (int k = 0; k < l.size(); ++k) if (l[k].getGravite() == s[i]) ++n;
        DonneeGraphique g = { s[i], double(n), couleurs[i] };
        d.push_back(g);
    }
    return d;
}

QVector<DonneeGraphique> donneesType(const QVector<Intervention> &l)
{
    QMap<QString, int> compte;
    for (int k = 0; k < l.size(); ++k) compte[l[k].getType()]++;
    QVector<QPair<int, QString> > tri;
    for (QMap<QString, int>::const_iterator it = compte.constBegin(); it != compte.constEnd(); ++it)
        tri.push_back(qMakePair(it.value(), it.key()));
    std::sort(tri.begin(), tri.end(), [](const QPair<int, QString> &a, const QPair<int, QString> &b) {
        return a.first != b.first ? a.first > b.first : a.second < b.second;
    });
    const QColor palette[] = { QColor("#1f2d3d"), QColor("#b91c1c"), QColor("#2563eb"),
                               QColor("#f59e0b"), QColor("#16a34a"), QColor("#7c3aed") };
    QVector<DonneeGraphique> d;
    int autres = 0;
    for (int i = 0; i < tri.size(); ++i) {
        if (i < 6) {
            DonneeGraphique g = { tri[i].second, double(tri[i].first), palette[i] };
            d.push_back(g);
        } else {
            autres += tri[i].first;
        }
    }
    if (autres > 0) {
        DonneeGraphique g = { QObject::tr("Autres"), double(autres), QColor("#9ca3af") };
        d.push_back(g);
    }
    return d;
}

// ---- Petits outils d'interface ----
void repolir(QWidget *w)
{
    w->style()->unpolish(w);
    w->style()->polish(w);
    w->update();
}

// Propriété dynamique lue par la feuille de style du .ui : erreur="true" (bordure rouge),
// actif="true" (bouton de tri / statistiques enfoncé), etat="..." (couleur du bandeau)
void definirPropriete(QWidget *w, const char *nom, const QVariant &valeur)
{
    if (w->property(nom) == valeur)
        return;
    w->setProperty(nom, valeur);
    repolir(w);
}

QTime heureSansSecondes(const QTime &t)
{
    return QTime(t.hour(), t.minute());
}

// Icônes dessinées en code (coche et croix) pour les boutons Enregistrer / Annuler
QIcon iconeCoche(const QColor &c)
{
    QPixmap pm(36, 36);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(c, 4.2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    p.drawPolyline(QPolygonF() << QPointF(7, 19) << QPointF(15, 27) << QPointF(29, 10));
    p.end();
    return QIcon(pm);
}

QIcon iconeCroix(const QColor &c)
{
    QPixmap pm(36, 36);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(c, 4.2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    p.drawLine(QPointF(9, 9), QPointF(27, 27));
    p.drawLine(QPointF(27, 9), QPointF(9, 27));
    p.end();
    return QIcon(pm);
}

} // namespace

// =====================================================================================
//  Construction
// =====================================================================================
GestionInterventions::GestionInterventions(QWidget *parent)
    : QWidget(parent), ui(new Ui::GestionInterventions), m_tri(TriId), m_idCroissant(true), m_dateCroissante(false),
      m_idAffectation(-1), m_mode(ModeListe), m_contexte(CtxAucun), m_idEdition(-1), m_ligneEdition(-1), m_touches(0),
      m_timerMessage(new QTimer(this))
{
    ui->setupUi(this);

    // Les propriétés posées par le .ui ne sont prises en compte par la feuille de style
    // qu'après un nouveau « polish »
    repolir(ui->bandeau);

    m_timerMessage->setSingleShot(true);
    connect(m_timerMessage, &QTimer::timeout, this, [this]() {
        if (m_mode == ModeListe && m_contexte == CtxAucun)
            ui->bandeau->setVisible(false);
    });

    // ---- Tableau : Type / Date / Zone / Gravité / Statut ont une largeur fixe (assez large pour
    // y loger les champs de saisie), l'adresse prend tout le reste
    QTableWidget *t = ui->tableInterventions;
    t->setSelectionMode(QAbstractItemView::SingleSelection);
    t->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QHeaderView *entete = t->horizontalHeader();
    entete->setStretchLastSection(false);
    entete->setSectionResizeMode(QHeaderView::Fixed);
    entete->setSectionResizeMode(0, QHeaderView::ResizeToContents);   // ID
    entete->setSectionResizeMode(2, QHeaderView::Stretch);            // Adresse
    t->setColumnWidth(1, 185);   // Type
    t->setColumnWidth(3, 240);   // Date et heure
    t->setColumnWidth(4, 175);   // Zone
    t->setColumnWidth(5, 125);   // Gravité
    t->setColumnWidth(6, 125);   // Statut
    t->setTextElideMode(Qt::ElideRight);
    connect(t, &QTableWidget::itemSelectionChanged, this, [this]() {
        if (m_contexte == CtxDetail) {          // la fiche suit la ligne sélectionnée
            if (idCourant() >= 0) consulter(); else masquerBandeau();
        }
        majBoutons();
    });
    connect(t, &QTableWidget::cellDoubleClicked, this, [this]() {
        if (m_mode == ModeListe && m_contexte != CtxSuppression)
            modifier();
    });

    // ---- Boutons -> slots
    connect(ui->btnAjouter,       &QPushButton::clicked, this, &GestionInterventions::ajouter);
    connect(ui->btnModifier,      &QPushButton::clicked, this, &GestionInterventions::modifier);
    connect(ui->btnSupprimer,     &QPushButton::clicked, this, &GestionInterventions::supprimer);
    connect(ui->btnConsulter,     &QPushButton::clicked, this, &GestionInterventions::consulter);
    connect(ui->btnExportPdf,     &QPushButton::clicked, this, &GestionInterventions::exporterPdf);
    connect(ui->btnTriDate,       &QPushButton::clicked, this, &GestionInterventions::trierParDate);
    connect(ui->btnTriId,         &QPushButton::clicked, this, &GestionInterventions::trierParId);
    connect(ui->btnStats,         &QPushButton::clicked, this, &GestionInterventions::afficherStats);
    connect(ui->btnRetourListe,   &QPushButton::clicked, this, [this]() { montrerListe(); });
    connect(ui->btnRetourParcours, &QPushButton::clicked, this, [this]() { montrerListe(); });
    connect(ui->btnAffectation,   &QPushButton::clicked, this, &GestionInterventions::afficherAffectation);
    connect(ui->btnRetourAffectation, &QPushButton::clicked, this, [this]() { montrerListe(); });
    connect(ui->btnValiderAffectation, &QPushButton::clicked, this, &GestionInterventions::validerAffectation);
    ui->btnValiderAffectation->setIcon(iconeCoche(Qt::white));
    ui->btnValiderAffectation->setIconSize(QSize(18, 18));
    connect(ui->btnParcours,      &QPushButton::clicked, this, &GestionInterventions::afficherParcours);
    connect(ui->btnPower,         &QPushButton::clicked, this, &GestionInterventions::close);
    connect(ui->btnEmployes,      &QPushButton::clicked, this, &GestionInterventions::ouvrirEmployes);   // integration
    connect(ui->btnBandeauOk,     &QPushButton::clicked, this, &GestionInterventions::bandeauValider);
    connect(ui->btnBandeauAnnuler, &QPushButton::clicked, this, &GestionInterventions::bandeauAnnuler);

    // ---- Filtres + recherche (le champ de recherche est limité : 50 caractères)
    connect(ui->comboGravite, SIGNAL(currentIndexChanged(int)), this, SLOT(rafraichir()));
    connect(ui->comboStatut,  SIGNAL(currentIndexChanged(int)), this, SLOT(rafraichir()));
    connect(ui->searchEdit,   SIGNAL(textChanged(QString)),     this, SLOT(rafraichir()));
    ui->searchEdit->setMaxLength(50);

    // ---- Icônes dessinées en code
    ui->btnBandeauOk->setIcon(iconeCoche(Qt::white));
    ui->btnBandeauAnnuler->setIcon(iconeCroix(QColor("#6b7280")));
    ui->btnBandeauOk->setIconSize(QSize(18, 18));
    ui->btnBandeauAnnuler->setIconSize(QSize(16, 16));
    ui->btnBandeauOk->setCursor(Qt::PointingHandCursor);
    ui->btnBandeauAnnuler->setCursor(Qt::PointingHandCursor);

    // ---- Raccourcis clavier
    QShortcut *nouveau = new QShortcut(QKeySequence("Ctrl+N"), this);
    connect(nouveau, &QShortcut::activated, this, [this]() { if (ui->btnAjouter->isEnabled()) ajouter(); });
    QShortcut *f2 = new QShortcut(QKeySequence(Qt::Key_F2), this);
    connect(f2, &QShortcut::activated, this, [this]() { if (ui->btnModifier->isEnabled()) modifier(); });
    QShortcut *suppr = new QShortcut(QKeySequence(Qt::Key_Delete), ui->tableInterventions, nullptr, nullptr,
                                     Qt::WidgetShortcut);
    connect(suppr, &QShortcut::activated, this, [this]() { if (ui->btnSupprimer->isEnabled()) supprimer(); });
    QShortcut *pdf = new QShortcut(QKeySequence("Ctrl+P"), this);
    connect(pdf, &QShortcut::activated, this, [this]() { if (ui->btnExportPdf->isEnabled()) exporterPdf(); });
    QShortcut *stats = new QShortcut(QKeySequence("Ctrl+T"), this);
    connect(stats, &QShortcut::activated, this, [this]() { if (ui->btnStats->isEnabled()) afficherStats(); });
    QShortcut *chercher = new QShortcut(QKeySequence("Ctrl+F"), this);
    connect(chercher, &QShortcut::activated, this, [this]() {
        if (ui->searchEdit->isEnabled()) { ui->searchEdit->setFocus(); ui->searchEdit->selectAll(); }
    });
    // Entrée : enregistre la ligne en cours d'édition ; Échap : annule l'édition / la confirmation / la fiche
    QShortcut *entree1 = new QShortcut(QKeySequence(Qt::Key_Return), this);
    QShortcut *entree2 = new QShortcut(QKeySequence(Qt::Key_Enter), this);
    connect(entree1, &QShortcut::activated, this, [this]() { if (m_mode != ModeListe) enregistrerEdition(); });
    connect(entree2, &QShortcut::activated, this, [this]() { if (m_mode != ModeListe) enregistrerEdition(); });
    QShortcut *echap = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(echap, &QShortcut::activated, this, [this]() { if (m_contexte != CtxAucun) bandeauAnnuler(); });

    chargerZones();
    rafraichir();
}

GestionInterventions::~GestionInterventions()
{
    delete ui;
}

void GestionInterventions::marquerMenu()
{
    ui->btnInterventions->setChecked(true);
}

int GestionInterventions::idCourant() const
{
    const int row = ui->tableInterventions->currentRow();
    if (row < 0 || !ui->tableInterventions->item(row, 0) || !ui->tableInterventions->item(row, 0)->isSelected())
        return -1;
    return ui->tableInterventions->item(row, 0)->data(Qt::UserRole).toInt();
}

// =====================================================================================
//  Zones (clé étrangère ID_ZONE)
// =====================================================================================
void GestionInterventions::chargerZones()
{
    m_zones.clear();
    const QVector<QPair<int, QString> > zones = Intervention::zones();
    for (int i = 0; i < zones.size(); ++i)
        m_zones.insert(zones[i].first, zones[i].second);
    if (zones.isEmpty())
        message(tr("Aucune zone de couverture dans la base : exécutez d'abord sql/safestation_bd.sql. %1")
                    .arg(Intervention::derniereErreur()), EtatErreur);
}

QString GestionInterventions::libelleZone(int idZone) const
{
    if (m_zones.contains(idZone))
        return QString("%1 - %2").arg(idZone).arg(m_zones.value(idZone));
    return QString::number(idZone);
}

// =====================================================================================
//  Bandeau : messages, Enregistrer / Annuler, confirmation de suppression, fiche
// =====================================================================================
// « html » est du HTML (les parties variables doivent être échappées par l'appelant)
void GestionInterventions::bandeau(const QString &html, Etat etat, const QString &boutonOk,
                                   const QString &boutonAnnuler, bool permanent)
{
    const char *v = (etat == EtatErreur) ? "erreur" : (etat == EtatOk) ? "ok"
                  : (etat == EtatEdition) ? "edition" : (etat == EtatDanger) ? "danger" : "info";
    if (ui->bandeau->property("etat").toString() != v) {
        ui->bandeau->setProperty("etat", v);
        repolir(ui->bandeau);
        repolir(ui->lblMessage);
        repolir(ui->btnBandeauOk);
        repolir(ui->btnBandeauAnnuler);
    }
    ui->lblMessage->setText(html);
    ui->btnBandeauOk->setText(boutonOk);
    ui->btnBandeauOk->setVisible(!boutonOk.isEmpty());
    ui->btnBandeauOk->setIcon(etat == EtatDanger ? QIcon() : iconeCoche(Qt::white));
    ui->btnBandeauAnnuler->setText(boutonAnnuler);
    ui->btnBandeauAnnuler->setVisible(!boutonAnnuler.isEmpty());
    ui->bandeau->setVisible(true);
    if (permanent)
        m_timerMessage->stop();
}

// Message temporaire (texte simple). Sans effet pendant une édition ou une confirmation.
void GestionInterventions::message(const QString &texte, Etat etat)
{
    if (m_mode != ModeListe || m_contexte == CtxSuppression)
        return;
    m_contexte = CtxAucun;
    bandeau(texte.toHtmlEscaped(), etat, QString(), QString(), false);
    m_timerMessage->start(8000);
}

void GestionInterventions::masquerBandeau()
{
    m_timerMessage->stop();
    ui->bandeau->setVisible(false);
    if (m_mode == ModeListe)
        m_contexte = CtxAucun;
}

void GestionInterventions::bandeauValider()
{
    if (m_contexte == CtxEdition)
        enregistrerEdition();
    else if (m_contexte == CtxSuppression)
        confirmerSuppression();
}

void GestionInterventions::bandeauAnnuler()
{
    if (m_contexte == CtxEdition) {
        annulerEdition();
    } else {                                   // confirmation de suppression ou fiche : on referme
        masquerBandeau();
        majBoutons();
    }
}

// =====================================================================================
//  Boutons : actifs seulement quand l'action a un sens
// =====================================================================================
void GestionInterventions::majBoutons()
{
    const bool enEdition    = (m_mode != ModeListe);
    const bool confirmation = (m_contexte == CtxSuppression);
    const int  page         = ui->stackedMain->currentIndex();   // 0 liste, 1 statistiques, 2 parcours, 3 affectation
    const bool liste        = (page == 0);
    const bool libre        = !enEdition && !confirmation;
    const bool selection    = liste && idCourant() >= 0;

    ui->btnAjouter->setEnabled(libre);
    ui->btnModifier->setEnabled(libre && selection);
    ui->btnConsulter->setEnabled(libre && selection);
    ui->btnSupprimer->setEnabled(libre && selection);
    ui->btnAffectation->setEnabled(libre && (selection || page == 3));
    ui->btnParcours->setEnabled(libre);
    ui->btnStats->setEnabled(libre);
    ui->btnExportPdf->setEnabled(libre && liste);
    ui->btnRetourListe->setEnabled(libre);
    ui->btnRetourParcours->setEnabled(libre);
    ui->btnRetourAffectation->setEnabled(libre);
    ui->btnValiderAffectation->setEnabled(libre && page == 3);

    // filtres, tri et recherche : seulement sur la liste, hors édition
    const bool filtres = libre && liste;
    ui->comboGravite->setEnabled(filtres);
    ui->comboStatut->setEnabled(filtres);
    ui->btnTriId->setEnabled(filtres);
    ui->btnTriDate->setEnabled(filtres);
    ui->searchEdit->setEnabled(filtres);

    ui->tableInterventions->setEnabled(!confirmation);
    definirPropriete(ui->btnStats, "actif", page == 1);
    definirPropriete(ui->btnParcours, "actif", page == 2);
    definirPropriete(ui->btnAffectation, "actif", page == 3);
}

void GestionInterventions::montrerListe()
{
    ui->stackedMain->setCurrentIndex(0);
    majBoutons();
}

// =====================================================================================
//  READ (afficher)
// =====================================================================================
void GestionInterventions::rafraichir()
{
    if (m_mode != ModeListe)
        return;                          // pas de rechargement pendant la saisie d'une ligne

    const QString g = ui->comboGravite->currentIndex() > 0 ? ui->comboGravite->currentText() : QString();
    const QString s = ui->comboStatut->currentIndex() > 0 ? ui->comboStatut->currentText() : QString();
    const int idSelection = idCourant();

    Intervention::cloturerPassees();     // date dépassée => « Terminée »
    m_liste = Intervention::afficher(g, s, ui->searchEdit->text());

    if (m_tri == TriDate) {
        std::stable_sort(m_liste.begin(), m_liste.end(),
                         [this](const Intervention &a, const Intervention &b) {
                             return m_dateCroissante ? a.dateHeure() < b.dateHeure()
                                                     : a.dateHeure() > b.dateHeure();
                         });
    } else {
        std::stable_sort(m_liste.begin(), m_liste.end(),
                         [this](const Intervention &a, const Intervention &b) {
                             return m_idCroissant ? a.getId() < b.getId() : a.getId() > b.getId();
                         });
    }

    QTableWidget *t = ui->tableInterventions;
    t->blockSignals(true);               // la reconstruction du tableau ne doit pas déclencher de slots
    t->setRowCount(0);
    t->setRowCount(m_liste.size());
    int ligneSelection = -1;
    for (int r = 0; r < m_liste.size(); ++r) {
        const Intervention &it = m_liste[r];
        if (it.getId() == idSelection)
            ligneSelection = r;
        QStringList cellules;
        cellules << QString::number(it.getId())
                 << it.getType()
                 << it.getAdresse()
                 << it.getDate().toString("dd/MM/yyyy") + "  " + it.getHeure().toString("HH:mm")
                 << libelleZone(it.getIdZone())
                 << QString::fromUtf8("\xE2\x97\x8F ") + it.getGravite()      // ● Critique
                 << QString::fromUtf8("\xE2\x97\x8F ") + it.getStatut();
        for (int c = 0; c < cellules.size(); ++c) {
            QTableWidgetItem *item = new QTableWidgetItem(cellules[c]);
            item->setData(Qt::UserRole, it.getId());
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            if (c == 2)
                item->setToolTip(it.getAdresse());
            if (c == 5 || c == 6) {
                item->setForeground(QBrush(c == 5 ? couleurGravite(it.getGravite())
                                                  : couleurStatut(it.getStatut())));
                QFont f = item->font();
                f.setBold(true);
                item->setFont(f);
            }
            t->setItem(r, c, item);
        }
    }
    if (ligneSelection >= 0)
        t->selectRow(ligneSelection);    // la ligne choisie reste surlignée
    t->blockSignals(false);

    ui->lblCompte->setText(m_liste.isEmpty() ? tr("Aucune intervention")
                           : m_liste.size() == 1 ? tr("1 intervention")
                           : tr("%1 interventions").arg(m_liste.size()));
    mettreAJourCartes();
    mettreAJourBoutonsTri();
    if (m_contexte == CtxDetail) {       // la fiche ouverte doit refléter la liste
        if (idCourant() >= 0) consulter(); else masquerBandeau();
    }
    majBoutons();
}

// Les cartes (et les statistiques) comptent TOUTES les interventions (sans filtre)
void GestionInterventions::mettreAJourCartes()
{
    int enCours = 0, enAttente = 0, terminee = 0;
    const QVector<Intervention> toutes = Intervention::afficher();
    for (int i = 0; i < toutes.size(); ++i) {
        const QString &s = toutes[i].getStatut();
        if (s == "En cours") ++enCours;
        else if (s == "En attente") ++enAttente;
        else if (s == "Terminée") ++terminee;
    }
    ui->lblTotal->setText(QString::number(toutes.size()));
    ui->lblEnCours->setText(QString::number(enCours));
    ui->lblEnAttente->setText(QString::number(enAttente));
    ui->lblTerminee->setText(QString::number(terminee));

    if (ui->stackedMain->currentIndex() == 1)
        mettreAJourStats(toutes);
    else if (ui->stackedMain->currentIndex() == 2)
        mettreAJourParcours();
}

void GestionInterventions::mettreAJourBoutonsTri()
{
    const bool parId = (m_tri == TriId);
    ui->btnTriId->setText(parId ? (m_idCroissant ? QString::fromUtf8("ID  \xE2\x96\xB2")      // ▲
                                                  : QString::fromUtf8("ID  \xE2\x96\xBC"))     // ▼
                                : QString("ID"));
    ui->btnTriDate->setText(parId ? QString("Date")
                                  : (m_dateCroissante ? QString::fromUtf8("Date  \xE2\x96\xB2")     // ▲ plus ancien d'abord
                                                      : QString::fromUtf8("Date  \xE2\x96\xBC")));  // ▼ plus récent d'abord
    definirPropriete(ui->btnTriId, "actif", parId);
    definirPropriete(ui->btnTriDate, "actif", !parId);
}

// =====================================================================================
//  Édition DIRECTEMENT dans le tableau (ajout = ligne vide en haut, modification = ligne choisie)
// =====================================================================================
QWidget *GestionInterventions::envelopper(QWidget *a, QWidget *b)
{
    QFrame *cellule = new QFrame;
    cellule->setObjectName("celluleEdition");
    QHBoxLayout *l = new QHBoxLayout(cellule);
    l->setContentsMargins(6, 5, 6, 5);
    l->setSpacing(6);
    l->addWidget(a);
    if (b)
        l->addWidget(b);
    return cellule;
}

void GestionInterventions::demarrerEdition(Mode mode, const Intervention &modele)
{
    montrerListe();
    masquerBandeau();                                  // ferme la fiche ou le message éventuel
    chargerZones();                                    // zones à jour pour la liste déroulante
    if (m_zones.isEmpty())
        return;

    QTableWidget *t = ui->tableInterventions;
    t->blockSignals(true);
    int ligne;
    if (mode == ModeAjout) {
        t->clearSelection();
        t->insertRow(0);                               // ligne vide EN HAUT du tableau
        for (int c = 0; c < t->columnCount(); ++c)
            t->setItem(0, c, new QTableWidgetItem);
        t->item(0, 0)->setData(Qt::UserRole, -1);
        t->item(0, 0)->setText(tr("Nouveau"));
        QFont f = t->item(0, 0)->font();
        f.setItalic(true);
        t->item(0, 0)->setFont(f);
        ligne = 0;
    } else {
        ligne = t->currentRow();
    }
    t->item(ligne, 0)->setBackground(QBrush(QColor("#fff7e0")));
    for (int c = 1; c < t->columnCount(); ++c)         // le texte est remplacé par les champs de saisie
        if (t->item(ligne, c))
            t->item(ligne, c)->setText(QString());
    t->blockSignals(false);

    // ---- champs de saisie
    LigneEdition &e = m_ed;
    e.type = new QComboBox;
    e.type->addItems(listeTypes());
    if (!modele.getType().isEmpty() && e.type->findText(modele.getType()) < 0)
        e.type->addItem(modele.getType());             // ancienne valeur déjà en base

    e.adresse = new QLineEdit;
    e.adresse->setMaxLength(100);
    e.adresse->setPlaceholderText(tr("Ex : 12 Rue de la Liberté, Tunis"));
    e.adresse->setProperty("infoBulle", tr("Format : [numéro] type de voie + nom, ville (ex. 12 Rue de la Liberté, Tunis)"));
    e.adresse->setToolTip(e.adresse->property("infoBulle").toString());
    // lettres, chiffres, espace , . ' - / seulement (le reste est refusé dès la frappe)
    e.adresse->setValidator(new QRegularExpressionValidator(
        QRegularExpression("[\\p{L}0-9 ,.'\\x{2019}/-]*"), e.adresse));

    e.date = new QDateEdit;
    e.date->setDisplayFormat("dd/MM/yyyy");
    e.date->setCalendarPopup(true);
    e.date->setDateRange(Intervention::dateMinimale(), QDate::currentDate());   // ni futur, ni avant 2026

    e.heure = new QTimeEdit;
    e.heure->setDisplayFormat("HH:mm");
    e.heure->setButtonSymbols(QAbstractSpinBox::NoButtons);

    e.zone = new QComboBox;
    for (QMap<int, QString>::const_iterator it = m_zones.constBegin(); it != m_zones.constEnd(); ++it)
        e.zone->addItem(libelleZone(it.key()), it.key());
    if (mode == ModeModif && e.zone->findData(modele.getIdZone()) < 0)
        e.zone->addItem(libelleZone(modele.getIdZone()), modele.getIdZone());   // zone disparue

    e.gravite = new QComboBox;
    e.gravite->addItems(listeGravites());
    e.statut = new QComboBox;
    e.statut->addItems(listeStatuts());

    // ---- valeurs initiales
    if (mode == ModeAjout) {
        e.date->setDate(QDate::currentDate());
        e.heure->setTime(heureSansSecondes(QTime::currentTime()));
        e.zone->setCurrentIndex(0);
        e.gravite->setCurrentIndex(0);
        e.statut->setCurrentText("En attente");
    } else {
        e.type->setCurrentIndex(qMax(0, e.type->findText(modele.getType())));
        e.adresse->setText(modele.getAdresse());
        e.date->setDate(modele.getDate());
        e.heure->setTime(modele.getHeure());
        e.zone->setCurrentIndex(qMax(0, e.zone->findData(modele.getIdZone())));
        e.gravite->setCurrentIndex(qMax(0, e.gravite->findText(modele.getGravite())));
        e.statut->setCurrentIndex(qMax(0, e.statut->findText(modele.getStatut())));
    }

    t->setCellWidget(ligne, 1, envelopper(e.type));
    t->setCellWidget(ligne, 2, envelopper(e.adresse));
    t->setCellWidget(ligne, 3, envelopper(e.date, e.heure));
    t->setCellWidget(ligne, 4, envelopper(e.zone));
    t->setCellWidget(ligne, 5, envelopper(e.gravite));
    t->setCellWidget(ligne, 6, envelopper(e.statut));
    QWidget::setTabOrder(e.type, e.adresse);
    QWidget::setTabOrder(e.adresse, e.date);
    QWidget::setTabOrder(e.date, e.heure);
    QWidget::setTabOrder(e.heure, e.zone);
    QWidget::setTabOrder(e.zone, e.gravite);
    QWidget::setTabOrder(e.gravite, e.statut);

    // ---- contrôle de saisie EN DIRECT (on capture les pointeurs : sûrs si la ligne est détruite)
    connect(e.type, &QComboBox::currentTextChanged, this, [this]() {
        if (m_mode == ModeListe) return;
        m_touches |= ChType; verifierEdition();
    });
    connect(e.adresse, &QLineEdit::textChanged, this, [this]() {
        if (m_mode == ModeListe) return;
        m_touches |= ChAdresse; verifierEdition();
    });
    QLineEdit *champAdresse = e.adresse;
    connect(champAdresse, &QLineEdit::editingFinished, this, [this, champAdresse]() {
        if (m_mode == ModeListe) return;
        const QString propre = champAdresse->text().simplified();      // espaces en double supprimés
        if (propre != champAdresse->text())
            champAdresse->setText(propre);
    });
    connect(e.date, &QDateEdit::dateChanged, this, [this]() {
        if (m_mode == ModeListe) return;
        m_touches |= ChDate; verifierEdition();
    });
    connect(e.heure, &QTimeEdit::timeChanged, this, [this]() {
        if (m_mode == ModeListe) return;
        m_touches |= ChDate; verifierEdition();
    });
    connect(e.zone, &QComboBox::currentTextChanged, this, [this]() {
        if (m_mode == ModeListe) return;
        m_touches |= ChZone; verifierEdition();
    });
    connect(e.gravite, &QComboBox::currentTextChanged, this, [this]() { verifierEdition(); });
    connect(e.statut,  &QComboBox::currentTextChanged, this, [this]() { verifierEdition(); });

    // ---- état
    m_mode = mode;
    m_contexte = CtxEdition;
    m_idEdition = (mode == ModeModif) ? modele.getId() : -1;
    m_ligneEdition = ligne;
    m_touches = 0;
    t->setSelectionMode(QAbstractItemView::NoSelection);   // une seule ligne à la fois : celle qu'on édite
    t->scrollToItem(t->item(ligne, 0));
    verifierEdition();
    majBoutons();
    e.adresse->setFocus();
    e.adresse->deselect();
}

// Referme la ligne éditable (sans enregistrer) et recharge le tableau
void GestionInterventions::terminerEdition()
{
    QTableWidget *t = ui->tableInterventions;
    const int ligne = m_ligneEdition;
    const int idSelection = (m_mode == ModeModif) ? m_idEdition : -1;

    m_ed = LigneEdition();
    m_mode = ModeListe;                       // les lambdas des champs deviennent inactives
    m_contexte = CtxAucun;
    m_idEdition = -1;
    m_ligneEdition = -1;
    m_touches = 0;

    if (ligne >= 0)
        for (int c = 0; c < t->columnCount(); ++c)
            t->removeCellWidget(ligne, c);
    t->setSelectionMode(QAbstractItemView::SingleSelection);
    masquerBandeau();
    if (idSelection >= 0) {                   // la ligne modifiée reste sélectionnée après le rechargement
        for (int r = 0; r < t->rowCount(); ++r)
            if (t->item(r, 0) && t->item(r, 0)->data(Qt::UserRole).toInt() == idSelection) {
                t->blockSignals(true);
                t->selectRow(r);
                t->blockSignals(false);
                break;
            }
    }
    rafraichir();
}

void GestionInterventions::definirErreur(QWidget *champ, const QString &erreur, bool afficher)
{
    definirPropriete(champ, "erreur", afficher);
    const QString info = champ->property("infoBulle").toString();
    champ->setToolTip(afficher ? erreur : info);
}

// Renvoie true si la ligne est valide. strict = true (clic sur Enregistrer) : une adresse
// vide est aussi signalée en rouge (sinon on attend que l'utilisateur ait commencé à écrire).
bool GestionInterventions::verifierEdition(bool strict)
{
    if (m_mode == ModeListe || !m_ed.type)
        return false;
    LigneEdition &e = m_ed;

    const QString eT = Intervention::erreurType(e.type->currentText());
    const QString eA = Intervention::erreurAdresse(e.adresse->text());
    const QDate date = e.date->date();
    const QTime heure = heureSansSecondes(e.heure->time());
    const QString eD = Intervention::erreurDateHeure(date, heure);
    const QString eZ = (e.zone->currentIndex() < 0) ? tr("Choisissez une zone.") : QString();

    // Date passée => statut forcé à « Terminée » (et verrouillé)
    const bool passee = Intervention::estPassee(date);
    if (passee && e.statut->currentText() != "Terminée") {
        const QSignalBlocker blocage(e.statut);
        e.statut->setCurrentText("Terminée");
    }
    e.statut->setEnabled(!passee);

    const bool vT = !eT.isEmpty();
    const bool vA = !eA.isEmpty() && (strict || (m_touches & ChAdresse));
    const bool vD = !eD.isEmpty();
    const bool vZ = !eZ.isEmpty();
    definirErreur(e.type, eT, vT);
    definirErreur(e.adresse, eA, vA);
    definirErreur(e.date, eD, vD);
    definirErreur(e.heure, eD, vD);
    definirErreur(e.zone, eZ, vZ);

    const QString titre = (m_mode == ModeAjout)
            ? tr("Nouvelle intervention")
            : tr("Modification de l'intervention n° %1").arg(m_idEdition);

    QStringList lignes;
    if (vT) lignes << "<b>" + tr("Type") + " :</b> " + eT.toHtmlEscaped();
    if (vA) lignes << "<b>" + tr("Adresse") + " :</b> " + eA.toHtmlEscaped();
    if (vD) lignes << "<b>" + tr("Date et heure") + " :</b> " + eD.toHtmlEscaped();
    if (vZ) lignes << "<b>" + tr("Zone") + " :</b> " + eZ.toHtmlEscaped();

    if (lignes.isEmpty()) {
        QString aide = tr("Saisissez directement dans la ligne du tableau — <b>Entrée</b> pour enregistrer, "
                          "<b>Échap</b> pour annuler.");
        if (passee)
            aide += "<br>" + tr("Date passée : le statut est automatiquement « Terminée ».");
        bandeau("<b>" + titre.toHtmlEscaped() + "</b><br>" + aide, EtatEdition,
                tr("Enregistrer"), tr("Annuler"), true);
    } else {
        bandeau("<b>" + titre.toHtmlEscaped() + "</b> — " + tr("corrigez les champs en rouge :") + "<br>"
                + lignes.join("<br>"), EtatErreur, tr("Enregistrer"), tr("Annuler"), true);
    }
    return eT.isEmpty() && eA.isEmpty() && eD.isEmpty() && eZ.isEmpty();
}

Intervention GestionInterventions::depuisEdition() const
{
    Intervention it;
    it.setId(m_mode == ModeModif ? m_idEdition : 0);
    it.setType(m_ed.type->currentText().simplified());
    it.setAdresse(m_ed.adresse->text().simplified());
    it.setDate(m_ed.date->date());
    it.setHeure(heureSansSecondes(m_ed.heure->time()));
    it.setIdZone(m_ed.zone->currentIndex() >= 0 ? m_ed.zone->currentData().toInt() : 0);
    it.setGravite(m_ed.gravite->currentText());
    it.setStatut(m_ed.statut->currentText());
    return it;
}

// =====================================================================================
//  CREATE / UPDATE : enregistrement de la ligne éditée
// =====================================================================================
void GestionInterventions::ajouter()
{
    if (m_mode != ModeListe || m_contexte == CtxSuppression)
        return;
    demarrerEdition(ModeAjout, Intervention());
}

void GestionInterventions::modifier()
{
    if (m_mode != ModeListe || m_contexte == CtxSuppression)
        return;
    montrerListe();
    const int id = idCourant();
    if (id < 0) {
        message(tr("Sélectionnez d'abord une intervention dans le tableau."), EtatErreur);
        return;
    }
    for (int i = 0; i < m_liste.size(); ++i) {
        if (m_liste[i].getId() == id) {
            demarrerEdition(ModeModif, m_liste[i]);
            return;
        }
    }
}

void GestionInterventions::enregistrerEdition()
{
    if (m_mode == ModeListe || !m_ed.type)
        return;

    // 1) contrôle de saisie complet (les champs invalides passent en rouge)
    if (!verifierEdition(true)) {
        if (!Intervention::erreurAdresse(m_ed.adresse->text()).isEmpty())
            m_ed.adresse->setFocus();
        else if (!Intervention::erreurDateHeure(m_ed.date->date(), m_ed.heure->time()).isEmpty())
            m_ed.date->setFocus();
        return;
    }
    Intervention it = depuisEdition();

    // 2) pas de doublon (même type, adresse, date et heure)
    const int doublon = it.idDoublon();
    if (doublon > 0) {
        definirPropriete(m_ed.adresse, "erreur", true);
        bandeau("<b>" + tr("Doublon :") + "</b> " +
                tr("l'intervention n° %1 a déjà le même type, la même adresse, la même date et la même heure. "
                   "Modifiez l'un de ces champs.").arg(doublon),
                EtatErreur, tr("Enregistrer"), tr("Annuler"), true);
        m_ed.adresse->setFocus();
        return;
    }

    // 3) enregistrement
    const bool ajout = (m_mode == ModeAjout);
    const int idModifie = m_idEdition;
    if (!(ajout ? it.ajouter() : it.modifier())) {
        bandeau("<b>" + (ajout ? tr("Ajout impossible :") : tr("Modification impossible :")) + "</b> "
                + Intervention::derniereErreur().toHtmlEscaped().replace("\n", "<br>"),
                EtatErreur, tr("Enregistrer"), tr("Annuler"), true);
        return;
    }

    terminerEdition();
    if (ajout) {                              // retrouve la nouvelle ligne pour la mettre en évidence
        int idNouveau = 0;
        for (int i = 0; i < m_liste.size(); ++i) {
            const Intervention &x = m_liste[i];
            if (x.getType().compare(it.getType(), Qt::CaseInsensitive) == 0
                    && x.getAdresse().compare(it.getAdresse(), Qt::CaseInsensitive) == 0
                    && x.getDate() == it.getDate() && x.getHeure() == it.getHeure()
                    && x.getId() > idNouveau)
                idNouveau = x.getId();
        }
        QTableWidget *t = ui->tableInterventions;
        for (int r = 0; idNouveau > 0 && r < t->rowCount(); ++r) {
            if (t->item(r, 0)->data(Qt::UserRole).toInt() == idNouveau) {
                t->selectRow(r);
                t->scrollToItem(t->item(r, 0));
                break;
            }
        }
        message(idNouveau > 0
                    ? tr("Intervention n° %1 ajoutée avec succès.").arg(idNouveau)
                    : tr("Intervention ajoutée (elle n'apparaît pas dans la liste à cause des filtres actifs)."),
                EtatOk);
    } else {
        ui->tableInterventions->scrollToItem(ui->tableInterventions->currentItem());
        message(tr("Intervention n° %1 modifiée avec succès.").arg(idModifie), EtatOk);
    }
}

void GestionInterventions::annulerEdition()
{
    if (m_mode == ModeListe)
        return;
    terminerEdition();
    message(tr("Saisie annulée : rien n'a été enregistré."));
}

// =====================================================================================
//  DELETE : confirmation dans le bandeau (aucune fenêtre supplémentaire)
// =====================================================================================
void GestionInterventions::supprimer()
{
    if (m_mode != ModeListe || m_contexte == CtxSuppression)
        return;
    montrerListe();
    const int id = idCourant();
    if (id < 0) {
        message(tr("Sélectionnez d'abord une intervention dans le tableau."), EtatErreur);
        return;
    }
    QString resume;
    for (int i = 0; i < m_liste.size(); ++i)
        if (m_liste[i].getId() == id)
            resume = QString("%1 – %2").arg(m_liste[i].getType(), m_liste[i].getAdresse()).toHtmlEscaped();
    m_contexte = CtxSuppression;
    bandeau(tr("Supprimer définitivement l'intervention n° <b>%1</b> (%2) ? Cette action est irréversible.")
                .arg(id).arg(resume),
            EtatDanger, tr("Oui, supprimer"), tr("Annuler"), true);
    majBoutons();
}

void GestionInterventions::confirmerSuppression()
{
    const int id = idCourant();            // le tableau est verrouillé pendant la confirmation
    masquerBandeau();
    m_contexte = CtxAucun;
    if (id < 0) {
        majBoutons();
        return;
    }
    if (Intervention::supprimer(id)) {
        rafraichir();
        message(tr("Intervention n° %1 supprimée.").arg(id), EtatOk);
    } else {
        const QString err = Intervention::derniereErreur();
        rafraichir();
        if (err.contains("02292"))
            message(tr("Suppression impossible : cette intervention est liée à des employés, véhicules ou "
                       "équipements (tables PARTICIPER_UTILISER / NECESSITER). Supprimez d'abord ces liens, "
                       "ou ajoutez ON DELETE CASCADE dans ces tables."), EtatErreur);
        else
            message(tr("Suppression impossible : %1").arg(err), EtatErreur);
    }
}

// =====================================================================================
//  Consulter : fiche en lecture seule dans le bandeau
// =====================================================================================
void GestionInterventions::consulter()
{
    if (m_mode != ModeListe || m_contexte == CtxSuppression)
        return;
    montrerListe();
    const int id = idCourant();
    if (id < 0) {
        message(tr("Sélectionnez d'abord une intervention dans le tableau."), EtatErreur);
        return;
    }
    for (int i = 0; i < m_liste.size(); ++i) {
        const Intervention &it = m_liste[i];
        if (it.getId() != id)
            continue;
        const QVector<int> aff = Affectation::compter(id);
        const QString resumeAffectation = (aff[0] + aff[2] == 0)
            ? tr("aucune (bouton « Affectation automatique »)")
            : tr("%1 véhicule(s), %2 employé(s), %3 équipement(s)").arg(aff[1]).arg(aff[0]).arg(aff[2]);
        const QString html =
            "<b>" + tr("Intervention n° %1").arg(id) + "</b>  ·  " + tr("Type") + " : <b>" + it.getType().toHtmlEscaped()
            + "</b>  ·  " + tr("Adresse") + " : <b>" + it.getAdresse().toHtmlEscaped() + "</b><br>"
            + tr("Date") + " : <b>" + it.getDate().toString("dd/MM/yyyy") + " " + tr("à") + " "
            + it.getHeure().toString("HH:mm") + "</b>  ·  " + tr("Zone") + " : <b>"
            + libelleZone(it.getIdZone()).toHtmlEscaped() + "</b>  ·  " + tr("Gravité") + " : <b style='color:"
            + couleurGravite(it.getGravite()).name() + ";'>" + it.getGravite().toHtmlEscaped() + "</b>  ·  "
            + tr("Statut") + " : <b style='color:" + couleurStatut(it.getStatut()).name() + ";'>"
            + it.getStatut().toHtmlEscaped() + "</b><br>" + tr("Affectation") + " : <b>" + resumeAffectation + "</b>";
        m_contexte = CtxDetail;
        bandeau(html, EtatInfo, QString(), tr("Fermer"), true);
        ui->btnBandeauAnnuler->setIcon(iconeCroix(QColor("#6b7280")));
        return;
    }
}

// ------------------------------------------------------------ Export PDF
// Rapport A4 : bandeau (logo + nom de l'application), cartes de synthèse, graphiques,
// tableau détaillé et pied de page numéroté. Il reflète la liste affichée (filtres + tri).
void GestionInterventions::exporterPdf()
{
    if (m_liste.isEmpty()) {
        QMessageBox::information(this, tr("Exporter PDF"), tr("Aucune intervention à exporter."));
        return;
    }
    const QString chemin = QFileDialog::getSaveFileName(
        this, tr("Exporter en PDF"),
        "interventions_" + QDate::currentDate().toString("yyyyMMdd") + ".pdf", "PDF (*.pdf)");
    if (chemin.isEmpty())
        return;

    QPdfWriter writer(chemin);
    writer.setPageLayout(QPageLayout(QPageSize(QPageSize::A4), QPageLayout::Portrait, QMarginsF(0, 0, 0, 0)));
    writer.setResolution(96);                    // 1 px = 1/96 pouce : tailles du HTML = tailles à l'écran
    writer.setTitle(tr("Rapport des interventions - FireStation Manager"));
    writer.setCreator("FireStation Manager");

    const int W = writer.width(), H = writer.height();
    const int mx = 42, mh = 36, mb = 54;         // marges (px) : côtés, haut, bas
    const int pageW = W - 2 * mx, pageH = H - mh - mb;

    // ---- chiffres et graphiques (calculés sur la liste exportée)
    int enCours = 0, enAttente = 0, terminee = 0;
    for (int i = 0; i < m_liste.size(); ++i) {
        const QString &s = m_liste[i].getStatut();
        if (s == "En cours") ++enCours; else if (s == "En attente") ++enAttente; else if (s == "Terminée") ++terminee;
    }

    QTextDocument doc;
    doc.documentLayout()->setPaintDevice(&writer);
    doc.setDefaultFont(QFont("Segoe UI", 9));
    doc.setDocumentMargin(0);
    doc.setPageSize(QSizeF(pageW, pageH));

    doc.addResource(QTextDocument::ImageResource, QUrl("logo"), QImage(":/images/logo.png"));
    doc.addResource(QTextDocument::ImageResource, QUrl("g_statut"),
                    Graphiques::versImage(GraphiqueWidget::Camembert, QSize(340, 225),
                                          tr("Répartition par statut"), donneesStatut(m_liste)));
    doc.addResource(QTextDocument::ImageResource, QUrl("g_gravite"),
                    Graphiques::versImage(GraphiqueWidget::Histogramme, QSize(340, 225),
                                          tr("Répartition par gravité"), donneesGravite(m_liste)));
    doc.addResource(QTextDocument::ImageResource, QUrl("g_type"),
                    Graphiques::versImage(GraphiqueWidget::Histogramme, QSize(700, 230),
                                          tr("Interventions par type"), donneesType(m_liste)));

    // ---- filtres actifs (rappelés sous le titre)
    QStringList filtres;
    if (ui->comboGravite->currentIndex() > 0) filtres << tr("Gravité : %1").arg(ui->comboGravite->currentText());
    if (ui->comboStatut->currentIndex() > 0)  filtres << tr("Statut : %1").arg(ui->comboStatut->currentText());
    if (!ui->searchEdit->text().trimmed().isEmpty())
        filtres << tr("Recherche : « %1 »").arg(ui->searchEdit->text().trimmed().toHtmlEscaped());
    const QString lignesFiltre = filtres.isEmpty() ? tr("Toutes les interventions")
                                                   : tr("Filtres : ") + filtres.join(" · ");

    // ---- HTML
    QString html;
    html += "<table width='100%' cellspacing='0' cellpadding='12' bgcolor='#1f2d3d'><tr>"
            "<td width='78' valign='middle'><img src='logo' width='64' height='64'></td>"
            "<td valign='middle'><span style='font-size:21pt; font-weight:bold; color:#ffffff;'>FireStation Manager</span><br>"
            "<span style='font-size:10pt; color:#fca5a5;'>Module Gestion des Interventions</span></td>"
            "<td align='right' valign='middle'><span style='font-size:9pt; color:#e5e7eb;'>Rapport généré le<br>"
            + QDateTime::currentDateTime().toString("dd/MM/yyyy 'à' HH:mm") + "</span></td></tr></table>"
            "<table width='100%' cellspacing='0' cellpadding='0' bgcolor='#b91c1c'><tr><td style='font-size:3pt;'>&nbsp;</td></tr></table>";

    html += "<p style='margin-top:14px; margin-bottom:2px; font-size:16pt; font-weight:bold; color:#1f2937;'>Rapport des interventions</p>";
    html += "<p style='margin-top:0px; font-size:9pt; color:#6b7280;'>" + lignesFiltre + "</p>";

    // cartes de synthèse
    auto carte = [](const QString &couleurFond, const QString &couleurTexte, const QString &titre, int valeur) {
        return QString("<td width='25%' bgcolor='%1' align='center'>"
                       "<span style='font-size:20pt; font-weight:bold; color:%2;'>%3</span><br>"
                       "<span style='font-size:9pt; font-weight:bold; color:%2;'>%4</span></td>")
            .arg(couleurFond, couleurTexte).arg(valeur).arg(titre);
    };
    html += "<table width='100%' cellspacing='8' cellpadding='8'><tr>"
            + carte("#e5e7eb", "#1f2937", "Total", m_liste.size())
            + carte("#fef3c7", "#b45309", "En cours", enCours)
            + carte("#fee2e2", "#b91c1c", "En attente", enAttente)
            + carte("#dcfce7", "#15803d", "Terminée", terminee)
            + "</tr></table>";

    // graphiques
    html += "<p style='margin-top:10px; margin-bottom:4px; font-size:12pt; font-weight:bold; color:#1f2937;'>Statistiques</p>";
    html += "<table width='100%' cellspacing='0' cellpadding='0'><tr>"
            "<td><img src='g_statut' width='340' height='225'></td>"
            "<td><img src='g_gravite' width='340' height='225'></td></tr></table>";
    html += "<img src='g_type' width='700' height='230'>";

    // tableau détaillé (l'en-tête est répété sur chaque page)
    html += "<p style='margin-top:12px; margin-bottom:4px; font-size:12pt; font-weight:bold; color:#1f2937;'>Liste détaillée</p>";
    html += "<table width='100%' cellspacing='0' cellpadding='6'>"
            "<thead><tr bgcolor='#1f2d3d'>"
            "<th width='6%' align='left'><font color='white'>ID</font></th>"
            "<th width='17%' align='left'><font color='white'>Type</font></th>"
            "<th width='30%' align='left'><font color='white'>Adresse</font></th>"
            "<th width='17%' align='left'><font color='white'>Date et heure</font></th>"
            "<th width='7%' align='left'><font color='white'>Zone</font></th>"
            "<th width='11%' align='left'><font color='white'>Gravité</font></th>"
            "<th width='12%' align='left'><font color='white'>Statut</font></th>"
            "</tr></thead>";
    for (int i = 0; i < m_liste.size(); ++i) {
        const Intervention &it = m_liste[i];
        const QString fond = (i % 2 == 0) ? "#ffffff" : "#f3f4f6";
        html += QString("<tr bgcolor='%1'><td>%2</td><td>%3</td><td>%4</td><td>%5</td><td>%6</td>"
                        "<td><b><font color='%7'>%8</font></b></td><td><b><font color='%9'>%10</font></b></td></tr>")
                    .arg(fond)
                    .arg(it.getId())
                    .arg(it.getType().toHtmlEscaped(), it.getAdresse().toHtmlEscaped(),
                         it.getDate().toString("dd/MM/yyyy") + " " + it.getHeure().toString("HH:mm"))
                    .arg(it.getIdZone())
                    .arg(couleurGravite(it.getGravite()).name(), it.getGravite().toHtmlEscaped(),
                         couleurStatut(it.getStatut()).name(), it.getStatut().toHtmlEscaped());
    }
    html += "</table>";
    doc.setHtml(html);

    // ---- écriture page par page + pied de page
    QPainter painter(&writer);
    if (!painter.isActive()) {
        QMessageBox::critical(this, tr("Exporter PDF"),
                              tr("Impossible d'écrire le fichier (est-il ouvert dans un autre programme ?)."));
        return;
    }
    const int pages = qMax(1, doc.pageCount());
    for (int p = 0; p < pages; ++p) {
        if (p > 0)
            writer.newPage();
        painter.save();
        painter.translate(mx, mh - p * pageH);
        doc.drawContents(&painter, QRectF(0, p * pageH, pageW, pageH));
        painter.restore();

        painter.save();
        painter.setPen(QPen(QColor("#d1d5db"), 1));
        painter.drawLine(mx, H - mb + 12, W - mx, H - mb + 12);
        painter.setPen(QColor("#6b7280"));
        painter.setFont(QFont("Segoe UI", 8));
        painter.drawText(QRect(mx, H - mb + 16, pageW, 20), Qt::AlignLeft | Qt::AlignVCenter,
                         "FireStation Manager - Gestion des interventions");
        painter.drawText(QRect(mx, H - mb + 16, pageW, 20), Qt::AlignRight | Qt::AlignVCenter,
                         tr("Page %1 / %2").arg(p + 1).arg(pages));
        painter.restore();
    }
    painter.end();

    if (QMessageBox::question(this, tr("Exporter PDF"),
                              tr("Fichier enregistré :\n%1\n\nOuvrir le fichier ?").arg(chemin))
            == QMessageBox::Yes)
        QDesktopServices::openUrl(QUrl::fromLocalFile(chemin));
}

// ------------------------------------------------------------ Tris
void GestionInterventions::trierParDate()
{
    // 1er clic : du plus récent au plus ancien, clic suivant : du plus ancien au plus récent, etc.
    m_dateCroissante = (m_tri == TriDate) ? !m_dateCroissante : false;
    m_tri = TriDate;
    rafraichir();
}

void GestionInterventions::trierParId()
{
    // 1er clic : ID croissant, clic suivant : ID décroissant, etc.
    m_idCroissant = (m_tri == TriId) ? !m_idCroissant : true;
    m_tri = TriId;
    rafraichir();
}

// ------------------------------------------------------------ Statistiques (graphiques)
// Affichées à la place du tableau (même page, pas de fenêtre) ; « Retour à la liste » les referme.
void GestionInterventions::afficherStats()
{
    if (m_mode != ModeListe || m_contexte == CtxSuppression)
        return;
    if (ui->stackedMain->currentIndex() == 1) {       // 2e clic : retour à la liste
        montrerListe();
        return;
    }
    masquerBandeau();
    mettreAJourStats(Intervention::afficher());
    ui->stackedMain->setCurrentIndex(1);
    majBoutons();
}

void GestionInterventions::mettreAJourStats(const QVector<Intervention> &toutes)
{
    ui->lblStatsTitle->setText(tr("Statistiques – %1 intervention(s)").arg(toutes.size()));

    QLayoutItem *li;
    while ((li = ui->statsChartsLayout->takeAt(0)) != nullptr) {
        delete li->widget();
        delete li;
    }
    ui->statsChartsLayout->addWidget(new GraphiqueWidget(GraphiqueWidget::Camembert, tr("Répartition par statut"),
                                                         donneesStatut(toutes)));
    ui->statsChartsLayout->addWidget(new GraphiqueWidget(GraphiqueWidget::Histogramme, tr("Répartition par gravité"),
                                                         donneesGravite(toutes)));
    ui->statsChartsLayout->addWidget(new GraphiqueWidget(GraphiqueWidget::Histogramme, tr("Interventions par type"),
                                                         donneesType(toutes)));
}

// ------------------------------------------------------------ Parcours recommandé
// Ordre conseillé de traitement des interventions « En cours » / « En attente » :
//   1) gravité décroissante (Critique d'abord) ; 2) à gravité égale, regroupement par zone pour limiter
//   les trajets ; 3) puis la plus ancienne en premier.
// Durées estimées : trajet = temps d'intervention de la zone d'arrivée (3 min si on reste dans la même
// zone) ; durée sur place selon la gravité (20 / 30 / 45 / 60 min).
namespace {
int rangGravite(const QString &g)
{
    if (g == "Critique") return 4;
    if (g == "Élevée")   return 3;
    if (g == "Moyenne")  return 2;
    return 1;
}
int dureeSurPlace(const QString &g)
{
    switch (rangGravite(g)) { case 4: return 60; case 3: return 45; case 2: return 30; default: return 20; }
}
QString formatDuree(int minutes)
{
    if (minutes < 60)
        return QString("%1 min").arg(minutes);
    return QString("%1 h %2 min").arg(minutes / 60).arg(minutes % 60, 2, 10, QChar('0'));
}
} // namespace

int GestionInterventions::mettreAJourParcours()
{
    QVector<Intervention> aTraiter;
    const QVector<Intervention> toutes = Intervention::afficher();
    for (int i = 0; i < toutes.size(); ++i)
        if (toutes[i].getStatut() != "Terminée")
            aTraiter.push_back(toutes[i]);

    std::stable_sort(aTraiter.begin(), aTraiter.end(), [](const Intervention &a, const Intervention &b) {
        if (rangGravite(a.getGravite()) != rangGravite(b.getGravite()))
            return rangGravite(a.getGravite()) > rangGravite(b.getGravite());
        if (a.getIdZone() != b.getIdZone())
            return a.getIdZone() < b.getIdZone();
        return a.dateHeure() < b.dateHeure();
    });

    const QMap<int, int> temps = Intervention::tempsZones();
    const QDateTime maintenant = QDateTime::currentDateTime();
    QDateTime curseur = maintenant;               // départ de la caserne : maintenant
    int zonePrecedente = -1, totalMinutes = 0;

    QTableWidget *t = ui->tableParcours;
    QHeaderView *entete = t->horizontalHeader();
    entete->setSectionResizeMode(QHeaderView::ResizeToContents);
    entete->setSectionResizeMode(3, QHeaderView::Stretch);               // Adresse
    entete->setStretchLastSection(false);
    t->setRowCount(0);
    t->setRowCount(aTraiter.size());
    for (int r = 0; r < aTraiter.size(); ++r) {
        const Intervention &it = aTraiter[r];
        const int trajet = (it.getIdZone() == zonePrecedente) ? 3 : temps.value(it.getIdZone(), 10);
        const int surPlace = dureeSurPlace(it.getGravite());
        curseur = curseur.addSecs(trajet * 60);
        const QDateTime arrivee = curseur;
        curseur = curseur.addSecs(surPlace * 60);
        totalMinutes += trajet + surPlace;
        zonePrecedente = it.getIdZone();
        const int attente = qMax(0, int(it.dateHeure().secsTo(maintenant) / 60));

        QStringList cellules;
        cellules << QString::number(r + 1) << QString::number(it.getId()) << it.getType() << it.getAdresse()
                 << libelleZone(it.getIdZone())
                 << QString::fromUtf8("\xE2\x97\x8F ") + it.getGravite()
                 << formatDuree(attente) << formatDuree(trajet)
                 << arrivee.toString("HH:mm") << curseur.toString("HH:mm");
        for (int c = 0; c < cellules.size(); ++c) {
            QTableWidgetItem *item = new QTableWidgetItem(cellules[c]);
            item->setTextAlignment(Qt::AlignVCenter | (c == 0 ? Qt::AlignHCenter : Qt::AlignLeft));
            if (c == 0 || c == 5 || c == 9 || c == 8) {
                QFont f = item->font();
                f.setBold(true);
                item->setFont(f);
            }
            if (c == 5)
                item->setForeground(QBrush(couleurGravite(it.getGravite())));
            if (c == 3)
                item->setToolTip(it.getAdresse());
            t->setItem(r, c, item);
        }
    }

    ui->lblParcoursResume->setText(aTraiter.isEmpty()
        ? tr("Aucune intervention en cours ou en attente.")
        : tr("%1 intervention(s) à traiter  ·  départ de la caserne à %2  ·  durée totale estimée : %3  ·  fin prévue à %4")
              .arg(aTraiter.size()).arg(maintenant.toString("HH:mm")).arg(formatDuree(totalMinutes))
              .arg(curseur.toString("HH:mm")));
    ui->lblParcoursInfo->setText(tr("Ordre recommandé : gravité décroissante (Critique d'abord), puis regroupement par zone "
                                    "pour limiter les trajets, puis la plus ancienne en premier. Trajet = temps d'intervention "
                                    "de la zone (3 min dans la même zone) ; durée sur place : 20 / 30 / 45 / 60 min selon la gravité."));
    return aTraiter.size();
}

void GestionInterventions::afficherParcours()
{
    if (m_mode != ModeListe || m_contexte == CtxSuppression)
        return;
    if (ui->stackedMain->currentIndex() == 2) {      // 2e clic : retour à la liste
        montrerListe();
        return;
    }
    masquerBandeau();
    if (mettreAJourParcours() == 0) {
        montrerListe();
        message(tr("Aucune intervention en cours ou en attente : il n'y a pas de parcours à recommander."));
        return;
    }
    ui->stackedMain->setCurrentIndex(2);
    majBoutons();
}

// ------------------------------------------------------------ Affectation automatique
// Propose véhicules + équipe + équipements pour l'intervention sélectionnée (ressources libres,
// adaptées au type et à la gravité). Rien n'est écrit en base avant « Valider l'affectation ».
void GestionInterventions::afficherAffectation()
{
    if (m_mode != ModeListe || m_contexte == CtxSuppression)
        return;
    if (ui->stackedMain->currentIndex() == 3) {      // 2e clic : retour à la liste
        montrerListe();
        return;
    }
    const int id = idCourant();
    if (id < 0) {
        message(tr("Sélectionnez d'abord une intervention dans le tableau."), EtatErreur);
        return;
    }
    Intervention cible;
    for (int i = 0; i < m_liste.size(); ++i)
        if (m_liste[i].getId() == id)
            cible = m_liste[i];
    if (cible.getStatut() == "Terminée") {
        message(tr("L'intervention n° %1 est terminée : on n'affecte plus de ressources.").arg(id), EtatErreur);
        return;
    }

    masquerBandeau();
    m_proposition = Affectation::proposer(cible);
    m_idAffectation = id;
    if (m_proposition.vehicules.isEmpty() || m_proposition.employes.isEmpty()) {
        message(tr("Affectation impossible : ") + m_proposition.avertissements.join(" "), EtatErreur);
        return;
    }

    // ---- tableau : chaque véhicule suivi de son équipe, puis les équipements
    QTableWidget *t = ui->tableAffectation;
    QHeaderView *entete = t->horizontalHeader();
    entete->setSectionResizeMode(QHeaderView::ResizeToContents);
    entete->setSectionResizeMode(1, QHeaderView::Stretch);
    entete->setStretchLastSection(false);
    t->setRowCount(0);
    auto ligne = [t](const QString &a, const QString &b, const QString &c2, const QString &d, bool gras, const QColor &fond) {
        const int r = t->rowCount();
        t->insertRow(r);
        const QString v[4] = { a, b, c2, d };
        for (int k = 0; k < 4; ++k) {
            QTableWidgetItem *item = new QTableWidgetItem(v[k]);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            if (gras) { QFont f = item->font(); f.setBold(true); item->setFont(f); }
            item->setBackground(QBrush(fond));
            t->setItem(r, k, item);
        }
    };
    const QColor fondVehicule("#e8edf3"), fondNormal("#ffffff"), fondEquip("#f4f8f1");
    for (int v = 0; v < m_proposition.vehicules.size(); ++v) {
        const PropositionAffectation::Vehicule &veh = m_proposition.vehicules[v];
        ligne(tr("Véhicule"), veh.immat + " – " + veh.type,
              (veh.modele.isEmpty() ? QString() : veh.modele + " · ") + tr("état : ") + (veh.etat.isEmpty() ? "—" : veh.etat),
              "—", true, fondVehicule);
        for (int e = 0; e < m_proposition.employes.size(); ++e) {
            const PropositionAffectation::Employe &emp = m_proposition.employes[e];
            if (emp.immat != veh.immat)
                continue;
            ligne(emp.chef ? tr("Chef d'équipe") : tr("Employé"), emp.nom,
                  emp.grade + (emp.specialite.isEmpty() ? QString() : " · " + emp.specialite), emp.immat, emp.chef, fondNormal);
        }
    }
    for (int q = 0; q < m_proposition.equipements.size(); ++q) {
        const PropositionAffectation::Equipement &eq = m_proposition.equipements[q];
        ligne(tr("Équipement"), eq.categorie,
              tr("%1 disponible(s)").arg(eq.disponible) + (eq.etat.isEmpty() ? QString() : " · " + eq.etat),
              eq.immat.isEmpty() ? tr("Dépôt (hors véhicule)") : eq.immat, false, fondEquip);
    }

    ui->lblAffectationTitle->setText(tr("Affectation automatique – intervention n° %1").arg(id));
    QString resume = tr("%1 · gravité %2 : besoin estimé %3 véhicule(s), %4 employé(s), %5 équipement(s)  →  proposé : "
                        "<b>%6 véhicule(s), %7 employé(s), %8 équipement(s)</b>")
        .arg(cible.getType().toHtmlEscaped(), cible.getGravite().toHtmlEscaped())
        .arg(m_proposition.besoinVehicules).arg(m_proposition.besoinEmployes).arg(m_proposition.besoinEquipements)
        .arg(m_proposition.vehicules.size()).arg(m_proposition.employes.size()).arg(m_proposition.equipements.size());
    ui->lblAffectationResume->setTextFormat(Qt::RichText);
    ui->lblAffectationResume->setText(resume);

    QStringList avert = m_proposition.avertissements;
    const QVector<int> deja = Affectation::compter(id);
    if (deja[0] + deja[2] > 0)
        avert.prepend(tr("Cette intervention a déjà une affectation (%1 véhicule(s), %2 employé(s), %3 équipement(s)) : "
                         "elle sera remplacée si vous validez.").arg(deja[1]).arg(deja[0]).arg(deja[2]));
    ui->lblAffectationAvert->setText(avert.join("\n"));
    ui->lblAffectationAvert->setVisible(!avert.isEmpty());

    ui->stackedMain->setCurrentIndex(3);
    majBoutons();
}

void GestionInterventions::validerAffectation()
{
    if (ui->stackedMain->currentIndex() != 3 || m_idAffectation < 0)
        return;
    const int id = m_idAffectation;
    if (!Affectation::enregistrer(id, m_proposition)) {
        ui->lblAffectationAvert->setText(tr("Enregistrement impossible : %1").arg(Affectation::derniereErreur()));
        ui->lblAffectationAvert->setVisible(true);
        return;
    }
    const PropositionAffectation p = m_proposition;
    m_idAffectation = -1;
    montrerListe();
    message(tr("Affectation enregistrée pour l'intervention n° %1 : %2 véhicule(s), %3 employé(s), %4 équipement(s).")
                .arg(id).arg(p.vehicules.size()).arg(p.employes.size()).arg(p.equipements.size()), EtatOk);
}

void GestionInterventions::fonctionnaliteAVenir()
{
    montrerListe();
    message(tr("Fonctionnalité à venir."));
}
