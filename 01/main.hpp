#include <stddef.h>

class Allocator {
 public:
  char *startPtr = nullptr;
  size_t capacity;
  size_t offset;

  void makeAllocator(size_t maxSize) {
    if (!startPtr) {
      delete[] startPtr;
    }
    startPtr = new char[maxSize];
    capacity = maxSize;
    offset = 0;
  };

  char *alloc(size_t size) {
    if (size > capacity - offset) {
      return nullptr;
    }
    char *ptr = startPtr + offset;
    offset += size;
    return ptr;
  };

  void reset() { offset = 0; }

  ~Allocator() { delete[] startPtr; };
};
