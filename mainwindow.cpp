#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "integ1/vehicule.h"

#include <QAbstractItemView>
#include <QColor>
#include <QCalendarWidget>
#include <QDate>
#include <QDateEdit>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFrame>
#include <QFont>
#include <QHeaderView>
#include <QIcon>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLinearGradient>
#include <QListWidget>
#include <QLineEdit>
#include <QLocale>
#include <QMap>
#include <QMessageBox>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
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
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSet>
#include <algorithm>
#include <limits>

namespace {
const QStringList vehicleTypes = {"Camion", "Échelle", "Ambulance", "Véhicule léger", "Camion-citerne", "Véhicule de secours", "Autre"};
const QStringList vehicleStatuses = {"Disponible", "En intervention", "En maintenance", "Hors service"};

struct MaintenanceInterval
{
    int kilometers;
    int months;
};

MaintenanceInterval maintenanceIntervalFor(int currentMileage)
{
    if (currentMileage >= 300000) return {5000, 4};
    if (currentMileage >= 200000) return {6000, 6};
    if (currentMileage >= 100000) return {8000, 9};
    return {10000, 12};
}

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
        : QWidget(parent), values(data) { setMinimumSize(700, 340); }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(rect(), Qt::white);
        if (values.isEmpty()) return;
        const int left = 48, right = 18, top = 24, bottom = 78;
        const int chartWidth = width() - left - right;
        const int chartHeight = height() - top - bottom;
        int maxValue = 1;
        for (int value : values) maxValue = qMax(maxValue, value);
        for (int tick = 0; tick <= 4; ++tick) {
            const int y = top + chartHeight - chartHeight * tick / 4;
            painter.setPen(QPen(QColor(tick == 0 ? "#d9e0e9" : "#edf0f4"), 1));
            painter.drawLine(left, y, width() - right, y);
            if (tick > 0) {
                painter.setPen(QColor("#8793a4"));
                painter.drawText(QRect(0, y - 8, left - 9, 16), Qt::AlignRight | Qt::AlignVCenter,
                                 QString::number(maxValue * tick / 4));
            }
        }
        const int gap = values.size() > 12 ? 7 : 14;
        const int barWidth = qMax(14, (chartWidth - (values.size() + 1) * gap) / values.size());
        int i = 0;
        for (auto it = values.cbegin(); it != values.cend(); ++it, ++i) {
            const int x = left + gap + i * (barWidth + gap);
            const int barHeight = (chartHeight - 25) * it.value() / maxValue;
            QRect bar(x, top + chartHeight - barHeight, barWidth, barHeight);
            painter.setPen(Qt::NoPen);
            QLinearGradient gradient(bar.topLeft(), bar.bottomLeft());
            gradient.setColorAt(0, QColor("#ef4444"));
            gradient.setColorAt(1, QColor("#b91219"));
            painter.setBrush(gradient);
            painter.drawRoundedRect(bar, 5, 5);
            painter.setPen(QColor("#253247"));
            QFont valueFont = painter.font();
            valueFont.setBold(true);
            painter.setFont(valueFont);
            painter.drawText(QRect(x - 8, bar.top() - 23, barWidth + 16, 20), Qt::AlignCenter, QString::number(it.value()));
            valueFont.setBold(false);
            painter.setFont(valueFont);
            painter.drawText(QRect(x - gap, top + chartHeight + 9, barWidth + gap * 2, 55), Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, it.key());
        }
    }

private:
    QMap<QString, int> values;
};

QIcon vehicleActionIcon(bool edit)
{
    QPixmap pixmap(20, 20);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    const QColor color(edit ? "#526176" : "#c52831");
    painter.setPen(QPen(color, 1.8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    if (edit) {
        QPainterPath pencil;
        pencil.moveTo(5, 14.5);
        pencil.lineTo(13.7, 5.8);
        pencil.lineTo(16, 8.1);
        pencil.lineTo(7.3, 16.8);
        pencil.lineTo(4, 17.5);
        pencil.closeSubpath();
        painter.setBrush(QColor("#eef2f7"));
        painter.drawPath(pencil);
        painter.drawLine(QPointF(12.6, 6.9), QPointF(14.9, 9.2));
        painter.drawLine(QPointF(4.3, 17.2), QPointF(7.1, 16.6));
    } else {
        painter.drawLine(QPointF(4, 6), QPointF(16, 6));
        painter.drawLine(QPointF(7, 4), QPointF(13, 4));
        painter.drawLine(QPointF(5.5, 7.5), QPointF(6.5, 16));
        painter.drawLine(QPointF(14.5, 7.5), QPointF(13.5, 16));
        painter.drawLine(QPointF(6.5, 16), QPointF(13.5, 16));
        painter.drawLine(QPointF(8.5, 9), QPointF(8.8, 14));
        painter.drawLine(QPointF(11.5, 9), QPointF(11.2, 14));
    }
    return QIcon(pixmap);
}
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Fire Station — Gestion des véhicules");
    loadVehicles();

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
        #rowEditButton, #rowDeleteButton { background: white; border: 1px solid #dfe4eb; border-radius: 8px; min-width: 34px; min-height: 32px; }
        #rowEditButton:hover { background: #edf3fb; border-color: #b9c9dc; }
        #rowDeleteButton { background: #fff8f8; border-color: #efc5c8; }
        #rowDeleteButton:hover { background: #fee9ea; border-color: #e7a5aa; }
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

    const QList<QPushButton *> navigation = {ui->dashboardButton, ui->vehiclesButton};
    for (int i = 0; i < navigation.size(); ++i) {
        connect(navigation[i], &QPushButton::clicked, this, [this, navigation, i]() {
            ui->stackedWidget->setCurrentIndex(i);
            for (int j = 0; j < navigation.size(); ++j) navigation[j]->setChecked(j == i);
        });
    }
    connect(ui->dashboardVehiclesButton, &QPushButton::clicked, ui->vehiclesButton, &QPushButton::click);
    connect(ui->addVehicleButton, &QPushButton::clicked, this, &MainWindow::resetForm);
    connect(ui->resetVehicle, &QPushButton::clicked, this, &MainWindow::resetForm);
    connect(ui->saveVehicle, &QPushButton::clicked, this, &MainWindow::saveVehicle);
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &MainWindow::refreshTable);
    connect(ui->globalSearch, &QLineEdit::textChanged, this, [this](const QString &text) {
        ui->searchEdit->setText(text);
        ui->stackedWidget->setCurrentIndex(1);
        ui->vehiclesButton->setChecked(true);
        ui->dashboardButton->setChecked(false);
    });
    connect(ui->typeFilter, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::refreshTable);
    connect(ui->statusFilter, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::refreshTable);
    connect(ui->sortCombo, qOverload<int>(&QComboBox::currentIndexChanged), this, &MainWindow::refreshTable);
    connect(ui->notificationsButton, &QToolButton::clicked, this, &MainWindow::showMaintenanceAlerts);
    connect(ui->calendarButton, &QToolButton::clicked, this, &MainWindow::showVehicleCalendar);
    connect(ui->maintenanceAlertsButton, &QPushButton::clicked, this, &MainWindow::showMaintenanceAlerts);
    connect(ui->statisticsButton, &QPushButton::clicked, this, &MainWindow::showStatistics);
    connect(ui->recommendButton, &QPushButton::clicked, this, &MainWindow::recommendVehicle);
    connect(ui->exportCsvButton, &QPushButton::clicked, this, &MainWindow::exportCsv);
    connect(ui->exportPdfButton, &QPushButton::clicked, this, &MainWindow::exportPdf);

    auto *notificationTimer = new QTimer(this);
    notificationTimer->setInterval(60000);
    connect(notificationTimer, &QTimer::timeout, this, &MainWindow::updateNotificationCenter);
    notificationTimer->start();

    refreshTable();
    refreshDashboard();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::loadVehicles()
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

int MainWindow::indexForRegistration(const QString &immatriculation) const
{
    for (int i = 0; i < vehicles.size(); ++i)
        if (vehicles[i].toObject().value("registration").toString().compare(immatriculation, Qt::CaseInsensitive) == 0) return i;
    return -1;
}

bool MainWindow::isMaintenanceDue(const QJsonObject &v) const
{
    const int currentKm = v.value("mileage").toInt();
    const MaintenanceInterval interval = maintenanceIntervalFor(currentKm);
    const QDate lastService = QDate::fromString(v.value("maintenanceDate").toString(), Qt::ISODate);
    const bool dateDue = lastService.isValid() && QDate::currentDate() >= lastService.addMonths(interval.months);
    const int serviceKm = v.value("maintenanceMileage").toInt(-1);
    const bool mileageDue = serviceKm >= 0 && currentKm >= serviceKm
        && currentKm - serviceKm >= interval.kilometers;
    return dateDue || mileageDue;
}

bool MainWindow::hasMaintenanceAlert(const QJsonObject &v) const
{
    if (isMaintenanceDue(v)) return true;
    const int currentKm = v.value("mileage").toInt();
    const MaintenanceInterval interval = maintenanceIntervalFor(currentKm);
    const QDate lastService = QDate::fromString(v.value("maintenanceDate").toString(), Qt::ISODate);
    if (lastService.isValid()
        && QDate::currentDate().daysTo(lastService.addMonths(interval.months)) <= 30) return true;
    const int serviceKm = v.value("maintenanceMileage").toInt(-1);
    if (serviceKm >= 0 && serviceKm + interval.kilometers - currentKm <= 1000) return true;
    return serviceKm < 0 && currentKm >= interval.kilometers;
}

void MainWindow::refreshTable()
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
        edit->setIcon(vehicleActionIcon(true));
        edit->setIconSize(QSize(18, 18));
        edit->setFixedSize(36, 34);
        edit->setCursor(Qt::PointingHandCursor);
        edit->setAccessibleName("Modifier le véhicule");
        edit->setToolTip("Modifier le véhicule");
        edit->setObjectName("rowEditButton");
        auto *remove = new QToolButton(actions);
        remove->setIcon(vehicleActionIcon(false));
        remove->setIconSize(QSize(18, 18));
        remove->setFixedSize(36, 34);
        remove->setCursor(Qt::PointingHandCursor);
        remove->setAccessibleName("Supprimer le véhicule");
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

void MainWindow::refreshDashboard()
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
    updateNotificationCenter();
}

void MainWindow::updateNotificationCenter()
{
    QStringList notificationLines;
    QStringList signatureParts;
    QSet<QString> notifiedVehicles;
    for (const QJsonValue &value : vehicles) {
        const QJsonObject v = value.toObject();
        const QString registration = v.value("registration").toString();
        const int currentKm = v.value("mileage").toInt();
        const MaintenanceInterval interval = maintenanceIntervalFor(currentKm);
        const int serviceKm = v.value("maintenanceMileage").toInt(-1);
        const QDate lastService = QDate::fromString(v.value("maintenanceDate").toString(), Qt::ISODate);
        const QDate dateDue = lastService.isValid() ? lastService.addMonths(interval.months) : QDate();
        const int daysLeft = dateDue.isValid() ? QDate::currentDate().daysTo(dateDue) : 100000;
        const int kmLeft = serviceKm >= 0 ? serviceKm + interval.kilometers - currentKm : 100000000;
        const bool due = daysLeft <= 0 || kmLeft <= 0;
        const bool upcoming = !due && (daysLeft <= 30 || kmLeft <= 1000);
        const bool incomplete = serviceKm < 0 && currentKm >= interval.kilometers;
        if (!due && !upcoming && !incomplete) continue;

        QStringList reasons;
        if (daysLeft <= 0) reasons << "date dépassée";
        else if (daysLeft <= 30) reasons << QString("date dans %1 jour(s)").arg(daysLeft);
        if (kmLeft <= 0) reasons << QString("seuil dépassé de %1 km").arg(-kmLeft);
        else if (kmLeft <= 1000) reasons << QString("seuil dans %1 km").arg(kmLeft);
        if (incomplete) reasons << QString("kilométrage de maintenance à renseigner (intervalle %1 km)")
            .arg(QLocale(QLocale::French).toString(interval.kilometers));
        notificationLines << QString("%1 — %2").arg(registration, reasons.join(" ; "));
        signatureParts << registration + (due ? QStringLiteral(":D") : QStringLiteral(":U"))
            + (incomplete ? QStringLiteral(":H") : QString());
        notifiedVehicles.insert(registration);
    }

    const int alertCount = notifiedVehicles.size();
    QPixmap bell = QIcon(":/icons/bell_badge.svg").pixmap(32, 32);
    if (bell.isNull()) bell = QPixmap(32, 32);
    if (alertCount > 0) {
        QPainter painter(&bell);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(QColor("#ffffff"), 1.5));
        painter.setBrush(QColor("#d71920"));
        const QString badge = alertCount > 9 ? QStringLiteral("9+") : QString::number(alertCount);
        const int badgeWidth = badge.size() > 1 ? 19 : 15;
        painter.drawEllipse(QRect(32 - badgeWidth, 0, badgeWidth, 15));
        painter.setPen(Qt::white);
        QFont badgeFont = painter.font();
        badgeFont.setBold(true);
        badgeFont.setPixelSize(badge.size() > 1 ? 7 : 9);
        painter.setFont(badgeFont);
        painter.drawText(QRect(32 - badgeWidth, 0, badgeWidth, 15), Qt::AlignCenter, badge);
    }
    ui->notificationsButton->setIcon(QIcon(bell));
    ui->notificationsButton->setToolTip(alertCount == 0
        ? QStringLiteral("Aucune alerte de maintenance")
        : QStringLiteral("%1 véhicule(s) à vérifier — ouvrir les alertes").arg(alertCount));

    signatureParts.sort();
    const QString signature = signatureParts.join('|');
    if (signature.isEmpty()) {
        lastNotificationSignature.clear();
        return;
    }
    if (signature == lastNotificationSignature) return;
    lastNotificationSignature = signature;

    auto *toast = new QDialog(this, Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    toast->setAttribute(Qt::WA_DeleteOnClose);
    toast->setObjectName("maintenanceToast");
    toast->setFixedWidth(370);
    auto *toastLayout = new QVBoxLayout(toast);
    toastLayout->setContentsMargins(16, 14, 16, 14);
    toastLayout->setSpacing(8);
    auto *title = new QLabel(QString("Alerte de maintenance · %1").arg(alertCount), toast);
    title->setStyleSheet("font-size:15px;font-weight:700;color:#253247;");
    toastLayout->addWidget(title);
    for (int i = 0; i < qMin(3, notificationLines.size()); ++i) {
        auto *line = new QLabel(notificationLines[i], toast);
        line->setWordWrap(true);
        line->setStyleSheet("color:#526176;font-size:12px;");
        toastLayout->addWidget(line);
    }
    if (notificationLines.size() > 3) {
        auto *more = new QLabel(QString("Et %1 autre(s) véhicule(s)…").arg(notificationLines.size() - 3), toast);
        more->setStyleSheet("color:#687589;font-size:12px;");
        toastLayout->addWidget(more);
    }
    auto *actions = new QHBoxLayout;
    actions->addStretch();
    auto *details = new QPushButton("Voir les alertes", toast);
    details->setStyleSheet("QPushButton{background:#d71920;color:white;border:0;border-radius:6px;padding:7px 12px;font-weight:600;} QPushButton:hover{background:#b91219;}");
    actions->addWidget(details);
    toastLayout->addLayout(actions);
    toast->setStyleSheet("QDialog#maintenanceToast{background:white;border:1px solid #e2e7ef;border-left:4px solid #d71920;border-radius:9px;}");
    connect(details, &QPushButton::clicked, this, [this, toast]() {
        toast->close();
        showMaintenanceAlerts();
    });
    toast->adjustSize();
    toast->move(mapToGlobal(QPoint(width() - toast->width() - 24, 88)));
    toast->show();
    QTimer::singleShot(9000, toast, &QDialog::close);
}

void MainWindow::resetForm()
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

void MainWindow::editVehicle(const QString &immatriculation)
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

void MainWindow::deleteVehicle(const QString &immatriculation)
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

void MainWindow::saveVehicle()
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

void MainWindow::showMaintenanceAlerts()
{
    QStringList urgentAlerts;
    QStringList upcomingAlerts;
    QStringList incompleteMileageHistory;
    for (const QJsonValue &value : vehicles) {
        const QJsonObject v = value.toObject();
        const int currentKm = v.value("mileage").toInt();
        const MaintenanceInterval interval = maintenanceIntervalFor(currentKm);
        const int serviceKm = v.value("maintenanceMileage").toInt(-1);
        const QDate lastService = QDate::fromString(v.value("maintenanceDate").toString(), Qt::ISODate);
        const QDate dateDue = lastService.isValid() ? lastService.addMonths(interval.months) : QDate();
        const int kmDue = serviceKm >= 0 ? serviceKm + interval.kilometers : -1;
        QStringList reasons;
        if (dateDue.isValid()) {
            const int daysLeft = QDate::currentDate().daysTo(dateDue);
            if (daysLeft <= 0) reasons << QString("échéance calendaire dépassée de %1 jour(s)").arg(-daysLeft);
            else if (daysLeft <= 30) reasons << QString("entretien à prévoir dans %1 jour(s)").arg(daysLeft);
        }
        if (kmDue >= 0) {
            const int kmLeft = kmDue - currentKm;
            if (kmLeft <= 0) reasons << QString("seuil kilométrique dépassé de %1 km").arg(-kmLeft);
            else if (kmLeft <= 1000) reasons << QString("entretien à prévoir dans %1 km").arg(kmLeft);
        } else if (serviceKm < 0 && currentKm >= interval.kilometers) {
            incompleteMileageHistory << QString("• %1 : kilométrage lors de la dernière maintenance inconnu; renseignez-le pour vérifier le seuil de %2 km.")
                .arg(v.value("registration").toString(), QLocale(QLocale::French).toString(interval.kilometers));
        }
        if (!reasons.isEmpty()) {
            const QString line = QString("• %1 — %2 : %3")
                .arg(v.value("registration").toString(), v.value("model").toString(), reasons.join(" ; "));
            const bool urgent = isMaintenanceDue(v);
            (urgent ? urgentAlerts : upcomingAlerts) << line;
        }
    }
    QDialog dialog(this);
    dialog.setObjectName("maintenanceAlertsDialog");
    dialog.setWindowTitle("Alertes de maintenance");
    dialog.setMinimumWidth(560);
    auto *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(22, 22, 22, 18);
    layout->setSpacing(13);

    auto *heading = new QLabel("Suivi de maintenance", &dialog);
    heading->setObjectName("alertsHeading");
    auto *subtitle = new QLabel("Échéances calculées à partir du kilométrage et de la date de dernière maintenance.", &dialog);
    subtitle->setObjectName("alertsSubtitle");
    subtitle->setWordWrap(true);
    layout->addWidget(heading);
    layout->addWidget(subtitle);

    auto addSection = [&dialog, layout](const QString &title, const QString &caption,
                                        const QStringList &items, const QString &objectName) {
        if (items.isEmpty()) return;
        auto *card = new QFrame(&dialog);
        card->setObjectName(objectName);
        auto *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(15, 12, 15, 13);
        cardLayout->setSpacing(7);
        auto *sectionTitle = new QLabel(QString("%1  ·  %2").arg(title).arg(items.size()), card);
        sectionTitle->setObjectName("alertsSectionTitle");
        auto *sectionCaption = new QLabel(caption, card);
        sectionCaption->setObjectName("alertsSectionCaption");
        cardLayout->addWidget(sectionTitle);
        cardLayout->addWidget(sectionCaption);
        for (const QString &item : items) {
            auto *line = new QLabel(item.mid(2), card);
            line->setObjectName("alertsItem");
            line->setWordWrap(true);
            cardLayout->addWidget(line);
        }
        layout->addWidget(card);
    };

    addSection("À traiter maintenant", "Une échéance annuelle ou kilométrique est dépassée.",
               urgentAlerts, "urgentAlertsCard");
    addSection("À planifier prochainement", "Échéance dans les 30 jours ou les 1 000 km.",
               upcomingAlerts, "upcomingAlertsCard");
    addSection("Historique à compléter", "Ajoutez le kilométrage de dernière maintenance pour fiabiliser le suivi.",
               incompleteMileageHistory, "incompleteAlertsCard");

    if (urgentAlerts.isEmpty() && upcomingAlerts.isEmpty() && incompleteMileageHistory.isEmpty()) {
        auto *okCard = new QFrame(&dialog);
        okCard->setObjectName("maintenanceOkCard");
        auto *okLayout = new QVBoxLayout(okCard);
        okLayout->setContentsMargins(16, 15, 16, 15);
        auto *okTitle = new QLabel("Tout est à jour", okCard);
        okTitle->setStyleSheet("font-size:15px;font-weight:700;color:#087a58;");
        auto *okText = new QLabel("Aucune échéance de maintenance n’est proche ou dépassée.", okCard);
        okText->setWordWrap(true);
        okText->setStyleSheet("color:#526176;");
        okLayout->addWidget(okTitle);
        okLayout->addWidget(okText);
        layout->addWidget(okCard);
    }

    auto *close = new QPushButton("Fermer", &dialog);
    close->setObjectName("alertsClose");
    connect(close, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout->addWidget(close, 0, Qt::AlignRight);
    dialog.setStyleSheet(R"(
        QDialog#maintenanceAlertsDialog { background: #f1f4f8; }
        QLabel#alertsHeading { color: #253247; font-size: 21px; font-weight: 700; }
        QLabel#alertsSubtitle { color: #687589; font-size: 13px; }
        QFrame#urgentAlertsCard, QFrame#upcomingAlertsCard, QFrame#incompleteAlertsCard, QFrame#maintenanceOkCard {
            background: white; border: 1px solid #e2e7ef; border-radius: 10px;
        }
        QFrame#urgentAlertsCard { border-left: 4px solid #d71920; }
        QFrame#upcomingAlertsCard { border-left: 4px solid #e0a12e; }
        QFrame#incompleteAlertsCard { border-left: 4px solid #6c7b91; }
        QFrame#maintenanceOkCard { border-left: 4px solid #07845e; }
        QLabel#alertsSectionTitle { color: #253247; font-size: 14px; font-weight: 700; border: none; }
        QLabel#alertsSectionCaption { color: #687589; font-size: 12px; border: none; }
        QLabel#alertsItem { color: #39475c; font-size: 13px; border: none; padding: 2px 0; }
        QPushButton#alertsClose { background: #d71920; border: 1px solid #d71920; color: white; border-radius: 7px; padding: 9px 18px; font-weight: 600; }
        QPushButton#alertsClose:hover { background: #b91219; }
    )");
    dialog.exec();
}

void MainWindow::showVehicleCalendar()
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

void MainWindow::showStatistics()
{
    QMap<QString, int> counts;
    int available = 0;
    int due = 0;
    for (const QJsonValue &value : vehicles) {
        const QJsonObject v = value.toObject();
        const QString label = v.value("type").toString() + "\n(" + v.value("status").toString() + ")";
        counts[label]++;
        if (v.value("status").toString() == "Disponible") ++available;
        if (isMaintenanceDue(v)) ++due;
    }
    QDialog dialog(this);
    dialog.setObjectName("statisticsDialog");
    dialog.setWindowTitle("Statistiques de la flotte");
    dialog.resize(850, 650);
    auto *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(22, 22, 22, 18);
    layout->setSpacing(14);
    auto *title = new QLabel("Vue d’ensemble de la flotte", &dialog);
    title->setObjectName("statisticsTitle");
    auto *subtitle = new QLabel("Répartition des véhicules enregistrés selon leur type et leur état.", &dialog);
    subtitle->setObjectName("statisticsSubtitle");
    layout->addWidget(title);
    layout->addWidget(subtitle);

    auto *metrics = new QHBoxLayout;
    metrics->setSpacing(12);
    auto addMetric = [&dialog, metrics](const QString &caption, int value, const QString &accent) {
        auto *card = new QFrame(&dialog);
        card->setObjectName("statisticsMetric");
        card->setStyleSheet(QString("QFrame#statisticsMetric { border-left:4px solid %1; }").arg(accent));
        auto *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(14, 10, 14, 10);
        cardLayout->setSpacing(2);
        auto *valueLabel = new QLabel(QString::number(value), card);
        valueLabel->setObjectName("metricValue");
        auto *captionLabel = new QLabel(caption, card);
        captionLabel->setObjectName("metricCaption");
        cardLayout->addWidget(valueLabel);
        cardLayout->addWidget(captionLabel);
        metrics->addWidget(card, 1);
    };
    addMetric("Véhicules enregistrés", vehicles.size(), "#d71920");
    addMetric("Disponibles", available, "#07845e");
    addMetric("Maintenance à vérifier", due, "#e0a12e");
    layout->addLayout(metrics);

    auto *chartCard = new QFrame(&dialog);
    chartCard->setObjectName("statisticsChartCard");
    auto *chartLayout = new QVBoxLayout(chartCard);
    chartLayout->setContentsMargins(14, 12, 14, 12);
    auto *chartTitle = new QLabel("Véhicules par type et par état", chartCard);
    chartTitle->setStyleSheet("font-size:14px;font-weight:700;color:#253247;");
    chartLayout->addWidget(chartTitle);
    auto *chart = new StatisticsChart(counts, chartCard);
    chartLayout->addWidget(chart, 1);
    layout->addWidget(chartCard, 1);

    auto *close = new QPushButton("Fermer", &dialog);
    close->setObjectName("statisticsClose");
    connect(close, &QPushButton::clicked, &dialog, &QDialog::accept);
    layout->addWidget(close, 0, Qt::AlignRight);
    dialog.setStyleSheet(R"(
        QDialog#statisticsDialog { background:#f1f4f8; }
        QLabel#statisticsTitle { color:#253247; font-size:21px; font-weight:700; }
        QLabel#statisticsSubtitle { color:#687589; font-size:13px; }
        QFrame#statisticsMetric, QFrame#statisticsChartCard { background:white; border:1px solid #e2e7ef; border-radius:10px; }
        QLabel#metricValue { color:#253247; font-size:23px; font-weight:700; }
        QLabel#metricCaption { color:#687589; font-size:12px; }
        QPushButton#statisticsClose { background:#d71920; border:1px solid #d71920; color:white; border-radius:7px; padding:9px 18px; font-weight:600; }
        QPushButton#statisticsClose:hover { background:#b91219; }
    )");
    dialog.exec();
}

void MainWindow::recommendVehicle()
{
    QDialog dialog(this);
    dialog.setObjectName("recommendDialog");
    dialog.setWindowTitle("Recommander un véhicule pour une intervention");
    dialog.setMinimumWidth(540);
    auto *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(22, 22, 22, 18);
    layout->setSpacing(16);

    auto *heading = new QLabel("Affecter un véhicule", &dialog);
    heading->setObjectName("recommendHeading");
    auto *intro = new QLabel("Décrivez les besoins de l’intervention. Les véhicules indisponibles seront écartés et les échéances de maintenance prises en compte.", &dialog);
    intro->setObjectName("recommendIntro");
    intro->setWordWrap(true);
    layout->addWidget(heading);
    layout->addWidget(intro);

    auto *formCard = new QFrame(&dialog);
    formCard->setObjectName("recommendFormCard");
    auto *formCardLayout = new QVBoxLayout(formCard);
    formCardLayout->setContentsMargins(16, 15, 16, 16);
    formCardLayout->setSpacing(10);
    auto *formCaption = new QLabel("BESOINS DE L’INTERVENTION", formCard);
    formCaption->setObjectName("recommendCaption");
    formCardLayout->addWidget(formCaption);
    auto *form = new QFormLayout;
    form->setHorizontalSpacing(18);
    form->setVerticalSpacing(12);
    auto *missionChoice = new QComboBox(&dialog);
    missionChoice->addItems({"Incendie", "Accident de la route", "Secours à personne",
                             "Fuite de gaz", "Inondation", "Autre / polyvalent"});
    auto *priorityChoice = new QComboBox(&dialog);
    priorityChoice->addItems({"Normale", "Élevée", "Critique"});
    form->addRow("Type d’intervention", missionChoice);
    form->addRow("Priorité", priorityChoice);
    formCardLayout->addLayout(form);
    layout->addWidget(formCard);

    auto *buttons = new QHBoxLayout;
    auto *cancel = new QPushButton("Annuler", &dialog);
    auto *search = new QPushButton("Trouver un véhicule", &dialog);
    cancel->setObjectName("recommendCancel");
    search->setObjectName("recommendSearch");
    buttons->addStretch(); buttons->addWidget(cancel); buttons->addWidget(search);
    layout->addLayout(buttons);
    dialog.setStyleSheet(R"(
        QDialog#recommendDialog { background: #f1f4f8; }
        QLabel#recommendHeading { color: #253247; font-size: 21px; font-weight: 700; }
        QLabel#recommendIntro { color: #687589; font-size: 13px; }
        QFrame#recommendFormCard { background: white; border: 1px solid #e2e7ef; border-radius: 11px; }
        QLabel#recommendCaption { color: #9a2630; font-size: 11px; font-weight: 700; letter-spacing: 1px; }
        QFormLayout QLabel { color: #425067; font-weight: 600; }
        QLineEdit, QComboBox { background: white; border: 1px solid #d8dee8; border-radius: 7px; padding: 9px 10px; min-height: 22px; }
        QLineEdit:focus, QComboBox:focus { border: 1px solid #d71920; }
        QPushButton { border-radius: 7px; padding: 9px 15px; min-height: 20px; font-weight: 600; }
        QPushButton#recommendCancel { background: white; border: 1px solid #d8dee8; color: #425067; }
        QPushButton#recommendCancel:hover { background: #f7f8fa; }
        QPushButton#recommendSearch { background: #d71920; border: 1px solid #d71920; color: white; }
        QPushButton#recommendSearch:hover { background: #b91219; }
    )");
    connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(search, &QPushButton::clicked, &dialog, &QDialog::accept);
    if (dialog.exec() != QDialog::Accepted) return;
    const QString mission = missionChoice->currentText();
    const QString priority = priorityChoice->currentText();
    const QMap<QString, QStringList> missionTypes = {
        {"Incendie", {"Camion-citerne", "Camion", "Échelle", "Véhicule de secours"}},
        {"Accident de la route", {"Ambulance", "Véhicule de secours", "Camion"}},
        {"Secours à personne", {"Ambulance", "Véhicule de secours"}},
        {"Fuite de gaz", {"Véhicule de secours", "Camion", "Camion-citerne"}},
        {"Inondation", {"Camion", "Camion-citerne", "Véhicule de secours"}},
        {"Autre / polyvalent", vehicleTypes}
    };
    const QStringList preferredTypes = missionTypes.value(mission, vehicleTypes);
    const bool highPriority = priority == "Élevée" || priority == "Critique";
    QList<QJsonObject> candidates;
    for (const QJsonValue &value : vehicles) {
        const QJsonObject v = value.toObject();
        if (v.value("status").toString() != "Disponible") continue;
        if (!preferredTypes.contains(v.value("type").toString())) continue;
        if (hasMaintenanceAlert(v)) continue;
        candidates.append(v);
    }
    auto adjustedTypeRank = [&preferredTypes, highPriority](const QJsonObject &vehicle) {
        int rank = preferredTypes.indexOf(vehicle.value("type").toString());
        const QString type = vehicle.value("type").toString();
        if (highPriority && (type == "Ambulance" || type == "Véhicule de secours"))
            rank = qMax(0, rank - 1);
        return rank;
    };
    std::sort(candidates.begin(), candidates.end(), [this, &adjustedTypeRank](const QJsonObject &a, const QJsonObject &b) {
        const int aRank = adjustedTypeRank(a);
        const int bRank = adjustedTypeRank(b);
        if (aRank != bRank) return aRank < bRank;
        const int aServiceKm = a.value("maintenanceMileage").toInt(-1);
        const int bServiceKm = b.value("maintenanceMileage").toInt(-1);
        const int aSinceService = aServiceKm >= 0 ? a.value("mileage").toInt() - aServiceKm : std::numeric_limits<int>::max();
        const int bSinceService = bServiceKm >= 0 ? b.value("mileage").toInt() - bServiceKm : std::numeric_limits<int>::max();
        if (aSinceService != bSinceService) return aSinceService < bSinceService;
        const MaintenanceInterval aInterval = maintenanceIntervalFor(a.value("mileage").toInt());
        const MaintenanceInterval bInterval = maintenanceIntervalFor(b.value("mileage").toInt());
        const QDate aDate = QDate::fromString(a.value("maintenanceDate").toString(), Qt::ISODate);
        const QDate bDate = QDate::fromString(b.value("maintenanceDate").toString(), Qt::ISODate);
        const int aDays = aDate.isValid() ? QDate::currentDate().daysTo(aDate.addMonths(aInterval.months)) : 100000;
        const int bDays = bDate.isValid() ? QDate::currentDate().daysTo(bDate.addMonths(bInterval.months)) : 100000;
        if (aDays != bDays) return aDays > bDays;
        return a.value("mileage").toInt() < b.value("mileage").toInt();
    });
    if (candidates.isEmpty()) {
        QMessageBox::information(this, "Aucune recommandation",
            QString("Aucun véhicule disponible, sans alerte de maintenance, ne correspond à la mission « %1 ».").arg(mission));
        return;
    }
    QDialog resultsDialog(this);
    resultsDialog.setWindowTitle("Véhicules recommandés");
    resultsDialog.setMinimumWidth(590);
    auto *resultsLayout = new QVBoxLayout(&resultsDialog);
    resultsLayout->setContentsMargins(22, 22, 22, 18);
    resultsLayout->setSpacing(12);
    auto *resultsTitle = new QLabel("Véhicules recommandés", &resultsDialog);
    resultsTitle->setStyleSheet("font-size:21px;font-weight:700;color:#253247;");
    auto *resultsIntro = new QLabel(QString("Mission : %1  ·  Priorité : %2\nVéhicules disponibles et sans alerte, classés selon la catégorie de mission puis les kilomètres depuis leur dernière maintenance.")
        .arg(mission, priority), &resultsDialog);
    resultsIntro->setWordWrap(true);
    resultsIntro->setStyleSheet("color:#687589;font-size:13px;");
    resultsLayout->addWidget(resultsTitle);
    resultsLayout->addWidget(resultsIntro);
    const int shown = qMin(3, candidates.size());
    for (int i = 0; i < shown; ++i) {
        const QJsonObject v = candidates[i];
        const int serviceKm = v.value("maintenanceMileage").toInt(-1);
        const int currentKm = v.value("mileage").toInt();
        const MaintenanceInterval interval = maintenanceIntervalFor(currentKm);
        const int kmRemaining = serviceKm >= 0 ? qMax(0, serviceKm + interval.kilometers - currentKm) : -1;
        const QDate lastService = QDate::fromString(v.value("maintenanceDate").toString(), Qt::ISODate);
        const int daysRemaining = lastService.isValid() ? qMax(0, QDate::currentDate().daysTo(lastService.addMonths(interval.months))) : -1;
        auto *card = new QFrame(&resultsDialog);
        card->setStyleSheet("QFrame { background:white; border:1px solid #e2e7ef; border-left:4px solid #d71920; border-radius:9px; }");
        auto *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(14, 12, 14, 12);
        cardLayout->setSpacing(5);
        auto *name = new QLabel(QString("%1  ·  %2").arg(v.value("registration").toString(), v.value("model").toString()), card);
        name->setStyleSheet("font-size:15px;font-weight:700;color:#253247;border:none;");
        const int preferredRank = preferredTypes.indexOf(v.value("type").toString());
        const bool rescueBonus = highPriority && (v.value("type").toString() == "Ambulance"
            || v.value("type").toString() == "Véhicule de secours") && preferredRank > 0;
        const QString rankText = rescueBonus ? "Priorité renforcée" : QString("Préférence mission : %1").arg(preferredRank + 1);
        auto *typeLabel = new QLabel(QString("%1   |   Disponible   |   %2").arg(v.value("type").toString(), rankText), card);
        typeLabel->setStyleSheet("color:#07845e;font-weight:600;border:none;");
        const QString kmText = kmRemaining >= 0 ? QString("%1 km avant le prochain seuil (%2 km)")
            .arg(QLocale(QLocale::French).toString(kmRemaining), QLocale(QLocale::French).toString(interval.kilometers))
            : QStringLiteral("Historique kilométrique de maintenance inconnu");
        const QString sinceServiceText = serviceKm >= 0
            ? QString("%1 km depuis la dernière maintenance").arg(QLocale(QLocale::French).toString(currentKm - serviceKm))
            : QStringLiteral("Kilométrage de dernière maintenance non renseigné");
        const QString dateText = daysRemaining >= 0 ? QString("%1 jour(s) avant l’échéance (%2 mois)")
            .arg(daysRemaining).arg(interval.months) : QStringLiteral("Date de maintenance inconnue");
        auto *details = new QLabel(QString("Kilométrage actuel : %1 km   ·   %2\n%3\n%4")
            .arg(QLocale(QLocale::French).toString(currentKm), sinceServiceText, kmText, dateText), card);
        details->setWordWrap(true);
        details->setStyleSheet("color:#687589;border:none;");
        cardLayout->addWidget(name);
        cardLayout->addWidget(typeLabel);
        cardLayout->addWidget(details);
        resultsLayout->addWidget(card);
    }
    auto *close = new QPushButton("Fermer", &resultsDialog);
    close->setStyleSheet("QPushButton{background:#d71920;color:white;border:1px solid #d71920;border-radius:7px;padding:9px 18px;font-weight:600;} QPushButton:hover{background:#b91219;}");
    connect(close, &QPushButton::clicked, &resultsDialog, &QDialog::accept);
    resultsLayout->addWidget(close, 0, Qt::AlignRight);
    resultsDialog.exec();
}

void MainWindow::exportCsv()
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

void MainWindow::exportPdf()
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
