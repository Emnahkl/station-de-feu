# Fire Station — schéma commun et module Véhicules

Le schéma Oracle partagé est défini dans [`database/oracle_schema.sql`](database/oracle_schema.sql). Il crée les neuf tables de l'application, leurs clés et leurs relations, ainsi que les séquences et triggers nécessaires à Oracle XE 11g.

Le module Qt livré dans ce projet gère uniquement l'entité `VEHICULES`. La classe [`vehicule.h`](vehicule.h) / [`vehicule.cpp`](vehicule.cpp) expose l'ajout, l'affichage/recherche, la modification et la suppression en utilisant les colonnes exactes `IMMATRICULATION`, `TYPE`, `ETAT`, `MODELE`, `KILOMETRAGE` et `DERNIERE_MAINTENANCE`.

Pour les étapes de création du compte Oracle, d'installation du schéma et de connexion depuis Qt Creator, suivre [`../GUIDE_ORACLE.md`](../GUIDE_ORACLE.md). Le compte Oracle utilisé par Qt doit être celui où les neuf tables ont été créées.
