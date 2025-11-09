#include <string.h>
#include "exercise.h"

int smart_append(TextBuffer* dest, const char* src) {
  if (dest == NULL || src == NULL) {
    return 1;
  }

  const size_t buffer_capacity = sizeof(dest->buffer); 
  const size_t src_len = strlen(src);
  const size_t current_dest_len = dest->length;

  if (current_dest_len >= buffer_capacity) {
      return 1;
  }

  size_t space_available = buffer_capacity - current_dest_len - 1;

  if (src_len > space_available) {
    if (space_available > 0) {
      strncat(dest->buffer, src, space_available);
      dest->length += space_available;
    }
    return 1;
  } else {
    // Enough space for the whole string.
    strcat(dest->buffer, src);
    dest->length += src_len;
    return 0; // Indicate success
  }
}