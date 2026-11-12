# Connexion Oracle depuis Qt

L’application se connecte à Oracle XE via le pilote Qt `QODBC` et le pilote ODBC 64 bits `Oracle in XE`. SQL Developer sert à administrer la base ; l’application Qt utilise directement le même service XE.

## Configuration locale

Le fichier `database/connection.ini` contient les paramètres locaux de connexion. Il est ignoré par Git pour éviter de publier les identifiants. Valeurs prises en charge dans la section `[oracle]` :

- `user` et `password` : compte de l’application Oracle ;
- `service` : nom du service, par défaut `XE` ;
- `driver` : nom du pilote ODBC, par défaut `Oracle in XE` ;
- `dsn` : facultatif, chaîne ODBC complète ou DSN nommé.

Le programme cherche ce fichier depuis le dossier du projet (l’exécutable Qt Creator se trouve dans `build/<kit>/debug`). On peut aussi définir `FIRESTATION_CONFIG` avec un chemin explicite. Les variables `FIRESTATION_DB_USER`, `FIRESTATION_DB_PASSWORD`, `FIRESTATION_DB_SERVICE`, `FIRESTATION_ODBC_DRIVER` et `FIRESTATION_DSN` remplacent les valeurs correspondantes du fichier.

## Schéma et données

Le schéma `FIRESTATION_APP` doit contenir `ZONE_COUVERTURE` avec les colonnes `ID_ZONE`, `NOM`, `REGION`, `SUPERFICIE`, `POPULATION`, `NIVEAU_RISQUE`, `TEMPS_INTERVENTION`, `LATITUDE` et `LONGITUDE`. Les opérations de création, lecture, modification et suppression ciblent cette table. La lecture des zones ne dépend pas des tables d’intervention.

Au démarrage, l’application tente de se connecter et charge les zones enregistrées. Si la connexion échoue, elle affiche l’erreur et ne prétend pas que les changements ont été enregistrés dans Oracle.

Pour une nouvelle installation, créez le compte applicatif et les tables avec les scripts SQL fournis dans `database/` via SQL Developer. Les scripts contenant `DROP TABLE` ou `DROP SEQUENCE` réinitialisent les données existantes ; ne les relancez que si c’est voulu.
