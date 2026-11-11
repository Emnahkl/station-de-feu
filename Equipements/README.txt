SAFESTATION — CRUD ÉQUIPEMENTS (Qt / C++ / Oracle)
==================================================

Le CRUD utilise la table commune EQUIPEMENTS (au pluriel), comme dans le MLD.
Il ne crée pas une table EQUIPEMENT séparée.

FONCTIONS
- Ajouter, consulter, modifier et supprimer un équipement.
- Rechercher et trier la liste.
- Exporter en PDF ou CSV lisible avec Excel.
- Afficher les statistiques par état et catégorie.
- Signaler les stocks bas, les contrôles et les péremptions proches.
- Préparer un bon de commande PDF.

AVANT LE PREMIER LANCEMENT
1. Dans SQL Developer, connecte-toi avec l'utilisateur SAFESTATION au service
   XEPDB1 (la même base que ton DSN).
2. Ouvre sql/setup_equipements_oracle.sql et exécute tout le script avec F5.
   Si EQUIPEMENTS existe déjà avec les colonnes ID_EQUIPEMENT, CATEGORIE,
   QUANTITE, ETAT et DATE_PEREMPTION, le script garde les données et ajoute
   DESIGNATION et les colonnes CRUD manquantes. DESIGNATION reste nullable
   pour préserver les anciennes lignes. S'il n'existe pas, il le crée selon
   le MLD et les champs nécessaires au module.
3. Le compte SAFESTATION doit être propriétaire de la table ou avoir le droit
   de la modifier. Si Oracle affiche ORA-01031, il faut faire exécuter le script
   par le propriétaire/administrateur de la base.

CONNEXION WINDOWS / QT
1. Vérifie que le DSN ODBC 64 bits s'appelle exactement SafeStationOracle et
   pointe vers le service Oracle XEPDB1.
2. Décompresse le ZIP et ouvre SafeStation_Equipements.pro dans Qt Creator.
3. Choisis un kit Qt 64 bits avec Qt SQL, Qt Charts et le pilote Qt QODBC.
4. Lance l'application. Saisis SAFESTATION et son mot de passe Oracle.
   Le mot de passe est demandé à l'exécution et n'est pas enregistré dans le code.

CHAMPS DU CRUD
Le formulaire reprend les informations de la fiche du module (nom/désignation,
type/catégorie, quantité, date d'acquisition, état, prochain contrôle et
caserne/véhicule), ainsi que la date de péremption du MLD et le seuil d'alerte.
La colonne IMMATRICULATION est facultative. Si VEHICULES a déjà une clé unique
sur IMMATRICULATION, le script ajoute aussi la clé étrangère correspondante.

FICHIERS PRINCIPAUX
- connexion.cpp/.h : connexion au DSN SafeStationOracle et vérification du schéma.
- equipement.cpp/.h : classe Equipement (CRUD Oracle) et EquipementModel (tableau/couleurs).
- mainwindow.cpp/.h/.ui : interface Qt.
- exports.cpp/.h : export PDF et CSV.
- sql/setup_equipements_oracle.sql : préparation/migration du schéma Oracle.
