CREATE TABLE IF NOT EXISTS users (
    id SERIAL,
    first_name VARCHAR(256) NOT NULL,
    last_name VARCHAR(256) NOT NULL,
    login VARCHAR(256) NOT NULL,
    password VARCHAR(256) NOT NULL,
    email VARCHAR(256) NULL
);

INSERT INTO users (first_name, last_name, login, password, email)
VALUES (
    'admin',
    'admin',
    'admin',
    '9Zo4pltuu+Lm+C96od64PQ==',
    'admin@example.com'
);


DO $$
DECLARE
    i INTEGER;
    first_names TEXT[] := ARRAY['Александр', 'Максим', 'Иван', 'Анна', 'Мария', 'Екатерина', 'Дмитрий', 'Сергей', 'Андрей', 'Ольга'];
    last_names TEXT[] := ARRAY['Иванов', 'Петров', 'Сидоров', 'Кузнецов', 'Попов', 'Смирнов', 'Миллер', 'Волков', 'Федоров', 'Воробьев'];

BEGIN
    FOR i IN 1..100 LOOP
        INSERT INTO users (first_name, last_name, login, password, email)
        VALUES (
            first_names[(floor(random() * array_length(first_names, 1)) + 1)::int],
            last_names[(floor(random() * array_length(last_names, 1)) + 1)::int],
            'user_' || i,
            '1O1PVBu1zE3Yrp6dq8iXVA==',
            'user' || i || '@example.com'
        );
    END LOOP;
END $$;