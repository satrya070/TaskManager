#pragma once
#include "Database.h"
#include "sqlite3.h"

class SqliteDatabase : public IDatabase {
public:
    SqliteDatabase(const std::string& databaseFile);
    ~SqliteDatabase() = default;

    void selectQuery(const std::string& query, std::function<void(sqlite3_stmt*)> callback) override;
    // deleteQuery
private:
    sqlite3* db;
    int createConn(const std::string& databaseFile);
    bool initTables(sqlite3* db, const std::string& filePath);
};