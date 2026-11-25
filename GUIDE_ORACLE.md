# Configurer la base Oracle pour GestionVehicules

Le schéma partagé contient les neuf tables définies dans `integ1/database/oracle_schema.sql`. L'interface Qt de ce projet réalise uniquement le CRUD des véhicules dans `VEHICULES`.

1. Démarrer Oracle XE et vérifier que le pilote Oracle ODBC 64 bits est installé pour le kit Qt 64 bits.
2. Dans SQL Developer, se connecter avec un compte DBA (par exemple `SYSTEM`). Ouvrir `integ1/database/create_app_user.sql`, remplacer `REMPLACER_PAR_UN_MOT_DE_PASSE` par un mot de passe choisi, puis exécuter le script avec **F5**.
3. Créer dans SQL Developer une connexion `GESTION_VEHICULE` vers le même hôte, port et service/PDB, avec le nouvel utilisateur.
4. Sélectionner cette connexion `GESTION_VEHICULE`, ouvrir `integ1/database/oracle_schema.sql` et exécuter le script avec **F5**. Vérifier dans le panneau Tables que les neuf tables sont présentes. Le script crée les zones de départ et ne supprime aucun objet existant.
5. Ouvrir `gestionvehicule.pro` dans Qt Creator, compiler et lancer. L'application se connecte à Oracle. Saisir le nom exact du pilote ODBC, le service Oracle, `GESTION_VEHICULE` et le mot de passe choisi à l'étape 2.
6. L'écran Véhicules ajoute, affiche, modifie et supprime les lignes de `VEHICULES`. Les autres tables existent dans Oracle pour le modèle commun, sans écran CRUD dans ce module Qt.

Ne pas exécuter le schéma applicatif avec `SYSTEM` et ne pas supprimer les anciennes tables de ce compte. Chaque membre peut installer le même schéma dans son compte Oracle. Les données sont réellement communes entre postes uniquement si tous se connectent au même serveur et au même schéma.
