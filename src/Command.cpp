#include "Command.h"
#include <iostream>


void showCommand::execute() {
	std::cout << "show out" << std::endl;
}

void addCommand::execute() {
    std::string insertQuery = "INSERT INTO tasks (name, deadline) VALUES ('" + taskName + "','" + deadline + "');";

    char* errorMessage;
    int resultCode = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, &errorMessage);
    if (resultCode != SQLITE_OK) {
        std::cout << resultCode << std::endl;
        std::cout << *errorMessage << std::endl;
        return;
    }

    std::cout << "Task: was added" << std::endl;
    return;
}

addCommand::addCommand(sqlite3* db, std::string taskName, std::string deadline) : 
	db(db), taskName(taskName), deadline(deadline) {}