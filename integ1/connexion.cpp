#include "connexion.h"

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVBoxLayout>

namespace {
constexpr auto connectionName = "firestation-main";

bool ensureEquipmentTable(QSqlDatabase &db, bool oracle)
{
    QSqlQuery query(db);
    if (!oracle) {
        return query.exec(
            "CREATE TABLE IF NOT EXISTS EQUIPEMENT ("
            " ID INTEGER PRIMARY KEY AUTOINCREMENT,"
            " NOM TEXT NOT NULL, TYPE_EQUIP TEXT, QUANTITE INTEGER NOT NULL DEFAULT 0,"
            " DATE_ACQUISITION TEXT, ETAT TEXT, DATE_CONTROLE TEXT, AFFECTATION TEXT,"
            " SEUIL_ALERTE INTEGER DEFAULT 5)");
    }

    if (!query.exec("SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME='EQUIPEMENT'"))
        return false;
    if (!query.next() || query.value(0).toInt() != 0)
        return true;
    if (!query.exec(
        "CREATE TABLE EQUIPEMENT ("
        " ID NUMBER PRIMARY KEY,"
        " NOM VARCHAR2(160) NOT NULL, TYPE_EQUIP VARCHAR2(100),"
        " QUANTITE NUMBER(10) DEFAULT 0 NOT NULL, DATE_ACQUISITION DATE,"
        " ETAT VARCHAR2(40), DATE_CONTROLE DATE, AFFECTATION VARCHAR2(200),"
        " SEUIL_ALERTE NUMBER(10) DEFAULT 5)"))
        return false;
    if (!query.exec("CREATE SEQUENCE SEQ_EQUIPEMENT START WITH 1 INCREMENT BY 1 NOCACHE"))
        return false;
    return query.exec(
        "CREATE OR REPLACE TRIGGER TRG_EQUIPEMENT_BI "
        "BEFORE INSERT ON EQUIPEMENT FOR EACH ROW "
        "WHEN (NEW.ID IS NULL) BEGIN "
        "SELECT SEQ_EQUIPEMENT.NEXTVAL INTO :NEW.ID FROM DUAL; END;");
}

bool ensureVehicleEventsTable(QSqlDatabase &db, QString *error)
{
    QSqlQuery query(db);
    if (!query.exec("SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME='VEHICULE_EVENEMENTS'")) {
        if (error) *error = query.lastError().text();
        return false;
    }
    if (!query.next()) {
        if (error) *error = query.lastError().text();
        return false;
    }
    if (query.value(0).toInt() != 0)
        return true;
    query.finish();
    if (!query.exec(
        "CREATE TABLE VEHICULE_EVENEMENTS ("
        " ID_EVENEMENT NUMBER PRIMARY KEY,"
        " IMMATRICULATION VARCHAR2(20) NOT NULL,"
        " TYPE_EVENEMENT VARCHAR2(40) NOT NULL,"
        " DATE_EVENEMENT DATE DEFAULT SYSDATE NOT NULL,"
        " DETAILS VARCHAR2(500))")) {
        if (error) *error = query.lastError().text();
        return false;
    }
    if (!query.exec("CREATE SEQUENCE SEQ_VEHICULE_EVENEMENTS START WITH 1 INCREMENT BY 1 NOCACHE")) {
        if (error) *error = query.lastError().text();
        return false;
    }
    if (!query.exec(
        "CREATE OR REPLACE TRIGGER TRG_VEH_EVENEMENT_BI "
        "BEFORE INSERT ON VEHICULE_EVENEMENTS FOR EACH ROW "
        "WHEN (NEW.ID_EVENEMENT IS NULL) BEGIN "
        "SELECT SEQ_VEHICULE_EVENEMENTS.NEXTVAL INTO :NEW.ID_EVENEMENT FROM DUAL; END;")) {
        if (error) *error = query.lastError().text();
        return false;
    }
    return true;
}

}

bool Connexion::ouvrir(QWidget *parent, bool oracleOnly)
{
    QSettings settings;
    QDialog dialog(parent);
    dialog.setWindowTitle("Connexion à la base de données");
    dialog.setMinimumWidth(450);
    auto *layout = new QVBoxLayout(&dialog);
    auto *form = new QFormLayout;
    auto *mode = new QComboBox(&dialog);
    mode->addItem("SQLite — démonstration locale");
    mode->addItem("Oracle — via pilote ODBC");
    mode->setCurrentIndex(oracleOnly ? 1 : settings.value("database/mode", 1).toInt());
    mode->setEnabled(!oracleOnly);
    QString savedDriver = settings.value("oracle/driver", "Oracle in XE").toString();
    QString savedService = settings.value("oracle/service", "XE").toString();
    if (savedDriver == "Oracle in OraDB26Home1") savedDriver = "Oracle in XE";
    if (savedService == "localhost:1521/FREEPDB1") savedService = "XE";
    auto *driver = new QLineEdit(savedDriver, &dialog);
    auto *service = new QLineEdit(savedService, &dialog);
    auto *username = new QLineEdit(settings.value("oracle/username").toString(), &dialog);
    auto *password = new QLineEdit(&dialog);
    password->setEchoMode(QLineEdit::Password);
    password->setPlaceholderText("Saisi à chaque fois, jamais enregistré");
    form->addRow("Base de données :", mode);
    form->addRow("Nom du pilote Oracle :", driver);
    form->addRow("Service Oracle / alias TNS :", service);
    form->addRow("Utilisateur Oracle :", username);
    form->addRow("Mot de passe :", password);
    layout->addLayout(form);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    buttons->button(QDialogButtonBox::Ok)->setText("Tester et continuer");
    buttons->button(QDialogButtonBox::Cancel)->setText("Annuler");
    layout->addWidget(buttons);
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    auto updateModeFields = [mode, driver, service, username, password]() {
        const bool oracle = mode->currentIndex() == 1;
        driver->setEnabled(oracle);
        service->setEnabled(oracle);
        username->setEnabled(oracle);
        password->setEnabled(oracle);
    };
    QObject::connect(mode, &QComboBox::currentIndexChanged, &dialog, updateModeFields);
    updateModeFields();
    dialog.setStyleSheet("QDialog { background:#f4f6fa; } QLabel { color:#26344b; }"
                         "QLineEdit,QComboBox { background:white; padding:7px; border:1px solid #d5dce6; border-radius:5px; }");

    if (dialog.exec() != QDialog::Accepted)
        return false;

    const bool oracle = mode->currentIndex() == 1;
    const QString requiredDriver = oracle ? QStringLiteral("QODBC")
                                          : QStringLiteral("QSQLITE");
    if (!QSqlDatabase::drivers().contains(requiredDriver)) {
        QMessageBox::critical(
            parent,
            QStringLiteral("Pilote Qt manquant"),
            QStringLiteral("Le pilote SQL %1 n'est pas disponible dans cette installation de Qt. "
                           "Installez le composant Qt SQL correspondant au kit utilisé "
                           "(Qt 6.7.3 MinGW 64-bit), puis relancez l'application.")
                .arg(requiredDriver));
        return false;
    }
    settings.setValue("database/mode", mode->currentIndex());
    QSqlDatabase db;
    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
        const QString wantedDriver = oracle ? QStringLiteral("QODBC") : QStringLiteral("QSQLITE");
        if (db.driverName() != wantedDriver) {
            db.close();
            db = QSqlDatabase();
            QSqlDatabase::removeDatabase(connectionName);
        }
    }
    if (!db.isValid())
        db = QSqlDatabase::addDatabase(oracle ? "QODBC" : "QSQLITE", connectionName);
    if (db.isOpen())
        db.close();

    if (oracle) {
        settings.setValue("oracle/driver", driver->text().trimmed());
        settings.setValue("oracle/service", service->text().trimmed());
        settings.setValue("oracle/username", username->text().trimmed());
        db.setDatabaseName(QStringLiteral("Driver={%1};DBQ=%2;")
                               .arg(driver->text().trimmed(), service->text().trimmed()));
        db.setUserName(username->text().trimmed());
        db.setPassword(password->text());
    } else {
        db.setDatabaseName("safestation.db");
        db.setUserName(QString());
        db.setPassword(QString());
    }

    if (!db.open()) {
        QMessageBox::critical(parent, "Connexion impossible", db.lastError().text());
        return false;
    }
    if (oracle) {
        QSqlQuery test(db);
        if (!test.exec("SELECT 1 FROM DUAL")) {
            QMessageBox::critical(parent, "Test Oracle échoué", test.lastError().text());
            db.close();
            return false;
        }
        QSqlQuery schema(db);
        if (!schema.exec("SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME IN ('ZONE_COUVERTURE','EMPLOYES','VEHICULES','EQUIPEMENTS','CAMPAGNES_SENSIBILISATION','INTERVENTIONS','PARTICIPER_UTILISER','NECESSITER','ANIMER')")
            || !schema.next() || schema.value(0).toInt() != 9) {
            QMessageBox::critical(parent, "Schéma Oracle manquant",
                "Le schéma commun n’est pas installé dans cet utilisateur Oracle. "
                "Dans SQL Developer, exécutez integ1/database/oracle_schema.sql (F5), puis relancez l’application.");
            db.close();
            return false;
        }
        QString historyError;
        if (!ensureVehicleEventsTable(db, &historyError)) {
            QMessageBox::critical(parent, "Historique des véhicules",
                "Impossible de préparer le calendrier des événements : " + historyError);
            db.close();
            return false;
        }
    }
    if (!oracle && !ensureEquipmentTable(db, false)) {
        QMessageBox::critical(parent, "Base de données", db.lastError().text());
        db.close();
        return false;
    }
    db.setPassword(QString()); // Ne conserve pas le secret dans l'objet Qt après le test.
    return true;
}
