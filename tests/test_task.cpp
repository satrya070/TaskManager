#include <gtest/gtest.h>
#include "Task.h"

TEST(TaskTest, TestTaskName) {
	Task task = Task(1, "task-a", "2025-10-01");

	EXPECT_EQ(task.getTaskId(), 1);
	EXPECT_EQ(task.getTaskName(), "task-a");
	EXPECT_EQ(task.getDeadlineDate(), "2025-10-01");
}
