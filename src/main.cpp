#include <iostream>
#include "sqlite3.h"
#include <vector>
#include <any>
#include <memory>
#include <format>


static void showTasksPreview(sqlite3* db) {
    // shows first 5 tasks
    /*std::string selectQuery = "SELECT * FROM tasks LIMIT 5;";
    char* errorMessage;
    int resultCode = sqlite3_exec(db, selectQuery.c_str(), nullptr, nullptr, &errorMessage);
    if (resultCode != SQLITE_OK) {
        std::cout << resultCode << ": " << errorMessage << std::endl;
        return;
    }*/

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
                //row_values.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
                break;
            case SQLITE_INTEGER:
                std::cout << sqlite3_column_int(stmt, i) << " | ";
                break;
            }
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<std::any>> fetch_data(sqlite3* db, std::string query) {

    std::vector<std::vector<std::any>> fetched_data;
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return fetched_data;
    }

    int row_count = sqlite3_column_count(stmt);

    // loop through rows
    int row_number = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int column_count = sqlite3_column_count(stmt);
        for (int i = 0; i < column_count; i++)
        {
            std::vector<std::any> row_values;

            //int id = sqlite3_column_int(stmt, 0);
            const char* columnName = sqlite3_column_name(stmt, i);

            std::string name;
            bool done = false;

            int columnType = sqlite3_column_type(stmt, i);
            switch (columnType) {
                case SQLITE_TEXT:
                    row_values.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
                    //std::string columnValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                    break;
                case SQLITE_INTEGER:
                    row_values.push_back(sqlite3_column_int(stmt, i));
                    break;
            }
            std::cout << name << " | " << done;
            fetched_data.push_back(row_values);
            //std::cout << " | ";
        }
        std::cout << std::endl;
        row_number++;
    }
    sqlite3_finalize(stmt);

    return fetched_data;
}

static void addTask(sqlite3 *db, std::string taskName, std::string deadline) {
    std::string insertQuery = "INSERT INTO tasks (name, deadline) VALUES ('" + taskName + "','" + deadline + "');";

    char* errorMessage;
    int resultCode = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, &errorMessage);
    if (resultCode != SQLITE_OK) {
        std::cout << resultCode << std::endl;
        std::cout << *errorMessage << std::endl;
        return;
    }

    std::cout << "Task was added" << std::endl;
    return;
}

static void deleteTask(sqlite3* db, int taskId) {
    std::string deleteQuery = std::format("DELETE FROM tasks WHERE id = {};", taskId);
    char* errorMessage;
    int resultCode = sqlite3_exec(db, deleteQuery.c_str(), nullptr, nullptr, &errorMessage);
    if (resultCode != SQLITE_OK) {
        std::cout << resultCode << ": " << errorMessage << std::endl;
        return;
    }
    
    std::cout << "Task was deleted" << std::endl;
    return;
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

    int choice;
    //std::cin >> choice;
    choice = 2;

    std::string query = "SELECT name, deadline, done FROM tasks;";
    std::vector<std::vector<std::any>> fetched_data = fetch_data(db, query);

    switch (choice) {
        case 0:
            std::cout << "Tasks: " << std::endl;
            break;
        case 1:
        {
            std::cout << "Adding task";
            std::string TaskName = "test0";
            std::string deadline = "2025-10-20";
            addTask(db, TaskName, deadline);
            break;
        }
        case 2:
            std::cout << "delete task";
            deleteTask(db, 4);
            break;
    }

    std::cout << "done";

    return 0;
}