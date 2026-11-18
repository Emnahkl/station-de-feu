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

Dans SQL Developer, connecte-toi comme `SAFESTATION` sur la même base/PDB que Qt, ouvre `SafeStation_Integrated_DB_Setup.sql`, puis lance tout le fichier avec F5. Ce script vérifie le schéma de base, complète les champs employés/planning, et met à niveau Équipements/Véhicules si des colonnes manquent. Il préserve les tables et les lignes déjà présentes.

Relance ensuite l'application avec le même service Oracle/PDB.

Ne lance pas un script de création complète d'une base déjà existante. Garde une copie privée de l'export SQL et ne le commit pas.
