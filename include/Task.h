#pragma once
#include <string>

class Task {
public:
	Task(std::string _task_name);
private:
	std::string task_name;
};