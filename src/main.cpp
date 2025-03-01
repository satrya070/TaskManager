#include <iostream>
#include "sqlite3.h"

int main() {
    sqlite3* db;
    sqlite3_open("MyDB", &db);

    std::string sql = "";
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK) {
        std::cout << "Task added succesfully!" << std::endl;
    }

    int choice;
    //std::cin >> choice;
    choice = 1;

    switch (choice) {
        case 0:
            std::cout << "show tasks"; break;
        case 1:
            std::cout << "add task"; break;
        case 2:
            std::cout << "delete task"; break;
    } 

    return 0;
}