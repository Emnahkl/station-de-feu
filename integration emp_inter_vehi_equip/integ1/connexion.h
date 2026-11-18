#ifndef CONNEXION_H
#define CONNEXION_H

class QWidget;

// Demande le choix SQLite (démonstration) ou Oracle (ODBC), sans stocker le mot de passe.
class Connexion
{
public:
    static bool ouvrir(QWidget *parent = nullptr, bool oracleOnly = false);
};

#endif // CONNEXION_H
