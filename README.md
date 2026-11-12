# FireStationManager - Gestion des campagnes de sensibilisation

Module "Campagnes de sensibilisation" (acteur : Officier prevention) du projet Smart Weather Management.
Application Qt 6 (C++) reliee a Oracle Database 21c XE via ODBC.

## Fonctionnalites
- CRUD des campagnes (ajouter, afficher, modifier, supprimer)
- Controle de saisie
- Recherche multicritere (titre, ID, theme, zone, organisateur, statut) et tri
- Export PDF
- Statistiques (QtCharts) par theme, statut, public cible et zone

## Base de donnees (MLD)
- EMPLOYE (id_employe, nom, prenom, email, role)
- ZONE (id_zone, nom_zone, gouvernorat, niveau_risque, population)
- CAMPAGNE (id_campagne, titre, theme, public_cible, description, date_debut, date_fin, nb_participants, budget, statut, score_impact, #id_employe, #id_zone)
- AFFICHE (id_affiche, chemin_image, prompt, date_generation, #id_campagne)

## Installation (Windows)
1. `0_installer_oracle.bat` (Oracle XE 21c, zip extrait dans Telechargements)
2. `1_installer_qt.bat` (Qt 6.8.3 MinGW + Qt Charts + Qt Creator)
3. `4_creer_base.bat` (utilisateur FIRESTATION + tables + donnees)
4. `3_compiler_et_lancer_sans_qtcreator.bat` ou `2_ouvrir_dans_qtcreator.bat`
