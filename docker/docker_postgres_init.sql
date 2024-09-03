CREATE SEQUENCE private_key_sq
MINVALUE 1042
START WITH 10002
INCREMENT BY 1
CACHE 5;

CREATE SEQUENCE public_key_sq
MINVALUE 2000
START WITH 2000
INCREMENT BY 1
CACHE 5;

CREATE SEQUENCE mytable_id_seq
MINVALUE 1
START WITH 1
INCREMENT BY 1
CACHE 1;

CREATE TABLE users (
    id BIGINT NOT NULL DEFAULT nextval('mytable_id_seq'),
    login BIGINT NOT NULL UNIQUE,
    registered_at timestamp without time zone DEFAULT now(),
    amount_pastes SMALLINT DEFAULT 0,
    PRIMARY KEY(id)
);

CREATE TABLE pastes (
    public_key VARCHAR(16) NOT NULL UNIQUE,
    private_key VARCHAR(16) NOT NULL UNIQUE,
    created_at timestamp without time zone DEFAULT now(),
    login  BIGINT NOT NULL,
    password VARCHAR(128) DEFAULT 0,
    title VARCHAR(128) DEFAULT 'Untilted',
    FOREIGN KEY(login) REFERENCES users(login)
);