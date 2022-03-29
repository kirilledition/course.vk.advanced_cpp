#include "main.hpp"

#include <iostream>

int main() {
  Allocator allocator;
  allocator.makeAllocator(10);
  allocator.alloc(3);
  allocator.alloc(11);
  allocator.reset();
  allocator.alloc(3);
}