#include <stdlib.h>
#include "task.h"

static int initialize(TASK *task)
{
    return 0;
}

static int execute(TASK *task)
{
    return 0;
}

static int isFinished(TASK *task)
{
    return 1;
}

static void end(TASK *task) {}

static void interrupted(TASK *task) {}

void initTask(TASK *task)
{
    task->prev = task->next = task->data = NULL;
    task->dataSize = task->priority = 0;
    task->target = NULL;
    task->initialize = initialize;
    task->execute = execute;
    task->isFinished = isFinished;
    task->end = end;
    task->interrupted = interrupted;
}
