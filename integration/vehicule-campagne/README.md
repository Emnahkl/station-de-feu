# Véhicules et campagnes

Application Qt Widgets réunissant la gestion des véhicules et des campagnes de sensibilisation dans une seule interface. Le menu latéral reprend le logo et l’image de fond de la caserne. Les sections Équipe et Équipements ne sont pas incluses.

## Compilation

Ouvrir `integration_vehicule_campagne.pro` dans Qt Creator avec Qt 6 et MinGW, puis compiler et lancer. Modules requis : Widgets, SQL et PrintSupport, ainsi que le pilote SQLite de Qt.

Les véhicules sont enregistrés dans le fichier JSON local de l’application. Les campagnes utilisent la base SQLite `firestation.db`.
