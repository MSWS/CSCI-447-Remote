#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "process.h"

void processHeader(Process *process, char *line);
void processArrival(Process *process, char *line);
void processInstruction(Process *process, char *line);

Process *init() {
  Process *result = (Process *)malloc(sizeof(Process));

  result->id = INIT_VALUE;
  result->priority = INIT_VALUE;
  result->arrivalTime = INIT_VALUE;
  result->currentInstruction = 0;
  result->instructionCount = 0;
  result->fastTicks = 0;
  result->ioCompleteTime = 0;
  result->readyTime = 0;
  result->terminated = false;
  result->instructionSize = 10;
  // Initialize instructions array to 10
  int *array = (int *)calloc(result->instructionSize, sizeof(int));
  result->instructions = array;

  for (int i = 0; i < result->instructionSize; i++) {
    (result->instructions)[i] = INIT_VALUE;
  }
  return result;
}

Process *parseProcess(FILE *fp) {
  Process *result = init();

  char *line = malloc(128);
  size_t len = 0;
  ssize_t read;

  if (fp == NULL) {
    fprintf(stderr, "fp is null\n");
    return NULL;
  }

  while ((read = getline(&line, &len, fp)) != -1) {
    // Strip trailing newline
    int len = strlen(line);
    if (len > 0 && line[len - 1] == '\n')
      line[len - 1] = 0;
    if (result->id == INIT_VALUE) {
      processHeader(result, line);
      continue;
    }
    if (result->arrivalTime == INIT_VALUE) {
      processArrival(result, line);
      continue;
    }

    if (strcmp(line, "terminate") == 0)
      return result;

    processInstruction(result, line);
  }

  if (result->id == INIT_VALUE)
    return NULL;

  if (line)
    free(line);

  return result;
}

void processHeader(Process *result, char *line) {
  // Just started processing new process
  if (*line != 'P') {
    fprintf(stderr, "Expected process header, got %s\n", line);
    return;
  }
  char *processHeader = line + 1; // Skip P prefix
  char *colonIndex = strchr(processHeader, ':');
  *(colonIndex) = '\0'; // Replace the : with a \0 to easily find the ID
  result->id = atoi(processHeader);
  result->priority = atoi(colonIndex + 1);
}

void processArrival(Process *result, char *line) {
  // Parsed ID and Priority, next line is arrival time
  const int prefixLength = strlen("arrival t:");
  if (strlen(line) < prefixLength ||
      strncmp(line, "arrival t:", prefixLength) != 0) {
    fprintf(stderr, "Expected process arrival time, got %s\n", line);
    return;
  }

  char *arrivalTime = line + prefixLength;
  result->arrivalTime = atoi(arrivalTime);
}

void processInstruction(Process *result, char *line) {
  char *instruction = line;
  char *colonIndex = strchr(line, ':');
  if (colonIndex == NULL) {
    fprintf(stderr, "Expected colon in io/exe instruction, got %s\n", line);
    return;
  }

  *(colonIndex) = '\0';
  int time = atoi(colonIndex + 1);
  if (strcmp(instruction, "exe") == 0)
    addInstructionToProcess(result, time);
  else if (strcmp(instruction, "io") == 0)
    addInstructionToProcess(result, -time);
  else
    fprintf(stderr, "Unknown instruction: %s from %s\n", instruction, line);
}

int addInstructionToProcess(Process *self, int time) {
  if (self->instructionCount >= self->instructionSize) {
    const int newSize = self->instructionCount * 2;
    printf("Reallocating instructions to %d\n", newSize);
    int *newInstructions =
        (int *)realloc(self->instructions, newSize * sizeof(int));
    if (newInstructions == NULL) {
      fprintf(stderr, "Failed to realloc instructions\n");
      return self->instructionCount;
    }

    self->instructions = newInstructions;
    self->instructionSize = newSize;
  }
  // *(self->instructions)[self->instructionCount++] = time;
  self->instructions[self->instructionCount++] = time;
  return self->instructionCount;
}

void tickProcess(Process *self, int time) {
  printf("Executing instruction %d\n", self->currentInstruction);
  if (self->currentInstruction >= self->instructionCount) {
    // OS should already have detected this
    self->terminated = true;
    fprintf(stderr, "Process %d terminated\n", self->id);
  }
  int instTime = self->instructions[self->currentInstruction];
  if (instTime < 0) {
    printf("IO instruction\n");
    // IO instruction
    self->ioCompleteTime = time + abs(instTime);
    self->currentInstruction++;
    self->parsedCurrentInstruction = false;
    return;
  }

  // CPU instruction
  printf("CPU instruction\n");

  self->instructions[self->currentInstruction] = --instTime;

  printf("Instruction time: %d\n", instTime);

  if (instTime == 0) {
    self->currentInstruction++;
    self->parsedCurrentInstruction = false;
  }
}

bool readyForCPU(Process *self, int time) {
  if (self == NULL || self->terminated)
    return false;
  if (time < self->arrivalTime)
    return false;
  if (self->ioCompleteTime > time) {
    printf("\t IO not complete until %d\n", self->ioCompleteTime);
    return false; // Still processing IO
  }
  return true;
}
