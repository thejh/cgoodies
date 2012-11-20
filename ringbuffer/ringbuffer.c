#include <sys/mman.h>
#include <errno.h>
#include <sys/user.h>
#include <ringbuffer.h>
#include <assert.h>

int ringbuffer_init(struct ringbuffer *b, size_t size, bool roundup_ok) {
  int err;
  
  if (size == 0 || size*2 < size || (size%PAGE_SIZE) != 0) {
    if (!roundup_ok) {
      errno = EINVAL;
      return -1;
    }
    size = ((size+PAGE_SIZE-1)/PAGE_SIZE)*PAGE_SIZE;
  }
  b->size = size;
  
  b->start = b->buf = mmap(NULL, size*2, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
  if (b->buf == NULL) return -1;
  
  if (remap_file_pages(b->buf+size, size, 0, size/PAGE_SIZE, 0)) goto unmap;
  
  return 0;
  
unmap:
  err = errno;
  munmap(b->buf, size*2);
  errno = err;
  return -1;
}

int ringbuffer_destroy(struct ringbuffer *b) {
  return munmap(b->buf, b->size*2);
}

void ringbuffer_bump(struct ringbuffer *b, size_t bumpsize) {
  b->start = b->buf + ((b->start - b->buf) + bumpsize)%b->size;
}

void ringbuffer_bump_neg(struct ringbuffer *b, size_t bumpsize) {
  assert(bumpsize <= b->size);
  b->start = b->start - bumpsize;
  if (b->start < b->buf) b->start += b->size;
}

void *ringbuffer_get_backread_ptr(struct ringbuffer *b) {
  return b->start + b->size;
}
