#ifndef TYPES_HEADER
#define TYPES_HEADER
#include <stdio.h>
#include "common.h"

typedef struct {
  int id;
  int priority;
  int arrivalTime;
  int ioInstructions[MAX_INSTRUCT];
  int exeInstructions[MAX_INSTRUCT];
} Process;

Process* parseProcess(FILE* fp);

typedef struct {
  int quantum;
  Process* processes[MAX_PROCESS];
  int currentProcess;
  int processCount;
} Queue;

int addToQueue(Queue* self, Process* process);
#endif // !TYPES_HEADER
