#pragma once

#include <stddef.h>

class Allocator {
 public:
  char *startPtr = nullptr;
  size_t capacity;
  size_t offset;

  void makeAllocator(size_t maxSize);

  char *alloc(size_t size);

  void reset();

  ~Allocator();
};
