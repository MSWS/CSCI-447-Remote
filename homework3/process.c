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
    printf("%s\n", line);
    if (result->id == INIT_VALUE) {
      processHeader(result, line);
      continue;
    }
    if (result->arrivalTime == INIT_VALUE) {
      processArrival(result, line);
      continue;
    }

    if (strcmp(line, "terminate") == 0) {
      printf("encounted terminate\n");
      return result;
    }

    processInstruction(result, line);
  }

  if(result->id == INIT_VALUE) {
    printf("Reached end, returning NULL\n");
    return NULL;
  }

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
  if (strcmp(instruction, "exe"))
    addInstructionToProcess(result, time);
  else if (strcmp(instruction, "io"))
    addInstructionToProcess(result, -time);
  else
    fprintf(stderr, "Unknown instruction: %s from %s\n", instruction, line);
}
