#ifndef GESTIONINTERVENTIONS_H
#define GESTIONINTERVENTIONS_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class GestionInterventions; }
QT_END_NAMESPACE

class GestionInterventions : public QWidget
{
    Q_OBJECT
public:
    explicit GestionInterventions(QWidget *parent = nullptr);
    ~GestionInterventions();

private:
    Ui::GestionInterventions *ui;
};

#endif // GESTIONINTERVENTIONS_H
