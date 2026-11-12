-- A executer dans SQL Developer connecte en SYSTEM sur XEPDB1
-- (Nom d'hote : localhost, Port : 1521, Nom du service : XEPDB1)

CREATE USER firestation IDENTIFIED BY esprit2026;
GRANT CONNECT, RESOURCE, CREATE VIEW TO firestation;
ALTER USER firestation QUOTA UNLIMITED ON USERS;
