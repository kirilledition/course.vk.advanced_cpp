#include <gtest/gtest.h>

#include <iostream>

#include "main.cpp"

struct A {
  int i = 77;
};

int get_i(const A& a) { return a.i; }

class PoolTest : public ::testing::Test {};

TEST_F(PoolTest, makePoolTest) {
  size_t n_threads = 3;
  ThreadPool pool(n_threads);
  EXPECT_EQ(pool.threads_.size(), n_threads);
};

TEST_F(PoolTest, simpleTaskTest) {
  ThreadPool pool(3);
  A a;

  auto task = pool.exec(get_i, a);

  EXPECT_EQ(task.get(), 77);
};

TEST_F(PoolTest, closureTaskTest) {
  ThreadPool pool(3);
  auto task = pool.exec([]() { return 1; });
  EXPECT_EQ(task.get(), 1);
};


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
};