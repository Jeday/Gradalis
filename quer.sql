DROP SCHEMA IF EXISTS grad CASCADE;
CREATE SCHEMA grad;

DROP TABLE IF EXISTS grad.clients CASCADE;
CREATE TABLE grad.clients(
    ID					BIGSERIAL  PRIMARY KEY		NOT NULL,
    NAME		TEXT								NOT NULL,
    DESCRIPTION		TEXT,
    LAWDESCRIPTION	TEXT								
);

INSERT INTO grad.clients (NAME) VALUES('NONE');

DROP TABLE IF EXISTS grad.companies CASCADE;
CREATE TABLE grad.companies(
    ID						BIGSERIAL  PRIMARY KEY		NOT NULL,
    NAME		TEXT								NOT NULL,
    DESCRIPTION		TEXT								,
    LAWDESCRIPTION		TEXT								
);

INSERT INTO grad.companies (NAME) VALUES('NONE');

DROP TABLE  IF EXISTS grad.providers CASCADE;
CREATE TABLE grad.providers(
    ID						BIGSERIAL  PRIMARY KEY		NOT NULL,
    NAME		TEXT								NOT NULL,
    DESCRIPTION		TEXT								,
    LAWDESCRIPTION		TEXT								
);

INSERT INTO grad.providers (NAME) VALUES('NONE');



DROP TABLE  IF EXISTS grad.manufacturers CASCADE;
CREATE TABLE grad.manufacturers(
    ID						BIGSERIAL  PRIMARY KEY		NOT NULL,
    NAME		TEXT								NOT NULL,
    DESCRIPTION		TEXT								,
    LAWDESCRIPTION		TEXT								
);

INSERT INTO grad.manufacturers (NAME) VALUES('NONE');



DROP TABLE  IF EXISTS grad.items CASCADE;
CREATE TABLE grad.items(
	ID						BIGSERIAL  PRIMARY KEY		NOT NULL,
    NAME		TEXT								NOT NULL,
    PROJECT		TEXT,
    MANUFACTURER	INT references grad.manufacturers(ID)
);
INSERT INTO grad.items (NAME,MANUFACTURER) VALUES('NONE',1);

DROP TABLE IF EXISTS 	grad.projects CASCADE;
CREATE TABLE grad.projects(
          ID		BIGSERIAL  PRIMARY KEY		NOT NULL,
	NAME		TEXT	NOT NULL,
	DESCRIPTION		TEXT
);

INSERT INTO grad.projects (NAME) VALUES('NONE');

DROP TABLE IF EXISTS  grad.checks_out CASCADE;
CREATE TABLE grad.checks_out(
	ID						BIGSERIAL  PRIMARY KEY		NOT NULL,
    NAME		VARCHAR(60)					NOT NULL,
    DESCRIPTION		TEXT								,
    REC_DATE	DATE						NOT NULL,
    PAY_DATE	DATE						NOT NULL,
    PAYED_DATE	DATE						NOT	NULL,
    RECEIEVER	INT 	references grad.clients(ID),
    SENDER		INT		references grad.companies(ID),
    PRJ			INT   references grad.projects(ID),
    ITEM_TABLE		VARCHAR(60),
    CURR		REAL
);

INSERT INTO grad.checks_out (NAME,RECEIEVER,SENDER,PRJ) VALUES('NONE',1,1,1);

DROP TABLE  IF EXISTS grad.checks_in CASCADE;
CREATE TABLE grad.checks_in(
	ID						BIGSERIAL  PRIMARY KEY		NOT NULL,
    NAME		VARCHAR(60)					NOT NULL,
    DESCRIPTION		TEXT								,
    REC_DATE	DATE						NOT NULL,
    PAY_DATE	DATE						NOT NULL,
    PAYED_DATE	DATE						NOT	NULL,
    RECEIEVER	INT	 		references grad.companies(ID),
    SENDER		INT			 references grad.providers(ID),
    PRJ			INT	  references grad.projects(ID),
    ITEM_TABLE		VARCHAR(60),
        CURR		REAL
);


INSERT INTO grad.checks_in (NAME,RECEIEVER,SENDER,PRJ) VALUES('NONE',1,1,1);


--CREATE TABLE films2 AS
--  TABLE films;

DROP TABLE  IF EXISTS grad.item_list_temp;
CREATE TABLE  grad.item_list_temp(
		ID						BIGSERIAL  PRIMARY KEY		NOT NULL,
		ITEM_ID  INT references grad.items(ID)	NOT NULL,
		QUNT	 INT NOT NULL,
		PRICE_PER_ITEM	money NOT NULL
);




