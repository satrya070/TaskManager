#pragma once
#include <memory>
#include "Command.h"
#include "TaskRepository.h"

class Manager {
public:
	Manager(TaskRepository& taskRepository) : taskRepository(taskRepository) {}
	void setCommand(std::unique_ptr<Command>&& _command);
	void executeCommand();

	TaskRepository taskRepository;
private:
	std::unique_ptr<Command> command;
};