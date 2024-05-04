#ifndef TYPES
#include "common.h"
#include <stdio.h>

typedef struct {
  int id;
  int priority;
  int arrivalTime;
  int ioInstructions[MAX_INSTRUCT];
  int exeInstructions[MAX_INSTRUCT];
} Process;

Process* parseProcess(FILE* fp);
#endif // !TYPES
