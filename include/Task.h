#pragma once
#include <string>

class Task {
public:
	Task(std::string _task_name);

	// TODO check
	const std::string& getTaskName() const {
		return task_name;
	}
private:
	std::string task_name;
};