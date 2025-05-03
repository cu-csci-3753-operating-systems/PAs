# Shared Array

## Introduction
In this assignment, you will build a thread-safe array, that will be used in a future assignment. Your code should be modular, in the sense that files you implement in this assignment (`array.{c,h}`) should contain nothing more than the necessary behaviour for the array. This approach is an application of the principal of [separation of concerns](https://en.wikipedia.org/wiki/Separation_of_concerns). To go about this, we will need an *interface* and an *implementation*.

### Interface and implementation
The first step to developing an abstraction is proposing an interface. In this scope, an interface contains a list of functions and behaviours that are available to the end user. As an example, consider the following function prototypes that comprise the interface for a stack that stores integers:
```c
int stack_init(stack *s);              // initialise the stack
int stack_push(stack *s, int element); // place element on the top of the stack
int stack_pop(stack *s, int *element); // remove element from the top of the stack
void stack_free(stack *s);             // free the stack's resources
```
In C, interfaces are defined in a standalone header file. A more complete version of this can be seen in [`stack.h`](stack.h), including a `struct` that holds our data.
```c
#pragma once

#define STACK_SIZE 10 // max elements in stack

typedef struct {
  int array[STACK_SIZE]; // storage array for integers
  int top;               // array index indicating where the top is
} stack;

int stack_init(stack *s);              // initialise the stack
int stack_push(stack *s, int element); // place element on the top of the stack
int stack_pop(stack *s, int *element); // remove element from the top of the stack
void stack_free(stack *s);             // free the stack's resources
```
Some notes about the above code:
- The `#pragma once` preprocessor directive ensures this header file is only included in *compilation* once, regardless of how many other source code files include it. This is synonymous for `#ifndef/#define` macro guard that you may have seen in previous courses
- Each method of our interface, other than `stack_free()` as that should never fail, returns an integer to indicate the status of an operation. Generally, a zero means success, and any other value means failure
- The `stack_pop()` function passes the popped value back the caller via a reference, rather than returning this value.

Now, the implementation of the aforementioned prototypes lives in the [`stack.c`](file):
```c
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
```

We can then test our code by writing a standalone program that calls each of the functions of our stack implementation:
```c
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
```

Compile and run this program with
```
$ gcc stack.c test.c
$ ./a.out
1 9 2 6 5 3 5 7 6 3
```

## Implementation
Your thread-safe array implementation will require you to implement a solution to the bounded buffer problem. You can organise the array as a FIFO (first in, first out) stack, LIFO (last in, first out) stack, or circular queue. Solutions that rely on linked lists, trees, dictionaries, etc. will **not** be accepted.

Your array should implement the following interface:
```c
int array_init(array *s);                 // initialise the array
int array_put(array *s, char *hostname);  // place element into the array, block when full
int array_get(array *s, char **hostname); // remove element from the array, block when empty
void array_free(array *s);                // free the array's resources
```
Notice the data types being used in the put and get functions - this array stores C strings that are given to you in the form of [hostnames](../common/hostnames/) (you can download the zip file here to obtain your program's input). Your array should store a **maximum of 8** strings at once. This, and other fixed values you may need should be defined as macros in your header file.

Your array must incorporate some form of synchronisation and/or signaling mechanisms (mutexes, semaphores, condition variables, etc.) to ensure there are no [race conditions](https://en.wikipedia.org/wiki/Race_condition), [buy waiting](https://en.wikipedia.org/wiki/Busy_waiting), or lock-ups. You are allowed to use external libraries to accomplish this, such as the C semaphore library.

## Testing
It is crucial (for this, and future assignments) to write a multi-threaded test program to verify the functionality of your implementation. This program should exercise the shared array in such a way that ensures [thread safety](https://en.wikipedia.org/wiki/Thread_safety). This can be accomplished using the C Pthreads library and calling the `pthread_create()` function to spawn a number of simultaneous threads, each of which will either put or get data to your shared array, multiple times. Follows is a pseudocode example of this:
```
produce() {
  // call array_put() some number of times
}

consume() {
  // call array_get() some number of times
}

main() {
  array_init()

  // call pthread_create some number of times

  // call pthread_join some number of times

  array_free()
}
```

## Submission requirements
- Your `array.c` source code
- Your `array.h` header file
- A 1 page report of things you learned, difficulties you encountered and how you overcame them, comments about the assignment, etc.
