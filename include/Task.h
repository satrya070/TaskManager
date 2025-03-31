#pragma once
#include <string>

class Task {
public:
	Task(unsigned int _task_id, std::string _task_name) : task_id(_task_id), task_name(_task_name) {}

	const unsigned int& getTaskId() const {
		return task_id;
	}

	const std::string& getTaskName() const {
		return task_name;
	}
private:
	unsigned int task_id;
	std::string task_name;
};