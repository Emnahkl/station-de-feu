#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

class QLineEdit;
class QLabel;

// Fenêtre de connexion affichée avant le menu principal.
// Pour le moment, « Se connecter » donne accès à l'application même si
// les champs sont vides : la vérification (table EMPLOYE, mot de passe haché,
// rôle) sera branchée plus tard dans verifierIdentifiants().
class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);

    QString nomUtilisateur() const;

private slots:
    void seConnecter();

private:
    bool verifierIdentifiants(const QString &nom, const QString &motDePasse) const;

    QLineEdit *m_nom = nullptr;
    QLineEdit *m_motDePasse = nullptr;
    QLabel *m_erreur = nullptr;
};

#endif // LOGINDIALOG_H
