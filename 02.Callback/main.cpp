#include <string>

using DefaultCallback = std::function<void(void)>;
using TokenCallback = std::function<void(const std::string &)>;

class TokenParser {
 public:
  TokenParser() = default;

  // Устанавливаем callback-функцию перед стартом парсинга.
  void SetStartCallback(DefaultCallback startCB) { _startCB = startCB; };

  // Устанавливаем callback-функцию после окончания парсинга.
  void SetEndCallback(DefaultCallback endCB) { _endCB = endCB; };

  // Устанавливаем callback-функцию для обработки чисел.
  void SetDigitTokenCallback(TokenCallback digitCB) { _numCB = digitCB; };

  // Тут другие методы для установки callback-функций.
  void SetStringTokenCallback(TokenCallback stringCB) { _strCB = stringCB; };

  void Parse(const std::string &text) {
    std::string token;
    char c;
    auto iter = text.begin();
    bool digit = true;

    _startCB();

    while (iter != text.end()) {
      c = *iter;
      if (!isspace(c)) {
        token.push_back(c);
        if (!isdigit(c)) digit = false;
      } else
        processToken(token, digit);
      iter++;
    }
    processToken(token, digit);
    _endCB();
  }

  void processToken(std::string &token, bool &digit) {
    if (token.size() != 0) {
      digit ? _numCB(token) : _strCB(token);
      token.clear();
      digit = true;
    }
  }

  DefaultCallback _startCB = [] {};
  DefaultCallback _endCB = [] {};
  TokenCallback _strCB = [](const std::string &) {};
  TokenCallback _numCB = [](const std::string &) {};

};
