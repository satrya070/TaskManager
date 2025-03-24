CREATE TABLE IF NOT EXISTS tasks (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	name varchar(255) NOT NULL,
	deadline DATE,
	done BOOLEAN DEFAULT false
);

-- some example tasks
/*INSERT INTO tasks (name, deadline)
VALUES
	('Create a database A', '2025-01-02'),
	('Write script B', '2025-01-03'),
	('Create tests C', '2025-02-02');
	*/