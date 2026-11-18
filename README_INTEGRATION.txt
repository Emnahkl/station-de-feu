SafeStation — intégration Gestion des véhicules + Gestion des équipements
==========================================================================

L'application s'ouvre sur le module Véhicules. Le module Équipements est
accessible dans le menu « Modules » > « Gestion des équipements ».
Les deux modules utilisent la même connexion Oracle ouverte au démarrage.

Ouverture dans Qt Creator
-------------------------
1. Extraire tout le ZIP dans un dossier simple (ne pas ouvrir les fichiers
   directement depuis l'archive).
2. Ouvrir gestionvehicule.pro dans Qt Creator.
3. Choisir un kit Qt Desktop 64 bits qui contient les modules SQL et Charts.
4. Compiler puis lancer. À la première ouverture, saisir le pilote Oracle,
   le service/PDB, le nom utilisateur et le mot de passe Oracle.
   Le mot de passe est demandé à chaque lancement et n'est pas enregistré.

Préparer Oracle
---------------
La base doit être la même pour les deux modules et contenir VEHICULES et
EQUIPEMENTS avec les colonnes utilisées par les deux CRUD.

Nouvelle base applicative : dans SQL Developer, connecté au compte applicatif,
ouvrir integ1/database/oracle_schema.sql et exécuter avec F5. Ce schéma crée
les tables communes et comprend les champs requis par le CRUD Équipements.

Schéma déjà créé : ne réexécutez pas le script de création complet. Ouvrir
integ1/database/migrate_equipements_oracle.sql connecté au même compte
applicatif et exécuter avec F5. Cette migration conserve les lignes présentes
et ajoute les colonnes CRUD absentes ainsi que la génération d'identifiant si
elle manque. Si Oracle signale que le compte ne peut pas faire ALTER/CREATE,
utiliser un compte administrateur seulement pour accorder les droits requis.

Le module Véhicules enregistre et lit VEHICULES. Le module Équipements fait
Ajouter, Consulter, Modifier et Supprimer dans EQUIPEMENTS, avec recherche,
tri, alertes de stock/contrôle/péremption, statistiques et exports PDF/CSV.
La colonne IMMATRICULATION d'un équipement reste facultative; si elle est
renseignée et qu'une clé étrangère est présente, le véhicule doit exister dans
VEHICULES.

Métiers innovants équipements
-----------------------------
Dans l'écran Équipements, sélectionne une ligne puis clique « QR de
l'équipement sélectionné ». Le PNG contient l'identifiant stable sous la forme
EQ:<ID>. Imprime une étiquette QR par référence d'équipement; pour un stock de
plusieurs unités de cette référence, scanne son étiquette autant de fois que la
quantité attendue pendant le contrôle.

Clique « Inventaire QR + email des manquants », saisis l'immatriculation puis
scanne les étiquettes avec un lecteur QR qui agit comme un clavier et envoie
Entrée après chaque scan. Le programme compare le nombre scanné avec QUANTITE
pour les équipements affectés à cette immatriculation. Si des pièces manquent,
il demande un destinataire et ouvre un brouillon prérempli dans l'application
de messagerie par défaut; vérifie-le puis clique Envoyer. Aucun mot de passe
mail n'est stocké par le programme.

Les fichiers originaux des deux archives ont été copiés dans ce projet
intégré; les archives sources ne sont pas modifiées. Avant le push GitHub,
vérifier le ZIP et tester l'application sur votre kit Qt et votre Oracle.
