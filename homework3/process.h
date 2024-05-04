#ifndef TYPES_PROCESS
#define TYPES_PROCESS
#include "common.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  int id;                 // Process ID
  int priority;           // Process Priority
  int arrivalTime;        // Process Arrival Time
  int instructionCount;   // Number of instructions
  int instructionSize;    // Size of instructions array, should always be >=
                          // instructionCount
  int currentInstruction; // Current instruction index
  int ioCompleteTime;            // IO ticks remaining
  int cpuTicks;           // CPU ticks used
  int fastTicks;          // Ticks where process used < quantum
  int readyTime;          // Time process has spent in ready queue
  bool parsedCurrentInstruction; // Whether the current instruction has been
                                 // parsed
  bool terminated;               // Process has terminated
  int *instructions;            // Time each instruction takes, - for IO
} Process;

Process *parseProcess(FILE *fp);
int addInstructionToProcess(Process *self, int time);
bool readyForCPU(Process *self, int time);
void tickProcess(Process *self, int time);

#endif // !TYPES_PROCESS
