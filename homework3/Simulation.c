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

bool switchProcess(Queue *queue, int time);

bool tickQueue(Queue *queue, int tick) {
  Process *current = queue->processes[queue->currentProcess];

  if (current == NULL)
    return false;

  if (current->terminated) {
    if(!switchProcess(queue, tick))
      return false;
  }

  debugProcess(current);

  current->cpuTicks++;

  bool used = tickProcess(current);

  if (current->cpuTicks > queueB->quantum) {
    // Cycle to next process
    switchProcess(queue, tick);
  }

  if (!used) {
    switchProcess(queue, tick);
  }

  if (queue->processes[queue->currentProcess]->terminated)
    return false;

  if (used) {
    current->fastTicks = 0;
  } else {
    current->fastTicks++;
  }

  return true;
}

void Simulate(int quantumA, int quantumB, int preEmp) {
  // A function whose input is the quanta for queues A and B,
  // well as whether preemption is enabled.
  printf("Quantum A val received : %d\n", quantumA);
  printf("Quantum B val received : %d\n", quantumB);
  printf("preEmpt val received   : %d\n", preEmp);

  queueA = initializeQueue();
  queueB = initializeQueue();

  queueA->quantum = quantumA;
  queueB->quantum = quantumB;

  queueA->prioritize = false;
  queueB->prioritize = preEmp != 0;

  Process *tmp;

  while ((tmp = parseProcess(fp)) != NULL) {
    addProcessToQueue(queueB, tmp);
  }

  fclose(fp);

  int tick = -1;
  while (true) {
    tick++;
    printf("BEGIN TICK %d\n", tick);
    usleep(1000 * 50);
    if (tickQueue(queueA, tick)) {
      printf("A ");
      continue;
    }

    int previousIndex = queueB->currentProcess;
    Process *previousProcess = queueB->processes[previousIndex];

    if (!tickQueue(queueB, tick))
      break;
    printf("B ");

    if (!previousProcess->terminated &&previousProcess->fastTicks == 3) {
      printf("PROMOTING %d TO A QUEUE\n", previousProcess->id);
      addProcessToQueue(queueA, previousProcess);
      queueB->processes[previousIndex] = NULL;
    }
  }
}

bool switchProcess(Queue *queue, int time) {
  int newIndex = getNextProcess(queue, time);
  if (newIndex == -1) {
    printf("Could not find new process\n");
    return false;
  }
  Process *process = queue->processes[newIndex];
  process->cpuTicks = 0;
  queue->currentProcess = newIndex;
  return true;
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
