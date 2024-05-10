#ifndef TYPES_IMPL
#define TYPES_IMPL
#include "queue.h"
#include <stdlib.h>

int addProcessToQueue(Queue *self, Process *process) {
  if (self->maxProcessCount >= self->processArraySize) {
    const int newSize = self->processArraySize * 2;
    debug("Reallocating processes to %d\n", newSize);
    Process **newProcesses =
        (Process **)realloc(self->processes, newSize * sizeof(Process *));
    if (newProcesses == NULL) {
      fprintf(stderr, "Failed to realloc processes\n");
      return self->maxProcessCount;
    }
    self->processArraySize = newSize;
    self->processes = newProcesses;
  }
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
  result->processArraySize = 10;
  result->processes = calloc(result->processArraySize, sizeof(Process *));
  for (int i = 0; i < result->processArraySize; i++) {
    result->processes[i] = NULL;
  }
  return result;
}

void incrementReadyTime(Queue *self, int time) {
  for (int i = 0; i < self->maxProcessCount; i++) {
    if (i == self->currentProcess)
      continue;
    if (self->processes[i] == NULL)
      continue;
    if (self->processes[i]->terminatedTime != 0)
      continue;
    if(!readyForCPU(self->processes[i], time))
      continue; 
    self->processes[i]->readyTime++;
  }
}

int getTotalReadyTime(Queue *self) {
  int total = 0;
  for (int i = 0; i < self->maxProcessCount; i++) {
    if (self->processes[i] == NULL)
      continue;
    if (self->processes[i]->terminatedTime == 0)
      fprintf(stderr, "Process %d is not terminated\n", i);
    total += self->processes[i]->readyTime;
  }
  return total;
}

int getMinReadyTime(Queue *self) {
  int min = -1;
  for (int i = 0; i < self->maxProcessCount; i++) {
    if (self->processes[i] == NULL)
      continue;
    if (self->processes[i]->terminatedTime == 0)
      fprintf(stderr, "Process %d is not terminated\n", i);
    if (min == -1 || self->processes[i]->readyTime < min)
      min = self->processes[i]->readyTime;
  }
  return min;
}

int getMaxReadyTime(Queue *self) {
  int max = 0;
  for (int i = 0; i < self->maxProcessCount; i++) {
    if (self->processes[i] == NULL)
      continue;
    if (self->processes[i]->terminatedTime == 0)
      fprintf(stderr, "Process %d is not terminated\n", i);
    if (self->processes[i]->readyTime > max)
      max = self->processes[i]->readyTime;
  }
  return max;
}

bool isQueueDone(Queue *self) {
  for (int i = 0; i < self->maxProcessCount; i++) {
    if (self->processes[i] == NULL)
      continue;
    if (self->processes[i]->terminatedTime == 0) {
      return false;
    }
  }
  return true;
}

bool switchProcess(Queue *self, int time) {
  int next = getNextProcess(self, time);
  if (next == -1 || next == self->currentProcess)
    return false;
  self->currentProcess = next;
  self->processes[next]->cpuTicks = 0;
  return true;
}
#endif /* ifndef TYPES_IMPL */
