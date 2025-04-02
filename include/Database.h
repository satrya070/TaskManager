#pragma once
#include <string>
#include <functional>
#include "sqlite3.h"

class IDatabase {
public:
	// can be extended in specific CRUD operations
	virtual void selectQuery(const std::string& query, std::function<void(sqlite3_stmt*)> callback) = 0;
};