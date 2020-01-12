#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"

typedef struct scheduler
{
    int (*addTask)(TASK *task);
    void (*run)();
    void (*quit)();
} SCHEDULER;

SCHEDULER *getScheduler();

#endif