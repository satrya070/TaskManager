#include <iostream>
#include "sqlite3.h"

void insert(sqlite3* db, const std::string sql) {
    int result = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);

    if (result != SQLITE_OK) {
        // TODO properly out error
        std::cout << "query failed" << std::endl;
    }
}

void show_tasks(sqlite3* db) {
    std::string sql = "SELECT * FROM tasks;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    int row_count = sqlite3_column_count(stmt);

    // loop through rows
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        for (int i = 0; i < row_count; i++)
        {
            int id = sqlite3_column_int(stmt, 0);
            const char* columnName = sqlite3_column_name(stmt, i);

            std::string name;
            int done;
            switch (sqlite3_column_type(stmt, i)) {
                case SQLITE_TEXT:
                    name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                    break;
                case SQLITE_INTEGER:
                    done = sqlite3_column_int(stmt, i);
                    break;
            }
            std::cout << name << " | " << done;

            std::cout << " | ";
        }
        std::cout << std::endl;
    }

    sqlite3_finalize(stmt);
}

int main() {
    sqlite3* db;
    if (sqlite3_open_v2("MyDBasdf", &db, SQLITE_OPEN_READWRITE, nullptr) != SQLITE_OK) {
        std::cout << "Failed to open DB: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    

    //std::string sql = "";
    //if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK) {
    //    std::cout << "Task added succesfully!" << std::endl;
    //}

    int choice;
    //std::cin >> choice;
    choice = 0;

    switch (choice) {
        case 0:
            std::cout << "Tasks: " << std::endl;
            show_tasks(db);
            break;
        case 1:
            std::cout << "add task"; break;
        case 2:
            std::cout << "delete task"; break;
    } 

    return 0;
}