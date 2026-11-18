# Intégration — Zones de couverture et équipements

Ce projet Qt/C++ fusionne Lalaf et le module Équipements (stock, contrôles, péremptions, QR, inventaire et email brouillon).

## Démarrage

1. Ouvrir `fire.pro` dans Qt Creator et choisir un kit Qt Desktop avec Widgets, SQL, Charts, Multimedia, PrintSupport et Network.
2. Pour configurer Oracle, copier `database/connection.ini.example` en `database/connection.ini` puis renseigner les identifiants locaux. Le fichier de configuration local n’est pas versionné.
3. Connecter SQL Developer comme `FIRESTATION_APP` et exécuter `database/migration_equipements.sql` avec F5 sur une base existante. La migration est relançable et conserve les lignes existantes.
4. Recompiler avec qmake puis lancer l’application.

`database/export_lalaf_compatible.sql` est l’export complet corrigé pour une installation vierge seulement. Ne pas l’exécuter sur une base existante.
