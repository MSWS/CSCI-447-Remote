#ifndef TYPES_PROCESS
#define TYPES_PROCESS
#include <stdio.h>
#include "common.h"
typedef struct {
  int id;
  int priority;
  int arrivalTime;
  int instructionCount;
  int instructions[MAX_INSTRUCT];
} Process;

Process* parseProcess(FILE* fp);
int addInstructionToProcess(Process* self, int time);
#endif // !TYPES_PROCESS
