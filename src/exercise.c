#include "exercise.h"

void snek_zero_out(void *ptr, snek_object_kind_t kind) 
{
  switch (kind)
  {
  case INTEGER:
    ((snek_int_t*)ptr)->value = 0;
    break;
  case FLOAT:
    ((snek_float_t*)ptr)->value = 0.0;
    break;
  case BOOL:
    ((snek_bool_t*)ptr)->value = 0;
    break;
  default:
    break;
  }
}
 