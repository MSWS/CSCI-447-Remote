/*
 *	PageSim.c
 *
 *	Demo of CSCI 447 memory management simulator
 *
 *	Filip Jagodzinski, Computer Science, WWU, 21 February 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include "MemSim.h"

#define TABLESIZE	97		// size of hash table of processes
#define MAXPROC		64		// maximum number of processes in the system
#define PAGESIZE	4096		// system page size

// data structure for process hash table entries
typedef struct procstruct {
	int pid;
    int page;
	struct procstruct* next;
} proc;

proc* table[TABLESIZE];		// process hash table
int procCount = 0;		// number of processes
int pageCount = 0;


// look for pid in the hash table
// if pid found, return 1
// if pid not found and number of processes < MAXPROC, add pid to hash table and return 1
// if pid not found and number of processes >= MAXPROC, return 0

int find(int pid, int page) {
	int index = (pid + page) % TABLESIZE;
	proc* node = table[index];
	proc* prev = NULL;

	// look along the chain for this hash table index
	while (node != NULL && (node->pid != pid || node ->page != page)) {
		prev = node;
		node = node->next;
	}

	// pid not found in hash table
	if (node == NULL) {
		if (procCount >= MAXPROC)
			return 0;		// too many processes

		else {				// add new process
			node = (proc*) malloc(sizeof(proc));
			node->pid = pid;
            node->page = page;
			node->next = NULL;
			procCount++;
            pageCount++;
			if (prev == NULL)
				table[index] = node;
			else
				prev->next = node;
		}
	}
	return 1;
}

// remove a pid from the hash table
void Remove(int pid) {
	int index = pid % TABLESIZE;
	proc* node = table[index];
	proc* prev = NULL;

	// look along the chain for this hash table index
	while (node != NULL && node->pid != pid) {
		prev = node;
		node = node->next;
	}

	// if pid found, remove it
	if (node != NULL) {
		if (prev == NULL)
			table[index] = node->next;
		else
			prev->next = node->next;
		free(node);
	}
}

// called in response to a memory access attempt by process pid to memory address
int Access(int pid, int address, int write) {
	if (find(pid, address/PAGESIZE)) {
		printf("pid %d wants %s access to address %d on page %d. Page count is %d\n", 
		       pid, (write) ? "write" : "read", address, address/PAGESIZE, pageCount);
		return 1;
	} else {
		printf("pid %d refused\n", pid);
		return 0;
	}
}

// called when process terminates
void Terminate(int pid) {
	printf("pid %d terminated\n", pid);
	procCount--;
	Remove(pid);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Simulate [LRU/RANDOM/LFU]\n");
    return 0;
  }

	int i;
	// initialize the process hash table
	for (i = 0; i < TABLESIZE; i++) table[i] = NULL;

	printf("MMU simulation started\n");
	Simulate(1000);
	printf("MMU simulation completed\n");
}
