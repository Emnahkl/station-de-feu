SET ECHO ON
CONNECT system/esprit2026@//localhost:1521/XEPDB1
@@1_creer_utilisateur.sql
CONNECT firestation/esprit2026@//localhost:1521/XEPDB1
@@2_creer_table.sql
EXIT
