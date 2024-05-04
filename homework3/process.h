#ifndef TYPES_PROCESS
#define TYPES_PROCESS
#include "common.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  int id;                         // Process ID
  int priority;                   // Process Priority
  int arrivalTime;                // Process Arrival Time
  int instructionCount;           // Number of instructions
  int instructions[MAX_INSTRUCT]; // Time each instruction takes, -1 for IO
  int currentInstruction;         // Current instruction index
  int ioTicks;                    // IO ticks remaining
  int cpuTicks;                   // CPU ticks used
  int fastTicks;                  // Ticks where process used < quantum
  int readyTime;                  // Time process has spent in ready queue
  bool terminated;                // Process has terminated
} Process;

Process *parseProcess(FILE *fp);
int addInstructionToProcess(Process *self, int time);
int getMinTCompletion(Process *self);
bool tickProcess(Process *self, int tick);

void debugProcess(Process *self);
#endif // !TYPES_PROCESS
