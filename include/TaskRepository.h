#pragma once
#include "Task.h"
#include "Database.h"
#include <vector>
#include "sqlite3.h"

class TaskRepository {
public:
	TaskRepository(IDatabase& db);

	void addTask(std::string taskName, std::string deadlineDate);
	//void archiveTask(int taskId, std::string taskName, std::string deadlineDate);
	//void deleteTask(int taskId);

	std::vector<Task> fetchTasks();
	void fetchArchivedTasks();

private:
	IDatabase& db;
};