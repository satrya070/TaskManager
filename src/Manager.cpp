#include "Manager.h"


void Manager::setCommand(std::unique_ptr<Command>&& _command) {
	command = std::move(_command);
}

void Manager::executeCommand() {
	if (command)
		command->execute();
}
