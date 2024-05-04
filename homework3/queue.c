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

  int highestPriority = 0;
  int highestIndex = -1;
  // Start at +1 to skip the current process
  for (int i = 1; i < self->maxProcessCount + 1; i++) {
    int index = (i + current) % self->maxProcessCount;
    Process *p = self->processes[index];
    if (!readyForCPU(p, time))
      continue;

    if (!self->prioritize)
      return index; // Return the first process that is ready

    if (p->priority < highestPriority)
      continue;
    highestPriority = p->priority;
    highestIndex = index;
  }
  return highestIndex;
}

Queue *initQueue() {
  Queue *result = calloc(1, sizeof(Queue));
  result->quantum = 0;
  result->burstTicks = 0;
  result->currentProcess = 0;
  result->maxProcessCount = 0;
  result->prioritize = false;
  result->preempt = false;
  return result;
}

bool isQueueDone(Queue *self) {
  for (int i = 0; i < self->maxProcessCount; i++) {
    if (self->processes[i] == NULL)
      continue;
    if (!self->processes[i]->terminated) {
      return false;
    }
  }
  return true;
}

bool switchProcess(Queue *self, int time) {
  int next = getNextProcess(self, time);
  if (next == -1)
    return false;
  self->currentProcess = next;
  return true;
}
#endif /* ifndef TYPES_IMPL */
