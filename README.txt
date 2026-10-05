FIRESTATION_GROUPE1 - Gestion des employes (Qt Widgets / C++ / CMake) - SANS base de donnees

OUVRIR LE PROJET
1. Decompresser le .zip (chemin sans espaces ni accents de preference).
2. Qt Creator > Fichier > Ouvrir un fichier ou un projet... > choisir CMakeLists.txt
3. Choisir un kit (Qt 6.x ou Qt 5.15) > Configure Project
4. Ctrl+R pour lancer.

ARBORESCENCE
firestation_groupe1/
  CMakeLists.txt      (declare les sources, le .ui et resources.qrc)
  main.cpp
  mainwindow.h / mainwindow.cpp / mainwindow.ui
  imagebgwidget.h     (widget du sidebar.png)
  qrgen.h             (generateur de vrai QR Code)
  resources.qrc       (liste des images -> acces par ":/images/nom.png")
  images/             (16 images .png)

AJOUTER UNE IMAGE
Clic droit sur resources.qrc > Ajouter des fichiers existants (ou editer le .qrc), puis l'utiliser avec QPixmap(":/images/nom.png").
