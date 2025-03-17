# pragma once
#include "sqlite3.h"
#include <string>

class Command {
public:
	virtual void execute() = 0;
	virtual ~Command() = default;
};
