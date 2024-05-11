/*
 *  File: Simulation.c ... a skeleton file
 *  Author: Filip Jagodzinski <filip.jagodzinski@wwu.edu>
 *  Last update : 08 February 2018
 */

#include <stdarg.h>
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

enum PResult { IDLE, ACTIVE, NEW };

Queue *queueA;
Queue *queueB;
int quantumA, quantumB, preemption;
int instructionCount = 0;
int processCount = 0;
int startTick = 0;
FILE *fp;

Process *terminated;
int terminatedCount = 0;

enum PResult tickQueue(Queue *queue, Queue *promotion, int time);

void Simulate(int quantumA, int quantumB, int preEmp) {
  queueA = initQueue();
  queueB = initQueue();

  queueA->quantum = quantumA;
  queueB->quantum = quantumB;

  queueA->prioritize = false;
  queueB->prioritize = true;

  queueA->preempt = false;
  queueB->preempt = preEmp != 0;

  Process *tmp;

  while ((tmp = parseProcess(fp)) != NULL) {
    addProcessToQueue(queueB, tmp);
    processCount++;
    instructionCount += tmp->instructionCount + 1; // +1 for termination
  }
  fclose(fp);

  terminated = calloc(sizeof(Process), processCount);

  int tick = 0;

  while (true) {
    Queue *currentQueue = queueA;
    if (isQueueDone(currentQueue))
      currentQueue = queueB;
    if (isQueueDone(currentQueue))
      break;
    debug("%d/%d\n", tick, currentQueue->burstTicks);
    enum PResult usedCPU = tickQueue(currentQueue, queueA, tick);
    if (usedCPU == IDLE && currentQueue == queueA) {
      currentQueue = queueB;
      usedCPU = tickQueue(queueB, queueA, tick);
    }
    if((usedCPU == ACTIVE || usedCPU == NEW) && startTick == 0)
      startTick = tick;
    tick++;
    // usleep(1000 * 1500);
  }

  printf("Start/end time: %d, %d\n", startTick, tick);
  printf("Processes Completed: %d\n", processCount);
  printf("Instructions Completed: %d\n", instructionCount);
  int totalTime = getTotalReadyTime(queueA) + getTotalReadyTime(queueB);
  int minATime = getMinReadyTime(queueA), maxATime = getMaxReadyTime(queueA);
  int minBTime = getMinReadyTime(queueB), maxBTime = getMaxReadyTime(queueB);
  int minTime = 0, maxTime = 0;
  if (minATime == -1)
    minTime = minBTime;
  else if (minBTime == -1)
    minTime = minATime;
  else
    minTime = minATime < minBTime ? minATime : minBTime;

  if (maxATime == -1)
    maxTime = maxBTime;
  else if (maxBTime == -1)
    maxTime = maxATime;
  else
    maxTime = maxATime > maxBTime ? maxATime : maxBTime;
  printf("Ready time average: %f\n", (float)totalTime / (processCount));
  printf("Min ready time: %d\n", minTime);
  printf("Max ready time: %d\n", maxTime);

  for (int i = 0; i < terminatedCount; i++) {
    Process p = terminated[i];
    printf("P%d time_completion:%d time_waiting:%d termination_queue:%s\n",
           p.id, abs(p.terminatedTime), p.readyTime,
           p.terminatedTime < 0 ? "A" : "B");
  }
}

enum PResult tickQueue(Queue *queue, Queue *promotion, int time) {
  int currentPIndex = queue->currentProcess;
  debug("Process Index %d\n", currentPIndex);
  if (currentPIndex == INIT_VALUE || queue->processes[currentPIndex] == NULL) {
    debug("Queue just initialized\n");
    // Queue was just initialized, get the first process
    currentPIndex = getNextProcess(queue, time);
    if (currentPIndex == INIT_VALUE) {
      // If the first process hasn't arrived yet, return
      debug("Idle...\n");
      return IDLE;
    }
  }

  enum PResult result = ACTIVE;

  if (queue->preempt) {
    if (switchProcess(queue, time)) {
      // debug("Pre-emption, switching to process %d (priority %d > %d)\n",
              // queue->currentProcess,
              // queue->processes[queue->currentProcess]->priority,
              // queue->processes[currentPIndex]->priority);
      if (queue->processes[currentPIndex] == NULL) {
        fprintf(stderr, "Error: Process %d is null\n", currentPIndex);
      }
      queue->processes[currentPIndex]->parsedCurrentInstruction = false;
      queue->processes[currentPIndex]->cpuTicks = 0;
      queue->processes[currentPIndex]->fastTicks++;
      result = NEW;
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
        debug("Promoting process %d to A queue\n", currentProcess->id);
        addProcessToQueue(promotion, currentProcess);
        queue->processes[currentPIndex] = NULL;
        return IDLE;
      }
    }
    currentPIndex = getNextProcess(queue, time);
    debug("Not ready for CPU, switching to process %d\n", currentPIndex);
    if (currentPIndex == INIT_VALUE) {
      // If no process is ready, return
      queue->currentProcess = currentPIndex;
      return IDLE;
    }
    if (!readyForCPU(queue->processes[currentPIndex], time))
      return IDLE;
  }
  currentProcess = queue->processes[currentPIndex];
  queue->currentProcess = currentPIndex;

  if (!currentProcess->parsedCurrentInstruction) {
    debug("Parsing next instruction\n");
    currentProcess->parsedCurrentInstruction = true;
    currentProcess->cpuTicks = 1;
    if (currentProcess->currentInstruction >=
        currentProcess->instructionCount) {
      debug("Process %d is done\n", currentProcess->id);
      // Offset by one since technically the process isn't
      // terminated until the next tick
      if (promotion == queue || promotion == NULL)
        currentProcess->terminatedTime = -time - 1; // Queue A
      else
        currentProcess->terminatedTime = time + 1; // Queue B
      terminated[terminatedCount++] = *currentProcess;
    } else {
      if (currentProcess->instructions[currentProcess->currentInstruction] <
          0) {
        // IO is executed immediately after we parsed
        tickProcess(currentProcess, time + 1);
      }
    }
  } else {
    debug("Executing process %d\n", currentProcess->id);
    tickProcess(currentProcess, time);
  }

  incrementReadyTime(queue, time);

  if (currentProcess->cpuTicks >= queue->quantum)  {
    debug("End of burst, switching if available...\n");
    // If the process has finished its burst, change to the next one
    currentProcess->parsedCurrentInstruction = false;
    currentProcess->fastTicks = 0;
    currentProcess->cpuTicks = 0;
    switchProcess(queue, time);
  }
  
  return result;
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
