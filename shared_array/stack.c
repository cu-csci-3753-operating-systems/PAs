#include "stack.h"

// initialise the stack
int stack_init(stack *s) {
  s->top = 0;
  return 0;
}

// place element on the top of the stack
int stack_push(stack *s, int element) {
  if (s->top >= STACK_SIZE)
    return -1; // return failed if the stack is full
  s->array[s->top++] = element;
  return 0;
}

// remove element from the top of the stack
int stack_pop(stack *s, int *element) {
  if (s->top < 1)
    return -1; // return failed if the stack is empty
  *element = s->array[--s->top];
  return 0;
}

// free the stack's resources
void stack_free(stack *s) {}
