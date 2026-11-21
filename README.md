# Gestion des campagnes de sensibilisation — version finale

Module de **Faouzene** du projet Fire Station (Qt 6 / C++ / Oracle SAFESTATION).
C'est le même code que dans l'intégration finale, présenté ici seul et compilable seul.

## Fonctionnalités

| Onglet | Contenu |
|---|---|
| Gestion des campagnes | Ajouter, modifier, supprimer, rechercher, trier ; animateurs multiples (table ANIMER) ; export PDF et Excel (CSV) |
| Statistiques | Répartition par thème, public cible, zone ou mois (Qt Charts) |
| Ciblage des zones | Classement des zones à sensibiliser en priorité : interventions sur 12 mois × poids du risque ÷ (1 + campagnes déjà faites), puis planification en un clic |
| Évaluation d'impact | Interventions N mois avant / après la campagne dans sa zone, variation et verdict, graphique |
| Affiche & GIF | Affiche générée automatiquement (slogan selon le thème), export PNG, aperçu animé et **export en GIF animé** (encodeur GIF89a intégré : `gifwriter.cpp`) |

## Tables utilisées (MCD de l'équipe)

`CAMPAGNES_SENSIBILISATION`, `ANIMER`, `EMPLOYE`, `ZONE_COUVERTURE`, `INTERVENTIONS`.

## Lancer

1. Base : exécuter `sql/safestation_bd.sql` puis `sql/integration_zones_equipements.sql` dans le schéma `safestation` (Oracle 21c XE, base `XEPDB1`).
2. Ouvrir `CMakeLists.txt` (ou `campagnes_sensibilisation.pro`) dans Qt Creator, kit Qt 6 MinGW 64 bits, puis Ctrl+R.

La connexion essaie d'abord le DSN ODBC `SAFESTATION`, puis se connecte directement à
`localhost:1521/XEPDB1` avec le pilote ODBC Oracle (utilisateur `safestation`).
