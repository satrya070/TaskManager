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


class ArchiveCommand : public Command {
public:
	ArchiveCommand(sqlite3* db, unsigned int taskId, std::string taskName, std::string finishDate);
	void execute() override;
private:
	sqlite3* db;
	int taskId;
	std::string taskName;
	std::string finishDate;
};


class DeleteCommand : public Command {
public:
    DeleteCommand(sqlite3* db, int taskId);
	void execute() override;
private:
    sqlite3* db;
    int taskId;
};
