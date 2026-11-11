-- =====================================================================
--  firestation_bd.sql  -  BASE COMMUNE du projet firestation_groupe1
--  Oracle 11g XE   |   utilisateur : PROJET / projet123
--
--  * Meme script pour TOUS les membres de l'equipe -> meme modele physique.
--  * Relancable sans danger : ce qui existe deja n'est ni supprime ni modifie.
--  * Contient les 9 tables du MLD valide + 5 sequences
--    + PLANNING_JOUR (calendrier de travail des employes, ajoutee pour l'interface).
--  * EMPLOYE contient 3 colonnes en plus du MLD (statut, emploi_temps, volontaire),
--    necessaires a l'interface : a faire valider par le tuteur.
-- =====================================================================

-- =====================================================================
--  PARTIE 1 : a executer UNE SEULE FOIS, dans la connexion XE_SYSTEM (F5)
--  (si l'utilisateur existe deja, l'erreur ORA-01920 est normale : continuez)
-- =====================================================================
CREATE USER projet IDENTIFIED BY projet123;
ALTER USER projet IDENTIFIED BY projet123 ACCOUNT UNLOCK;
GRANT CONNECT, RESOURCE TO projet;
GRANT UNLIMITED TABLESPACE TO projet;

-- =====================================================================
--  PARTIE 2 : a executer dans la connexion PROJET (F5)
--  Verifier d'abord :  SELECT USER FROM DUAL;   ->  PROJET
-- =====================================================================
DECLARE
    -- execute une instruction DDL ; ignore l'erreur "nom deja utilise" (ORA-00955)
    PROCEDURE ddl(p_sql VARCHAR2) IS
    BEGIN
        EXECUTE IMMEDIATE p_sql;
    EXCEPTION
        WHEN OTHERS THEN
            IF SQLCODE != -955 THEN
                RAISE;
            END IF;
    END;
BEGIN
    -- ---------- tables sans cle etrangere ----------
    ddl(q'[CREATE TABLE ZONE_COUVERTURE (
        id_zone                  NUMBER(10) PRIMARY KEY,
        nom                      VARCHAR2(50) NOT NULL,
        region_quartier          VARCHAR2(100),
        superficie               NUMBER(10,2) CHECK (superficie >= 0),
        population               NUMBER(10)   CHECK (population >= 0),
        niveau_risque            VARCHAR2(20),
        temps_intervention_moyen NUMBER(5,2)
    )]');

    ddl(q'[CREATE TABLE EMPLOYE (
        id_employe    NUMBER(10)    PRIMARY KEY,
        nom           VARCHAR2(50)  NOT NULL,
        prenom        VARCHAR2(50)  NOT NULL,
        photo         VARCHAR2(255),
        grade         VARCHAR2(50),
        specialite    VARCHAR2(50),
        telephone     VARCHAR2(15),
        statut        VARCHAR2(30)  DEFAULT 'En service',
        emploi_temps  VARCHAR2(100) DEFAULT 'Aucune',
        volontaire    NUMBER(1)     DEFAULT 0
    )]');

    ddl(q'[CREATE TABLE VEHICULE (
        immatriculation VARCHAR2(20) PRIMARY KEY,
        type            VARCHAR2(50) NOT NULL,
        etat            VARCHAR2(30),
        kilometrage     NUMBER(10) DEFAULT 0 CHECK (kilometrage >= 0)
    )]');

    -- ---------- tables avec cles etrangeres ----------
    ddl(q'[CREATE TABLE INTERVENTION (
        id_intervention NUMBER(10) PRIMARY KEY,
        type            VARCHAR2(50) NOT NULL,
        adresse         VARCHAR2(150),
        date_heure      TIMESTAMP DEFAULT SYSTIMESTAMP,
        gravite         VARCHAR2(20),
        id_zone         NUMBER(10) NOT NULL,
        CONSTRAINT fk_interv_zone FOREIGN KEY (id_zone) REFERENCES ZONE_COUVERTURE(id_zone)
    )]');

    ddl(q'[CREATE TABLE SENSIBILISATION (
        id_campagne     NUMBER(10) PRIMARY KEY,
        titre           VARCHAR2(100) NOT NULL,
        theme           VARCHAR2(100),
        date_campagne   DATE,
        lieu            VARCHAR2(100),
        public_cible    VARCHAR2(100),
        nb_participants NUMBER(6) CHECK (nb_participants >= 0),
        id_zone         NUMBER(10) NOT NULL,
        CONSTRAINT fk_sensib_zone FOREIGN KEY (id_zone) REFERENCES ZONE_COUVERTURE(id_zone)
    )]');

    ddl(q'[CREATE TABLE EQUIPEMENT (
        id_equipement   NUMBER(10) PRIMARY KEY,
        categorie       VARCHAR2(50),
        designation     VARCHAR2(100) NOT NULL,
        quantite        NUMBER(6) DEFAULT 0 CHECK (quantite >= 0),
        etat            VARCHAR2(30),
        date_peremption DATE,
        immatriculation VARCHAR2(20),
        CONSTRAINT fk_equip_vehicule FOREIGN KEY (immatriculation) REFERENCES VEHICULE(immatriculation)
    )]');

    -- ---------- tables d'association ----------
    ddl(q'[CREATE TABLE PARTICIPER (
        id_employe      NUMBER(10),
        immatriculation VARCHAR2(20),
        id_intervention NUMBER(10),
        role_equipe     VARCHAR2(50),
        CONSTRAINT pk_participer PRIMARY KEY (id_employe, immatriculation, id_intervention),
        CONSTRAINT fk_part_emp FOREIGN KEY (id_employe)      REFERENCES EMPLOYE(id_employe),
        CONSTRAINT fk_part_veh FOREIGN KEY (immatriculation) REFERENCES VEHICULE(immatriculation),
        CONSTRAINT fk_part_int FOREIGN KEY (id_intervention) REFERENCES INTERVENTION(id_intervention)
    )]');

    ddl(q'[CREATE TABLE NECESSITER (
        id_intervention NUMBER(10),
        id_equipement   NUMBER(10),
        qte_utilisee    NUMBER(6) CHECK (qte_utilisee >= 0),
        CONSTRAINT pk_necessiter PRIMARY KEY (id_intervention, id_equipement),
        CONSTRAINT fk_nec_int   FOREIGN KEY (id_intervention) REFERENCES INTERVENTION(id_intervention),
        CONSTRAINT fk_nec_equip FOREIGN KEY (id_equipement)   REFERENCES EQUIPEMENT(id_equipement)
    )]');

    ddl(q'[CREATE TABLE ANIMER (
        id_employe  NUMBER(10),
        id_campagne NUMBER(10),
        CONSTRAINT pk_animer PRIMARY KEY (id_employe, id_campagne),
        CONSTRAINT fk_anim_emp  FOREIGN KEY (id_employe)  REFERENCES EMPLOYE(id_employe),
        CONSTRAINT fk_anim_camp FOREIGN KEY (id_campagne) REFERENCES SENSIBILISATION(id_campagne)
    )]');

    -- calendrier de travail par jour (module Employe : bouton Calendrier)
    ddl(q'[CREATE TABLE PLANNING_JOUR (
        id_employe  NUMBER(10) NOT NULL,
        jour        DATE       NOT NULL,
        garde       VARCHAR2(100),
        CONSTRAINT pk_planning_jour PRIMARY KEY (id_employe, jour),
        CONSTRAINT fk_plan_emp FOREIGN KEY (id_employe) REFERENCES EMPLOYE(id_employe)
    )]');

    -- ---------- sequences ----------
    ddl('CREATE SEQUENCE seq_employe      START WITH 1 INCREMENT BY 1');
    ddl('CREATE SEQUENCE seq_zone         START WITH 1 INCREMENT BY 1');
    ddl('CREATE SEQUENCE seq_intervention START WITH 1 INCREMENT BY 1');
    ddl('CREATE SEQUENCE seq_campagne     START WITH 1 INCREMENT BY 1');
    ddl('CREATE SEQUENCE seq_equipement   START WITH 1 INCREMENT BY 1');
END;
/

-- Si EMPLOYE existait deja SANS les 3 colonnes de l'interface, on les ajoute
DECLARE
    PROCEDURE ajout_col(p_col VARCHAR2, p_def VARCHAR2) IS
        n NUMBER;
    BEGIN
        SELECT COUNT(*) INTO n FROM user_tab_columns
         WHERE table_name = 'EMPLOYE' AND column_name = p_col;
        IF n = 0 THEN
            EXECUTE IMMEDIATE 'ALTER TABLE EMPLOYE ADD (' || p_col || ' ' || p_def || ')';
        END IF;
    END;
BEGIN
    ajout_col('STATUT',       q'[VARCHAR2(30) DEFAULT 'En service']');
    ajout_col('EMPLOI_TEMPS', q'[VARCHAR2(100) DEFAULT 'Aucune']');
    ajout_col('VOLONTAIRE',   'NUMBER(1) DEFAULT 0');
END;
/

-- Controles de saisie sur EMPLOYE (ignores s'ils existent deja)
BEGIN
    EXECUTE IMMEDIATE q'[ALTER TABLE EMPLOYE ADD CONSTRAINT ck_emp_statut
        CHECK (statut IN ('En service', 'En pause', 'En formation'))]';
EXCEPTION WHEN OTHERS THEN NULL;
END;
/
BEGIN
    EXECUTE IMMEDIATE q'[ALTER TABLE EMPLOYE ADD CONSTRAINT ck_emp_specialite
        CHECK (specialite IN ('Fire', 'Medic', 'Logistics'))]';
EXCEPTION WHEN OTHERS THEN NULL;
END;
/
BEGIN
    EXECUTE IMMEDIATE 'ALTER TABLE EMPLOYE ADD CONSTRAINT ck_emp_volontaire CHECK (volontaire IN (0, 1))';
EXCEPTION WHEN OTHERS THEN NULL;
END;
/

-- Donnees de test EMPLOYE (inserees seulement si la table est vide)
DECLARE
    n NUMBER;
    PROCEDURE ins(p_grade VARCHAR2, p_spec VARCHAR2, p_statut VARCHAR2, p_garde VARCHAR2) IS
    BEGIN
        INSERT INTO EMPLOYE (id_employe, nom, prenom, photo, grade, specialite,
                             telephone, statut, emploi_temps, volontaire)
        VALUES (seq_employe.NEXTVAL, 'Agent', TO_CHAR(seq_employe.CURRVAL), NULL,
                p_grade, p_spec,
                '2162000' || LPAD(TO_CHAR(seq_employe.CURRVAL), 4, '0'),
                p_statut, p_garde, 0);
    END;
BEGIN
    SELECT COUNT(*) INTO n FROM EMPLOYE;
    IF n = 0 THEN
        ins('1', 'Fire',      'En service',   'Garde de jour (08h-20h)');
        ins('2', 'Medic',     'En pause',     'Garde de jour (08h-20h)');
        ins('2', 'Medic',     'En pause',     'Garde de nuit (20h-08h)');
        ins('2', 'Logistics', 'En formation', 'Aucune');
        ins('2', 'Medic',     'En pause',     'Garde de nuit (20h-08h)');
        ins('2', 'Logistics', 'En formation', 'Aucune');
        ins('2', 'Logistics', 'En formation', 'Astreinte week-end');
        ins('3', 'Fire',      'En service',   'Garde de jour (08h-20h)');
        ins('3', 'Logistics', 'En pause',     'Aucune');
        ins('3', 'Logistics', 'En formation', 'Aucune');
        ins('3', 'Logistics', 'En formation', 'Aucune');
        ins('3', 'Medic',     'En service',   'Garde de nuit (20h-08h)');
        COMMIT;
    END IF;
END;
/

-- =====================================================================
--  VERIFICATION (a lancer avec Ctrl+Entree, une requete a la fois)
-- =====================================================================
-- SELECT table_name FROM user_tables ORDER BY table_name;        -> 10 tables (9 du MLD + PLANNING_JOUR)
-- SELECT sequence_name FROM user_sequences ORDER BY 1;           -> 5 sequences
-- SELECT COUNT(*) FROM EMPLOYE;                                  -> 12 (si base neuve)
