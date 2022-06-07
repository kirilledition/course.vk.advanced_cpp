#include "main.hpp"

#include <iostream>

void Allocator::makeAllocator(size_t maxSize) {
  if(maxSize < 1) throw std::invalid_argument("argument must be greater then 0");
  if (!startPtr) {
    delete[] startPtr;
  }
  startPtr = new char[maxSize];
  capacity = maxSize;
  offset = 0;
};

char *Allocator::alloc(size_t size) {
  if(size < 1) throw std::invalid_argument("argument must be greater then 0");
  if (startPtr == nullptr) {
    return nullptr;
  }
  if (size > capacity - offset) {
    return nullptr;
  }
  char *ptr = startPtr + offset;
  offset += size;
  return ptr;
};

void Allocator::reset() { offset = 0; }

Allocator::~Allocator() { delete[] startPtr; };
