#include <gtest/gtest.h>

#include <iostream>

#include "main.cpp"

class MatrixTest : public ::testing::Test {};

TEST_F(MatrixTest, testMakeMatrix) {
  const size_t rows = 5;
  const size_t cols = 3;

  Matrix m(rows, cols);

  EXPECT_EQ(m.getRows(), rows);
  EXPECT_EQ(m.getColumns(), cols);
};

TEST_F(MatrixTest, testAssignValue) {
  const size_t rows = 5;
  const size_t cols = 3;

  Matrix m(rows, cols);

  m[1][2] = 5;

  EXPECT_EQ(m[1][2], int32_t(5));
};

TEST_F(MatrixTest, testMutiply) {
  const size_t rows = 5;
  const size_t cols = 3;

  Matrix m(rows, cols);

  m[1][2] = 5;

  m *= 2;

  EXPECT_EQ(m[1][2], int32_t(10));
  EXPECT_EQ(m[1][1], int32_t(0));
};

TEST_F(MatrixTest, testAdd) {
  const size_t rows = 5;
  const size_t cols = 3;

  Matrix m0(rows, cols);
  Matrix m1(rows, cols);

  m0[1][2] = 5;
  m1[1][2] = 5;
  m0[0][2] = 4;

  Matrix m2 = m0 + m1;

  EXPECT_EQ(m2[1][2], int32_t(10));
  EXPECT_EQ(m2[0][2], int32_t(4));
  EXPECT_EQ(m2[0][0], int32_t(0));
};

TEST_F(MatrixTest, testAssignError) {
  Matrix m(2, 2);
  try {
    m[0][2] = 5;
  } catch (std::out_of_range &e) {
    EXPECT_EQ(e.what(), std::string("[]"));
  } catch (...) {
    FAIL() << "Expected std::out_of_range";
  }
  try {
    m[2][0] = 5;
  } catch (std::out_of_range &e) {
    EXPECT_EQ(e.what(), std::string("[]"));
  } catch (...) {
    FAIL() << "Expected std::out_of_range";
  }
};

TEST_F(MatrixTest, testAddError) {
  Matrix m0(2, 2);
  Matrix m1(2, 3);
  try {
    Matrix m2 = m0 + m1;
  } catch (std::invalid_argument &e) {
    EXPECT_EQ(e.what(), std::string("+"));
  } catch (...) {
    FAIL() << "Expected std::invalid_argument";
  }
};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
};
