#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "process.h"

void processHeader(Process *process, char *line);
void processArrival(Process *process, char *line);
void processInstruction(Process *process, char *line);

Process *parseProcess(FILE *fp) {
  Process *result = (Process *)malloc(sizeof(Process));

  result->id = INIT_VALUE;
  result->priority = INIT_VALUE;
  result->arrivalTime = INIT_VALUE;
  result->currentInstruction = 0;
  result->instructionCount = 0;
  result->fastTicks = 0;
  result->cpuTicks = 0;
  result->ioTicks = 0;
  result->terminated = false;

  for (int i = 0; i < MAX_INSTRUCT; i++)
    result->instructions[i] = INIT_VALUE;

  char *line = NULL;
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
  self->instructions[self->instructionCount++] = time;
  return self->instructionCount;
}

int getMinTCompletion(Process *self) {
  int sum = self->arrivalTime;
  for (int i = 0; i < self->instructionCount; i++) {
    int time = self->instructions[i];
    sum++; // Process each instruction
    if (time < 0)
      continue;
    sum += time + 1;
  }

  return sum;
}

// Return true if we used CPU
bool tickProcess(Process *self) {
  if (self->ioTicks > 0) {
    self->ioTicks--;
  }

  if (self->instructions[self->currentInstruction] == 0) {
    // Finished previous instruction, simulate parsing next
    self->currentInstruction++;
    if (self->currentInstruction > self->instructionCount) {
      self->terminated = true;
      printf("Process %d finished\n", self->id);
    }
    return false;
  }

  int time = self->instructions[self->currentInstruction];
  if (time < 0) {
    time = abs(time);
    // IO Operation
    if (self->ioTicks == 0) {
      self->ioTicks = time;
      self->instructions[self->currentInstruction] =
          0; // Update array to reflect we've added it to IO buffer
      return false;
    }

    // IO Busy and this instruction is also IO
    return false;
  }

  self->instructions[self->currentInstruction] = time - 1;
  return true;
}

void debugProcess(Process *self) {
  printf("PID %d ID %d (%d)\n", self->id, self->currentInstruction,
         self->instructions[self->currentInstruction]);
  printf("IO %d F %d\n", self->ioTicks, self->fastTicks);
}
