#include <stddef.h>
#include <stdbool.h>

struct ringbuffer {
  void *buf; /* location of the ringbuffer in memory */
  size_t size; /* size of the ringbuffer (of the data, not of the virtual memory area) */
  void *start; /* exclusive end and inclusive start of the current data window (with size `size`) */
};

int ringbuffer_init(struct ringbuffer *b, size_t size, bool roundup_ok);
int ringbuffer_destroy(struct ringbuffer *b);
void ringbuffer_bump(struct ringbuffer *b, size_t bumpsize);
void ringbuffer_bump_neg(struct ringbuffer *b, size_t bumpsize);
