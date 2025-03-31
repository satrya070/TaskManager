#include "Command.h"

AddCommand::AddCommand(sqlite3* db, std::string taskName, std::string deadline) :
    db(db), taskName(taskName), deadline(deadline) {}

void AddCommand::execute() {
    std::string insertQuery = "INSERT INTO tasks (name, deadline) VALUES ('" + taskName + "','" + deadline + "');";
    // TODO handle except properly
    char* errorMessage;
    int resultCode = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, &errorMessage);
    if (resultCode != SQLITE_OK) {
        std::cout << resultCode << std::endl;
        std::cout << *errorMessage << std::endl;
        return;
    }
}
