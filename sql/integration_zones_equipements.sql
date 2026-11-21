-- =====================================================================
--  integration_zones_equipements.sql  -  Integration finale
--  Adapte la base commune SAFESTATION aux modules Zones de couverture
--  et Equipements (avant, ils utilisaient leur propre schema FIRESTATION_APP).
--
--  A executer dans la connexion SAFESTATION (F5), APRES safestation_bd.sql.
--  Relancable sans danger : rien n'est supprime, les lignes sont conservees.
-- =====================================================================
DECLARE
    PROCEDURE ajouter_colonne(p_table VARCHAR2, p_colonne VARCHAR2, p_definition VARCHAR2) IS
        n NUMBER;
    BEGIN
        SELECT COUNT(*) INTO n FROM USER_TAB_COLUMNS
         WHERE TABLE_NAME = p_table AND COLUMN_NAME = p_colonne;
        IF n = 0 THEN
            EXECUTE IMMEDIATE 'ALTER TABLE ' || p_table || ' ADD (' || p_colonne || ' ' || p_definition || ')';
        END IF;
    END;
BEGIN
    -- Module Zones : coordonnees pour la carte OpenStreetMap
    ajouter_colonne('ZONE_COUVERTURE', 'LATITUDE',  'NUMBER(10,7)');
    ajouter_colonne('ZONE_COUVERTURE', 'LONGITUDE', 'NUMBER(10,7)');

    -- Module Equipements : controles, acquisition et seuil d'alerte de stock
    ajouter_colonne('EQUIPEMENTS', 'DATE_ACQUISITION',       'DATE');
    ajouter_colonne('EQUIPEMENTS', 'DATE_PROCHAIN_CONTROLE', 'DATE');
    ajouter_colonne('EQUIPEMENTS', 'SEUIL_ALERTE',           'NUMBER(10) DEFAULT 5 NOT NULL');

    -- Acceleration du calcul du risque par zone (12 derniers mois)
    BEGIN
        EXECUTE IMMEDIATE 'CREATE INDEX IX_INTERVENTIONS_ZONE_DATE ON INTERVENTIONS (ID_ZONE, DATE_INTERVENTION)';
    EXCEPTION
        WHEN OTHERS THEN
            IF SQLCODE NOT IN (-955, -1408) THEN RAISE; END IF;   -- index deja present
    END;
END;
/

-- Coordonnees des zones de demonstration (seulement si elles sont vides)
UPDATE ZONE_COUVERTURE SET LATITUDE = 36.7990, LONGITUDE = 10.1800 WHERE ID_ZONE = 1 AND LATITUDE IS NULL;  -- Bab El Bhar
UPDATE ZONE_COUVERTURE SET LATITUDE = 36.8780, LONGITUDE = 10.3250 WHERE ID_ZONE = 2 AND LATITUDE IS NULL;  -- La Marsa
UPDATE ZONE_COUVERTURE SET LATITUDE = 36.8625, LONGITUDE = 10.1956 WHERE ID_ZONE = 3 AND LATITUDE IS NULL;  -- Ariana Ville
UPDATE ZONE_COUVERTURE SET LATITUDE = 36.8092, LONGITUDE = 10.1340 WHERE ID_ZONE = 4 AND LATITUDE IS NULL;  -- Le Bardo

-- Valeurs par defaut pour les equipements deja presents
UPDATE EQUIPEMENTS SET DATE_ACQUISITION = TRUNC(SYSDATE) - 180 WHERE DATE_ACQUISITION IS NULL;
UPDATE EQUIPEMENTS SET DATE_PROCHAIN_CONTROLE = TRUNC(SYSDATE) + 90 WHERE DATE_PROCHAIN_CONTROLE IS NULL;
COMMIT;

-- Le module Equipements insere sans ID : on le prend dans seq_equipement
CREATE OR REPLACE TRIGGER TRG_EQUIPEMENTS_ID
BEFORE INSERT ON EQUIPEMENTS
FOR EACH ROW
WHEN (NEW.ID_EQUIPEMENT IS NULL)
BEGIN
    SELECT seq_equipement.NEXTVAL INTO :NEW.ID_EQUIPEMENT FROM DUAL;
END;
/

-- Verification
SELECT ID_ZONE, NOM, LATITUDE, LONGITUDE FROM ZONE_COUVERTURE ORDER BY ID_ZONE;
SELECT ID_EQUIPEMENT, DESIGNATION, QUANTITE, SEUIL_ALERTE, DATE_PROCHAIN_CONTROLE FROM EQUIPEMENTS ORDER BY ID_EQUIPEMENT;
