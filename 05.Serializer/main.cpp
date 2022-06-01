#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

enum class Error { NoError, CorruptedArchive };

class Serializer {
  static constexpr char Separator = ' ';
  std::ostream& out_;

 public:
  explicit Serializer(std::ostream& out) : out_(out) {}

  template <class T>
  Error save(T& object) {
    return object.serialize(*this);
  }
  template <class... ArgsT>
  Error operator()(ArgsT&... args) {
    return process(args...);
  }
  template <class T>
  Error process(T& t) {
    return toStream(t);
  }
  template <class T, class... ArgsT>
  Error process(T& t, ArgsT&... args) {
    if (toStream(t) == Error::CorruptedArchive) return Error::CorruptedArchive;
    return process(args...);
  }
  Error toStream(bool& var) const {
    if (var)
      out_ << "true" << Separator;
    else
      out_ << "false" << Separator;

    return Error::NoError;
  }
  Error toStream(uint64_t& var) const {
    out_ << var << Separator;
    return Error::NoError;
  }
  template <class T>
  Error toStream(T& t) const {
    return Error::CorruptedArchive;
  }
};

class Deserializer {
  std::istream& in_;

 public:
  explicit Deserializer(std::istream& in) : in_(in) {}

  template <class T>
  Error load(T& object) {
    return object.deserialize(*this);
  }
  template <class... ArgsT>
  Error operator()(ArgsT&... args) {
    return process(args...);
  }
  template <class T>
  Error process(T& t) {
    return fromStream(t);
  }
  template <class T, class... ArgsT>
  Error process(T& t, ArgsT&... args) {
    if (fromStream(t) == Error::CorruptedArchive)
      return Error::CorruptedArchive;
    return process(args...);
  }
  Error fromStream(bool& var) {
    std::string text;
    in_ >> text;

    if (text == "true") {
      var = true;
      return Error::NoError;
    } else if (text == "false") {
      var = false;
      return Error::NoError;
    }
    return Error::CorruptedArchive;
  }

  Error fromStream(uint64_t& var) {
    std::string text;
    in_ >> text;

    if (!isdigit(text[0])) {
      return Error::CorruptedArchive;
    }

    try {
      var = std::stoull(text);
      return Error::NoError;
    } catch (std::exception& x) {
      return Error::CorruptedArchive;
    }
  }

  template <class T>
  Error fromStream(T& t) const {
    return Error::CorruptedArchive;
  }
};
