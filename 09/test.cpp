#include <gtest/gtest.h>

#include <array>
#include <iostream>
#include <string>

#include "main.cpp"

bool checkResult(const std::string &filename,
                 const std::vector<uint64_t> nums) {
  std::ifstream stream;
  uint64_t number;
  bool mistake = false;
  stream.open(filename);
  for (size_t i = 0; i < nums.size(); i++) {
    stream.read((char *)&number, sizeof(uint64_t));
    if (number != nums[i]) {
      mistake = true;
      break;
    }
  }
  stream.close();
  return !mistake;
}

class SorterTest : public ::testing::Test {};

TEST_F(SorterTest, test1) {
  Sorter sorter;
  std::ofstream stream;

  std::vector<uint64_t> in1 = {5, 3, 2, 1, 67, 32, 4, 8, 0, 12, 12, 45, 322};
  std::vector<uint64_t> out1 = {0, 1, 2, 3, 4, 5, 8, 12, 12, 32, 45, 67, 322};

  stream.open("in1", std::ios::binary);
  for (size_t i = 0; i < in1.size(); i++)
    stream.write((char *)&in1[i], sizeof(uint64_t));
  stream.close();

  sorter.sort("in1", "out1", 13, 5);
  EXPECT_TRUE(checkResult("out1", out1));
};

TEST_F(SorterTest, test2) {
  Sorter sorter;
  std::ofstream stream;

  std::vector<uint64_t> in2 = {134,  56, 78, 234, 6,  0, 9, 2, 2,  2,
                               4578, 4,  3,  2,   66, 8, 9, 0, 53, 230};
  std::vector<uint64_t> out2 = {0, 0, 2,  2,  2,  2,  3,   4,   6,   8,
                                9, 9, 53, 56, 66, 78, 134, 230, 234, 4578};

  stream.open("in2", std::ios::binary);
  for (size_t i = 0; i < in2.size(); i++)
    stream.write((char *)&in2[i], sizeof(uint64_t));
  stream.close();

  sorter.sort("in2", "out2", 20, 5);
  EXPECT_TRUE(checkResult("out2", out2));
};

TEST_F(SorterTest, test3) {
  Sorter sorter;
  std::ofstream stream;

  size_t max_size = 100000;

  std::vector<uint64_t> out3;
  for (size_t i = 0; i < max_size; i++) out3.push_back(i);

  std::vector<uint64_t> in3;
  for (size_t i = 0; i < max_size; i++) in3.push_back(max_size - 1 - out3[i]);

  stream.open("in3", std::ios::binary);
  for (size_t i = 0; i < in3.size(); i++)
    stream.write((char *)&in3[i], sizeof(uint64_t));
  stream.close();

  sorter.sort("in3", "out3", max_size, 100);

  EXPECT_TRUE(checkResult("out3", out3));
};

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
};
