
DROP TABLE IF EXISTS tmp; -- else it is dropped at end of session automatically
CREATE TEMP TABLE tmp (

   id                BIGINT                  NOT NULL,
   betweenness       DOUBLE PRECISION,
   level             INTEGER
);

COPY tmp FROM '/home/rohithreddy/mystuff/research/pgrouting-optimisation/data/chandigarh_cars_betweenness.csv' delimiter ',' csv;
CREATE INDEX results_index ON tmp(id);

UPDATE cleaned_ways
SET    betweenness = tmp.betweenness, level = tmp.level, promoted_level = tmp.level
FROM   tmp
WHERE  cleaned_ways.id = tmp.id
/*
UPDATE cleaned_ways_vertices_pgr
FROM tmp
WHERE cleaned_ways_vertices_pgr.id = tmp.source;
*/
DROP TABLE tmp; -- else it is dropped at end of session automatically