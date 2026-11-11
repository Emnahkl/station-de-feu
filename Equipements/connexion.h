#ifndef CONNEXION_H
#define CONNEXION_H

#include <QString>

// Ouvre le DSN Oracle et vérifie la table EQUIPEMENTS attendue par le MLD.
class Connexion
{
public:
    static bool ouvrir();
    static QString dernierErreur();
};

#endif // CONNEXION_H
