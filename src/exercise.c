#include "snekstack.h"
#include "stdlib.h"
#include <string.h>

void scary_double_push(snek_stack_t *s) {
  stack_push(s, (void*)1337);

  int *ptr = malloc(sizeof(int));
  *ptr = 1024;
  stack_push(s, ptr);
}


void stack_push_multiple_types(snek_stack_t *s) {
  float *flt_ptr = malloc(sizeof(float));
  *flt_ptr = 3.14;
  stack_push(s, flt_ptr);
  
  char *string_on_stack = "Sneklang is blazingly slow!";
  size_t len = strlen(string_on_stack);
  float *string_on_heap = malloc(len);
  strcpy(string_on_heap, string_on_stack);
  stack_push(s, string_on_heap);
}
