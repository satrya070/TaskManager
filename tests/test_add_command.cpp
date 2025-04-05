#include <gtest/gtest.h>
#include "Command.h"
#include "sqlite3.h"


TEST(AddCommandTest, testAddQuery) {
	sqlite3* db;
	sqlite3_open(":memory:", &db);

	// create table
	std::string task_table_create_query =
		"CREATE TABLE IF NOT EXISTS tasks("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"name varchar(255) NOT NULL,"
		"deadline DATE,"
		"done BOOLEAN DEFAULT false"
		");"
	;
	sqlite3_exec(db, task_table_create_query.c_str(), nullptr, nullptr, nullptr);

	// 
	AddCommand addCommand = AddCommand(db, "task-b", "2020-04-01");
	//addCommand.execute();


}
