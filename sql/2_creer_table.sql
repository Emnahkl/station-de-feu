-- Tables du module Campagnes de sensibilisation (selon le MCD)
-- A executer connecte en FIRESTATION sur XEPDB1

BEGIN EXECUTE IMMEDIATE 'DROP TABLE AFFICHE CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE CAMPAGNE CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE EMPLOYE CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/
BEGIN EXECUTE IMMEDIATE 'DROP TABLE ZONE CASCADE CONSTRAINTS'; EXCEPTION WHEN OTHERS THEN NULL; END;
/

-- EMPLOYE (id_employe, nom, prenom, email, role)
CREATE TABLE EMPLOYE (
    ID_EMPLOYE  NUMBER(6)     PRIMARY KEY,
    NOM         VARCHAR2(50)  NOT NULL,
    PRENOM      VARCHAR2(50)  NOT NULL,
    EMAIL       VARCHAR2(100) UNIQUE,
    ROLE        VARCHAR2(50)
);

-- ZONE (id_zone, nom_zone, gouvernorat, niveau_risque, population)
CREATE TABLE ZONE (
    ID_ZONE        NUMBER(6)     PRIMARY KEY,
    NOM_ZONE       VARCHAR2(100) NOT NULL,
    GOUVERNORAT    VARCHAR2(50)  NOT NULL,
    NIVEAU_RISQUE  VARCHAR2(20),
    POPULATION     NUMBER(10)
);

-- CAMPAGNE (id_campagne, titre, theme, public_cible, description, date_debut, date_fin,
--           nb_participants, budget, statut, score_impact, #id_employe, #id_zone)
-- Organiser : Campagne (1,1) - Employe (0,N)  => cle etrangere ID_EMPLOYE
-- Cibler    : Campagne (1,1) - Zone (0,N)     => cle etrangere ID_ZONE
CREATE TABLE CAMPAGNE (
    ID_CAMPAGNE      NUMBER(6)      PRIMARY KEY,
    TITRE            VARCHAR2(100)  NOT NULL,
    THEME            VARCHAR2(50)   NOT NULL,
    PUBLIC_CIBLE     VARCHAR2(50)   NOT NULL,
    DESCRIPTION      VARCHAR2(500),
    DATE_DEBUT       DATE           NOT NULL,
    DATE_FIN         DATE           NOT NULL,
    NB_PARTICIPANTS  NUMBER(6)      CHECK (NB_PARTICIPANTS > 0),
    BUDGET           NUMBER(10,2)   CHECK (BUDGET >= 0),
    STATUT           VARCHAR2(20)   NOT NULL,
    SCORE_IMPACT     NUMBER(5,2)    CHECK (SCORE_IMPACT BETWEEN 0 AND 100),
    ID_EMPLOYE       NUMBER(6)      NOT NULL,
    ID_ZONE          NUMBER(6)      NOT NULL,
    CONSTRAINT CK_CAMPAGNE_DATES CHECK (DATE_FIN >= DATE_DEBUT),
    CONSTRAINT FK_CAMPAGNE_EMPLOYE FOREIGN KEY (ID_EMPLOYE) REFERENCES EMPLOYE(ID_EMPLOYE),
    CONSTRAINT FK_CAMPAGNE_ZONE FOREIGN KEY (ID_ZONE) REFERENCES ZONE(ID_ZONE)
);

-- AFFICHE (id_affiche, chemin_image, prompt, date_generation, #id_campagne)
-- Posseder : Affiche (1,1) - Campagne (0,N)  => cle etrangere ID_CAMPAGNE
CREATE TABLE AFFICHE (
    ID_AFFICHE       NUMBER(6)      PRIMARY KEY,
    CHEMIN_IMAGE     VARCHAR2(255)  NOT NULL,
    PROMPT           VARCHAR2(1000),
    DATE_GENERATION  DATE           DEFAULT SYSDATE,
    ID_CAMPAGNE      NUMBER(6)      NOT NULL,
    CONSTRAINT FK_AFFICHE_CAMPAGNE FOREIGN KEY (ID_CAMPAGNE)
        REFERENCES CAMPAGNE(ID_CAMPAGNE) ON DELETE CASCADE
);

-- ===== Donnees d'exemple =====
INSERT INTO EMPLOYE VALUES (1, 'Ben Salah', 'Amine', 'amine.bensalah@protection-civile.tn', 'Officier prevention');
INSERT INTO EMPLOYE VALUES (2, 'Trabelsi', 'Sarra', 'sarra.trabelsi@protection-civile.tn', 'Officier prevention');
INSERT INTO EMPLOYE VALUES (3, 'Gharbi', 'Youssef', 'youssef.gharbi@protection-civile.tn', 'Chef de caserne');

INSERT INTO ZONE VALUES (1, 'Ain Draham', 'Jendouba', 'Eleve', 35000);
INSERT INTO ZONE VALUES (2, 'Ariana Ville', 'Ariana', 'Moyen', 115000);
INSERT INTO ZONE VALUES (3, 'Manouba', 'Manouba', 'Moyen', 30000);
INSERT INTO ZONE VALUES (4, 'Ghazela', 'Ariana', 'Faible', 25000);
INSERT INTO ZONE VALUES (5, 'Ben Arous', 'Ben Arous', 'Eleve', 90000);
INSERT INTO ZONE VALUES (6, 'Nabeul', 'Nabeul', 'Eleve', 75000);

INSERT INTO CAMPAGNE VALUES (1, 'Prevention incendies de foret', 'Feux de foret', 'Grand public',
    'Sensibilisation des habitants aux gestes qui evitent les feux de foret en ete',
    DATE '2026-06-01', DATE '2026-06-05', 250, 1500, 'Terminee', 82, 1, 1);
INSERT INTO CAMPAGNE VALUES (2, 'Gestes qui sauvent', 'Premiers secours', 'Eleves',
    'Initiation aux premiers secours pour les lyceens',
    DATE '2026-10-10', DATE '2026-10-10', 120, 400, 'Terminee', 90, 2, 2);
INSERT INTO CAMPAGNE VALUES (3, 'Securite gaz a la maison', 'Incendie domestique', 'Grand public',
    'Prevention des fuites de gaz et des incendies domestiques',
    DATE '2026-11-05', DATE '2026-11-20', 80, 650, 'En cours', 60, 1, 3);
INSERT INTO CAMPAGNE VALUES (4, 'Exercice evacuation ESPRIT', 'Evacuation', 'Etudiants',
    'Exercice d''evacuation grandeur nature dans un etablissement universitaire',
    DATE '2026-12-02', DATE '2026-12-02', 600, 900, 'Planifiee', 0, 3, 4);
INSERT INTO CAMPAGNE VALUES (5, 'Risques electriques au travail', 'Risques electriques', 'Employes',
    'Prevention des risques electriques en milieu industriel',
    DATE '2026-12-15', DATE '2026-12-17', 150, 1200, 'Planifiee', 0, 2, 5);
INSERT INTO CAMPAGNE VALUES (6, 'Inondations que faire', 'Intemperies', 'Grand public',
    'Conduite a tenir en cas d''inondation',
    DATE '2026-09-20', DATE '2026-09-21', 300, 2000, 'Terminee', 75, 3, 6);

INSERT INTO AFFICHE VALUES (1, 'affiches/campagne1.png',
    'Affiche prevention feux de foret, foret verte, pompier, message ne jetez pas de megots',
    DATE '2026-05-25', 1);
INSERT INTO AFFICHE VALUES (2, 'affiches/campagne2.png',
    'Affiche gestes qui sauvent, eleves, trousse de secours', DATE '2026-10-01', 2);

COMMIT;

SELECT * FROM CAMPAGNE;
