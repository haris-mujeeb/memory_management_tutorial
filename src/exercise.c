#include <string.h>
#include "exercise.h"

int smart_append(TextBuffer* dest, const char* src) {
  if (dest == NULL || src == NULL) {
    return 1; // Indicate error for NULL pointers
  }

  const size_t buffer_capacity = sizeof(dest->buffer); // Total size of the buffer array (64)
  const size_t src_len = strlen(src);
  const size_t current_dest_len = dest->length;

  // Calculate available space for new characters, leaving room for null terminator
  if (current_dest_len >= buffer_capacity) { // Defensive check for already full buffer
      return 1;
  }
  size_t space_available = buffer_capacity - current_dest_len - 1;

  if (src_len > space_available) {
    // Not enough space for the whole string. Append as much as possible.
    if (space_available > 0) {
      // Append 'space_available' characters from src.
      // strncat appends at most n characters and null-terminates.
      strncat(dest->buffer, src, space_available);
      dest->length += space_available; // Update length by the number of chars appended
    }
    // Even if some characters were appended, it's an overflow condition.
    return 1;
  } else {
    // Enough space for the whole string.
    strcat(dest->buffer, src); // Safe because we checked the total length
    dest->length += src_len;
    return 0; // Indicate success
  }
}