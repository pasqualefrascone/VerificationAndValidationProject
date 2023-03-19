SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;
CREATE DATABASE sistema WITH TEMPLATE = template0 ENCODING = 'UTF8';

CREATE USER server with password 'password';
CREATE USER client with password 'password';
ALTER DATABASE sistema OWNER TO postgres;

\connect sistema

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

CREATE SCHEMA agent;
ALTER SCHEMA agent OWNER TO postgres;

SET default_tablespace = '';
SET default_table_access_method = heap;

CREATE TABLE agent.logs (
    who character varying(30) COLLATE pg_catalog."default" NOT NULL,
    whenn timestamp(6) with time zone NOT NULL,
    log character varying(128) COLLATE pg_catalog."default" NOT NULL,
    pc smallint NOT NULL,
    CONSTRAINT logs_pkey PRIMARY KEY (who, whenn)
);
ALTER TABLE agent.logs OWNER TO postgres;

GRANT ALL ON DATABASE sistema TO server;
GRANT ALL ON DATABASE sistema TO client;
GRANT ALL ON SCHEMA agent TO server;
GRANT ALL ON SCHEMA agent TO client;
GRANT ALL ON  agent.logs TO server;
GRANT ALL ON  agent.logs TO client;

