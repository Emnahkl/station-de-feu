#ifndef CONNEXION_H
#define CONNEXION_H

// Ouvre la base de données et crée la table EQUIPEMENT si elle n'existe pas.
class Connexion
{
public:
    static bool ouvrir();
};

#endif // CONNEXION_H
