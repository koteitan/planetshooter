#include <Arduboy.h>
#include "common.h"

signed long getMargin(void){
  uint8_t *heapptr, *stackptr;
  stackptr = (uint8_t *)malloc(4); // use stackptr temporarily
  heapptr = stackptr; // save value of heap pointer
  free(stackptr); // free up the memory again (sets stackptr to 0)
  stackptr = (uint8_t *)(SP); // save value of stack pointer
  return heapptr?(stackptr-heapptr):0;
}

