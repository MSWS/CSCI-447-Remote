/*
 *  File: Simulation.c ... a skeleton file
 *  Author: Filip Jagodzinski <filip.jagodzinski@wwu.edu>
 *  Last update : 08 February 2018
 */

#include <stdio.h>
#include <stdlib.h>

/* If there are custom classes/source files that you write, with
   custom functions, and you want those functions available for use in
   THIS .c file, then include the header file for the custom .c
   file(s) you've written, using the #include directive. For example:
 */

#include "types.c"

Queue* queueA;
Queue* queueB;
int quantumA, quantumB, preemption;

void Simulate(int quantumA, int quantumB, int preEmp) {
  // A function whose input is the quanta for queues A and B,
  // well as whether preemption is enabled.
  printf("Quantum A val received : %d\n", quantumA);
  printf("Quantum B val received : %d\n", quantumB);
  printf("preEmpt val received   : %d\n", preEmp);

  queueA = malloc(sizeof(Queue));
  queueB = malloc(sizeof(Queue));

  queueA->quantum = quantumA;
  queueB->quantum = quantumB;
}

int main(int argc, char **argv) {
  if(argc != 5) {
    printf("Incorrect num of arguments\n");
    return 1;
  }
  quantumA = atoi(argv[2]); 
  quantumB = atoi(argv[3]);
  preemption = atoi(argv[4]);
  // Run simulation
  Simulate(quantumA, quantumB, preemption);
}

