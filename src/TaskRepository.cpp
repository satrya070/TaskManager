#include "TaskRepository.h"
#include <functional>
#include <format>
#include <chrono>

TaskRepository::TaskRepository(IDatabase& db) : db(db) {}

void TaskRepository::addTask(std::string taskName, std::string deadlineDate) {
	// generate the date of creation
	auto now = std::chrono::system_clock::now();
	std::chrono::year_month_day date = std::chrono::floor<std::chrono::days>(now);
	std::string createDate = std::format("{:%F}", date);

	std::string insertQuery = std::format(
		"INSERT INTO tasks(name, created, deadline) VALUES ('{}', '{}', '{}')",
		taskName, createDate, deadlineDate);
	db.insertQuery(insertQuery);
}

void TaskRepository::archiveTask(int taskId, std::string taskName, std::string deadlineDate) {
	std::string insertQuery = std::format(
		"INSERT INTO tasks_archive(id, name, finish_date) VALUES ({}, '{}', '{}')",
		taskId, taskName, deadlineDate
	);
	db.insertQuery(insertQuery);

	// delete from current tasks table
	std::string deleteQuery = std::format("DELETE FROM tasks WHERE id = {}", taskId);
	db.deleteQuery(deleteQuery);
}

void TaskRepository::deleteTask(int taskId) {
	std::string query = std::format("DELETE FROM tasks_archive WHERE id = {};", taskId);
	db.deleteQuery(query);
}

std::vector<Task> TaskRepository::fetchTasks() {
	std::vector<Task> tasks;
	std::string selectQuery = "SELECT id, name, created, deadline FROM tasks;";

	db.selectQuery(selectQuery, [&](sqlite3_stmt* stmt) {
		int id = sqlite3_column_int(stmt, 0);
		std::string name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
		std::string created(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
		std::string deadline(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));

		tasks.emplace_back(id, name, created, deadline);
	});

    return tasks;
}

std::vector<Task> TaskRepository::fetchArchivedTasks() {
	std::vector<Task> tasks;
	std::string selectQuery = "SELECT id, name, finish_date FROM tasks_archive;";

	db.selectQuery(selectQuery, [&](sqlite3_stmt* stmt) {
		int id = sqlite3_column_int(stmt, 0);
		std::string name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
		std::string finish_date(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));

		// technically different from `Task` but keeping it simple for now.
		tasks.emplace_back(id, name, finish_date);
		});

	return tasks;
}