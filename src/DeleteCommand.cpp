#include "Command.h"


DeleteCommand::DeleteCommand(sqlite3* db, int taskId) : db(db), taskId(taskId) {}

void DeleteCommand::execute() {
    std::string deleteQuery = std::format("DELETE FROM tasks WHERE id = {};", taskId);
    char* errorMessage;
    int resultCode = sqlite3_exec(db, deleteQuery.c_str(), nullptr, nullptr, &errorMessage);
    if (resultCode != SQLITE_OK) {
        // TODO handle except properly
        std::cout << resultCode << ": " << errorMessage << std::endl;
        return;
    }
    std::cout << "Task " << taskId << " was deleted" << std::endl;
    return;
}
