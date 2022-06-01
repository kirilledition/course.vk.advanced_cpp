#include <gtest/gtest.h>

#include "main.hpp"

class AllocatorTest : public ::testing::Test {};

TEST_F(AllocatorTest, testMakeAllocator) {
  Allocator allocator;
  size_t maxSize = 10;
  allocator.makeAllocator(maxSize);
  EXPECT_NE(allocator.startPtr, nullptr);
  EXPECT_EQ(allocator.capacity, maxSize);
};

TEST_F(AllocatorTest, testRepeatedMakeAllocator) {
  Allocator allocator;
  size_t maxSize = 10;
  allocator.makeAllocator(maxSize);
  void *firstPrt = reinterpret_cast<void *>(allocator.startPtr);
  allocator.makeAllocator(maxSize);
  void *secondPrt = reinterpret_cast<void *>(allocator.startPtr);
  EXPECT_NE(secondPrt, firstPrt);
};

TEST_F(AllocatorTest, testAllocation) {
  Allocator allocator;
  size_t maxSize = 10;
  size_t allocationSize = 3;
  allocator.makeAllocator(maxSize);
  char *newMemoryPtr = allocator.alloc(allocationSize);
  EXPECT_EQ(newMemoryPtr, allocator.startPtr);
  EXPECT_EQ(allocator.offset, allocationSize);
};

TEST_F(AllocatorTest, testRepeatedAllocation) {
  Allocator allocator;
  size_t maxSize = 10;
  size_t allocationSize = 3;
  allocator.makeAllocator(maxSize);
  allocator.alloc(allocationSize);
  char *newMemoryPtr = allocator.alloc(allocationSize);
  EXPECT_EQ(newMemoryPtr, allocator.startPtr + allocationSize);
  EXPECT_EQ(allocator.offset, allocationSize * 2);
};

TEST_F(AllocatorTest, testOutOfBoundAllocation) {
  Allocator allocator;
  size_t maxSize = 10;
  size_t allocationSize = 11;
  allocator.makeAllocator(maxSize);
  char *newMemoryPtr = allocator.alloc(allocationSize);
  EXPECT_EQ(newMemoryPtr, nullptr);
};

TEST_F(AllocatorTest, testReset) {
  Allocator allocator;
  size_t maxSize = 10;
  size_t allocationSize = 3;
  allocator.makeAllocator(maxSize);
  allocator.alloc(allocationSize);
  allocator.reset();
  EXPECT_EQ(allocator.offset, size_t(0));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
};
