#include <gtest/gtest.h>
#include "Task.h"

TEST(TaskTest, TestTaskName) {
	Task task = Task("task-a");
	EXPECT_EQ(task.getTaskName(), "task-a");
}
