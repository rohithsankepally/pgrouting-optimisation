CREATE TEMP TABLE tmp (

   id                BIGINT                  NOT NULL,
   source            BIGINT                  NOT NULL,
   target            BIGINT                  NOT NULL,
   betweenness       DOUBLE PRECISION,
   level             INTEGER
);

COPY tmp FROM '/home/vrgeo/rohith/research/pgrouting-optimisation/data/square/out_chandigarh_cars.csv' delimiter ',' csv;
CREATE INDEX results_index ON tmp(id, source, target);

UPDATE cleaned_ways
SET    betweenness = tmp.betweenness, level = tmp.level, promoted_level = tmp.level
FROM   tmp
WHERE  cleaned_ways.id = tmp.id
AND cleaned_ways.source = tmp.source AND cleaned_ways.target = tmp.target;
/*
UPDATE cleaned_ways_vertices_pgr
FROM tmp
WHERE cleaned_ways_vertices_pgr.id = tmp.source;
*/
DROP TABLE tmp; -- else it is dropped at end of session automatically
