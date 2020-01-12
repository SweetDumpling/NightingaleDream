#ifndef TASK_H
#define TASK_H

typedef struct subject SUBJECT;
typedef struct task TASK;

struct subject
{
    TASK *currentTask;
    TASK *defaultTask;
};

struct task
{
    void *data;
    int dataSize;
    SUBJECT *target;
    int priority;
    int (*initialize)(TASK *task);
    int (*execute)(TASK *task);
    int (*isFinished)(TASK *task);
    void (*end)(TASK *task);
    void (*interrupted)(TASK *task);
    TASK *prev;
    TASK *next;
};

void initTask(TASK *task);

#endif