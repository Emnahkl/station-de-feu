SafeStation — Integration complète
=================================

Ce projet regroupe les modules Employés, Véhicules, Interventions et Équipements dans une seule application Qt. Ouvre `gestionvehicule.pro` à la racine du dossier dans Qt Creator. Le menu latéral commun permet de naviguer entre les modules.

Interface Équipements
--------------------
La page Équipements utilise la navigation commune du projet et reprend sa palette rouge, blanche et gris clair. Son ancien menu latéral autonome est masqué dans l'application intégrée.

Fonctions du module
-------------------
- CRUD des équipements, recherche, tri, alertes de stock, contrôles et péremption.
- Export PDF/CSV et statistiques.
- QR par équipement, puis inventaire en scannant les QR. Le lecteur QR doit agir comme un clavier et envoyer Entrée après chaque code.
- Si des unités manquent sur le véhicule choisi, l'application ouvre un brouillon d'email prérempli dans l'application de messagerie par défaut. Il faut vérifier puis envoyer le message.

Compilation
-----------
Utilise un kit Qt Desktop compatible avec les modules Widgets, SQL, Charts, PrintSupport et Multimedia. Le projet est en C++17.

Connexion Oracle et base
------------------------
L'application utilise la connexion Oracle commune `firestation-main`. Saisis le nom exact du pilote ODBC Oracle installé, le service/PDB, l'utilisateur et le mot de passe. Le mot de passe n'est pas sauvegardé par l'application.

Le code utilise le schéma SAFESTATION et les tables/pluriels du MLD fourni : `EMPLOYES`, `VEHICULES`, `INTERVENTIONS`, `EQUIPEMENTS`, etc. L'export SQL transmis a été consulté pour aligner le CRUD Équipements sur la table `EQUIPEMENTS` et ses colonnes réelles. L'export complet contenant le schéma privé n'est pas inclus dans ce dépôt.

Dans SQL Developer, connecté avec le même utilisateur applicatif que Qt :
1. Exécute `gestionvehicule_integration.sql` avec F5 pour compléter les champs Employés et le planning attendus par cette intégration.
2. Exécute `integ1/database/migrate_equipements_oracle.sql` avec F5. Le script vérifie la table `EQUIPEMENTS`, ajoute uniquement les colonnes manquantes et préserve les lignes existantes.
3. Relance l'application avec le service Oracle/PDB dans lequel se trouve le schéma.

Ne lance pas un script de création complète d'une base déjà existante. Garde une copie privée de l'export SQL et ne le commit pas.
