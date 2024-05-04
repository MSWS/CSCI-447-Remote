#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

Process* parseProcess(FILE* fp) {
  Process* result = (Process*) malloc(sizeof(Process));

  char* line = NULL;
  size_t len = 0;
  ssize_t read;

  if(fp == NULL) {
    perror("fp is null");
    return NULL;
  }

  while((read = getline(&line, &len, fp)) != -1) {
    printf("%s", line);
  }

  fclose(fp);

  if(line)
    free(line);

  return result;
}

typedef struct {
  int quantum;
  Process processes[MAX_PROCESS];
} Queue;
