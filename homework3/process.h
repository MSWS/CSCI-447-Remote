#ifndef TYPES_PROCESS
#define TYPES_PROCESS
#include "common.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct {
  int id;
  int priority;
  int arrivalTime;
  int instructionCount;
  int instructions[MAX_INSTRUCT];
  int currentInstruction;
  int ioTicks;
  int cpuTicks;
  int fastTicks;
  bool terminated;
} Process;

Process *parseProcess(FILE *fp);
int addInstructionToProcess(Process *self, int time);
int getMinTCompletion(Process *self);
bool tickProcess(Process *self);

void debugProcess(Process *self);
#endif // !TYPES_PROCESS
