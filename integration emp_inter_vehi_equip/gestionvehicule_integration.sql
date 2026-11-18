-- Extension du schema GESTION_VEHICULE pour les modules Employes / Planning.
-- A executer une seule fois (ou a relancer sans risque) avec SQL Developer,
-- connecte au meme schema Oracle que l'application, via F5 / Run Script.
-- Le schema existant reste en place : EMPLOYES, INTERVENTIONS et leurs liens
-- ne sont ni renommes ni supprimes.

SET DEFINE OFF

DECLARE
  PROCEDURE ajouter_colonne_si_absente(p_table VARCHAR2, p_colonne VARCHAR2, p_definition VARCHAR2) IS
    v_count NUMBER;
  BEGIN
    SELECT COUNT(*) INTO v_count
      FROM USER_TAB_COLUMNS
     WHERE TABLE_NAME = UPPER(p_table)
       AND COLUMN_NAME = UPPER(p_colonne);
    IF v_count = 0 THEN
      EXECUTE IMMEDIATE 'ALTER TABLE ' || DBMS_ASSERT.SIMPLE_SQL_NAME(UPPER(p_table)) ||
                        ' ADD (' || DBMS_ASSERT.SIMPLE_SQL_NAME(UPPER(p_colonne)) || ' ' || p_definition || ')';
    END IF;
  END;
BEGIN
  ajouter_colonne_si_absente('EMPLOYES', 'PHOTO', 'VARCHAR2(500)');
  ajouter_colonne_si_absente('EMPLOYES', 'STATUT', 'VARCHAR2(30) DEFAULT ''En service''');
  ajouter_colonne_si_absente('EMPLOYES', 'EMPLOI_TEMPS', 'VARCHAR2(100) DEFAULT ''Aucune''');
  ajouter_colonne_si_absente('EMPLOYES', 'VOLONTAIRE', 'NUMBER(1) DEFAULT 0 NOT NULL');
  EXECUTE IMMEDIATE 'ALTER TABLE EMPLOYES MODIFY (MOT_DE_PASSE DEFAULT ''N/A'')';
END;
/

-- Calendrier de garde des employes (le module source cree cette table lui-meme,
-- mais l'inclure ici evite une creation de schema au lancement de l'application).
DECLARE
  v_count NUMBER;
BEGIN
  SELECT COUNT(*) INTO v_count FROM USER_TABLES WHERE TABLE_NAME = 'PLANNING_JOUR';
  IF v_count = 0 THEN
    EXECUTE IMMEDIATE
      'CREATE TABLE PLANNING_JOUR (' ||
      ' ID_EMPLOYE NUMBER(10) NOT NULL,' ||
      ' JOUR DATE NOT NULL,' ||
      ' GARDE VARCHAR2(100),' ||
      ' CONSTRAINT PK_PLANNING_JOUR PRIMARY KEY (ID_EMPLOYE, JOUR),' ||
      ' CONSTRAINT FK_PLANNING_EMPLOYE FOREIGN KEY (ID_EMPLOYE)' ||
      ' REFERENCES EMPLOYES (ID_EMPLOYE) ON DELETE CASCADE)';
  END IF;
END;
/

-- La table INTERVENTIONS de votre export contient deja les champs attendus :
-- TYPE_INTERVENTION, ADRESSE, DATE_INTERVENTION, HEURE, GRAVITE, STATUT, ID_ZONE.
-- La table PARTICIPER_UTILISER relie deja les employes, vehicules et interventions.
-- Les modules C++ doivent utiliser EMPLOYES (pluriel) et la sequence SEQ_EMPLOYES
-- de votre schema, et enregistrer le nom complet dans NOM_PRENOM.

COMMIT;
