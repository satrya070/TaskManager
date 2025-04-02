#include "Database.h"
#include "sqlite3.h"

static bool initTables(sqlite3* db, const std::string& filePath) {
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


class SqliteDatabase : public IDatabase {
public:
	SqliteDatabase(std::string databaseFile) {
        int returnCode = sqlite3_open_v2(
            "tasks_database.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr
        );
        if (returnCode != SQLITE_OK) {
            std::cout << returnCode << ": Failed to open DB: " << sqlite3_errmsg(db) << std::endl;
            return 1;
        }

        initTables(db, "setup.sql");
	}

	void executeQuery(std::string query) override {

	}

private:
	sqlite3* db;
};