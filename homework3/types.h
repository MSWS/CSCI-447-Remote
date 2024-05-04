#include "common.h"

#ifndef TYPES

typedef struct {
  int id;
  int priority;
  int arrivalTime;
  int ioInstructions[MAX_INSTRUCT];
  int exeInstructions[MAX_INSTRUCT];
} Process;

#endif // !TYPES
