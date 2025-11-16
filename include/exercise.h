#ifndef EXERCISE_H
#define EXERCISE_H

#ifdef __cplusplus
extern "C" {
  #endif
  
  #include "snekstack.h"
  
  void stack_push_multiple_types(snek_stack_t *s);
  void scary_double_push(snek_stack_t *s);
  
  #ifdef __cplusplus
}
#endif

#endif // EXERCISE_H