-- Migration ponctuelle : colonnes de coordonnées attendues par la carte Qt.
-- À exécuter une seule fois après création du schéma FireStation fourni.
ALTER TABLE ZONE_COUVERTURE ADD (
  LATITUDE  NUMBER(10,7),
  LONGITUDE NUMBER(10,7)
);

-- Accélère le calcul du risque sur les 12 derniers mois.
CREATE INDEX IX_INTERVENTIONS_ZONE_DATE
  ON INTERVENTIONS (ID_ZONE, DATE_INTERVENTION);
