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

bool tickQueue(Queue *queue, Queue *promotion, int time, bool endOfBurst);

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

  int tick = 0;

  while (true) {
    Queue *currentQueue = queueA;
    if (isQueueDone(currentQueue))
      currentQueue = queueB;
    if (isQueueDone(currentQueue))
      break;
    printf("%d/%d\n", tick, currentQueue->burstTicks);
    bool endOfBurst = currentQueue->burstTicks == currentQueue->quantum - 1;
    bool usedCPU = tickQueue(currentQueue, queueA, tick, endOfBurst);
    if (!usedCPU && currentQueue == queueA) {
      currentQueue = queueB;
      endOfBurst = currentQueue->burstTicks == currentQueue->quantum - 1;
      usedCPU = tickQueue(queueB, queueA, tick, endOfBurst);
    }
    if (usedCPU) {
      currentQueue->burstTicks++;
    } else {
      currentQueue->burstTicks = 0;
    }
    if (endOfBurst)
      currentQueue->burstTicks = 0;
    tick++;
    //    usleep(1000 * 1500);
  }

  printf("Simulation done in %d ticks\n", tick);
}

bool tickQueue(Queue *queue, Queue *promotion, int time, bool endOfBurst) {
  int currentPIndex = queue->currentProcess;
  printf("Process Index %d\n", currentPIndex);
  if (currentPIndex == INIT_VALUE) {
    printf("Queue just initialized\n");
    // Queue was just initialized, get the first process
    currentPIndex = getNextProcess(queue, time);
    if (currentPIndex == INIT_VALUE) {
      // If the first process hasn't arrived yet, return
      printf("Idle...\n");
      return false;
    }
  }

  if (queue->preempt) {
    if (switchProcess(queue, time)) {
      if (queue->currentProcess != currentPIndex)
        printf("Pre-emption, switching to process %d (priority %d > %d)\n",
               queue->currentProcess,
               queue->processes[queue->currentProcess]->priority,
               queue->processes[currentPIndex]->priority);
    }
    currentPIndex = queue->currentProcess;
  }

  Process *currentProcess = queue->processes[currentPIndex];
  if (!readyForCPU(currentProcess, time)) {
    // If the current process isn't ready, change to the next one
    if (currentProcess != NULL) {
      currentProcess
          ->fastTicks++; // Previously executed, so give credit for < quantum
      if (promotion != NULL && promotion != queue &&
          currentProcess->fastTicks > 3) {
        printf("Promoting process %d to A queue\n", currentProcess->id);
        addProcessToQueue(promotion, currentProcess);
        queue->processes[currentPIndex] = NULL;
        return false;
      }
    }
    currentPIndex = getNextProcess(queue, time);
    printf("Not ready for CPU, switching to process %d\n", currentPIndex);
    if (currentPIndex == INIT_VALUE) {
      // If no process is ready, return
      // queue->currentProcess = currentPIndex;
      return false;
    }
    if (!readyForCPU(queue->processes[currentPIndex], time))
      return false;
  }
  currentProcess = queue->processes[currentPIndex];
  queue->currentProcess = currentPIndex;

  if (!currentProcess->parsedCurrentInstruction) {
    printf("Parsing next instruction\n");
    currentProcess->parsedCurrentInstruction = true;
    if (currentProcess->currentInstruction >=
        currentProcess->instructionCount) {
      printf("Process %d is done\n", currentProcess->id);
      currentProcess->terminated = true;
    } else {
      if (currentProcess->instructions[currentProcess->currentInstruction] <
          0) {
        // IO is executed immediately after we parsed
        tickProcess(currentProcess, time + 1);
      }
    }
    return true;
  }

  printf("Executing process %d\n", currentProcess->id);
  tickProcess(currentProcess, time);

  if (endOfBurst) {
    printf("End of burst, switching if available...\n");
    // If the process has finished its burst, change to the next one
    currentProcess->parsedCurrentInstruction = false;
    currentProcess->fastTicks = 0;
    switchProcess(queue, time);
  }
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
