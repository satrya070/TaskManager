#pragma once

class IDatabase {
public:
	// can be extended in specific CRUD operations
	virtual void executeQuery(std::string query) = 0;
};