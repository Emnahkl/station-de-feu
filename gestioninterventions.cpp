#include "gestioninterventions.h"
#include "ui_gestioninterventions.h"
#include <QHeaderView>

GestionInterventions::GestionInterventions(QWidget *parent)
    : QWidget(parent), ui(new Ui::GestionInterventions)
{
    ui->setupUi(this);
    ui->tableInterventions->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

GestionInterventions::~GestionInterventions()
{
    delete ui;
}
