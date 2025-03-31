#include <gtest/gtest.h>
#include "Task.h"

TEST(TaskTest, TestTaskName) {
	Task task = Task(1, "task-a");

	EXPECT_EQ(task.getTaskId(), 1);
	EXPECT_EQ(task.getTaskName(), "task-a");
}
