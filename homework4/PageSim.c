#include "MemSim.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 64
#define PAGE_SIZE 4096
#define TABLE_SIZE 2048

struct PageTableEntry {
  int pid;
  int page;
  bool dirty;
  bool read;
  bool valid;
};

int lastEvictedPage = 0;
bool IsPIDInTable(int pid);
int GetUniquePIDCount();
int GetExistingPage(int pid, int page, bool write);
int GetFreePage();
int GetNewPage();
void EvictPage(int page);

struct PageTableEntry pageTable[TABLE_SIZE];

int lfu[TABLE_SIZE];

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
    printf("LRU Policy\n");
    policy = LRU;
  } else if (strcmp(arg, "RANDOM") == 0) {
    printf("RANDOM Policy\n");
    policy = RANDOM;
  } else if (strcmp(arg, "LFU") == 0) {
    printf("LFU Policy\n");
    policy = LFU;
    for (int i = 0; i < TABLE_SIZE; i++) {
      lfu[i] = -1;
    }
  }

  Simulate(1000);
  return 0;
}

void Terminate(int pid) { printf("omg much wow bye %d\n", pid); }

int Access(int pid, int address, int write) {
  if (!IsPIDInTable(pid)) {
    if (GetUniquePIDCount() >= MAX_PROCESSES) {
      // printf("pid: %d, address: %d, write: %d\n", pid, address, write);
      // Terminate(pid);
      return 0;
    }
  }

  int page = address / PAGE_SIZE;
  int existing = GetExistingPage(pid, page, write);
  if (existing != -1) {
    return 1;
  }

  int newPage = GetFreePage();
  if(newPage != -1) {
  pageTable[newPage].pid = pid;
  pageTable[newPage].page = page;
  pageTable[newPage].dirty = write;
  pageTable[newPage].read = true;
  pageTable[newPage].valid = true;
  }

  int newPage = GetNewPage();
  if (pageTable[newPage].valid)
    EvictPage(newPage);

  pageTable[newPage].pid = pid;
  pageTable[newPage].page = page;
  pageTable[newPage].dirty = write;
  pageTable[newPage].read = true;
  pageTable[newPage].valid = true;
  return 1;
}

int GetExistingPage(int pid, int page, bool write) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (pageTable[i].valid == false)
      continue;
    if (pageTable[i].pid != pid || pageTable[i].page != page)
      continue;
    pageTable[i].read = true;
    if (write)
      pageTable[i].dirty = true;
    return i;
  }
  return -1;
}

int GetFreePage() {
  for(int i = 0; i < TABLE_SIZE; i++) {
    if(!pageTable[i].valid)
      return i;
  }
  return -1;
}

int GetNewPage() {
  int minIndex = 0, minValue = 0;
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
      if (!pageTable[index].valid)
        return index;
      int value = pageTable[index].read + pageTable[index].dirty * 2;
      if (value < minValue) {
        minValue = value;
        minIndex = index;
      }
    }
    for (int i = 0; i < lastEvictedPage; i++) {
      if (pageTable[i].valid && pageTable[i].read) {
        pageTable[i].read = false;
        lfu[i]++;
      }
    }
    return minIndex;
  }
  return -1;
}

int GetUniquePIDCount() {
  int count = 0;
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (pageTable[i].valid)
      count++;
  }
  return count;
}

void EvictPage(int page) { pageTable[page].valid = false; }

bool IsPIDInTable(int pid) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (pageTable[i].valid && pageTable[i].pid == pid)
      return true;
  }
  return false;
}
