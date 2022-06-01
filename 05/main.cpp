#include <algorithm>
#include <cstddef>
#include <iosfwd>
#include <iostream>

class BigInt {
 public:
  BigInt(const std::string &str) {
      size_t len;
    if(str.size() == 0) {
        len = 1;
        data = new char[len+1];
        data[0] = '0';
        data[1] = '\0';
    }
    else {
        bool minus = str[0] == '-';
        bool plus = str[0] == '+';
        size_t j = minus + plus;
        size_t str_size = str.size();
        
        /* Drop meaningless zeros */
        for(; j < str_size - 1 and str[j] == '0'; j++);
        len = str.size() - j + minus;
        data = new char[len+1];
        char t;
        if(minus) data[0] = str[0];
        for(size_t i=minus; i<len; i++) {
            t = str[j + i - minus];
            if(!isdigit(t)) throw std::invalid_argument(std::string(1, t));
            else data[i] = t;
        };
    }
    data[len] = '\0';
}
;
  BigInt(int n) {
    sign = n >= 0;
    if (!sign) {
      n = -n;
    }
    data = new char[block_size];
    size = 0;
    buf_size = block_size;
    do {
      if (size == buf_size) {
        buf_size += block_size;
        char* tmp = new char[buf_size];
        std::copy(data, data + size, tmp);
        delete[] data;
        data = tmp;
      }
      data[size++] = char(n % 10 + '0');
      n /= 10;
    } while (n != 0);
    std::reverse(data, data + size);
  };

  BigInt(const BigInt& other) {
    buf_size = other.buf_size;
    size = other.size;
    data = new char[buf_size];
    sign = other.sign;
    std::copy(other.data, other.data + size, data);
  };
  ~BigInt() { delete[] data; };
  BigInt& operator=(const BigInt& other) {
    if (this == &other) {
      return *this;
    }
    buf_size = other.buf_size;
    size = other.size;
    sign = other.sign;
    auto ptr = new char[buf_size];
    delete[] data;
    data = ptr;
    std::copy(other.data, other.data + size, data);
    return *this;
  };
  BigInt(BigInt&& other) {
    size = other.size;
    data = other.data;
    sign = other.sign;
    buf_size = other.buf_size;
    other.data = nullptr;
    other.size = other.buf_size = 0;
  };
  BigInt& operator=(BigInt&& other) {
    if (this == &other) {
      return *this;
    }
    buf_size = other.buf_size;
    size = other.size;
    sign = other.sign;
    delete[] data;
    data = other.data;
    other.data = nullptr;
    other.size = other.buf_size = 0;
    return *this;
  };
  friend std::ostream& operator<<(std::ostream& os, const BigInt& obj) {
    if (!obj.sign) {
      os << '-';
    }
    for (size_t i = 0; i < obj.size; ++i) {
      os << obj.data[i];
    }
    return os;
  };
  BigInt operator-() const {
    BigInt res = *this;
    res.sign = !sign;
    return res;
  };
  BigInt operator+(const BigInt& other) const {
    if (sign == other.sign) {
      return add(*this, other);
    } else {
      if (sign) {
        return sub(*this, other);
      } else {
        return sub(other, *this);
      }
    }
    return *this;
  };
  BigInt operator-(const BigInt& other) const {
    if (sign == other.sign) {
      return sub(*this, other);
    } else {
      return add(*this, other);
    }
  };
  BigInt operator+(int n) const { return *this + BigInt(n); };
  BigInt operator-(int n) const { return *this - BigInt(n); };
  bool operator==(const BigInt& other) const {
    if (this == &other) return true;
    if (sign != other.sign || size != other.size) {
      return false;
    }
    for (size_t i = 0; i < size; ++i) {
      if (data[i] != other.data[i]) {
        return false;
      }
    }
    return true;
  };
  bool operator!=(const BigInt& other) const { return !(*this == other); };
  bool operator<(const BigInt& other) const {
    if (sign != other.sign) {
      return sign < other.sign;
    }
    if (size != other.size) {
      return size < other.size;
    }
    for (size_t i = 0; i < size; ++i) {
      if (data[i] > other.data[i]) {
        return false;
      } else if (data[i] < other.data[i]) {
        return true;
      }
    }
    return false;
  };
  bool operator<=(const BigInt& other) const {
    return (*this < other) || (*this == other);
  };
  bool operator>(const BigInt& other) const { return !(*this < other); };
  bool operator>=(const BigInt& other) const {
    return !(*this < other) && (*this != other);
  };
  BigInt(size_t size, char* data, bool sign, size_t buf_size) {
    this->size = size;
    this->data = data;
    this->sign = sign;
    this->buf_size = buf_size;
    this->data = new char[buf_size];
    std::copy(data, data + size, this->data);
  };
  BigInt add(const BigInt& obj1, const BigInt& obj2) const {
    size_t min_size = std::min(obj1.size, obj2.size);
    size_t max_size = std::max(obj1.size, obj2.size);
    const char* max_data = size == max_size ? obj1.data : obj2.data;
    unsigned n = 0;
    size_t new_buf_size = std::max(obj1.buf_size, obj2.buf_size);
    char* res = new char[new_buf_size];
    for (size_t i = 0; i < min_size; ++i) {
      unsigned sum = obj1.data[obj1.size - 1 - i] - '0' +
                     obj2.data[obj2.size - 1 - i] - '0';
      res[max_size - 1 - i] = char(sum % 10 + n + '0');
      n = sum / 10;
    }
    for (size_t i = min_size; i < max_size; ++i) {
      if (max_data[max_size - 1 - i] == '9' && n == 1) {
        res[max_size - 1 - i] = '0';
      } else {
        res[max_size - 1 - i] = max_data[max_size - 1 - i] + n;
        n = 0;
      }
    }
    if (n > 0) {
      if (max_size == new_buf_size) {
        new_buf_size += block_size;
        char* tmp = new char[new_buf_size];
        std::copy(res, res + max_size, tmp + 1);
        delete[] res;
        res = tmp;
      } else {
        std::copy(res, res + max_size, res + 1);
      }
      res[0] = '1';
      ++max_size;
    }
    auto sum = BigInt(max_size, res, obj1.sign, new_buf_size);
    delete[] res;
    return sum;
  };
  BigInt sub(const BigInt& obj1, const BigInt& obj2) const {
    bool new_sign = BigInt(obj1.size, obj1.data, true, obj1.buf_size) >
                    BigInt(obj2.size, obj2.data, true, obj2.buf_size);
    size_t min_size = std::min(obj1.size, obj2.size);
    size_t max_size = std::max(obj1.size, obj2.size);
    const char* max_data = size == max_size ? obj1.data : obj2.data;
    int n = 0;
    size_t new_buf_size = std::max(obj1.buf_size, obj2.buf_size);
    char* res = new char[new_buf_size];
    for (size_t i = 0; i < min_size; ++i) {
      int diff = 0;
      if (new_sign) {
        diff = obj1.data[obj1.size - 1 - i] - obj2.data[obj2.size - 1 - i] - n;
      } else {
        diff = obj2.data[obj2.size - 1 - i] - obj1.data[obj1.size - 1 - i] - n;
      }
      if (diff < 0) {
        diff += 10;
        n = 1;
      } else {
        n = 0;
      }
      res[max_size - 1 - i] = char(diff + '0');
    }
    for (size_t i = min_size; i < max_size; ++i) {
      if (max_data[max_size - 1 - i] == '0') {
        res[max_size - 1 - i] = '9';
        n = 1;
      } else {
        res[max_size - 1 - i] = max_data[max_size - 1 - i] - n;
        n = 0;
      }
    }
    size_t i = 0;
    while (res[i] == '0') {
      ++i;
      if (i == max_size) {
        break;
      }
    };
    max_size -= i;
    if (max_size == 0) {
      max_size = 1;
      new_sign = true;
    } else {
      if (i != 0) {
        for (size_t j = 0; j < max_size; ++j) {
          res[j] = res[j + i];
        }
      }
      new_sign = (new_sign == obj1.sign);
    }
    auto diff = BigInt(max_size, res, new_sign, new_buf_size);
    delete[] res;
    return diff;
  };
  size_t size;
  char* data;
  bool sign;
  size_t block_size = 10;
  size_t buf_size;
};
