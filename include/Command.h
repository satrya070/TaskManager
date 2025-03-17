# pragma once
#include "sqlite3.h"
#include <string>

class Command {
public:
	virtual void execute() = 0;
	virtual ~Command() = default;
};

class showCommand : public Command {
public:
	void execute() override;
};

class addCommand : public Command {
public:
	addCommand(sqlite3* db, std::string taskName, std::string deadline);
	void execute() override;

private:
	sqlite3* db;
	std::string taskName;
	std::string deadline;
};