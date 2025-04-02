#pragma once
#include "Task.h"
#include "Database.h"

class TaskRepository {
public:
	TaskRepository(IDatabase& db);

	void addTask(std::string taskName, std::string deadlineDate);
	void archiveTask(int taskId, std::string taskName, std::string deadlineDate);
	void deleteTask(int taskId);

	void fetchTasks();
	void fetchArchivedTasks();

private:
	IDatabase& db;
};