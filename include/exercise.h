#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  size_t length;
  char buffer[64];
} TextBuffer;

int smart_append(TextBuffer* dest, const char* src);

#ifdef __cplusplus
}
#endif