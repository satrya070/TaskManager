#include <iostream>
#include "sqlite3.h"
#include <vector>
#include <any>
#include <memory>
#include <format>
#include "Manager.h"
#include "Command.h"
#include "Command.cpp"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include "SDL3/SDL.h"


static void showTasksPreview(sqlite3* db) {
    std::string selectQuery = "SELECT name, deadline, done FROM tasks LIMIT 5;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        for (int i = 0; i < 3; i++) {
            int columnType = sqlite3_column_type(stmt, i);

            switch (columnType) {
            case SQLITE_TEXT:
                std::cout << sqlite3_column_text(stmt, i) << " | ";
                break;
            case SQLITE_INTEGER:
                std::cout << sqlite3_column_int(stmt, i) << " | ";
                break;
            }
        }
        std::cout << std::endl;
    }
    sqlite3_finalize(stmt);
}


int main() {
    sqlite3* db;
    int returnCode = sqlite3_open_v2("../../../../database/tasks_database.db", &db, SQLITE_OPEN_READWRITE, nullptr);
    if (returnCode != SQLITE_OK) {
        std::cout << returnCode << std::endl;
        std::cout << "Failed to open DB: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // always the show the first 5 tasks
    showTasksPreview(db);

    int input;
    //std::cin >> choice;
    input = 0;

    Manager taskManager;

    // TODO ---- refactor to gui

    // set and execute command
    //taskManager.setCommand(std::make_unique<addCommand>(db, "buy eggs", "2025-06-14"));
    //taskManager.executeCommand();

    // delete command
    //taskManager.setCommand(std::make_unique<deleteCommand>(db, 10));
    //taskManager.executeCommand();

    // TODO
    //update command
    

    std::cout << "done";

    return 0;
}