#include <stdlib.h>
#include <string.h>
#include "scheduler.h"
#include "task.h"

#define TASK_POOL_SIZE 256

static TASK *taskPool;
static TASK *runningTask;
static TASK *newTask;
static SCHEDULER *scheduler;

static void removeTask(TASK *task)
{
    if (task->prev)
        task->prev->next = task->next;
    if (task->next)
        task->next->prev = task->prev;
    if (runningTask == task)
        runningTask = task->next;
    if (newTask == task)
        newTask = task->next;
}

static int addTask(TASK *task)
{
    TASK *p;
    for (p = taskPool; p < taskPool + TASK_POOL_SIZE; p++)
    {
        if (p->priority < 0)
        {
            task->next = newTask;
            task->prev = NULL;
            memcpy(p, task, sizeof(TASK));
            newTask = p;
            return 0;
        }
    }
    return 1;
}

static void moveToRunningTask(TASK *task)
{
    removeTask(task);
    task->next = runningTask;
    task->prev = NULL;
    runningTask = task;
}

static void run()
{
    TASK *targetTask, *nextTask, *task = runningTask;
    while (task)
    {
        nextTask = task->next;
        if (task->priority < 0)
        {
            removeTask(task);
        }
        else if (task->isFinished(task))
        {
            task->end(task);
            task->priority = -1;
            if (task->target)
                task->target->currentTask = NULL;
        }
        else if (task->execute(task))
        {
            task->interrupted(task);
            task->priority = -1;
            if (task->target)
                task->target->currentTask = NULL;
        }
        task = nextTask;
    }
    task = newTask;
    while (task)
    {
        nextTask = task->next;
        if (task->priority < 0)
        {
            removeTask(task);
        }
        else if (!task->target)
        {
            if (task->initialize(task))
                task->priority = -1;
            else
                moveToRunningTask(task);
        }
        else if (!(targetTask = task->target->currentTask))
        {
            if (task->initialize(task))
                task->priority = -1;
            else
            {
                task->target->currentTask = task;
                moveToRunningTask(task);
            }
        }
        else if (task->priority >= targetTask->priority)
        {
            if (task->initialize(task))
                task->priority = -1;
            else
            {
                targetTask->interrupted(targetTask);
                targetTask->priority = -1;
                task->target->currentTask = task;
                moveToRunningTask(task);
            }
        }
        else
        {
            task->priority = -1;
        }
        task = nextTask;
    }
}

static void quit()
{
    TASK *task = runningTask;
    while (task)
    {
        task->interrupted(task);
        task = task->next;
    }
    free(taskPool);
    free(scheduler);
    scheduler = NULL;
    taskPool = runningTask = newTask = NULL;
}

SCHEDULER *getScheduler()
{
    TASK *p;
    if (scheduler == NULL)
    {
        taskPool = (TASK *)malloc(sizeof(TASK) * TASK_POOL_SIZE);
        for (p = taskPool; p < taskPool + TASK_POOL_SIZE; p++)
            p->priority = -1;
        scheduler = (SCHEDULER *)malloc(sizeof(SCHEDULER));
        scheduler->addTask = addTask;
        scheduler->run = run;
        scheduler->quit = quit;
    }
    return scheduler;
}