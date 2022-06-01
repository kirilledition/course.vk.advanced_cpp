#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
using std::string;

class Format {
  std::stringstream arg;
  std::vector<string> args;
  size_t cnt_args;
  string str;

 public:
  Format(const char* str) : cnt_args(0), str(str) {}
  size_t getNum(string::iterator& it, const string::iterator& end) {
    size_t num = 0;
    if (!std::isdigit(*it)) {
      string error("invalid symbol in string: ");
      throw std::runtime_error(error + *it);
    }
    while ((it != end) && (std::isdigit(*it))) {
      num = num * 10 + (*it - '0');
      ++it;
    }
    if (*it != '}') {
      string error("invalid symbol in string: ");
      throw std::runtime_error(error + *it);
    }
    return num;
  }
  template <typename T>
  void addArgs(T&& arg) {
    ++cnt_args;
    this->arg << arg;  // проверяем, можно ли записать в поток
    this->args.push_back(this->arg.str());
    this->arg.str(string());  // очищаем поток
  }
  template <typename T, typename... Args>
  void addArgs(T&& arg, Args&&... args) {
    ++cnt_args;
    this->arg << arg;
    this->args.push_back(this->arg.str());
    this->arg.str(string());
    addArgs(std::forward<Args>(args)...);
  }
  string getRes() {
    string res;
    for (auto it = str.begin(); it != str.end(); ++it) {
      if (*it == '{') {
        ++it;
        size_t num = getNum(it, str.end());
        if (num >= cnt_args) {
          throw std::runtime_error("invalid template: argument number too large");
        }
        res += args[num];
      } else if (*it == '}') {
        throw std::runtime_error("invalid symbol in string: }");
      } else {
        res += *it;
      }
    }
    return res;
  }
};

template <typename... Args>
string format(const char* str, Args&&... args) {
  Format f(str);
  f.addArgs(std::forward<Args>(args)...);
  return f.getRes();
}

string format(const char* str) {
  Format f(str);
  return f.getRes();
}
