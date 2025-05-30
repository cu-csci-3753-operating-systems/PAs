#pragma once

#define ARRAY_SIZE 8

int array_init(array *s);                 // initialise the array
int array_put(array *s, char *hostname);  // place element into the array, block when full
int array_get(array *s, char **hostname); // remove element from the array, block when empty
void array_free(array *s);                // free the array's resources
