#pragma once
#include <string>
#include <iostream>
#include <format>
#include "sqlite3.h"

class Command {
public:
	virtual void execute() = 0;
	virtual ~Command() = default;
};


class AddCommand : public Command {
public:
	AddCommand(sqlite3* db, std::string taskName, std::string deadline);
	void execute() override;

private:
	sqlite3* db;
	std::string taskName;
	std::string deadline;
};


class DeleteCommand : public Command {
public:
    DeleteCommand(sqlite3* db, int taskId);

	void execute() override;
private:
    sqlite3* db;
    int taskId;
};
