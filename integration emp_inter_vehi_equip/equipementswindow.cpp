#include "equipementswindow.h"
#include "ui_equipements.h"
#include "exports.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QDialog>
#include <QDateEdit>
#include <QGridLayout>
#include <QLabel>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLineEdit>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>
#include <QUrlQuery>
#include <QMap>
#include <QSet>
#include <QTableWidget>
#include <QHeaderView>
#include <QMenuBar>
#include <QSqlError>
#include <QSqlDatabase>
#include <QRegularExpression>
#include <QTimer>
#include <QtCharts>
#include <array>
#include <vector>

// Noms réels des colonnes SQL, dans le même ordre que les listes déroulantes du .ui
static const QStringList CHAMPS_RECHERCHE = { "DESIGNATION", "CATEGORIE", "ETAT", "IMMATRICULATION" };
static const QStringList CHAMPS_TRI = { "ID_EQUIPEMENT", "DESIGNATION", "CATEGORIE", "QUANTITE",
                                        "DATE_ACQUISITION", "ETAT", "DATE_PROCHAIN_CONTROLE" };

namespace {
using QrMatrix = std::array<std::array<bool, 21>, 21>;

void appendBits(std::vector<bool> &bits, unsigned int value, int count)
{
    for (int i = count - 1; i >= 0; --i) bits.push_back(((value >> i) & 1U) != 0);
}

unsigned char gfMultiply(unsigned char x, unsigned char y)
{
    unsigned char z = 0;
    for (int i = 7; i >= 0; --i) {
        z = static_cast<unsigned char>((z << 1) ^ (((z >> 7) & 1U) * 0x11D));
        z ^= static_cast<unsigned char>(((y >> i) & 1U) * x);
    }
    return z;
}

QrMatrix qrForEquipmentId(const QByteArray &payload)
{
    std::vector<bool> bits;
    appendBits(bits, 0x4, 4); // byte mode
    appendBits(bits, static_cast<unsigned int>(payload.size()), 8);
    for (unsigned char c : payload) appendBits(bits, c, 8);
    const int dataCapacity = 19 * 8; // QR version 1-L
    for (int i = 0; i < 4 && static_cast<int>(bits.size()) < dataCapacity; ++i) bits.push_back(false);
    while (bits.size() % 8 != 0) bits.push_back(false);
    std::vector<unsigned char> data;
    for (size_t i = 0; i < bits.size(); i += 8) {
        unsigned char byte = 0;
        for (int j = 0; j < 8; ++j) byte = static_cast<unsigned char>((byte << 1) | (bits[i + j] ? 1 : 0));
        data.push_back(byte);
    }
    for (unsigned char pad = 0xEC; data.size() < 19; pad = (pad == 0xEC ? 0x11 : 0xEC)) data.push_back(pad);

    const std::array<unsigned char, 7> divisor = {87, 229, 146, 149, 238, 102, 21};
    std::array<unsigned char, 7> remainder{};
    for (unsigned char byte : data) {
        const unsigned char factor = byte ^ remainder[0];
        for (int i = 0; i < 6; ++i) remainder[i] = remainder[i + 1];
        remainder[6] = 0;
        for (int i = 0; i < 7; ++i) remainder[i] ^= gfMultiply(divisor[i], factor);
    }
    data.insert(data.end(), remainder.begin(), remainder.end());

    QrMatrix matrix{};
    std::array<std::array<bool, 21>, 21> reserved{};
    auto setFunction = [&](int x, int y, bool dark) {
        if (x >= 0 && x < 21 && y >= 0 && y < 21) {
            matrix[y][x] = dark;
            reserved[y][x] = true;
        }
    };
    auto finder = [&](int cx, int cy) {
        for (int dy = -4; dy <= 4; ++dy) for (int dx = -4; dx <= 4; ++dx) {
            const int x = cx + dx, y = cy + dy;
            const int d = qMax(qAbs(dx), qAbs(dy));
            setFunction(x, y, d <= 3 && (d == 3 || d <= 1));
        }
    };
    finder(3, 3); finder(17, 3); finder(3, 17);
    for (int i = 8; i < 13; ++i) {
        setFunction(i, 6, i % 2 == 0);
        setFunction(6, i, i % 2 == 0);
    }
    // Reserve both format-information copies and the fixed dark module before
    // placing data modules; their final values are written after masking.
    for (int i = 0; i <= 5; ++i) setFunction(8, i, false);
    setFunction(8, 7, false); setFunction(8, 8, false); setFunction(7, 8, false);
    for (int i = 9; i < 15; ++i) setFunction(14 - i, 8, false);
    for (int i = 0; i < 8; ++i) setFunction(20 - i, 8, false);
    for (int i = 8; i < 15; ++i) setFunction(8, 21 - 15 + i, false);
    setFunction(8, 13, false);

    int bitIndex = 0;
    bool upward = true;
    for (int right = 20; right >= 1; right -= 2) {
        if (right == 6) --right;
        for (int vert = 0; vert < 21; ++vert) {
            const int y = upward ? 20 - vert : vert;
            for (int j = 0; j < 2; ++j) {
                const int x = right - j;
                if (reserved[y][x]) continue;
                const bool value = bitIndex < static_cast<int>(data.size() * 8)
                    ? ((data[bitIndex >> 3] >> (7 - (bitIndex & 7))) & 1U) != 0 : false;
                matrix[y][x] = value ^ ((x + y) % 2 == 0); // mask 0
                ++bitIndex;
            }
        }
        upward = !upward;
    }

    const int formatData = (1 << 3); // error correction L, mask 0
    int rem = formatData;
    for (int i = 0; i < 10; ++i) rem = (rem << 1) ^ (((rem >> 9) & 1) * 0x537);
    const int format = ((formatData << 10) | rem) ^ 0x5412;
    auto formatBit = [&](int i) { return ((format >> i) & 1) != 0; };
    for (int i = 0; i <= 5; ++i) setFunction(8, i, formatBit(i));
    setFunction(8, 7, formatBit(6)); setFunction(8, 8, formatBit(7)); setFunction(7, 8, formatBit(8));
    for (int i = 9; i < 15; ++i) setFunction(14 - i, 8, formatBit(i));
    for (int i = 0; i < 8; ++i) setFunction(20 - i, 8, formatBit(i));
    for (int i = 8; i < 15; ++i) setFunction(8, 21 - 15 + i, formatBit(i));
    setFunction(8, 13, true);
    return matrix;
}

QImage qrImage(const QString &payload)
{
    const QrMatrix matrix = qrForEquipmentId(payload.toLatin1());
    constexpr int scale = 10, border = 4, side = (21 + border * 2) * scale;
    QImage image(side, side, QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    for (int y = 0; y < 21; ++y) for (int x = 0; x < 21; ++x)
        if (matrix[y][x]) painter.drawRect((x + border) * scale, (y + border) * scale, scale, scale);
    return image;
}
}

EquipementsWindow::EquipementsWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::EquipementsWindow), model(new EquipementModel(this)),
      dePeremption(nullptr)
{
    ui->setupUi(this);
    auto *lblPeremption = new QLabel("Date de péremption", this);
    dePeremption = new QDateEdit(QDate::currentDate().addYears(2), this);
    dePeremption->setObjectName(QStringLiteral("dePeremption"));
    dePeremption->setCalendarPopup(true);
    dePeremption->setDisplayFormat(QStringLiteral("dd/MM/yyyy"));
    ui->gridForm->addWidget(lblPeremption, 9, 0);
    ui->gridForm->addWidget(dePeremption, 9, 1);
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    auto *qrButton = new QPushButton(QStringLiteral("QR de l'équipement sélectionné"), this);
    ui->gridBtns->addWidget(qrButton, 2, 0, 1, 2);
    connect(qrButton, &QPushButton::clicked, this, [this]() { afficherQrSelectionne(); });
    auto *inventoryButton = new QPushButton(QStringLiteral("Inventaire QR + email des manquants"), this);
    inventoryButton->setObjectName(QStringLiteral("btnInventaireQrEmail"));
    ui->layoutHeader->addWidget(inventoryButton);
    connect(inventoryButton, &QPushButton::clicked, this, [this]() { lancerInventaireQr(); });
    viderFormulaire();
    rafraichir();
}

EquipementsWindow::~EquipementsWindow() { delete ui; }

void EquipementsWindow::afficherQrSelectionne()
{
    bool ok = false;
    const int id = ui->leId->text().toInt(&ok);
    if (!ok || id <= 0) {
        QMessageBox::information(this, QStringLiteral("QR équipement"),
            QStringLiteral("Sélectionne une ligne d'équipement avant de générer son QR."));
        return;
    }
    const QString payload = QStringLiteral("EQ:%1").arg(id);
    const QImage image = qrImage(payload);

    QDialog dialog(this);
    dialog.setWindowTitle(QStringLiteral("QR de l'équipement %1").arg(id));
    auto *layout = new QVBoxLayout(&dialog);
    auto *preview = new QLabel(&dialog);
    preview->setAlignment(Qt::AlignCenter);
    preview->setPixmap(QPixmap::fromImage(image));
    layout->addWidget(preview);
    auto *text = new QLabel(QStringLiteral("Code lu par le scanner : %1").arg(payload), &dialog);
    text->setAlignment(Qt::AlignCenter);
    layout->addWidget(text);
    auto *buttons = new QHBoxLayout;
    auto *save = new QPushButton(QStringLiteral("Enregistrer PNG"), &dialog);
    auto *close = new QPushButton(QStringLiteral("Fermer"), &dialog);
    buttons->addWidget(save);
    buttons->addWidget(close);
    layout->addLayout(buttons);
    connect(save, &QPushButton::clicked, &dialog, [this, image, id]() {
        const QString path = QFileDialog::getSaveFileName(this, QStringLiteral("Enregistrer le QR"),
            QStringLiteral("equipement_%1_qr.png").arg(id), QStringLiteral("Image PNG (*.png)"));
        if (path.isEmpty()) return;
        if (!image.save(path, "PNG"))
            QMessageBox::warning(this, QStringLiteral("QR équipement"), QStringLiteral("Impossible d'enregistrer l'image."));
    });
    connect(close, &QPushButton::clicked, &dialog, &QDialog::accept);
    dialog.exec();
}

void EquipementsWindow::lancerInventaireQr()
{
    bool ok = false;
    const QString immatriculation = QInputDialog::getText(this,
        QStringLiteral("Inventaire par QR"), QStringLiteral("Immatriculation du véhicule :"),
        QLineEdit::Normal, QString(), &ok).trimmed().toUpper();
    if (!ok) return;
    if (immatriculation.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("Inventaire par QR"), QStringLiteral("Saisis l'immatriculation du véhicule."));
        return;
    }

    struct Attendu { QString nom; QString type; int quantite = 0; };
    QMap<int, Attendu> attendus;
    QSqlQuery query = Equipement::affectesVehicule(immatriculation);
    if (query.lastError().isValid()) {
        QMessageBox::critical(this, QStringLiteral("Inventaire par QR"), query.lastError().text());
        return;
    }
    while (query.next()) {
        Attendu a;
        a.nom = query.value(1).toString();
        a.type = query.value(2).toString();
        a.quantite = qMax(0, query.value(3).toInt());
        attendus.insert(query.value(0).toInt(), a);
    }
    if (attendus.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("Inventaire par QR"),
            QStringLiteral("ما فما حتى معدّة مسجّلة على العربة %1. راجع خانة Caserne / Véhicule في المعدات.").arg(immatriculation));
        return;
    }

    QMap<int, int> scannes;
    QDialog dialog(this);
    dialog.setWindowTitle(QStringLiteral("جرد المعدات — %1").arg(immatriculation));
    dialog.resize(600, 430);
    auto *layout = new QVBoxLayout(&dialog);
    layout->addWidget(new QLabel(QStringLiteral("امسح QR كل قطعة. القارئ يكتب EQ:<ID> ويضغط Enter تلقائيًا."), &dialog));
    auto *scanner = new QLineEdit(&dialog);
    scanner->setPlaceholderText(QStringLiteral("EQ:123 — امسح هنا"));
    layout->addWidget(scanner);
    auto *status = new QLabel(QStringLiteral("0 قطعة ممسوحة"), &dialog);
    layout->addWidget(status);
    auto *log = new QTableWidget(&dialog);
    log->setColumnCount(3);
    log->setHorizontalHeaderLabels({QStringLiteral("الرقم"), QStringLiteral("المعدّة"), QStringLiteral("الممسوح / المنتظر")});
    log->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    log->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(log);
    auto *finish = new QPushButton(QStringLiteral("إنهاء الجرد ومراجعة النواقص"), &dialog);
    layout->addWidget(finish);
    connect(scanner, &QLineEdit::returnPressed, &dialog, [&, scanner, status, log]() {
        const QString raw = scanner->text().trimmed();
        scanner->clear();
        static const QRegularExpression qrPattern(QStringLiteral("^EQ:(\\d+)$"), QRegularExpression::CaseInsensitiveOption);
        const auto match = qrPattern.match(raw);
        if (!match.hasMatch()) {
            status->setText(QStringLiteral("QR غير معروف؛ يلزم يكون بالشكل EQ:<ID>."));
            scanner->setFocus();
            return;
        }
        const int id = match.captured(1).toInt();
        if (!attendus.contains(id)) {
            status->setText(QStringLiteral("المعدّة %1 موش مسجّلة على العربة %2.").arg(id).arg(immatriculation));
            scanner->setFocus();
            return;
        }
        scannes[id] += 1;
        const Attendu a = attendus.value(id);
        bool found = false;
        for (int row = 0; row < log->rowCount(); ++row) {
            if (log->item(row, 0)->text().toInt() == id) {
                log->item(row, 2)->setText(QStringLiteral("%1 / %2").arg(scannes.value(id)).arg(a.quantite));
                found = true;
                break;
            }
        }
        if (!found) {
            const int row = log->rowCount();
            log->insertRow(row);
            log->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
            log->setItem(row, 1, new QTableWidgetItem(a.nom + QStringLiteral(" — ") + a.type));
            log->setItem(row, 2, new QTableWidgetItem(QStringLiteral("%1 / %2").arg(scannes.value(id)).arg(a.quantite)));
        }
        int total = 0;
        for (int count : scannes) total += count;
        status->setText(QStringLiteral("%1 قطعة ممسوحة").arg(total));
        scanner->setFocus();
    });
    connect(finish, &QPushButton::clicked, &dialog, &QDialog::accept);
    QTimer::singleShot(0, scanner, [scanner]() { scanner->setFocus(); });
    if (dialog.exec() != QDialog::Accepted) return;

    QStringList missing;
    for (auto it = attendus.cbegin(); it != attendus.cend(); ++it) {
        const int manque = it.value().quantite - scannes.value(it.key());
        if (manque > 0)
            missing << QStringLiteral("- %1 (%2) : %3 قطعة ناقصة")
                .arg(it.value().nom, it.value().type).arg(manque);
    }
    if (missing.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("نتيجة الجرد"),
            QStringLiteral("الجرد كامل: ما فما حتى معدّة ناقصة في العربة %1.").arg(immatriculation));
        return;
    }

    const QString recipient = QInputDialog::getText(this, QStringLiteral("إرسال النواقص بالبريد"),
        QStringLiteral("إلى أي بريد إلكتروني نجهّز رسالة النواقص؟"), QLineEdit::Normal, QString(), &ok).trimmed();
    if (!ok) return;
    if (!recipient.contains('@')) {
        QMessageBox::warning(this, QStringLiteral("البريد الإلكتروني"), QStringLiteral("اكتب عنوان بريد صحيح."));
        return;
    }
    const QString body = QStringLiteral("Inventaire du véhicule %1\nDate : %2\n\nMatériel manquant :\n%3\n\nMerci de vérifier le stock.")
        .arg(immatriculation, QDate::currentDate().toString(QStringLiteral("dd/MM/yyyy")), missing.join('\n'));
    QUrl email;
    email.setScheme(QStringLiteral("mailto"));
    email.setPath(recipient);
    QUrlQuery params;
    params.addQueryItem(QStringLiteral("subject"), QStringLiteral("Matériel manquant — véhicule %1").arg(immatriculation));
    params.addQueryItem(QStringLiteral("body"), body);
    email.setQuery(params);
    if (!QDesktopServices::openUrl(email))
        QMessageBox::warning(this, QStringLiteral("Envoi e-mail"),
            QStringLiteral("Impossible d'ouvrir l'application e-mail par défaut. Vérifie qu'un client e-mail est configuré."));
}

// ============================================================ Utilitaires
void EquipementsWindow::rafraichir()
{
    model->setQuery(Equipement::lister(
        CHAMPS_RECHERCHE.value(ui->cbChampRecherche->currentIndex()),
        ui->leRecherche->text(),
        CHAMPS_TRI.value(ui->cbTri->currentIndex()),
        ui->cbOrdre->currentIndex() == 0));
    ui->tableView->resizeColumnsToContents();
    verifierAlertes(false);
}

void EquipementsWindow::viderFormulaire()
{
    ui->leId->clear();
    ui->leNom->clear();
    ui->cbType->setCurrentIndex(0);
    ui->sbQuantite->setValue(0);
    ui->sbSeuil->setValue(5);
    ui->deAcquisition->setDate(QDate::currentDate());
    ui->cbEtat->setCurrentIndex(0);
    ui->deControle->setDate(QDate::currentDate().addMonths(6));
    if (dePeremption) dePeremption->setDate(QDate::currentDate().addYears(2));
    ui->leAffectation->clear();
    ui->tableView->clearSelection();
}

// Contrôle de saisie + création de l'objet Equipement
bool EquipementsWindow::lireFormulaire(Equipement &e)
{
    const QString nom = ui->leNom->text().trimmed();
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Saisie invalide", "Le nom est obligatoire.");
        return false;
    }
    if (ui->deControle->date() < ui->deAcquisition->date()) {
        QMessageBox::warning(this, "Saisie invalide",
            "La date du prochain contrôle doit être après la date d'acquisition.");
        return false;
    }
    e = Equipement(nom, ui->cbType->currentText(), ui->sbQuantite->value(),
                   ui->deAcquisition->date(), ui->cbEtat->currentText(),
                   ui->deControle->date(), ui->leAffectation->text().trimmed(),
                   ui->sbSeuil->value(), dePeremption->date());
    return true;
}

// ================================================================== CRUD
void EquipementsWindow::on_btnAjouter_clicked()
{
    Equipement e;
    if (!lireFormulaire(e)) return;
    if (e.ajouter()) {
        rafraichir();
        viderFormulaire();
        QMessageBox::information(this, "Ajout", "Équipement ajouté avec succès.");
    } else {
        QMessageBox::critical(this, "Ajout", "Échec de l'ajout.");
    }
}

void EquipementsWindow::on_btnModifier_clicked()
{
    if (ui->leId->text().isEmpty()) {
        QMessageBox::warning(this, "Modifier", "Sélectionnez d'abord un équipement dans le tableau.");
        return;
    }
    Equipement e;
    if (!lireFormulaire(e)) return;
    e.setId(ui->leId->text().toInt());
    if (e.modifier()) {
        rafraichir();
        QMessageBox::information(this, "Modification", "Équipement modifié avec succès.");
    } else {
        QMessageBox::critical(this, "Modification", "Échec de la modification.");
    }
}

void EquipementsWindow::on_btnSupprimer_clicked()
{
    if (ui->leId->text().isEmpty()) {
        QMessageBox::warning(this, "Supprimer", "Sélectionnez d'abord un équipement dans le tableau.");
        return;
    }
    if (QMessageBox::question(this, "Suppression",
            "Voulez-vous vraiment supprimer cet équipement ?") != QMessageBox::Yes)
        return;
    Equipement e;
    if (e.supprimer(ui->leId->text().toInt())) {
        rafraichir();
        viderFormulaire();
    } else {
        QMessageBox::critical(this, "Suppression", "Échec de la suppression.");
    }
}

void EquipementsWindow::on_btnVider_clicked() { viderFormulaire(); }

// Bouton "+ Ajouter un équipement" (en haut de la liste) : prépare un formulaire vide
void EquipementsWindow::on_btnNouveau_clicked()
{
    viderFormulaire();
    ui->leNom->setFocus();
}

// Clic sur une ligne -> remplit le formulaire
void EquipementsWindow::on_tableView_clicked(const QModelIndex &index)
{
    const int r = index.row();
    auto val = [&](int col) { return model->data(model->index(r, col)).toString(); };
    ui->leId->setText(val(COL_ID));
    ui->leNom->setText(val(COL_NOM));
    ui->cbType->setCurrentText(val(COL_TYPE));
    ui->sbQuantite->setValue(val(COL_QTE).toInt());
    ui->deAcquisition->setDate(QDate::fromString(val(COL_DATE_ACQ), Qt::ISODate));
    ui->cbEtat->setCurrentText(val(COL_ETAT));
    ui->deControle->setDate(QDate::fromString(val(COL_DATE_CTRL), Qt::ISODate));
    dePeremption->setDate(QDate::fromString(val(COL_DATE_EXP), Qt::ISODate));
    ui->leAffectation->setText(val(COL_AFFECT) == "-" ? QString() : val(COL_AFFECT));
    ui->sbSeuil->setValue(val(COL_SEUIL).toInt());
}

// ========================================================= Recherche + tri
void EquipementsWindow::on_leRecherche_textChanged(const QString &)      { rafraichir(); }
void EquipementsWindow::on_cbChampRecherche_currentIndexChanged(int)     { rafraichir(); }
void EquipementsWindow::on_cbTri_currentIndexChanged(int)                { rafraichir(); }
void EquipementsWindow::on_cbOrdre_currentIndexChanged(int)              { rafraichir(); }

// ================================================================ Exports
void EquipementsWindow::on_btnExportPdf_clicked()
{
    const QString f = QFileDialog::getSaveFileName(this, "Exporter en PDF",
                                                   "equipements.pdf", "PDF (*.pdf)");
    if (f.isEmpty()) return;
    const bool ok = Exports::ecrirePdf(f, Exports::htmlDepuisModele(model, "Liste des équipements"));
    QMessageBox::information(this, "Export PDF", ok ? "PDF créé." : "Échec de l'export.");
}

void EquipementsWindow::on_btnExportExcel_clicked()
{
    const QString f = QFileDialog::getSaveFileName(this, "Exporter vers Excel",
                                                   "equipements.csv", "Excel / CSV (*.csv)");
    if (f.isEmpty()) return;
    const bool ok = Exports::ecrireCsv(f, model);
    QMessageBox::information(this, "Export Excel", ok ? "Fichier créé (ouvrable avec Excel)." : "Échec de l'export.");
}

// ============================================================ Statistiques
void EquipementsWindow::on_btnStats_clicked()
{
    // Camembert : équipements par état
    auto *pie = new QPieSeries();
    const auto parEtat = Equipement::statsParEtat();
    for (auto it = parEtat.constBegin(); it != parEtat.constEnd(); ++it) {
        QPieSlice *part = pie->append(QString("%1 (%2)").arg(it.key()).arg(it.value()), it.value());
        // Couleurs de la charte : vert = fonctionnel, rouge = en panne, orange = en réparation
        if (it.key() == "Fonctionnel")        part->setBrush(QColor("#2E7D32"));
        else if (it.key() == "En panne")      part->setBrush(QColor("#C62828"));
        else if (it.key() == "En réparation") part->setBrush(QColor("#FF6F00"));
    }
    pie->setLabelsVisible(true);
    auto *chartEtat = new QChart();
    chartEtat->addSeries(pie);
    chartEtat->setTitle("Répartition par état");

    // Histogramme : équipements par catégorie
    auto *bar = new QBarSeries();
    const auto parType = Equipement::statsParType();
    QStringList categories;
    const QStringList palette = { "#C62828", "#FF6F00", "#2B2B2B", "#2E7D32", "#8E1B1B" };
    int i = 0;
    for (auto it = parType.constBegin(); it != parType.constEnd(); ++it, ++i) {
        auto *set = new QBarSet(it.key());
        *set << it.value();
        set->setColor(QColor(palette.at(i % palette.size())));
        bar->append(set);
    }
    categories << "Catégories";
    auto *chartType = new QChart();
    chartType->addSeries(bar);
    chartType->setTitle("Nombre d'équipements par catégorie");
    auto *axeX = new QBarCategoryAxis();
    axeX->append(categories);
    chartType->addAxis(axeX, Qt::AlignBottom);
    bar->attachAxis(axeX);
    auto *axeY = new QValueAxis();
    axeY->setLabelFormat("%d");
    axeY->setMin(0);
    chartType->addAxis(axeY, Qt::AlignLeft);
    bar->attachAxis(axeY);

    auto *dlg = new QDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setWindowTitle("Statistiques des équipements");
    dlg->resize(700, 480);
    auto *tabs = new QTabWidget(dlg);
    tabs->addTab(new QChartView(chartEtat), "Par état");
    tabs->addTab(new QChartView(chartType), "Par type");
    auto *lay = new QVBoxLayout(dlg);
    lay->addWidget(tabs);
    dlg->show();
}

// ================================== Métier innovant 1 : stock + bon de commande
void EquipementsWindow::on_btnBonCommande_clicked()
{
    QSqlQuery q = Equipement::stockEnAlerte();
    QString lignes;
    int n = 0;
    while (q.next()) {
        const int stock = q.value(COL_QTE).toInt();
        const int seuil = q.value(COL_SEUIL).toInt();
        const int aCommander = qMax(1, 2 * seuil - stock);   // on recommande jusqu'à 2 x le seuil
        lignes += QString("<tr><td>%1</td><td>%2</td><td align='center'>%3</td>"
                          "<td align='center'>%4</td><td align='center'><b>%5</b></td></tr>")
                  .arg(q.value(COL_NOM).toString().toHtmlEscaped(),
                       q.value(COL_TYPE).toString().toHtmlEscaped())
                  .arg(stock).arg(seuil).arg(aCommander);
        ++n;
    }
    if (n == 0) {
        QMessageBox::information(this, "Bon de commande", "Aucun équipement sous le seuil d'alerte.");
        return;
    }
    const QString f = QFileDialog::getSaveFileName(this, "Enregistrer le bon de commande",
                                                   "bon_de_commande.pdf", "PDF (*.pdf)");
    if (f.isEmpty()) return;

    const QString html =
        "<h2 align='center'>BON DE COMMANDE</h2>"
        "<p align='center'>SafeStation - " + QDate::currentDate().toString("dd/MM/yyyy") + "</p>"
        "<table border='1' cellspacing='0' cellpadding='4' width='100%'>"
        "<tr bgcolor='#C62828' style='color:white'><th>Équipement</th><th>Type</th>"
        "<th>Stock actuel</th><th>Seuil</th><th>Quantité à commander</th></tr>" + lignes + "</table>";
    const bool ok = Exports::ecrirePdf(f, html);
    QMessageBox::information(this, "Bon de commande",
        ok ? QString("Bon de commande généré (%1 article(s)).").arg(n) : "Échec de la génération.");
}

// ===== Métiers innovants 1 + 2 : alertes de stock et de contrôles périodiques
void EquipementsWindow::verifierAlertes(bool avecPopup)
{
    int nStock = 0, nCtrl = 0, nPeremption = 0;
    QSqlQuery qs = Equipement::stockEnAlerte();
    while (qs.next()) ++nStock;
    QSqlQuery qc = Equipement::controlesProches(30);
    while (qc.next()) ++nCtrl;
    QSqlQuery qp = Equipement::peremptionsProches(30);
    while (qp.next()) ++nPeremption;

    if (nStock == 0 && nCtrl == 0 && nPeremption == 0) {
        ui->lblAlertes->setText("✔ Aucune alerte");
        ui->lblAlertes->setStyleSheet("background:#E8F5E9; color:#2E7D32; padding:8px 12px; border-radius:8px; font-weight:bold;");
        return;
    }
    const QString msg = QString("⚠ %1 équipement(s) sous le seuil de stock  |  "
                                "%2 contrôle(s) et %3 péremption(s) proche(s) (30 jours)")
                        .arg(nStock).arg(nCtrl).arg(nPeremption);
    ui->lblAlertes->setText(msg);
    ui->lblAlertes->setStyleSheet("background:#FFF3E0; color:#E65100; padding:8px 12px; border-radius:8px; font-weight:bold;");
    if (avecPopup)
        QMessageBox::warning(this, "Alertes équipements", msg);
}
