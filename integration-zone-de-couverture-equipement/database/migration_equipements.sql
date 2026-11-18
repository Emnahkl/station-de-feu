
--------------------------------------------------------------------------------
-- Extension idempotente pour le CRUD Équipements intégré à Lalaf
-- À exécuter connecté comme FIRESTATION_APP. Préserve les lignes existantes.
--------------------------------------------------------------------------------
DECLARE
  PROCEDURE add_column_if_missing(p_column VARCHAR2, p_definition VARCHAR2) IS
    v_count NUMBER;
  BEGIN
    SELECT COUNT(*) INTO v_count FROM USER_TAB_COLUMNS
     WHERE TABLE_NAME = 'EQUIPEMENTS' AND COLUMN_NAME = UPPER(p_column);
    IF v_count = 0 THEN
      EXECUTE IMMEDIATE 'ALTER TABLE EQUIPEMENTS ADD (' || p_column || ' ' || p_definition || ')';
    END IF;
  END;
BEGIN
  add_column_if_missing('DESIGNATION', 'VARCHAR2(150 CHAR)');
  add_column_if_missing('DATE_ACQUISITION', 'DATE');
  add_column_if_missing('DATE_PROCHAIN_CONTROLE', 'DATE');
  add_column_if_missing('SEUIL_ALERTE', 'NUMBER(10) DEFAULT 5 NOT NULL');
END;
/
COMMIT;
