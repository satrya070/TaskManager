#pragma once
#include <string>

class Task {
public:
	Task(unsigned int _task_id, std::string _task_name, std::string _create_date, std::string _deadline) :
		task_id(_task_id), task_name(_task_name), create_date(_create_date), deadline_date(_deadline) {}
	
	Task(unsigned int _task_id, std::string _task_name, std::string _deadline) : task_id(_task_id), task_name(_task_name), deadline_date(_deadline) {}

	const unsigned int& getTaskId() const {
		return task_id;
	}

	const std::string& getTaskName() const {
		return task_name;
	}

	const std::string& getCreateDate() const {
		return create_date;
	}

	const std::string& getDeadlineDate() const {
		return deadline_date;
	}
private:
	unsigned int task_id;
	std::string task_name;
	std::string create_date;
	std::string deadline_date;
};