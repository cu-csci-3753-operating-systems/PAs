#include "simulator.h"
#include <stdio.h>
#include <stdlib.h>

// skeleton for predictive paging implementation

void pageit(Pentry q[MAXPROCESSES]) {

  // this file contains the stub for a predictive pager
  // you may need to add/remove/modify any part of this file

  // static variables
  static int initialized = 0;
  static int tick = 1; // artificial time

  // local variables

  // initialise static variables on first run
  if (!initialized) {
    initialized = 1;
  }

  // TODO: implement predictive paging
  fprintf(stderr, "pager-predict not yet implemented. Exiting...\n");
  exit(EXIT_FAILURE);

  // advance time for next pageit() iteration
  tick++;
}
