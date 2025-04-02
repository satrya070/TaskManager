#include "SqliteDatabase.h"
#include "sqlite3.h"
#include <iostream>
#include <fstream>
#include <sstream>

SqliteDatabase::SqliteDatabase(const std::string& databaseFile) {

    if (SqliteDatabase::createConn(databaseFile) != 0) {
        std::cout << "Error opening database" << std::endl;
    }

    if (!SqliteDatabase::initTables(db, "setup.sql")) {
        std::cout << "Failed to initialise tables" << std::endl;
    }
}

int SqliteDatabase::createConn(const std::string& databaseFile) {
    int returnCode = sqlite3_open_v2(
        databaseFile.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr
    );
    if (returnCode != SQLITE_OK) {
        std::cout << returnCode << ": Failed to open DB: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    return 0;
}

//template <typename Callback>
void SqliteDatabase::executeQuery(const std::string& query, std::function<void(sqlite3_stmt*)> callback) {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        callback(stmt);
    }

    sqlite3_finalize(stmt);

    /*std::vector<Task> tasks;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        unsigned int task_id;
        std::string task_name;
        std::string deadline;

        const unsigned char* text;
        // loop through the columns of this row
        for (int i = 0; i < 3; i++) {
            int columnType = sqlite3_column_type(stmt, i);

            switch (columnType) {
            case SQLITE_TEXT:
                text = sqlite3_column_text(stmt, i);
                if (strcmp(sqlite3_column_name(stmt, i), "name") == 0) {
                    task_name = text ? reinterpret_cast<const char*>(text) : "";
                    //std::cout << task_name << std::endl;
                }
                else {
                    deadline = text ? reinterpret_cast<const char*>(text) : "";
                }
                break;
            case SQLITE_INTEGER:
                task_id = sqlite3_column_int(stmt, i);
                break;
            }
        }
        tasks.emplace_back(task_id, task_name);
    }
    sqlite3_finalize(stmt);*/
}

bool SqliteDatabase::initTables(sqlite3* db, const std::string& filePath) {
    // creates the database tables if they don't exist yet
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Error: file doesn't exist." << std::endl;
        return false;
    }

    // read content into string buffer
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sql = buffer.str();

    // execute sql
    char* errorMessage = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Failed to create tables with: " << filePath << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }

    return true;
}
