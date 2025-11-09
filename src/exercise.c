#include "exercise.h"

void concat_strings(char *str1, const char *str2) {
  char null_terminator_pos = 0;
  while(*(str1 + null_terminator_pos) != '\0') {
    null_terminator_pos++;
  }

  
  char null_terminator_pos_2 = 0;
  while(*(str2 + null_terminator_pos_2) != '\0') {
    *(str1 + null_terminator_pos + null_terminator_pos_2) = *(str2 + null_terminator_pos_2);
    null_terminator_pos_2++;
  }
}
