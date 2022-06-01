#include <gtest/gtest.h>

#include <iostream>

#include "main.cpp"

class BigIntTest : public ::testing::Test {};

TEST_F(BigIntTest, testComparators) {
  const BigInt a1("1000000000");
  const BigInt a2 = 2000000000;
  BigInt a3 = a2;

  EXPECT_TRUE(a3 == a2);
  EXPECT_TRUE(a1 < a2);
  EXPECT_TRUE(a3 <= a2);
  EXPECT_TRUE(a2 <= a3);
};

TEST_F(BigIntTest, testLevel) {
  EXPECT_EQ(BigInt(999) + BigInt(2), BigInt(1001));
  EXPECT_EQ(BigInt(999) + BigInt(999), BigInt(1998));
  EXPECT_EQ(BigInt(2000) - BigInt(1001), BigInt(999));
  EXPECT_EQ(BigInt(1000) - BigInt(1), BigInt(999));
};

TEST_F(BigIntTest, testSigns) {
  EXPECT_EQ(BigInt(-2) + BigInt(-3), BigInt(-5));
  EXPECT_EQ(BigInt(9) + BigInt(-2), BigInt(7));
  EXPECT_EQ(BigInt(9) - BigInt(-2), BigInt(11));
  EXPECT_EQ(BigInt(-2) - BigInt(9), BigInt(-11));
  EXPECT_EQ(BigInt(-2) - BigInt(-9), BigInt(7));
  EXPECT_EQ(BigInt(9) - BigInt(-20), BigInt(29));
  EXPECT_EQ(BigInt(2020) - BigInt(2000), BigInt(20));
};

TEST_F(BigIntTest, testBuffer) {
  BigInt a(1400000000);
  a = a + 999;

  EXPECT_EQ(a - BigInt(1001), BigInt(1399999998));
};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
};
