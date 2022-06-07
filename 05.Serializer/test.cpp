#include <gtest/gtest.h>

#include "main.cpp"

struct DataOneElem {
  uint64_t a;
  Error serialize(Serializer& source) { return source(a); }
  Error deserialize(Deserializer& source) { return source(a); }
};

struct DataMultipleElems {
  uint64_t a;
  bool b;
  uint64_t c;
  Error serialize(Serializer& source) { return source(a, b, c); }
  Error deserialize(Deserializer& source) { return source(a, b, c); }
};

struct DataWrongTypesElems {
  char a;
  char b;
  Error serialize(Serializer& source) { return source(a, b); }
  Error deserialize(Deserializer& source) { return source(a, b); }
};

class SerializerTest : public ::testing::Test {};

TEST_F(SerializerTest, testOneElem) {
  DataOneElem x{100};
  std::stringstream stream;
  Serializer serializer(stream);

  const Error err_ser = serializer.save(x);

  Deserializer deserializer(stream);
  DataOneElem y{0};

  const Error err_deser = deserializer.load(y);

  EXPECT_EQ(err_ser, Error::NoError);
  EXPECT_EQ(err_deser, Error::NoError);
  EXPECT_EQ(x.a, y.a);
};

TEST_F(SerializerTest, testMultipleElem) {
  DataMultipleElems x{1, true, 2};
  std::stringstream stream;
  Serializer serializer(stream);

  const Error err_ser = serializer.save(x);

  Deserializer deserializer(stream);
  DataMultipleElems y{0, false, 1};

  const Error err_deser = deserializer.load(y);

  EXPECT_EQ(err_ser, Error::NoError);
  EXPECT_EQ(err_deser, Error::NoError);
  EXPECT_EQ(x.a, y.a);
  EXPECT_EQ(x.b, y.b);
  EXPECT_EQ(x.c, y.c);
};

TEST_F(SerializerTest, testWrongDtypeS) {
  DataWrongTypesElems x{'a', 'b'};
  std::stringstream stream;
  Serializer serializer(stream);

  const Error err = serializer.save(x);

  EXPECT_EQ(err, Error::CorruptedArchive);
};

TEST_F(SerializerTest, testWrongDtypeD) {
  DataOneElem x{0};
  std::stringstream stream;
  stream << "zero";
  Deserializer deserializer(stream);

  const Error err = deserializer.load(x);

  EXPECT_EQ(err, Error::CorruptedArchive);
};

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
};
