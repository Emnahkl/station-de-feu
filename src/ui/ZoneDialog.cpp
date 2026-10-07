#include "ZoneDialog.h"
#include "ui_ZoneDialog.h"
#include <QPushButton>

ZoneDialog::ZoneDialog(QWidget* parent) : QDialog(parent), ui(new Ui::ZoneDialog)
{
    ui->setupUi(this);
    for (RiskLevel r : {RiskLevel::Faible, RiskLevel::Moyen, RiskLevel::Eleve})
        ui->cbRisque->addItem(RiskUtil::label(r), static_cast<int>(r));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Enregistrer");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Annuler");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setObjectName("primary");
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &ZoneDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

ZoneDialog::~ZoneDialog() { delete ui; }

void ZoneDialog::setZone(const Zone& z, bool isNew)
{
    m_zone = z;
    setWindowTitle(isNew ? QString::fromUtf8("Nouvelle zone de couverture")
                         : QString::fromUtf8("Modifier la zone %1").arg(z.idLabel()));
    ui->leId->setText(isNew ? QString::fromUtf8("(attribué automatiquement)") : z.idLabel());
    ui->leNom->setText(z.nom);
    ui->leRegion->setText(z.region);
    ui->dsbSuperficie->setValue(z.superficie);
    ui->sbPopulation->setValue(z.population);
    ui->cbRisque->setCurrentIndex(ui->cbRisque->findData(static_cast<int>(z.risque)));
    ui->dsbTemps->setValue(z.tempsMoyen);
    ui->sbEquipes->setValue(z.nbEquipes);
    ui->sbVehicules->setValue(z.nbVehicules);
    ui->dsbLat->setValue(z.latitude);
    ui->dsbLon->setValue(z.longitude);
    ui->dsbRayon->setValue(z.rayonKm);
}

Zone ZoneDialog::zone() const
{
    Zone z = m_zone;
    z.nom = ui->leNom->text().trimmed();
    z.region = ui->leRegion->text().trimmed();
    z.superficie = ui->dsbSuperficie->value();
    z.population = ui->sbPopulation->value();
    z.risque = RiskUtil::fromInt(ui->cbRisque->currentData().toInt());
    z.tempsMoyen = ui->dsbTemps->value();
    z.nbEquipes = ui->sbEquipes->value();
    z.nbVehicules = ui->sbVehicules->value();
    z.latitude = ui->dsbLat->value();
    z.longitude = ui->dsbLon->value();
    z.rayonKm = ui->dsbRayon->value();
    return z;
}

void ZoneDialog::accept()
{
    if (ui->leNom->text().trimmed().isEmpty()) {
        ui->lbError->setText(QString::fromUtf8("Le nom de la zone est obligatoire."));
        ui->leNom->setFocus();
        return;
    }
    if (ui->leRegion->text().trimmed().isEmpty()) {
        ui->lbError->setText(QString::fromUtf8("La région / le quartier est obligatoire."));
        ui->leRegion->setFocus();
        return;
    }
    QDialog::accept();
}
