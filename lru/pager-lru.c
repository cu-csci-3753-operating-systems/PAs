#include "simulator.h"
#include <stdio.h>
#include <stdlib.h>

// skeleton for LRU paging implementation

void pageit(Pentry q[MAXPROCESSES]) {

  // static variables
  static int initialized = 0;
  static int tick = 1; // artificial time
  static int timestamps[MAXPROCESSES][MAXPROCPAGES];

  // local variables
  int proc;
  int page;

  // initiali timestamp array on first run
  if (!initialized) {
    for (proc = 0; proc < MAXPROCESSES; proc++) {
      for (page = 0; page < MAXPROCPAGES; page++) {
        timestamps[proc][page] = 0;
      }
    }
    initialized = 1;
  }

  // TODO: implement LRU paging
  fprintf(stderr, "pager-lru not yet implemented. Exiting...\n");
  exit(EXIT_FAILURE);

  // advance time for next pageit() iteration
  tick++;
}
