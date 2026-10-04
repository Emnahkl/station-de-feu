#include <QApplication>
#include "gestioninterventions.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GestionInterventions w;
    w.showMaximized();
    return a.exec();
}
