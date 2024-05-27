#include "MemSim.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 64
#define PAGE_SIZE 4096
#define TABLE_SIZE 2048
#define LFU_RATE 1000

struct PageTableEntry {
  int pid;
  int page;
  bool dirty;
  bool read;
  bool valid;
};

int lastEvictedPage = 0;
int GetPIDIndex(int pid);
bool IsPIDInTable(int pid);
int GetUniquePIDCount();
int GetExistingPage(int pid, int page, bool write);
int GetFreePage();
int GetNewPage();
void EvictPage(int page);
void PrintResults();

struct PageTableEntry pageTable[TABLE_SIZE];

int lfuTick = 0;
int lfu[TABLE_SIZE];

int readSwaps = 0, writeSwaps = 0;
int readAndWriteSwaps = 0;

enum Policy { LRU, RANDOM, LFU } policy;

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: ./Simulate [LRU/RANDOM/LFU]\n");
    return 0;
  }

  char *arg = argv[1];
  if (strcmp(arg, "LRU") != 0 && strcmp(arg, "RANDOM") != 0 &&
      strcmp(arg, "LFU") != 0) {
    printf("Usage: ./Simulate [LRU/RANDOM/LFU]\n");
    return 0;
  }

  for (int i = 0; i < TABLE_SIZE; i++) {
    pageTable[i].pid = -1;
    pageTable[i].page = -1;
    pageTable[i].dirty = false;
    pageTable[i].read = false;
    pageTable[i].valid = false;
  }

  if (strcmp(arg, "LRU") == 0) {
    policy = LRU;
  } else if (strcmp(arg, "RANDOM") == 0) {
    policy = RANDOM;
  } else if (strcmp(arg, "LFU") == 0) {
    policy = LFU;
    for (int i = 0; i < TABLE_SIZE; i++)
      lfu[i] = -1;
  }

  Simulate(100000);
  PrintResults();
  return 0;
}

void PrintResults() {
  // <row,PID,p,dirty bit,reference bit>
  printf("row, PID,    p, dirty bit ,reference bit\n");
  for (int i = 0; i < TABLE_SIZE; i++) {
    printf("%3d, %3d, %5d, %1d, %1d\n", i, pageTable[i].pid, pageTable[i].page,
           pageTable[i].dirty, pageTable[i].read);
  }

  printf("Read Swaps: %d\n", readSwaps);
  printf("Write Swaps: %d\n", writeSwaps);
  printf("Total Swaps: %d\n", readSwaps + writeSwaps);
  printf("%% of page faults that required both writing and reading: %f\n",
         (float)readAndWriteSwaps / (readSwaps + writeSwaps) * 100);
}

void Terminate(int pid) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (pageTable[i].pid == pid)
      pageTable[i].valid = false;
  }
}

int Access(int pid, int address, int write) {
  if (policy == LFU) {
    lfuTick++;
    if (lfuTick % LFU_RATE == 0) {
      for (int i = 0; i < TABLE_SIZE; i++) {
        if (lfu[i] > 0)
          lfu[i] = lfu[i] / 2;
      }
    }
  }

  if (!IsPIDInTable(pid))
    if (GetUniquePIDCount() >= MAX_PROCESSES)
      return 0;

  int page = address / PAGE_SIZE;
  int existing = GetExistingPage(pid, page, write);
  if (existing != -1)
    return 1;

  int newPage = GetFreePage();
  if (newPage != -1) {
    pageTable[newPage].pid = pid;
    pageTable[newPage].page = page;
    pageTable[newPage].dirty = write;
    pageTable[newPage].read = !write;
    pageTable[newPage].valid = true;
    lfu[newPage] = 0;
    readSwaps++;
    return 1;
  }

  newPage = GetNewPage();
  if (!write && pageTable[newPage].dirty)
    readAndWriteSwaps++;

  if (pageTable[newPage].valid)
    EvictPage(newPage);

  pageTable[newPage].pid = pid;
  pageTable[newPage].page = page;
  pageTable[newPage].dirty = write;
  pageTable[newPage].read = !write;
  pageTable[newPage].valid = true;
  lfu[newPage] = 0;
  return 1;
}

int GetExistingPage(int pid, int page, bool write) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (!pageTable[i].valid)
      continue;
    if (pageTable[i].pid != pid || pageTable[i].page != page)
      continue;
    if (write)
      pageTable[i].dirty = true;
    else
      pageTable[i].read = true;
    if (policy == LFU)
      lfu[i]++;
    return i;
  }
  return -1;
}

int GetFreePage() {
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (!pageTable[i].valid)
      return i;
  }
  return -1;
}

int GetNewPage() {
  int minIndex = 0, minValue = 0, count = 0;
  switch (policy) {
  case RANDOM:
    return rand() % TABLE_SIZE;
  case LFU:
    for (int i = 0; i < TABLE_SIZE; i++) {
      if (lfu[i] < lfu[minIndex])
        minIndex = i;
    }
    return minIndex;
  case LRU:
    for (int i = 0; i < TABLE_SIZE; i++) {
      int index = (lastEvictedPage + i) % TABLE_SIZE;
      int value = pageTable[index].read + pageTable[index].dirty * 2;
      if (value < minValue) {
        minValue = value;
        minIndex = index;
        count = i;
      }
    }
    for (int i = 0; i < count; i++) {
      int index = (lastEvictedPage + i) % TABLE_SIZE;
      pageTable[index].read = false;
    }
    return minIndex;
  }
  perror("We so fucked up\n");
  return -1;
}

int GetUniquePIDCount() {
  int count = 0;
  int seen[MAX_PROCESSES];
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (!pageTable[i].valid)
      continue;
    bool found = false;
    for (int j = 0; j < count; j++) {
      if (seen[j] == pageTable[i].pid) {
        found = true;
        break;
      }
    }
    if (!found)
      seen[count++] = pageTable[i].pid;
  }
  return count;
}

void EvictPage(int page) {
  if (pageTable[page].dirty)
    writeSwaps++;
  else
    readSwaps++;
  pageTable[page].valid = false;
  lastEvictedPage = page;
}

int GetPIDIndex(int pid) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (pageTable[i].valid && pageTable[i].pid == pid)
      return i;
  }
  return -1;
}

bool IsPIDInTable(int pid) { return GetPIDIndex(pid) != -1; }
