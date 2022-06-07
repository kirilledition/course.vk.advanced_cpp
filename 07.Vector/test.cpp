#include <gtest/gtest.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "main.cpp"

template <class TT>
std::string print(const vector<TT>& vec) {
  std::stringstream ss;
  for (size_t i = 0; i < vec.size(); i++) ss << " " << vec[i];
  return ss.str();
}

class VectorTest : public ::testing::Test {};

TEST_F(VectorTest, testOne) {
  vector<int> vec = {1, 2, 3};
  auto s = print(vec);
  EXPECT_EQ(s, " 1 2 3");

  vec.resize(5, 11);
  s = print(vec);
  EXPECT_EQ(s, " 1 2 3 11 11");

  vec.pop_back();
  s = print(vec);
  EXPECT_EQ(s, " 1 2 3 11");
};

TEST_F(VectorTest, testTwo) {
  vector<char> vec = {'w', 'n'};
  auto s = print(vec);
  EXPECT_EQ(s, " w n");

  vec.push_back('z');
  vec.push_back('l');
  s = print(vec);
  EXPECT_EQ(s, " w n z l");

  vec.erase(vec.begin() + 1);
  s = print(vec);
  EXPECT_EQ(s, " w z l");
};

TEST_F(VectorTest, testThree) {
  vector<std::string> vec(4, "word");
  auto s = print(vec);
  EXPECT_EQ(s, " word word word word");

  vec.push_back("new");
  s = print(vec);
  EXPECT_EQ(s, " word word word word new");

  vec.resize(7, "empty");
  s = print(vec);
  EXPECT_EQ(s, " word word word word new empty empty");

  vec.clear();
  EXPECT_TRUE(vec.empty());
};

TEST_F(VectorTest, testFour) {
  int i = 5;
  int j = 6;
  Iterator<int> iter1(&i);
  Iterator<int> iter2(&j);
  swap(iter1, iter2);
  iter2 -= 6;
  iter2 = 6 + iter2;
  EXPECT_TRUE(iter2[1] == *(iter2 + 1));
};

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
};
