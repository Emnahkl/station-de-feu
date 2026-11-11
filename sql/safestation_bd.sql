-- =====================================================================
--  safestation_bd.sql  -  BASE UNIQUE "SAFESTATION" (integration_empl_interv)
--  Oracle 11g XE   |   utilisateur / schema : SAFESTATION / safestation123
--
--  Fusionne : module Employes (firestation_bd.sql) + module Interventions.
--  Noms de tables = MLD de l'equipe (INTERVENTIONS, PARTICIPER_UTILISER,
--  VEHICULES, EQUIPEMENTS, CAMPAGNES_SENSIBILISATION...), sauf EMPLOYE qui
--  garde la structure du module Employes (nom, prenom, statut, emploi_temps...).
--  Relancable sans danger : ce qui existe deja n'est ni supprime ni modifie.
-- =====================================================================

-- =====================================================================
--  PARTIE 1 : a executer UNE SEULE FOIS, connexion XE_SYSTEM (F5)
--  (ORA-01920 "utilisateur existe deja" = normal, continuez)
-- =====================================================================
CREATE USER safestation IDENTIFIED BY safestation123;
ALTER USER safestation IDENTIFIED BY safestation123 ACCOUNT UNLOCK;
GRANT CONNECT, RESOURCE, CREATE VIEW TO safestation;
GRANT UNLIMITED TABLESPACE TO safestation;

-- =====================================================================
--  PARTIE 2 : a executer dans la connexion SAFESTATION (F5)
--  Verifier d'abord :  SELECT USER FROM DUAL;   ->  SAFESTATION
-- =====================================================================
DECLARE
    PROCEDURE ddl(p_sql VARCHAR2) IS
    BEGIN
        EXECUTE IMMEDIATE p_sql;
    EXCEPTION
        WHEN OTHERS THEN
            IF SQLCODE != -955 THEN   -- ORA-00955 : nom deja utilise -> on ignore
                RAISE;
            END IF;
    END;
BEGIN
    -- ---------- tables sans cle etrangere ----------
    ddl(q'[CREATE TABLE ZONE_COUVERTURE (
        ID_ZONE            NUMBER PRIMARY KEY,
        NOM                VARCHAR2(50) NOT NULL,
        REGION             VARCHAR2(50),
        SUPERFICIE         NUMBER,
        POPULATION         NUMBER,
        NIVEAU_RISQUE      VARCHAR2(20),
        TEMPS_INTERVENTION NUMBER
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

    ddl(q'[CREATE TABLE VEHICULES (
        IMMATRICULATION      VARCHAR2(20) PRIMARY KEY,
        TYPE                 VARCHAR2(50) NOT NULL,
        ETAT                 VARCHAR2(30),
        MODELE               VARCHAR2(50),
        KILOMETRAGE          NUMBER(10) DEFAULT 0 CHECK (KILOMETRAGE >= 0),
        DERNIERE_MAINTENANCE DATE
    )]');

    -- ---------- tables avec cles etrangeres ----------
    ddl(q'[CREATE TABLE INTERVENTIONS (
        ID_INTERVENTION    NUMBER PRIMARY KEY,
        TYPE_INTERVENTION  VARCHAR2(50)  NOT NULL,
        ADRESSE            VARCHAR2(150) NOT NULL,
        DATE_INTERVENTION  DATE          NOT NULL,
        HEURE              VARCHAR2(5)   NOT NULL,
        GRAVITE            VARCHAR2(20),
        STATUT             VARCHAR2(20),
        ID_ZONE            NUMBER        NOT NULL,
        CONSTRAINT FK_INTER_ZONE FOREIGN KEY (ID_ZONE) REFERENCES ZONE_COUVERTURE (ID_ZONE)
    )]');

    ddl(q'[CREATE TABLE CAMPAGNES_SENSIBILISATION (
        id_campagne     NUMBER(10) PRIMARY KEY,
        titre           VARCHAR2(100) NOT NULL,
        theme           VARCHAR2(100),
        date_campagne   DATE,
        lieu            VARCHAR2(100),
        public_cible    VARCHAR2(100),
        nb_participants NUMBER(6) CHECK (nb_participants >= 0),
        id_zone         NUMBER(10),
        CONSTRAINT fk_camp_zone FOREIGN KEY (id_zone) REFERENCES ZONE_COUVERTURE(id_zone)
    )]');

    ddl(q'[CREATE TABLE EQUIPEMENTS (
        ID_EQUIPEMENT   NUMBER(10) PRIMARY KEY,
        CATEGORIE       VARCHAR2(50),
        DESIGNATION     VARCHAR2(100),
        QUANTITE        NUMBER(6) DEFAULT 0 CHECK (QUANTITE >= 0),
        ETAT            VARCHAR2(30),
        DATE_PEREMPTION DATE,
        IMMATRICULATION VARCHAR2(20),
        CONSTRAINT fk_equip_vehicule FOREIGN KEY (IMMATRICULATION) REFERENCES VEHICULES(IMMATRICULATION)
    )]');

    -- ---------- tables d'association ----------
    ddl(q'[CREATE TABLE PARTICIPER_UTILISER (
        ID_EMPLOYE      NUMBER(10),
        IMMATRICULATION VARCHAR2(20),
        ID_INTERVENTION NUMBER,
        CONSTRAINT pk_participer PRIMARY KEY (ID_EMPLOYE, IMMATRICULATION, ID_INTERVENTION),
        CONSTRAINT fk_part_emp FOREIGN KEY (ID_EMPLOYE)      REFERENCES EMPLOYE(id_employe),
        CONSTRAINT fk_part_veh FOREIGN KEY (IMMATRICULATION) REFERENCES VEHICULES(IMMATRICULATION),
        CONSTRAINT fk_part_int FOREIGN KEY (ID_INTERVENTION) REFERENCES INTERVENTIONS(ID_INTERVENTION)
    )]');

    ddl(q'[CREATE TABLE NECESSITER (
        ID_INTERVENTION NUMBER,
        ID_EQUIPEMENT   NUMBER(10),
        QTE_UTILISEE    NUMBER(6) CHECK (QTE_UTILISEE >= 0),
        CONSTRAINT pk_necessiter PRIMARY KEY (ID_INTERVENTION, ID_EQUIPEMENT),
        CONSTRAINT fk_nec_int   FOREIGN KEY (ID_INTERVENTION) REFERENCES INTERVENTIONS(ID_INTERVENTION),
        CONSTRAINT fk_nec_equip FOREIGN KEY (ID_EQUIPEMENT)   REFERENCES EQUIPEMENTS(ID_EQUIPEMENT)
    )]');

    ddl(q'[CREATE TABLE ANIMER (
        id_employe  NUMBER(10),
        id_campagne NUMBER(10),
        CONSTRAINT pk_animer PRIMARY KEY (id_employe, id_campagne),
        CONSTRAINT fk_anim_emp  FOREIGN KEY (id_employe)  REFERENCES EMPLOYE(id_employe),
        CONSTRAINT fk_anim_camp FOREIGN KEY (id_campagne) REFERENCES CAMPAGNES_SENSIBILISATION(id_campagne)
    )]');

    -- calendrier de travail par jour (module Employes : bouton Calendrier)
    ddl(q'[CREATE TABLE PLANNING_JOUR (
        id_employe  NUMBER(10) NOT NULL,
        jour        DATE       NOT NULL,
        garde       VARCHAR2(100),
        CONSTRAINT pk_planning_jour PRIMARY KEY (id_employe, jour),
        CONSTRAINT fk_plan_emp FOREIGN KEY (id_employe) REFERENCES EMPLOYE(id_employe)
    )]');

    -- ---------- sequences ----------
    ddl('CREATE SEQUENCE seq_employe       START WITH 1 INCREMENT BY 1');
    ddl('CREATE SEQUENCE seq_zone          START WITH 5 INCREMENT BY 1');
    ddl('CREATE SEQUENCE SEQ_INTERVENTIONS START WITH 1 INCREMENT BY 1 NOCACHE');
    ddl('CREATE SEQUENCE seq_campagne      START WITH 1 INCREMENT BY 1');
    ddl('CREATE SEQUENCE seq_equipement    START WITH 1 INCREMENT BY 1');
END;
/

-- Numero d'intervention automatique (le module Interventions n'envoie pas l'ID)
CREATE OR REPLACE TRIGGER TRG_INTERVENTIONS_ID
BEFORE INSERT ON INTERVENTIONS
FOR EACH ROW
WHEN (NEW.ID_INTERVENTION IS NULL)
BEGIN
  SELECT SEQ_INTERVENTIONS.NEXTVAL INTO :NEW.ID_INTERVENTION FROM DUAL;
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

-- =====================================================================
--  DONNEES DE TEST (chaque bloc n'insere que si la table est vide)
-- =====================================================================
-- ---- zones (module Interventions)
DECLARE n NUMBER;
BEGIN
    SELECT COUNT(*) INTO n FROM ZONE_COUVERTURE;
    IF n = 0 THEN
        INSERT INTO ZONE_COUVERTURE VALUES (1, 'Bab El Bhar',  'Tunis',       3.2,  45000,  'Moyen', 7);
        INSERT INTO ZONE_COUVERTURE VALUES (2, 'La Marsa',     'Tunis Nord',  25.0, 92000,  'Eleve', 12);
        INSERT INTO ZONE_COUVERTURE VALUES (3, 'Ariana Ville', 'Ariana',      18.5, 114000, 'Eleve', 11);
        INSERT INTO ZONE_COUVERTURE VALUES (4, 'Le Bardo',     'Tunis Ouest', 12.0, 73000,  'Moyen', 9);
        COMMIT;
    END IF;
END;
/

-- ---- employes (module Employes) : grade 1..3 etoiles
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

-- ---- vehicules + equipements (utilises par l'affectation automatique)
DECLARE n NUMBER;
BEGIN
    SELECT COUNT(*) INTO n FROM VEHICULES;
    IF n = 0 THEN
        INSERT INTO VEHICULES VALUES ('101 TUN 1001', 'Camion incendie FPT', 'Disponible',  'Renault Midlum',  45200, SYSDATE-60);
        INSERT INTO VEHICULES VALUES ('102 TUN 1002', 'Camion citerne',      'Disponible',  'Mercedes Atego',  61800, SYSDATE-90);
        INSERT INTO VEHICULES VALUES ('103 TUN 1003', 'Ambulance VSAV',      'Disponible',  'Renault Master',  38900, SYSDATE-30);
        INSERT INTO VEHICULES VALUES ('104 TUN 1004', 'Ambulance VSAV',      'Disponible',  'Peugeot Boxer',   52300, SYSDATE-45);
        INSERT INTO VEHICULES VALUES ('105 TUN 1005', 'Secours routier VSR', 'Disponible',  'Iveco Daily',     29700, SYSDATE-20);
        INSERT INTO VEHICULES VALUES ('106 TUN 1006', 'Camion echelle',      'En panne',    'Mercedes Econic', 88100, SYSDATE-200);
        INSERT INTO VEHICULES VALUES ('107 TUN 1007', 'Vehicule pompe',      'Disponible',  'Iveco Eurocargo', 70400, SYSDATE-75);
        COMMIT;
    END IF;
END;
/
DECLARE n NUMBER;
BEGIN
    SELECT COUNT(*) INTO n FROM EQUIPEMENTS;
    IF n = 0 THEN
        INSERT INTO EQUIPEMENTS VALUES (seq_equipement.NEXTVAL, 'Extincteur',     'Extincteur CO2 6 kg',        6,  'Bon etat', SYSDATE+400, '101 TUN 1001');
        INSERT INTO EQUIPEMENTS VALUES (seq_equipement.NEXTVAL, 'Lance',          'Lance a incendie 45 mm',     4,  'Bon etat', NULL,        '101 TUN 1001');
        INSERT INTO EQUIPEMENTS VALUES (seq_equipement.NEXTVAL, 'Tuyau',          'Tuyau d''incendie 20 m',     10, 'Bon etat', NULL,        '102 TUN 1002');
        INSERT INTO EQUIPEMENTS VALUES (seq_equipement.NEXTVAL, 'Respirateur',    'Appareil respiratoire ARI',  6,  'Bon etat', SYSDATE+300, NULL);
        INSERT INTO EQUIPEMENTS VALUES (seq_equipement.NEXTVAL, 'Casque',         'Casque de pompier F1',       12, 'Bon etat', NULL,        NULL);
        INSERT INTO EQUIPEMENTS VALUES (seq_equipement.NEXTVAL, 'Defibrillateur', 'Defibrillateur DAE',         3,  'Bon etat', SYSDATE+500, '103 TUN 1003');
        INSERT INTO EQUIPEMENTS VALUES (seq_equipement.NEXTVAL, 'Brancard',       'Brancard pliant',            4,  'Bon etat', NULL,        '104 TUN 1004');
        INSERT INTO EQUIPEMENTS VALUES (seq_equipement.NEXTVAL, 'Trousse',        'Trousse de premiers secours',8,  'Bon etat', SYSDATE+250, NULL);
        INSERT INTO EQUIPEMENTS VALUES (seq_equipement.NEXTVAL, 'Oxygene',        'Bouteille d''oxygene 5 L',   5,  'Bon etat', SYSDATE+365, '103 TUN 1003');
        INSERT INTO EQUIPEMENTS VALUES (seq_equipement.NEXTVAL, 'Desincarceration','Kit de desincarceration',  2,  'Bon etat', NULL,        '105 TUN 1005');
        INSERT INTO EQUIPEMENTS VALUES (seq_equipement.NEXTVAL, 'Motopompe',      'Motopompe d''epuisement',    3,  'Bon etat', NULL,        '107 TUN 1007');
        INSERT INTO EQUIPEMENTS VALUES (seq_equipement.NEXTVAL, 'Detecteur gaz',  'Detecteur de gaz portable',  2,  'Bon etat', SYSDATE+200, NULL);
        COMMIT;
    END IF;
END;
/

-- ---- quelques interventions de test (dates relatives a aujourd'hui)
DECLARE n NUMBER;
BEGIN
    SELECT COUNT(*) INTO n FROM INTERVENTIONS;
    IF n = 0 THEN
        INSERT INTO INTERVENTIONS (TYPE_INTERVENTION, ADRESSE, DATE_INTERVENTION, HEURE, GRAVITE, STATUT, ID_ZONE)
          VALUES ('Incendie',             'Avenue Habib Bourguiba, Tunis', TRUNC(SYSDATE),   '08:15', 'Critique', 'En cours',   1);
        INSERT INTO INTERVENTIONS (TYPE_INTERVENTION, ADRESSE, DATE_INTERVENTION, HEURE, GRAVITE, STATUT, ID_ZONE)
          VALUES ('Accident de la route', 'Route de La Marsa, La Marsa',   TRUNC(SYSDATE),   '09:40', 'Élevée',   'En attente', 2);
        INSERT INTO INTERVENTIONS (TYPE_INTERVENTION, ADRESSE, DATE_INTERVENTION, HEURE, GRAVITE, STATUT, ID_ZONE)
          VALUES ('Secours à personne',   'Rue de la Liberte, Ariana',     TRUNC(SYSDATE),   '10:05', 'Moyenne',  'En attente', 3);
        INSERT INTO INTERVENTIONS (TYPE_INTERVENTION, ADRESSE, DATE_INTERVENTION, HEURE, GRAVITE, STATUT, ID_ZONE)
          VALUES ('Fuite de gaz',         'Rue 8 Mars 1934, Le Bardo',     TRUNC(SYSDATE)-3, '14:20', 'Élevée',   'Terminée',   4);
        INSERT INTO INTERVENTIONS (TYPE_INTERVENTION, ADRESSE, DATE_INTERVENTION, HEURE, GRAVITE, STATUT, ID_ZONE)
          VALUES ('Inondation',           'Avenue du 14 Janvier, Tunis',   TRUNC(SYSDATE)-10,'07:30', 'Faible',   'Terminée',   1);
        COMMIT;
    END IF;
END;
/

-- =====================================================================
--  VERIFICATION (une requete a la fois, Ctrl+Entree)
-- =====================================================================
-- SELECT table_name FROM user_tables ORDER BY 1;     -> 11 tables
-- SELECT COUNT(*) FROM EMPLOYE;                       -> 12
-- SELECT COUNT(*) FROM INTERVENTIONS;                 -> 5
-- SELECT COUNT(*) FROM VEHICULES;                     -> 7
