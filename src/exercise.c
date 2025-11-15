#include <stdlib.h>
#include <string.h>

void swap(void *vp1, void *vp2, size_t size) {
  void *tmp = malloc(size);
  if (tmp == NULL) return;  
  memcpy(tmp, vp1, size);
  memcpy(vp1, vp2, size);
  memcpy(vp2, tmp, size);
  free(tmp);
}