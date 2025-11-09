#include <stdio.h>
#include "exercise.h"


void format_object(snek_object_t obj, char *buffer) {
    switch (obj.kind)
    {
    case 0:
        sprintf(buffer, "int:%d", obj.data.v_int);
        break;
    case 1:        
        sprintf(buffer, "string:%s", obj.data.v_string);
        break;
    default:
        break;
    }
}


snek_object_t new_integer(int i) {
  return (snek_object_t){
    .kind = INTEGER,
    .data = {.v_int = i}
  };
}


snek_object_t new_string(char *str) {
  // NOTE: We will learn how to copy this data later.
  return (snek_object_t){
    .kind = STRING,
    .data = {.v_string = str}
  };
}