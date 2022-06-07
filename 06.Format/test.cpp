#include <gtest/gtest.h>

#include <cassert>

#include "main.cpp"

class FormatTest : public ::testing::Test {};

TEST_F(FormatTest, testFormatting) {
  auto text = format("{1}*{1} = {0}", 4, "two");
  EXPECT_EQ(text, "two*two = 4");

  text = format("I know {1} and {0}", "c++", "python");
  EXPECT_EQ(text, "I know python and c++");

  text = format("{0}+{1} = 1", "zero", "one", "two");
  EXPECT_EQ(text, "zero+one = 1");

  text = format("pure string");
  EXPECT_EQ(text, "pure string");
};

TEST_F(FormatTest, testInvalidString) {
  auto text = format("");
  try {
    text = format("{}");
  } catch (std::runtime_error& e) {
    string err = e.what();
    EXPECT_EQ(err, "invalid symbol in string: }");
  }
  try {
    text = format("invalid {string}", "arg");
  } catch (std::runtime_error& e) {
    string err = e.what();
    EXPECT_EQ(err, "invalid symbol in string: s");
  }
  try {
    text = format("bad}str");
  } catch (std::runtime_error& e) {
    string err = e.what();
    EXPECT_EQ(err, "invalid symbol in string: }");
  }
  try {
    text = format("bad{}str");
  } catch (std::runtime_error& e) {
    string err = e.what();
    EXPECT_EQ(err, "invalid symbol in string: }");
  }
};

TEST_F(FormatTest, testArgNum) {
  try {
    auto text = format("{0}+{1}+{3} = 4", "zero", "one", "three");
  } catch (std::runtime_error& e) {
    string err = e.what();
    EXPECT_EQ(err, "invalid template: argument number too large");
  }
};

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
};
