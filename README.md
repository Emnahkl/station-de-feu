# FireStation Manager — Module « Gestion des zones de couverture »

Projet Qt Widgets / C++17 (Qt 5.15+ ou Qt 6). Aucune dépendance externe (pas de QtWebEngine ni QtCharts).

## Compilation
**Qt Creator** : ouvrir `CMakeLists.txt` (ou `FireStationZones.pro` avec qmake) → Configurer → Exécuter.

**Ligne de commande** :
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/FireStationZones
```
Le formulaire `src/ui/ZoneDialog.ui` s'ouvre et se modifie dans **Qt Designer**.
Le logo et le fond de la barre latérale sont embarqués via `resources/resources.qrc` (remplacer `logo.png` / `sidebar_bg.jpg` pour les changer).

## Architecture (modulaire)
| Dossier | Rôle | Dépend de |
|---|---|---|
| `core/` | Modèle & métier : `Zone`, `ZoneRepository` (CRUD + JSON), `ZoneTableModel`, `ZoneFilterProxy` (recherche/tri), `CoverageAnalyzer` (alertes, stats), `Exporter` (PDF/Excel) | Qt Core/Gui uniquement |
| `widgets/` | Composants réutilisables : `ZoneMapWidget` (carte interactive), `StatsChartWidget`, `AlertPanel`, `ZoneDetailPanel`, `KpiCard`, `LogoWidget` | `core/` |
| `ui/` | Assemblage : `ZoneDialog` (.ui), `ZoneManagementPage`, `MainWindow`, `Theme` (QSS) | `core/`, `widgets/` |

Les données sont sauvegardées automatiquement dans `zones.json` (dossier AppData de l'application).

## Correspondance avec le cahier des charges
- **Entité Zone de couverture** : ID zone, Nom, Région/quartier, Superficie, Population, Niveau de risque, Temps d'intervention moyen.
- **Utilisateur** : Chef de centre.
- **CRUD** : créer (bouton ou clic sur la carte), consulter (fiche), modifier (bouton / double-clic), supprimer.
- **Métiers basiques** : tri par niveau de risque ou temps d'intervention ; recherche par nom ou région ; export PDF et Excel (CSV `;` UTF-8) ; statistiques par zone ou par niveau de risque (onglet « Statistiques »).
- **Métiers innovants** : carte des zones à risque selon l'historique des interventions (bouton « Enregistrer une intervention » recalcule le temps moyen) ; **alerte de zone sous-couverte** quand le temps moyen dépasse le seuil réglable (anneau pulsant sur la carte + liste d'alertes).
- **Fonction territoriale** : carte de Tunis avec identification des zones par ID, visualisation des risques, liaison avec équipes / véhicules / interventions (fiche de zone).

## Carte
Molette = zoom · glisser = déplacer · clic = sélectionner · double-clic = modifier ·
« Placer sur la carte » = clic pour positionner une nouvelle zone · « Déplacer » = glisser une zone.

« Fond de carte » permet de charger une capture/image de carte. Elle est étirée sur l'emprise
lat 36.70→36.93, lon 10.07→10.38 (constantes `kLatMin/kLatMax/kLonMin/kLonMax` dans `ZoneMapWidget.cpp`).
