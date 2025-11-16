#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>

typedef struct SnekStack {
  size_t count;
  size_t capacity;
  void **data;
} snek_stack_t;

snek_stack_t *stack_new(size_t capacity);
void stack_push(snek_stack_t *stack, void *obj);
void *stack_pop(snek_stack_t *stack);
void stack_free(snek_stack_t *stack);

#ifdef __cplusplus
}
#endif