#include "TaskRepository.h"
#include <functional>
#include <format>

TaskRepository::TaskRepository(IDatabase& db) : db(db) {}

void TaskRepository::addTask(std::string taskName, std::string deadlineDate) {
	std::string insertQuery = std::format("INSERT INTO tasks(name, deadline) VALUES ('{}', '{}')", taskName, deadlineDate);
	db.insertQuery(insertQuery);
}

/*void TaskRepository::archiveTask(int taskId, std::string taskName, std::string deadlineDate) {
	std::string insertQuery = "";
	db.executeQuery(insertQuery);

	// delete from current tasks table
	TaskRepository::deleteTask(taskId);
}

void TaskRepository::deleteTask(int taskId) {
	std::string deleteQuery = "";
	db.executeQuery(deleteQuery);
}*/

std::vector<Task> TaskRepository::fetchTasks() {
	std::vector<Task> tasks;
	std::string selectQuery = "SELECT id, name, deadline FROM tasks;";

	db.selectQuery(selectQuery, [&](sqlite3_stmt* stmt) {
		int id = sqlite3_column_int(stmt, 0);
		std::string name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
		std::string deadline(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));

		tasks.emplace_back(id, name);
	});

    return tasks;
}

void TaskRepository::fetchArchivedTasks() {

}