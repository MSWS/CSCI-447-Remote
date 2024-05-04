#ifndef TYPES_QUEUE
#define TYPES_QUEUE
#include "common.h"
#include "process.h"
#include <stdbool.h>
#include <stdio.h>
typedef struct {
  int quantum;
  Process *processes[MAX_PROCESS];
  int currentProcess;
  int maxProcessCount;
  int minTimeCompletion;
  bool prioritize;
  bool preempt;
} Queue;

int addProcessToQueue(Queue *self, Process *process);

/**
 * Gets the next process that the queue should execute,
 * assuming its current process has finished.
 *
 * May return NULL if there are no processes remaining,
 * OR if the next process (and thus all remaining processes)
 * have not yet arrived at the given time.
 */
int getNextProcess(Queue *self, int time);
int getMinQCompletion(Queue* self);
Queue *initializeQueue();
#endif // !TYPES_QUEUE
