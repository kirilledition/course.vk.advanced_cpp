#include <gtest/gtest.h>

#include "main.cpp"

class ParserTest : public ::testing::Test {};

TEST_F(ParserTest, counterCallbackTest) {
  TokenParser parser;
  int n_digits = 0;
  int n_strings = 0;
  std::string text =
      "string 12345 123 123nodigit 987654321 no123digit456 54321";

  parser.SetDigitTokenCallback([&n_digits](const std::string&) { n_digits++; });
  parser.SetStringTokenCallback(
      [&n_strings](const std::string&) { n_strings++; });
  parser.Parse(text);

  EXPECT_EQ(n_digits, 4);
  EXPECT_EQ(n_strings, 3);
};

TEST_F(ParserTest, multipleDelimitersTest) {
  TokenParser parser;
  int n_digits = 0;
  int n_strings = 0;
  std::string text =
      "string \t\n \n 12345 123 123nodigit \n \t\t\t 987654321 123456n 54321";

  parser.SetDigitTokenCallback([&n_digits](const std::string&) { n_digits++; });
  parser.SetStringTokenCallback(
      [&n_strings](const std::string&) { n_strings++; });
  parser.Parse(text);

  EXPECT_EQ(n_digits, 4);
  EXPECT_EQ(n_strings, 3);
};

TEST_F(ParserTest, trailingDelimitersTest) {
  TokenParser parser;
  int n_digits = 0;
  int n_strings = 0;
  std::string text = "string 12345 123nodigit\n";

  parser.SetDigitTokenCallback([&n_digits](const std::string&) { n_digits++; });
  parser.SetStringTokenCallback(
      [&n_strings](const std::string&) { n_strings++; });
  parser.Parse(text);

  EXPECT_EQ(n_digits, 1);
  EXPECT_EQ(n_strings, 2);
};

TEST_F(ParserTest, defaultCallbacksTest) {
  TokenParser parser;
  bool started = false;
  bool ended = false;
  int n_digits = 0;
  int n_strings = 0;
  std::string text = "string 12345 123nodigit\n\t\n \n \n \n";

  parser.SetStartCallback([&started]() { started = true; });
  parser.SetDigitTokenCallback([&n_digits](const std::string&) { n_digits++; });
  parser.SetStringTokenCallback(
      [&n_strings](const std::string&) { n_strings++; });
  parser.SetEndCallback([&ended]() { ended = true; });
  parser.Parse(text);

  EXPECT_EQ(n_strings, 2);
  EXPECT_EQ(n_digits, 1);
  EXPECT_TRUE(started);
  EXPECT_TRUE(ended);
};

TEST_F(ParserTest, Test) {
  TokenParser parser;
  bool started = false;
  bool ended = false;
  int n_digits = 0;
  int n_strings = 0;
  std::string text = "string 12345 123nodigit\n\t\n \n \n \n";

  parser.SetStartCallback([&started]() { started = true; });
  parser.SetDigitTokenCallback([&n_digits](const std::string&) { n_digits++; });
  parser.SetStringTokenCallback(
      [&n_strings](const std::string&) { n_strings++; });
  parser.SetEndCallback([&ended]() { ended = true; });
  parser.Parse(text);

  EXPECT_EQ(n_strings, 2);
  EXPECT_EQ(n_digits, 1);
  ASSERT_TRUE(started);
  ASSERT_TRUE(ended);
};

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
};
