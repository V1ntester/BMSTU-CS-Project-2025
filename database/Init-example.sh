#!/bin/bash

# Параметры подключения к БД
DB_NAME=
DB_USER=
DB_PASSWORD=
DB_HOST=
DB_PORT="5432"

# SQL-запрос для Users
CREATE_USERS_TABLE_SQL=$(cat <<EOF
CREATE TABLE IF NOT EXISTS public."Users"
(
    id integer NOT NULL DEFAULT nextval('"Users_id_seq"'::regclass),
    login character varying(255) COLLATE pg_catalog."default" NOT NULL,
    email character varying(255) COLLATE pg_catalog."default" NOT NULL,
    password_hash character varying(255) COLLATE pg_catalog."default" NOT NULL,
    verification_code character varying(64) COLLATE pg_catalog."default" NOT NULL,
    updating_code character varying(64) COLLATE pg_catalog."default" NOT NULL,
    verified boolean NOT NULL DEFAULT false,
    updated boolean NOT NULL DEFAULT false,
    created_at timestamp without time zone NOT NULL,
    updated_at timestamp without time zone,
    CONSTRAINT "Users_pkey" PRIMARY KEY (id),
    CONSTRAINT "Users_email_key" UNIQUE (email),
    CONSTRAINT "Users_login_key" UNIQUE (login)
)
TABLESPACE pg_default;
EOF
)

# SQL-запрос для Tasks
CREATE_TASKS_TABLE_SQL=$(cat <<EOF
CREATE TABLE IF NOT EXISTS public."Tasks"
(
    id integer NOT NULL DEFAULT nextval('"Tasks_id_seq"'::regclass),
    title character varying(255) COLLATE pg_catalog."default" NOT NULL,
    description text COLLATE pg_catalog."default",
    priority integer NOT NULL,
    category integer NOT NULL,
    deadline character varying(255) COLLATE pg_catalog."default",
    estimatedminutes integer NOT NULL,
    completed boolean NOT NULL DEFAULT false,
    userid integer NOT NULL,
    created_at timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT "Tasks_pkey" PRIMARY KEY (id),
    CONSTRAINT "Tasks_userid_fkey" FOREIGN KEY (userid)
        REFERENCES public."Users" (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION,
    CONSTRAINT "Tasks_priority_check" CHECK (priority >= 1 AND priority <= 3),
    CONSTRAINT "Tasks_category_check" CHECK (category >= 1 AND category <= 3),
    CONSTRAINT "Tasks_estimatedminutes_check" CHECK (estimatedminutes > 0)
)
TABLESPACE pg_default;
EOF
)

if ! command -v psql &> /dev/null
then
    echo "psql не установлен. Пожалуйста, установите PostgreSQL."
    exit 1
fi

echo "Создание последовательностей (если необходимо)..."
PGPASSWORD="$DB_PASSWORD" psql -h "$DB_HOST" -p "$DB_PORT" -U "$DB_USER" -d "$DB_NAME" -c "CREATE SEQUENCE IF NOT EXISTS \"Users_id_seq\";"
PGPASSWORD="$DB_PASSWORD" psql -h "$DB_HOST" -p "$DB_PORT" -U "$DB_USER" -d "$DB_NAME" -c "CREATE SEQUENCE IF NOT EXISTS \"Tasks_id_seq\";"

echo "Инициализация таблицы Users в базе данных $DB_NAME..."
PGPASSWORD="$DB_PASSWORD" psql -h "$DB_HOST" -p "$DB_PORT" -U "$DB_USER" -d "$DB_NAME" -c "$CREATE_USERS_TABLE_SQL"

if [ $? -ne 0 ]; then
    echo "Ошибка при создании таблицы Users."
    exit 1
fi

echo "Инициализация таблицы Tasks в базе данных $DB_NAME..."
PGPASSWORD="$DB_PASSWORD" psql -h "$DB_HOST" -p "$DB_PORT" -U "$DB_USER" -d "$DB_NAME" -c "$CREATE_TASKS_TABLE_SQL"

if [ $? -ne 0 ]; then
    echo "Ошибка при создании таблицы Tasks."
    exit 1
fi

echo "Таблицы Users и Tasks успешно созданы."
exit 0