#ifndef TYPES_IMPL
#define TYPES_IMPL
#include "queue.h"
#include <stdlib.h>

int addProcessToQueue(Queue *self, Process *process) {
  self->processes[self->maxProcessCount++] = process;
  return self->maxProcessCount;
}

int getNextProcess(Queue *self, int time) {
  int current = self->currentProcess;

  if (self->prioritize) {
    int highestPriority = 0;
    int highestIndex = -1;
    for (int i = 1; i < self->maxProcessCount; i++) {
      int index = (i + current) % self->maxProcessCount;
      Process *p = self->processes[index];
      if (p == NULL) // Promoted or otherwise no longer in queue
        continue;
      if (p->terminated)
        continue;
      if (p->arrivalTime > time)
        continue;
      if (p->priority < highestPriority)
        continue;
      highestPriority = p->priority;
      highestIndex = index;
    }
    return highestIndex;
  }

  for (int i = 1; i < self->maxProcessCount; i++) {
    int index = (i + current) % self->maxProcessCount;
    Process *p = self->processes[index];
    if (p == NULL)
      continue;
    if (p->terminated)
      continue;
    if (p->arrivalTime > time)
      continue;
    return index;
  }
  return -1;
}

Queue *initQueue() {
  Queue *result = calloc(1, sizeof(Queue));
  result->quantum = 0;
  result->currentProcess = 0;
  result->maxProcessCount = 0;
  result->prioritize = false;
  result->preempt = false;
  return result;
}
#endif /* ifndef TYPES_IMPL */
