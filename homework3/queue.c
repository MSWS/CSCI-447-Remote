#ifndef TYPES_IMPL
#define TYPES_IMPL
#include "queue.h"

int addProcessToQueue(Queue *self, Process *process) {
  self->processes[self->processCount++] = process;
  return self->processCount;
}

int addInstructionToProcess(Process *self, int time) {
  self->instructions[self->instructionCount++] = time;
  return self->instructionCount;
}
#endif /* ifndef TYPES_IMPL */
