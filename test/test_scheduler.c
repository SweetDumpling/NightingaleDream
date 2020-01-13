#include <stdio.h>
#include "scheduler.h"
#include "task.h"
#include "unity.h"

SCHEDULER *scheduler;
TASK task1, task2;
SUBJECT subject;
int sum, sumIsThree, i;

void setUp(void)
{
    i = sum = sumIsThree = 0;
    subject.currentTask = subject.defaultTask = NULL;
    scheduler = getScheduler();
    initTask(&task1);
    initTask(&task2);
}

void tearDown(void)
{
    scheduler->quit();
}

int plusOne(TASK *task)
{
    sum++;
    return 0;
}

int isFiveTimes(TASK *task)
{
    if (i == 4)
        return 1;
    else
        return 0;
}

void checkSum(TASK *task)
{
    if (sum == 3)
        sumIsThree = 1;
}

int getFalse(TASK *task)
{
    return 0;
}

void test_initialize(void)
{
    task1.initialize = plusOne;
    task1.isFinished = getFalse;
    scheduler->addTask(&task1);

    for (i = 0; i < 10; i++)
        scheduler->run();

    TEST_ASSERT_TRUE(sum == 1);
}

void test_execute_end(void)
{
    task1.execute = plusOne;
    task1.isFinished = isFiveTimes;
    task1.end = checkSum;
    scheduler->addTask(&task1);

    for (i = 0; i < 10; i++)
        scheduler->run();

    TEST_ASSERT_TRUE(sum == 3);
    TEST_ASSERT_TRUE(sumIsThree);
}

void test_interrupt(void)
{
    task1.execute = plusOne;
    task1.isFinished = getFalse;
    task1.interrupted = checkSum;
    task1.target = &subject;

    task2.target = &subject;

    scheduler->addTask(&task1);

    for (i = 0; i < 10; i++)
    {
        if (i == 3)
            scheduler->addTask(&task2);
        //add task2 to interrupt task1
        scheduler->run();
    }
    TEST_ASSERT_TRUE(sum == 3);
    TEST_ASSERT_TRUE(sumIsThree);
}

void test_priority(void)
{
    task1.execute = plusOne;
    task1.isFinished = getFalse;
    task1.interrupted = checkSum;
    task1.target = &subject;
    task1.priority = 2;

    task2.target = &subject;

    scheduler->addTask(&task1);

    for (i = 0; i < 10; i++)
    {
        if (i == 3)
            scheduler->addTask(&task2);
        //add task2 to interrupt task1
        //but it has a lower priority:)
        scheduler->run();
    }
    TEST_ASSERT_TRUE(sum == 9);
    TEST_ASSERT_FALSE(sumIsThree);
}

void test_parallel_task(void)
{
    task1.initialize = plusOne;
    task1.execute = plusOne;
    task1.isFinished = isFiveTimes;

    task2.initialize = plusOne;
    task2.execute = plusOne;
    task2.isFinished = isFiveTimes;

    scheduler->addTask(&task1);
    scheduler->addTask(&task2);

    for (i = 0; i < 10; i++)
        scheduler->run();

    TEST_ASSERT_TRUE(sum == 8);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_initialize);
    RUN_TEST(test_execute_end);
    RUN_TEST(test_interrupt);
    RUN_TEST(test_priority);
    RUN_TEST(test_parallel_task);
    return UNITY_END();
}