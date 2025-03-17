#include "Command.h"
#include <iostream>
#include <format>


class addCommand : public Command {
public:
    addCommand(sqlite3* db, std::string taskName, std::string deadline) :
        db(db), taskName(taskName), deadline(deadline) {}
    
    void execute() override {
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

private:
    sqlite3* db;
    std::string taskName;
    std::string deadline;
};


class deleteCommand : public Command {
public:
    deleteCommand(sqlite3* db, int taskId) : db(db), taskId(taskId) {}
        
    void execute() override {
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
private:
    sqlite3* db;
    int taskId;
};