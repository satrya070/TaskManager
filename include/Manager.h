#pragma once
#include <memory>
#include "Command.h"

class Manager {
public:
	void setCommand(std::unique_ptr<Command>&& _command);
	void executeCommand();

private:
	std::unique_ptr<Command> command;
};