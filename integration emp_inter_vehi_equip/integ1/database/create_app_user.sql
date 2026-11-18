-- Run once from SQL Developer while connected as SYSTEM (or another DBA).
-- Replace the password below locally before execution. Do not commit/share it.
-- Use the same Oracle service/PDB as the application connection.

CREATE USER GESTION_VEHICULE IDENTIFIED BY "REMPLACER_PAR_UN_MOT_DE_PASSE"
    DEFAULT TABLESPACE USERS
    TEMPORARY TABLESPACE TEMP
    QUOTA UNLIMITED ON USERS;

GRANT CREATE SESSION, CREATE TABLE, CREATE SEQUENCE, CREATE TRIGGER
    TO GESTION_VEHICULE;
