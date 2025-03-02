#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  stack my_stack;

  // initialise stack, exit if failed
  if (stack_init(&my_stack) < 0) {
    exit(-1);
  }

  // push STACK_SIZE values to stack
  for (int i = 0; i < STACK_SIZE; i++) {
    int val = rand() % 10;

    if (stack_push(&my_stack, val) == 0) {
      printf("pushed: %d\n", val);
    } else { // -1 if failed
      printf("failed to push to stack\n");
    }
  }
  printf("\n");

  int pop_val;
  // pop values and print until empty
  while (stack_pop(&my_stack, &pop_val) == 0)
    printf("popped: %d\n", pop_val);

  stack_free(&my_stack); // free data used by data
  return 0;
}
