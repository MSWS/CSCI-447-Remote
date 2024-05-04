#ifndef TYPES_QUEUE
#define TYPES_QUEUE
#include "common.h"
#include "process.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  int quantum;                     // Quantum for this queue
  Process *processes[MAX_PROCESS]; // Processes in this queue
  int currentProcess;              // Index of the current process
  int maxProcessCount;             // Number of processes in the queue
  bool prioritize;                 // Whether to prioritize processes
  bool preempt;                    // Whether to preempt processes
} Queue;

int addProcessToQueue(Queue *self, Process *process);

/**
 * Gets the next process that the queue should execute,
 * assuming its current process has finished.
 *
 * Returns -1 if no process is available. (Either
 * because none are ready, or because they haven't
 * arrived yet.)
 */
int getNextProcess(Queue *self, int time);
Queue *initQueue();
#endif // !TYPES_QUEUE
