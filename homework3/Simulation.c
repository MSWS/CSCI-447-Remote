/*
 *  File: Simulation.c ... a skeleton file
 *  Author: Filip Jagodzinski <filip.jagodzinski@wwu.edu>
 *  Last update : 08 February 2018
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* If there are custom classes/source files that you write, with
   custom functions, and you want those functions available for use in
   THIS .c file, then include the header file for the custom .c
   file(s) you've written, using the #include directive. For example:
 */

#include "process.h"
#include "queue.h"

Queue *queueA;
Queue *queueB;
int quantumA, quantumB, preemption;
FILE *fp;

void Simulate(int quantumA, int quantumB, int preEmp) {
  queueA = initQueue();
  queueB = initQueue();

  queueA->quantum = quantumA;
  queueB->quantum = quantumB;

  queueA->prioritize = false;
  queueB->prioritize = true;

  queueA->preempt = preEmp != 0;
  queueB->preempt = preEmp != 0;

  Process *tmp;

  while ((tmp = parseProcess(fp)) != NULL) {
    addProcessToQueue(queueB, tmp);
  }

  fclose(fp);
}

int main(int argc, char **argv) {
  if (argc != 5) {
    printf("Incorrect num of arguments\n");
    return 1;
  }
  quantumA = atoi(argv[2]);
  quantumB = atoi(argv[3]);
  preemption = atoi(argv[4]);

  fp = fopen(argv[1], "rt");
  if (fp == NULL) {
    perror("Invalid file");
    return 1;
  }

  // Run simulation
  Simulate(quantumA, quantumB, preemption);
  return 0;
}
