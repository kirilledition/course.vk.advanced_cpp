#include <gtest/gtest.h>

#include "main.cpp"

class AllocatorTest : public ::testing::Test {};

TEST_F(AllocatorTest, testInvalidArgument) {
  Allocator allocator;
  try {
    allocator.makeAllocator(0);
  } catch (std::invalid_argument &e) {
    EXPECT_EQ(e.what(), std::string("argument must be greater then 0"));
  } catch (...) {
    FAIL() << "Expected std::invalid_argument";
  }

  try {
    allocator.makeAllocator(1);
    allocator.alloc(0);
  } catch (std::invalid_argument &e) {
    EXPECT_EQ(e.what(), std::string("argument must be greater then 0"));
  } catch (...) {
    FAIL() << "Expected std::invalid_argument";
  }
};

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

TEST_F(AllocatorTest, testChain) {
  Allocator allocator;

  allocator.makeAllocator(50);
  char* p1 = allocator.alloc(20);
  char* p2 = allocator.alloc(30);
  allocator.alloc(1);
  allocator.reset();
  allocator.alloc(50);

  EXPECT_EQ(p2 - p1, 20);
  EXPECT_EQ(allocator.offset, size_t(50));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
};
