#pragma once
#include <string>

class IDatabase {
public:
	// can be extended in specific CRUD operations
	virtual void executeQuery(const std::string& query) = 0;
};