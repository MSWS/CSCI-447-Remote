#ifndef TYPES_QUEUE
#define TYPES_QUEUE
#include <stdio.h>
#include "common.h"
#include "process.h"

typedef struct {
  int quantum;
  Process* processes[MAX_PROCESS];
  int currentProcess;
  int processCount;
} Queue;

int addProcessToQueue(Queue* self, Process* process);
#endif // !TYPES_QUEUE
