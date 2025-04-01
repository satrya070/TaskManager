#include "Command.h"

ArchiveCommand::ArchiveCommand(sqlite3* db, unsigned int taskId, std::string taskName, std::string finishDate) :
    db(db), taskId(taskId), taskName(taskName), finishDate(finishDate) {}

void ArchiveCommand::execute() {
    std::string insertQuery = std::format("INSERT INTO tasks_archive (id, name, finish_date) VALUES ({}, '{}', '{}');",
        taskId, taskName, finishDate);

    char* errorMessage;
    int resultCode = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, &errorMessage);
    if (resultCode != SQLITE_OK) {
        std::cout << "Data insert error(" << resultCode << "): " << *errorMessage << std::endl;
        return;
    }
}