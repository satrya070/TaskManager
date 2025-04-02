#include "TaskRepository.h"

TaskRepository::TaskRepository(IDatabase& db) : db(db) {}

void TaskRepository::addTask(std::string taskName, std::string deadlineDate) {
	std::string insertQuery = "";
	db.executeQuery(insertQuery);
}

void TaskRepository::archiveTask(int taskId, std::string taskName, std::string deadlineDate) {
	std::string insertQuery = "";
	db.executeQuery(insertQuery);

	// delete from current tasks table
	TaskRepository::deleteTask(taskId);
}

void TaskRepository::deleteTask(int taskId) {
	std::string deleteQuery = "";
	db.executeQuery(deleteQuery);
}

void TaskRepository::fetchTasks() {

}

void TaskRepository::fetchArchivedTasks() {

}