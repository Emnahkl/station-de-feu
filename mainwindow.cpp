#include "mainwindow.h"
#include "ui_vehiclewindow.h"
#include "integ1/vehicule.h"

#include <QAbstractItemView>
#include <QColor>
#include <QCalendarWidget>
#include <QDate>
#include <QDateEdit>
#include <QEvent>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QHeaderView>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QLocale>
#include <QMap>
#include <QMessageBox>
#include <QPainter>
#include <QPageLayout>
#include <QPrinter>
#include <QPushButton>
#include <QComboBox>
#include <QStandardPaths>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QTextEdit>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSet>
#include <algorithm>

namespace {
const QStringList vehicleTypes = {"Camion", "Échelle", "Ambulance", "Véhicule léger", "Camion-citerne", "Véhicule de secours", "Autre"};
const QStringList vehicleStatuses = {"Disponible", "En intervention", "En maintenance", "Hors service"};

QString maintenanceDateString(const QJsonObject &vehicle)
{
    const QDate date = QDate::fromString(vehicle.value("maintenanceDate").toString(), Qt::ISODate);
    return date.isValid() ? QLocale(QLocale::French).toString(date, "dd/MM/yyyy") : QStringLiteral("—");
}

QString csvCell(QString value)
{
    value.replace('"', "\"\"");
    return '"' + value + '"';
}

class StatisticsChart final : public QWidget
{
public:
    explicit StatisticsChart(const QMap<QString, int> &data, QWidget *parent = nullptr)
        : QWidget(parent), values(data) { setMinimumSize(590, 300); }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(rect(), Qt::white);
        if (values.isEmpty()) return;
        const int left = 42, right = 18, top = 20, bottom = 76;
        const int chartWidth = width() - left - right;
        const int chartHeight = height() - top - bottom;
        int maxValue = 1;
        for (int value : values) maxValue = qMax(maxValue, value);
        painter.setPen(QColor("#e4e8ee"));
        painter.drawLine(left, top + chartHeight, width() - right, top + chartHeight);
        const int gap = 16;
        const int barWidth = qMax(22, (chartWidth - (values.size() + 1) * gap) / values.size());
        int i = 0;
        for (auto it = values.cbegin(); it != values.cend(); ++it, ++i) {
            const int x = left + gap + i * (barWidth + gap);
            const int barHeight = (chartHeight - 25) * it.value() / maxValue;
            QRect bar(x, top + chartHeight - barHeight, barWidth, barHeight);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor("#d71920"));
            painter.drawRoundedRect(bar, 5, 5);
            painter.setPen(QColor("#253247"));
            painter.drawText(QRect(x - 8, bar.top() - 23, barWidth + 16, 20), Qt::AlignCenter, QString::number(it.value()));
            painter.drawText(QRect(x - gap, top + chartHeight + 9, barWidth + gap * 2, 55), Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, it.key());
        }
    }

private:
    QMap<QString, int> values;
};
}

VehicleWindow::VehicleWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::VehicleWindow)
{
    ui->setupUi(this);
    setWindowTitle("Fire Station — Gestion des véhicules");
    loadVehicles();

    interventionsButton = new QPushButton(QStringLiteral("🚨   Interventions"), this);
    interventionsButton->setCheckable(false);
    ui->sidebarLayout->insertWidget(5, interventionsButton);
    connect(interventionsButton, &QPushButton::clicked, this, &VehicleWindow::ouvrirInterventions);

    ui->plate->setMaxLength(20);
    ui->plate->setValidator(new QRegularExpressionValidator(
        QRegularExpression(QStringLiteral("^[A-Za-z0-9]+(?:-[A-Za-z0-9]+)*$")), ui->plate));
    ui->type->addItems(vehicleTypes);
    ui->typeFilter->addItems(vehicleTypes);
    ui->status->addItems(vehicleStatuses);
    ui->statusFilter->addItems(vehicleStatuses);
    ui->maintenanceDate->setDate(QDate::currentDate());
    ui->maintenanceDate->setMaximumDate(QDate::currentDate());
    ui->maintenanceMileage->setMinimum(0);
    ui->maintenanceMileage->setMaximum(99999999);
    ui->maintenanceMileage->setSpecialValueText(QStringLiteral("Inconnu"));
    ui->saveVehicle->setProperty("editImmat", QString());

    ui->vehicleTable->setHorizontalHeaderLabels({"Clé", "Immatriculation", "Type", "Modèle", "État", "Zone", "Kilométrage", "Dernière maintenance", "Actions"});
    ui->vehicleTable->setColumnHidden(0, true);
    ui->vehicleTable->setColumnHidden(5, true); // ID_ZONE does not belong to VEHICULES.
    ui->vehicleTable->setAlternatingRowColors(true);
    ui->vehicleTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->vehicleTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->vehicleTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->vehicleTable->verticalHeader()->hide();
    ui->vehicleTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->vehicleTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->vehicleTable->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);

    // L'identifiant de zone et les notes ne font pas partie du modèle VEHICULES.
    ui->zoneLabel->hide();
    ui->zone->hide();
    ui->notesLabel->hide();
    ui->notes->hide();

    setStyleSheet(R"(
        * { font-family: "Segoe UI", Arial; font-size: 13px; color: #253247; }
        QMainWindow, #centralwidget, #stackedWidget, #dashboardPage, #vehiclesPage { background: #f1f4f8; }
        #sidebar { border-image: url(:/images/sidebar_bg.png) 0 0 0 0 stretch stretch; }
        #sidebar QLabel { color: white; background: transparent; }
        #brandLabel { font-size: 17px; font-weight: 700; letter-spacing: 1px; }
        QPushButton { border: 1px solid #e0e5ec; border-radius: 8px; padding: 9px 12px; background: white; }
        #sidebar QPushButton { text-align: left; color: white; background: rgba(76, 5, 8, 190); border: 0; padding: 12px 10px; min-height: 22px; }
        #sidebar QPushButton:checked, #sidebar QPushButton:hover { background: #d71920; }
        #topbar { background: white; border-bottom: 1px solid #e6eaf0; }
        #topTitle { font-size: 17px; font-weight: 700; }
        #profileLabel { font-weight: 600; }
        #notificationsButton, #calendarButton { border: 0; border-radius: 8px; background: transparent; padding: 4px; }
        #notificationsButton:hover, #calendarButton:hover { background: #f2f5f9; }
        #rowEditButton, #rowDeleteButton { background: white; border: 1px solid #dfe4eb; border-radius: 7px; min-width: 30px; min-height: 28px; color: #334155; }
        #rowDeleteButton { color: #c52831; border-color: #efc5c8; font-size: 18px; }
        #listCard, #formCard, #totalCard, #availableCard, #maintenanceCard { background: white; border: 1px solid #e5e9ef; border-radius: 10px; }
        #totalCard, #availableCard, #maintenanceCard { min-height: 116px; }
        #totalCaption, #availableCaption, #maintenanceCaption { color: #687589; font-size: 13px; }
        #totalValue, #availableValue, #maintenanceValue { font-size: 29px; font-weight: 700; color: #d71920; }
        #dashboardHeading, #listTitle, #formTitle { font-size: 18px; font-weight: 700; }
        #dashboardSubtitle, #formSubtitle { color: #687589; }
        #addVehicleButton, #saveVehicle { background: #d71920; border-color: #d71920; color: white; font-weight: 600; }
        #addVehicleButton:hover, #saveVehicle:hover { background: #b91219; }
        #resetVehicle { color: #bd1820; border-color: #e0a5a8; background: white; }
        QLineEdit, QComboBox, QDateEdit, QSpinBox, QTextEdit { background: white; border: 1px solid #dfe4eb; border-radius: 7px; padding: 8px; }
        QComboBox, QDateEdit, QSpinBox { min-height: 20px; }
        #vehicleTable { background: white; border: 1px solid #edf0f4; gridline-color: #edf0f4; alternate-background-color: #f8fafc; selection-background-color: #fbe8e9; }
        QHeaderView::section { background: #f5f7fa; border: 0; border-bottom: 1px solid #e6eaf0; padding: 10px 6px; font-weight: 600; }
        #storageHint { color: #7b8796; font-size: 11px; }
        QToolButton { border: 0; border-radius: 7px; }
        QToolButton:hover { background: #f2f5f9; }
    )");

    connect(ui->dashboardButton, &QPushButton::clicked, this, &VehicleWindow::ouvrirTableauDeBord);
    connect(ui->vehiclesButton, &QPushButton::clicked, this, [this]() { afficherVehicules(); });
    connect(ui->dashboardVehiclesButton, &QPushButton::clicked, this, [this]() { afficherVehicules(); });
    connect(ui->addVehicleButton, &QPushButton::clicked, this, &VehicleWindow::resetForm);
    connect(ui->resetVehicle, &QPushButton::clicked, this, &VehicleWindow::resetForm);
    connect(ui->saveVehicle, &QPushButton::clicked, this, &VehicleWindow::saveVehicle);
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &VehicleWindow::refreshTable);
    connect(ui->globalSearch, &QLineEdit::textChanged, this, [this](const QString &text) {
        ui->searchEdit->setText(text);
        afficherVehicules();
    });
    connect(ui->typeFilter, qOverload<int>(&QComboBox::currentIndexChanged), this, &VehicleWindow::refreshTable);
    connect(ui->statusFilter, qOverload<int>(&QComboBox::currentIndexChanged), this, &VehicleWindow::refreshTable);
    connect(ui->sortCombo, qOverload<int>(&QComboBox::currentIndexChanged), this, &VehicleWindow::refreshTable);
    connect(ui->notificationsButton, &QToolButton::clicked, this, &VehicleWindow::showMaintenanceAlerts);
    connect(ui->calendarButton, &QToolButton::clicked, this, &VehicleWindow::showVehicleCalendar);
    connect(ui->maintenanceAlertsButton, &QPushButton::clicked, this, &VehicleWindow::showMaintenanceAlerts);
    connect(ui->statisticsButton, &QPushButton::clicked, this, &VehicleWindow::showStatistics);
    connect(ui->recommendButton, &QPushButton::clicked, this, &VehicleWindow::recommendVehicle);
    connect(ui->exportCsvButton, &QPushButton::clicked, this, &VehicleWindow::exportCsv);
    connect(ui->exportPdfButton, &QPushButton::clicked, this, &VehicleWindow::exportPdf);

    ui->dashboardButton->setChecked(false);

    refreshTable();
    refreshDashboard();
}

VehicleWindow::~VehicleWindow() { delete ui; }

void VehicleWindow::afficherVehicules()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->vehiclesButton->setChecked(true);
    ui->dashboardButton->setChecked(false);
    if (interventionsButton) interventionsButton->setChecked(false);
    setWindowTitle(QStringLiteral("Fire Station — Gestion des véhicules"));
    ui->topTitle->setText(QStringLiteral("FIRE STATION\nGestion des véhicules"));
}

void VehicleWindow::loadVehicles()
{
    QString error;
    const QList<Vehicule> rows = Vehicule::afficher(QString(), QString(), QString(), QStringLiteral("IMMATRICULATION ASC"), &error);
    vehicles = QJsonArray();
    for (const Vehicule &record : rows) {
        QJsonObject v;
        v["id"] = record.immatriculation;
        v["registration"] = record.immatriculation;
        v["type"] = record.type;
        v["model"] = record.modele;
        v["status"] = record.etat;
        v["zone"] = QString();
        v["mileage"] = record.kilometrage;
        v["maintenanceMileage"] = record.kilometrageDerniereMaintenance;
        v["maintenanceDate"] = record.derniereMaintenance.toString(Qt::ISODate);
        v["notes"] = QString();
        v["createdAt"] = QString();
        vehicles.append(v);
    }
    if (!error.isEmpty()) QMessageBox::warning(this, "Lecture de la base", error);
}

int VehicleWindow::indexForRegistration(const QString &immatriculation) const
{
    for (int i = 0; i < vehicles.size(); ++i)
        if (vehicles[i].toObject().value("registration").toString().compare(immatriculation, Qt::CaseInsensitive) == 0) return i;
    return -1;
}

bool VehicleWindow::isMaintenanceDue(const QJsonObject &v) const
{
    const QDate date = QDate::fromString(v.value("maintenanceDate").toString(), Qt::ISODate);
    // Alerte préventive 30 jours avant l'échéance annuelle.
    const bool dateDue = date.isValid() && date.addMonths(11) <= QDate::currentDate();
    const int currentKm = v.value("mileage").toInt();
    const int serviceKm = v.value("maintenanceMileage").toInt(-1);
    const bool mileageDue = serviceKm >= 0 && currentKm >= serviceKm
        && currentKm - serviceKm >= 9000; // alerte 1 000 km avant le seuil de 10 000 km
    return dateDue || mileageDue;
}

void VehicleWindow::refreshTable()
{
    QList<QJsonObject> filtered;
    const QString query = ui->searchEdit->text().trimmed();
    for (const QJsonValue &value : vehicles) {
        const QJsonObject v = value.toObject();
        const bool matchesQuery = query.isEmpty() || v.value("registration").toString().contains(query, Qt::CaseInsensitive)
            || v.value("type").toString().contains(query, Qt::CaseInsensitive)
            || v.value("model").toString().contains(query, Qt::CaseInsensitive);
        const bool matchesType = ui->typeFilter->currentIndex() == 0 || v.value("type").toString() == ui->typeFilter->currentText();
        const bool matchesStatus = ui->statusFilter->currentIndex() == 0 || v.value("status").toString() == ui->statusFilter->currentText();
        if (matchesQuery && matchesType && matchesStatus) filtered.append(v);
    }
    const int sort = ui->sortCombo->currentIndex();
    std::sort(filtered.begin(), filtered.end(), [sort](const QJsonObject &a, const QJsonObject &b) {
        switch (sort) {
        case 1: return a.value("mileage").toInt() < b.value("mileage").toInt();
        case 2: return a.value("mileage").toInt() > b.value("mileage").toInt();
        case 3: return a.value("status").toString() < b.value("status").toString();
        case 4: return a.value("registration").toString() < b.value("registration").toString();
        default: return a.value("registration").toString() < b.value("registration").toString();
        }
    });

    ui->vehicleTable->setRowCount(0);
    for (const QJsonObject &v : filtered) {
        const int row = ui->vehicleTable->rowCount();
        ui->vehicleTable->insertRow(row);
        const QString immatriculation = v.value("registration").toString();
        const QStringList cells = {immatriculation, immatriculation, v.value("type").toString(),
            v.value("model").toString(), v.value("status").toString(), v.value("zone").toString(),
            QLocale(QLocale::French).toString(v.value("mileage").toInt()) + " km", maintenanceDateString(v)};
        for (int col = 0; col < cells.size(); ++col) {
            auto *item = new QTableWidgetItem(cells[col]);
            if (col == 0) item->setData(Qt::UserRole, immatriculation);
            if (col == 4) {
                const QString status = v.value("status").toString();
                item->setForeground(status == "Disponible" ? QColor("#07845e") : (status == "En intervention" ? QColor("#c52831") : QColor("#b56b00")));
            }
            if (col == 7 && isMaintenanceDue(v)) item->setForeground(QColor("#c52831"));
            ui->vehicleTable->setItem(row, col, item);
        }
        auto *actions = new QWidget(ui->vehicleTable);
        auto *actionLayout = new QHBoxLayout(actions);
        actionLayout->setContentsMargins(2, 2, 2, 2);
        actionLayout->setSpacing(4);
        auto *edit = new QToolButton(actions);
        edit->setText("✎");
        edit->setToolTip("Modifier le véhicule");
        edit->setObjectName("rowEditButton");
        auto *remove = new QToolButton(actions);
        remove->setText("×");
        remove->setToolTip("Supprimer le véhicule");
        remove->setObjectName("rowDeleteButton");
        actionLayout->addWidget(edit);
        actionLayout->addWidget(remove);
        ui->vehicleTable->setCellWidget(row, 8, actions);
        connect(edit, &QToolButton::clicked, this, [this, immatriculation]() { editVehicle(immatriculation); });
        connect(remove, &QToolButton::clicked, this, [this, immatriculation]() { deleteVehicle(immatriculation); });
    }
    refreshDashboard();
}

void VehicleWindow::refreshDashboard()
{
    int available = 0, due = 0;
    for (const QJsonValue &value : vehicles) {
        const QJsonObject v = value.toObject();
        if (v.value("status").toString() == "Disponible") ++available;
        if (isMaintenanceDue(v)) ++due;
    }
    ui->totalValue->setText(QString::number(vehicles.size()));
    ui->availableValue->setText(QString::number(available));
    ui->maintenanceValue->setText(QString::number(due));
}

void VehicleWindow::resetForm()
{
    ui->saveVehicle->setProperty("editImmat", QString());
    ui->formTitle->setText("Ajouter un véhicule");
    ui->plate->clear();
    ui->type->setCurrentIndex(0);
    ui->model->clear();
    ui->status->setCurrentText("Disponible");
    ui->zone->clear();
    ui->maintenanceDate->setDate(QDate::currentDate());
    ui->mileage->setValue(0);
    ui->maintenanceMileage->setValue(0);
    ui->notes->clear();
    ui->plate->setFocus();
}

void VehicleWindow::editVehicle(const QString &immatriculation)
{
    const int index = indexForRegistration(immatriculation);
    if (index < 0) return;
    const QJsonObject v = vehicles[index].toObject();
    ui->saveVehicle->setProperty("editImmat", immatriculation);
    ui->formTitle->setText("Modifier le véhicule");
    ui->plate->setText(v.value("registration").toString());
    ui->type->setCurrentText(v.value("type").toString());
    ui->model->setText(v.value("model").toString());
    ui->status->setCurrentText(v.value("status").toString());
    ui->zone->setText(v.value("zone").toString());
    const QDate date = QDate::fromString(v.value("maintenanceDate").toString(), Qt::ISODate);
    ui->maintenanceDate->setDate(date.isValid() ? date : QDate::currentDate());
    ui->mileage->setValue(v.value("mileage").toInt());
    ui->maintenanceMileage->setValue(qMax(0, v.value("maintenanceMileage").toInt(-1)));
    ui->notes->setPlainText(v.value("notes").toString());
}

void VehicleWindow::deleteVehicle(const QString &immatriculation)
{
    const int index = indexForRegistration(immatriculation);
    if (index < 0) return;
    const QString plate = vehicles[index].toObject().value("registration").toString();
    if (QMessageBox::question(this, "Supprimer le véhicule", "Supprimer le véhicule " + plate + " ?") != QMessageBox::Yes) return;
    QString error;
    if (!Vehicule::supprimer(immatriculation, &error)) {
        QMessageBox::warning(this, "Suppression impossible", error);
        return;
    }
    loadVehicles();
    refreshTable();
}

void VehicleWindow::saveVehicle()
{
    const QString registration = ui->plate->text().trimmed().toUpper();
    const QString model = ui->model->text().trimmed();
    if (registration.isEmpty() || model.isEmpty()) {
        QMessageBox::warning(this, "Informations manquantes", "Renseignez l’immatriculation et le modèle du véhicule.");
        return;
    }
    static const QRegularExpression registrationPattern(
        QStringLiteral("^[A-Za-z0-9]+(?:-[A-Za-z0-9]+)*$"));
    if (registration.size() > 20 || !registrationPattern.match(registration).hasMatch()) {
        QMessageBox::warning(this, "Immatriculation invalide",
            "Utilisez uniquement des lettres, des chiffres et des tirets, sans tiret au début ou à la fin.");
        return;
    }
    const QString ancienneImmatriculation = ui->saveVehicle->property("editImmat").toString();
    for (const QJsonValue &value : vehicles) {
        const QJsonObject existing = value.toObject();
        if (existing.value("registration").toString().compare(ancienneImmatriculation, Qt::CaseInsensitive) != 0
            && existing.value("registration").toString().compare(registration, Qt::CaseInsensitive) == 0) {
            QMessageBox::warning(this, "Immatriculation existante", "Cette immatriculation est déjà enregistrée.");
            return;
        }
    }

    Vehicule v;
    v.immatriculation = registration;
    v.type = ui->type->currentText();
    v.etat = ui->status->currentText();
    v.modele = model;
    v.kilometrage = ui->mileage->value();
    v.kilometrageDerniereMaintenance = ui->maintenanceMileage->value() > 0
        ? ui->maintenanceMileage->value() : -1;
    if (v.kilometrageDerniereMaintenance > v.kilometrage) {
        QMessageBox::warning(this, "Kilométrage invalide",
            "Le kilométrage de la dernière maintenance ne peut pas dépasser le kilométrage actuel.");
        return;
    }
    if (ui->maintenanceDate->date() > QDate::currentDate()) {
        QMessageBox::warning(this, "Date invalide",
            "La date de maintenance ne peut pas être dans le futur.");
        return;
    }
    v.derniereMaintenance = ui->maintenanceDate->date();
    QString error;
    const bool saved = !ancienneImmatriculation.isEmpty()
        ? Vehicule::modifier(ancienneImmatriculation, v, &error)
        : Vehicule::ajouter(v, &error);
    if (!saved) {
        QMessageBox::warning(this, "Enregistrement impossible", error);
        return;
    }
    loadVehicles();
    refreshTable();
    resetForm();
}

void VehicleWindow::showMaintenanceAlerts()
{
    QStringList alerts;
    QStringList incompleteMileageHistory;
    for (const QJsonValue &value : vehicles) {
        const QJsonObject v = value.toObject();
        const int serviceKm = v.value("maintenanceMileage").toInt(-1);
        if (isMaintenanceDue(v)) {
            QString reason;
            const QDate date = QDate::fromString(v.value("maintenanceDate").toString(), Qt::ISODate);
            if (date.isValid() && date.addMonths(11) <= QDate::currentDate()) {
                const int days = date.daysTo(QDate::currentDate());
                reason = days >= 365 ? "maintenance annuelle dépassée de " + QString::number(days - 365) + " jour(s)"
                                     : "maintenance annuelle à prévoir dans " + QString::number(365 - days) + " jour(s)";
            }
            const int kmSince = v.value("mileage").toInt() - serviceKm;
            if (serviceKm >= 0 && kmSince >= 9000) {
                const QString kmReason = kmSince >= 10000
                    ? QString("seuil dépassé de %1 km").arg(kmSince - 10000)
                    : QString("maintenance à prévoir dans environ %1 km").arg(10000 - kmSince);
                reason += (reason.isEmpty() ? "" : " et ")
                    + QString("%1 (%2 km depuis la maintenance)").arg(kmReason).arg(kmSince);
            }
            if (reason.isEmpty()) reason = "échéance de maintenance atteinte";
            alerts << QString("• %1 — %2 : %3").arg(v.value("registration").toString(), v.value("model").toString(), reason);
        } else if (serviceKm < 0 && v.value("mileage").toInt() >= 10000) {
            incompleteMileageHistory << QString("• %1 : kilométrage lors de la dernière maintenance inconnu; renseignez-le pour vérifier le seuil des 10 000 km.")
                .arg(v.value("registration").toString());
        }
    }
    QStringList messages = alerts;
    if (!incompleteMileageHistory.isEmpty()) {
        if (!messages.isEmpty()) messages << QString();
        messages << "Vérification kilométrique incomplète :";
        messages.append(incompleteMileageHistory);
    }
    if (messages.isEmpty())
        messages << "Aucun véhicule ne nécessite actuellement une maintenance.";
    QMessageBox::information(this, "Alertes de maintenance", messages.join('\n'));
}

void VehicleWindow::showVehicleCalendar()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Calendrier des véhicules");
    dialog.resize(680, 650);
    auto *layout = new QVBoxLayout(&dialog);
    auto *hint = new QLabel("Les dates marquées contiennent un événement. Sélectionnez un jour pour voir l’historique.", &dialog);
    hint->setWordWrap(true);
    layout->addWidget(hint);

    auto *calendar = new QCalendarWidget(&dialog);
    calendar->setGridVisible(true);
    calendar->setFirstDayOfWeek(Qt::Monday);
    calendar->setSelectedDate(QDate::currentDate());
    layout->addWidget(calendar);

    auto *eventsTitle = new QLabel("Événements du jour", &dialog);
    eventsTitle->setStyleSheet("font-weight: 700; font-size: 15px;");
    layout->addWidget(eventsTitle);
    auto *eventsList = new QListWidget(&dialog);
    eventsList->setMinimumHeight(170);
    layout->addWidget(eventsList, 1);

    auto *close = new QPushButton("Fermer", &dialog);
    connect(close, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout->addWidget(close, 0, Qt::AlignRight);

    QSet<QDate> markedDates;
    auto refreshDay = [this, calendar, eventsList, eventsTitle]() {
        const QDate selected = calendar->selectedDate();
        eventsTitle->setText("Événements du "
            + QLocale(QLocale::French).toString(selected, QStringLiteral("dddd d MMMM yyyy")));
        eventsList->clear();
        QString error;
        const QList<EvenementVehicule> events = Vehicule::evenementsEntre(selected, selected, &error);
        if (!error.isEmpty()) {
            eventsList->addItem("Impossible de lire l’historique : " + error);
            return;
        }
        if (events.isEmpty()) {
            eventsList->addItem("Aucun événement enregistré pour cette date.");
            return;
        }
        for (const EvenementVehicule &event : events) {
            const QString time = event.date.isValid() ? event.date.toString(QStringLiteral("HH:mm")) : QStringLiteral("--:--");
            eventsList->addItem(QStringLiteral("%1  •  %2  •  %3 — %4")
                .arg(time, event.type, event.immatriculation, event.details));
        }
    };
    auto refreshMonth = [this, calendar, eventsList, &markedDates, refreshDay]() {
        for (const QDate &date : markedDates)
            calendar->setDateTextFormat(date, QTextCharFormat());
        markedDates.clear();

        const QDate first(calendar->yearShown(), calendar->monthShown(), 1);
        const QDate last = first.addMonths(1).addDays(-1);
        QString error;
        const QList<EvenementVehicule> events = Vehicule::evenementsEntre(first, last, &error);
        if (!error.isEmpty()) {
            eventsList->clear();
            eventsList->addItem("Impossible de charger le calendrier : " + error);
            return;
        }
        QTextCharFormat format;
        format.setBackground(QColor("#fde8e8"));
        format.setForeground(QColor("#b9151d"));
        format.setFontWeight(QFont::Bold);
        for (const EvenementVehicule &event : events) {
            const QDate date = event.date.date();
            if (!date.isValid() || markedDates.contains(date)) continue;
            markedDates.insert(date);
            calendar->setDateTextFormat(date, format);
        }
        refreshDay();
    };
    connect(calendar, &QCalendarWidget::clicked, &dialog, [refreshDay](const QDate &) { refreshDay(); });
    connect(calendar, &QCalendarWidget::currentPageChanged, &dialog,
            [refreshMonth](int, int) { refreshMonth(); });
    refreshMonth();
    dialog.exec();
}

void VehicleWindow::showStatistics()
{
    QMap<QString, int> counts;
    for (const QJsonValue &value : vehicles) {
        const QJsonObject v = value.toObject();
        const QString label = v.value("type").toString() + "\n(" + v.value("status").toString() + ")";
        counts[label]++;
    }
    QDialog dialog(this);
    dialog.setWindowTitle("Statistiques de la flotte");
    dialog.resize(700, 420);
    auto *layout = new QVBoxLayout(&dialog);
    auto *title = new QLabel("Répartition des véhicules par type et par état", &dialog);
    title->setStyleSheet("font-size: 17px; font-weight: 700; padding: 8px;");
    layout->addWidget(title);
    layout->addWidget(new StatisticsChart(counts, &dialog), 1);
    auto *close = new QPushButton("Fermer", &dialog);
    connect(close, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout->addWidget(close, 0, Qt::AlignRight);
    dialog.exec();
}

void VehicleWindow::recommendVehicle()
{
    QDialog dialog(this);
    dialog.setWindowTitle(QStringLiteral("Recommandation pour une intervention"));
    auto *layout = new QVBoxLayout(&dialog);
    auto *typeLabel = new QLabel(QStringLiteral("Type d’intervention :"), &dialog);
    auto *interventionType = new QComboBox(&dialog);
    interventionType->addItems({QStringLiteral("Incendie"), QStringLiteral("Accident de la route"),
                                 QStringLiteral("Secours à personne"), QStringLiteral("Fuite de gaz"),
                                 QStringLiteral("Inondation"), QStringLiteral("Autre / polyvalent")});
    auto *gravityLabel = new QLabel(QStringLiteral("Gravité :"), &dialog);
    auto *gravity = new QComboBox(&dialog);
    gravity->addItems({QStringLiteral("Faible"), QStringLiteral("Moyenne"),
                       QStringLiteral("Élevée"), QStringLiteral("Critique")});
    layout->addWidget(typeLabel);
    layout->addWidget(interventionType);
    layout->addWidget(gravityLabel);
    layout->addWidget(gravity);
    auto *recommend = new QPushButton(QStringLiteral("Recommander"), &dialog);
    recommend->setDefault(true);
    layout->addWidget(recommend);
    connect(recommend, &QPushButton::clicked, &dialog, &QDialog::accept);
    if (dialog.exec() != QDialog::Accepted) return;

    const QString mission = interventionType->currentText();
    const QString severity = gravity->currentText();
    QStringList preferredTypes;
    if (mission == "Incendie") preferredTypes = {"Camion-citerne", "Camion", "Échelle", "Véhicule de secours"};
    else if (mission == "Accident de la route") preferredTypes = {"Ambulance", "Véhicule de secours", "Camion"};
    else if (mission == "Secours à personne") preferredTypes = {"Ambulance", "Véhicule de secours"};
    else if (mission == "Fuite de gaz") preferredTypes = {"Véhicule de secours", "Camion", "Camion-citerne"};
    else if (mission == "Inondation") preferredTypes = {"Camion", "Camion-citerne", "Véhicule de secours"};
    else preferredTypes = vehicleTypes;

    QList<QJsonObject> candidates;
    for (const QJsonValue &value : vehicles) {
        const QJsonObject v = value.toObject();
        if (v.value("status").toString() != "Disponible") continue;
        if (isMaintenanceDue(v)) continue;
        if (!preferredTypes.contains(v.value("type").toString())) continue;
        candidates.append(v);
    }
    const auto score = [&preferredTypes, &severity, &mission](const QJsonObject &v) {
        int result = preferredTypes.indexOf(v.value("type").toString()) * 100;
        const int currentKm = v.value("mileage").toInt();
        const int serviceKm = v.value("maintenanceMileage").toInt(-1);
        if (serviceKm >= 0) result += qMax(0, currentKm - serviceKm) / 100;
        else result += 30; // historique de maintenance incomplet
        if (severity == "Critique" || severity == "Élevée") {
            if (v.value("type").toString() == "Véhicule de secours") result -= 15;
            if (v.value("type").toString() == "Ambulance" && mission != "Incendie") result -= 10;
        }
        return result;
    };
    std::sort(candidates.begin(), candidates.end(), [&score](const QJsonObject &a, const QJsonObject &b) {
        return score(a) < score(b);
    });
    if (candidates.isEmpty()) {
        QMessageBox::information(this, QStringLiteral("Aucune recommandation"),
            QStringLiteral("Aucun véhicule adapté n’est à la fois disponible et à jour de maintenance pour cette mission."));
        return;
    }
    const QJsonObject v = candidates.first();
    QMessageBox::information(this, QStringLiteral("Véhicule recommandé"),
        QStringLiteral("Mission : %1 (%2)\n\n%3 — %4\nType : %5\nÉtat : %6\nKilométrage : %7 km\n\nSélectionné selon l’adéquation au type de mission, la disponibilité, l’état et la charge kilométrique depuis la maintenance.")
            .arg(mission, severity, v.value("registration").toString(), v.value("model").toString(),
                 v.value("type").toString(), v.value("status").toString())
            .arg(v.value("mileage").toInt()));
}

void VehicleWindow::exportCsv()
{
    const QString path = QFileDialog::getSaveFileName(this, "Exporter les véhicules", "vehicules.csv", "Fichier CSV (*.csv)");
    if (path.isEmpty()) return;
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::warning(this, "Export impossible", "Le fichier ne peut pas être créé.");
        return;
    }
    QString csv = "Immatriculation;Type;État;Modèle;Kilométrage;Dernière maintenance\r\n";
    for (const QJsonValue &value : vehicles) {
        const QJsonObject v = value.toObject();
        QStringList row = {v.value("registration").toString(), v.value("type").toString(),
            v.value("status").toString(), v.value("model").toString(), QString::number(v.value("mileage").toInt()),
            maintenanceDateString(v)};
        for (QString &cell : row) cell = csvCell(cell);
        csv += row.join(';') + "\r\n";
    }
    file.write(QByteArray::fromHex("efbbbf"));
    file.write(csv.toUtf8());
    QMessageBox::information(this, "Export terminé", "Le fichier CSV est prêt. Vous pouvez l’ouvrir avec Excel.");
}

void VehicleWindow::exportPdf()
{
    const QString path = QFileDialog::getSaveFileName(this, "Exporter les véhicules en PDF", "vehicules.pdf", "Document PDF (*.pdf)");
    if (path.isEmpty()) return;
    QString html = "<html><meta charset='utf-8'><style>body{font-family:Arial;color:#253247}h1{color:#b9151d}table{border-collapse:collapse;width:100%}th{background:#f1f4f8}td,th{border:1px solid #dfe4eb;padding:7px;text-align:left}</style><h1>Fire Station — Parc des véhicules</h1><table><tr><th>Immatriculation</th><th>Type</th><th>État</th><th>Modèle</th><th>Km</th><th>Maintenance</th></tr>";
    for (const QJsonValue &value : vehicles) {
        const QJsonObject v = value.toObject();
        html += "<tr><td>" + v.value("registration").toString().toHtmlEscaped() + "</td><td>"
            + v.value("type").toString().toHtmlEscaped() + "</td><td>" + v.value("status").toString().toHtmlEscaped() + "</td><td>"
            + v.value("model").toString().toHtmlEscaped() + "</td><td>" + QString::number(v.value("mileage").toInt())
            + "</td><td>" + maintenanceDateString(v) + "</td></tr>";
    }
    html += "</table></html>";
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(path);
    QTextDocument document;
    document.setHtml(html);
    document.print(&printer);
    QMessageBox::information(this, "Export terminé", "Le PDF des véhicules a été créé.");
}
