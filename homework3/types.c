#ifndef TYPES_IMPL
#define TYPES_IMPL

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_VALUE -1

Process *parseProcess(FILE *fp) {
  Process *result = (Process *)malloc(sizeof(Process));

  result->id = INIT_VALUE;
  result->priority = INIT_VALUE;
  result->arrivalTime = INIT_VALUE;

  for (int i = 0; i < MAX_INSTRUCT; i++) {
    result->ioInstructions[i] = INIT_VALUE;
    result->exeInstructions[i] = INIT_VALUE;
  }

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  if (fp == NULL) {
    perror("fp is null");
    return NULL;
  }

  while ((read = getline(&line, &len, fp)) != -1) {
    // Strip trailing newline
    int len = strlen(line);
    if (len > 0 && line[len - 1] == '\n')
      line[len - 1] = 0;
    printf("%s\n", line);
    if (result->id == INIT_VALUE) {
      // Just started processing new process
      if (*line != 'P') {
        fprintf(stderr, "Expected process header, got %s\n", line);
        return NULL;
      }
      char *processHeader = line + 1; // Skip P prefix
      char *colonIndex = strchr(processHeader, ':');
      *(colonIndex) = '\0'; // Replace the : with a \0 to easily find the ID
      result->id = atoi(processHeader);
      result->priority = atoi(colonIndex + 1);
    } else if (result->arrivalTime == INIT_VALUE) {
      // Parsed ID and Priority, next line is arrival time
      const int prefixLength = strlen("arrival t:");
      if (strlen(line) < prefixLength ||
          strncmp(line, "arrival t:", prefixLength) != 0) {
        fprintf(stderr, "Expected process arrival time, got %s\n", line);
        return NULL;
      }

      char *arrivalTime = line + prefixLength;
      result->arrivalTime = atoi(arrivalTime);
    }

    if (strcmp(line, "terminate") == 0)
      return result;

    char* instruction = line;
    char* colonIndex = strchr(line, ':');
    if(colonIndex == NULL) {
      fprintf(stderr, "Expected colon in io/exe instruction, got %s\n", line);
      return NULL;
    }
  }

  if (line)
    free(line);

  return result;
}

int addToQueue(Queue *self, Process *process) {
  self->processes[self->processCount++] = process;
  return self->processCount;
}
#endif /* ifndef TYPES_IMPL */
