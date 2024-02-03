#include "big_integer.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

big_integer::big_integer() = default;

big_integer::big_integer(const big_integer& other) = default;

big_integer::big_integer(int a) : big_integer(static_cast<long long>(a)) {}

big_integer::big_integer(unsigned int a) : big_integer(static_cast<unsigned long long>(a)) {}

big_integer::big_integer(long a) : big_integer(static_cast<long long>(a)) {}

big_integer::big_integer(unsigned long a) : big_integer(static_cast<unsigned long long>(a)) {}

big_integer::big_integer(long long a)
    : big_integer(static_cast<unsigned long long>((a > 0 ? a : (a == std::numeric_limits<int64_t>::min() ? a : -a)))) {
  sign = false;
  if (a < 0) {
    if (a == std::numeric_limits<int64_t>::min()) {
      sign = true;
    } else {
      negate();
    }
  }
  shrink();
}

big_integer::big_integer(unsigned long long a) : sign(false) {
  number = {static_cast<uint32_t>((a & MAX_VALUE)), static_cast<uint32_t>(a >> degree_radix)};
  shrink();
}

big_integer::big_integer(const std::string& str) : big_integer() {
  if (str.empty() || str.size() - (str[0] == '-') == 0) {
    throw std::invalid_argument("didn't find digits");
  }
  if (!std::all_of(str.begin() + (str[0] == '-'), str.end(), ::isdigit)) {
    throw std::invalid_argument("char number is not digit");
  }
  size_t i = (str[0] == '-');
  for (; i + 9 <= str.size(); i += 9) {
    *this *= tens_in_radix;
    *this += std::stoi(str.substr(i, 9));
  }
  [[maybe_unused]] size_t ost = str.size() - i;
  if (ost > 0) {
    *this *= static_cast<uint32_t>(std::pow(10, ost));
    *this += std::stoi(str.substr(str.size() - ost, ost));
  }
  if (str[0] == '-') {
    negate();
  }
}

big_integer::~big_integer() = default;

big_integer& big_integer::operator=(const big_integer& other) {
  if (this != &other) {
    big_integer(other).swap(*this);
  }
  return *this;
}

big_integer& big_integer::operator+=(const int32_t rhs) {
  reserve(size() + 2);
  int32_t carry_flag = rhs;
  for (size_t i = 0; i != size(); ++i) {
    uint64_t res = static_cast<uint64_t>(number[i]) + carry_flag;
    carry_flag = (res > MAX_VALUE);
    number[i] = res;
  }
  sign = getSign();
  shrink();
  return *this;
}

big_integer& big_integer::operator+=(const big_integer& rhs) {
  size_t new_size = std::max(size(), rhs.size());
  reserve(new_size + 2);
  bool carry_flag = 0;
  for (size_t i = 0; i != size(); ++i) {
    uint64_t res =
        static_cast<uint64_t>(number[i]) + carry_flag + (i < rhs.number.size() ? rhs.number[i] : rhs.neutral_value());
    carry_flag = (res > MAX_VALUE);
    number[i] = res;
  }
  sign = getSign();
  shrink();
  return *this;
}

big_integer& big_integer::operator-=(const int32_t rhs) {
  return *this += -rhs;
}

big_integer& big_integer::operator-=(const big_integer& rhs) {
  size_t new_size = std::max(size(), rhs.size());
  reserve(new_size + 2);
  bool carry_flag = 0;
  for (size_t i = 0; i != size(); ++i) {
    int64_t res =
        static_cast<int64_t>(number[i]) - carry_flag - (i < rhs.number.size() ? rhs.number[i] : rhs.neutral_value());
    carry_flag = (res < 0);
    number[i] = (res < 0 ? res + MAX_VALUE + 1 : res);
  }
  sign = getSign();
  shrink();
  return *this;
}

big_integer& big_integer::operator*=(const int32_t rhs) {
  if (rhs == 0 || compareToZero() == 0) {
    return *this = 0;
  }
  uint64_t res, carry = 0, mul = (rhs < 0 ? -static_cast<int64_t>(rhs) : rhs);
  reserve(size() + 1);
  for (size_t i = 0; i != size(); ++i) {
    res = static_cast<uint64_t>(number[i]) * mul + carry;
    carry = res >> degree_radix;
    number[i] = res & MAX_VALUE;
  }
  shrink();
  if (rhs < 0) {
    negate();
  }
  return *this;
}

big_integer& big_integer::operator*=(const big_integer& rhs) {
  if (rhs.compareToZero() == 0) {
    return *this = 0;
  }
  bool signRes = false;
  if (sign) {
    negate();
    signRes = !signRes;
  }
  uint64_t res, carry;
  size_t i = size();
  uint32_t mul;
  reserve(size() + rhs.size() + 2);
  for (; i != 0; --i) {
    carry = 0;
    mul = number[i - 1];
    for (size_t j = 0; i + j < size() + 1; ++j) {
      res = (j ? number[i + j - 1] : 0) +
            static_cast<uint64_t>(j < rhs.size() ? rhs.number[j] : rhs.neutral_value()) * mul + carry;
      carry = res >> degree_radix;
      number[i + j - 1] = res - (carry << degree_radix);
    }
  }
  sign = getSign();
  shrink();
  if (signRes) {
    negate();
  }
  return *this;
}

big_integer& big_integer::operator/=(const big_integer& rhs) {
  modDivide(rhs);
  return *this;
}

big_integer& big_integer::operator%=(const big_integer& rhs) {
  modDivide(rhs).swap(*this);
  return *this;
}

big_integer& big_integer::operator&=(const big_integer& rhs) {
  reserve(std::max(size(), rhs.size()) + 1);
  for (size_t i = 0; i != size(); ++i) {
    number[i] = number[i] & (i < rhs.size() ? rhs.number[i] : rhs.neutral_value());
  }
  sign = getSign();
  shrink();
  return *this;
}

big_integer& big_integer::operator|=(const big_integer& rhs) {
  reserve(std::max(size(), rhs.size()) + 1);
  for (size_t i = 0; i != size(); ++i) {
    number[i] = number[i] | (i < rhs.size() ? rhs.number[i] : rhs.neutral_value());
  }
  sign = getSign();
  shrink();
  return *this;
}

big_integer& big_integer::operator^=(const big_integer& rhs) {
  reserve(std::max(size(), rhs.size()) + 1);
  for (size_t i = 0; i != size(); ++i) {
    number[i] = number[i] ^ (i < rhs.size() ? rhs.number[i] : rhs.neutral_value());
  }
  sign = getSign();
  shrink();
  return *this;
}

big_integer& big_integer::operator<<=(int rhs) {
  if (rhs < 0) {
    return *this >>= -rhs;
  }
  size_t dif_block = rhs / degree_radix;
  rhs %= degree_radix;
  reserve(size() + dif_block);
  for (size_t i = size(); i != 0; --i) {
    number[i - 1] = (i >= dif_block + 1 ? number[i - 1 - dif_block] : 0);
  }
  if (rhs) {
    reserve(size() + 1);
    for (size_t i = size(); i != 0; --i) {
      number[i - 1] = (i >= 2 ? (number[i - 2]) >> (degree_radix - rhs) : 0) + (i >= 1 ? (number[i - 1] << rhs) : 0);
    }
  }
  shrink();
  return *this;
}

big_integer& big_integer::operator>>=(int rhs) {
  if (rhs < 0) {
    return *this <<= -rhs;
  }
  size_t dif_block = rhs / degree_radix;
  rhs %= degree_radix;
  size_t end = size();
  reserve(size() + dif_block + 1);
  for (size_t i = 0; i != end; ++i) {
    number[i] = number[i + dif_block];
  }
  if (rhs) {
    for (size_t i = 0; i != end; ++i) {
      uint32_t big_ost = (number[i + 1] << (degree_radix - rhs)) >> (degree_radix - rhs);
      uint32_t small_ost = (number[i] >> rhs);
      number[i] = small_ost + (big_ost << (degree_radix - rhs));
    }
  }
  shrink();
  return *this;
}

big_integer big_integer::operator+() const {
  return big_integer(*this);
}

big_integer big_integer::operator-() const {
  if ((*this).compareToZero() == 0) {
    return *this;
  }
  big_integer tmp(~*this);
  tmp += 1;
  return tmp;
}

big_integer big_integer::operator~() const {
  if ((*this).compareToZero() == 0) {
    return *this;
  }
  big_integer tmp(*this);
  for (size_t i = 0; i != tmp.number.size(); ++i) {
    tmp.number[i] = ~tmp.number[i];
  }
  tmp.sign = !tmp.sign;
  return tmp;
}

big_integer& big_integer::operator++() {
  return *this += 1;
}

big_integer big_integer::operator++(int) {
  big_integer res(*this);
  ++*this;
  return res;
}

big_integer& big_integer::operator--() {
  return *this -= 1;
}

big_integer big_integer::operator--(int) {
  big_integer res(*this);
  --*this;
  return res;
}

big_integer operator+(const big_integer& a, const big_integer& b) {
  return big_integer(a) += b;
}

big_integer operator-(const big_integer& a, const big_integer& b) {
  return big_integer(a) -= b;
}

big_integer operator*(const big_integer& a, const big_integer& b) {
  return big_integer(a) *= b;
}

big_integer operator/(const big_integer& a, const big_integer& b) {
  return big_integer(a) /= b;
}

big_integer operator%(const big_integer& a, const big_integer& b) {
  return big_integer(a) %= b;
}

big_integer operator&(const big_integer& a, const big_integer& b) {
  return big_integer(a) &= b;
}

big_integer operator|(const big_integer& a, const big_integer& b) {
  return big_integer(a) |= b;
}

big_integer operator^(const big_integer& a, const big_integer& b) {
  return big_integer(a) ^= b;
}

big_integer operator<<(const big_integer& a, int b) {
  return big_integer(a) <<= b;
}

big_integer operator>>(const big_integer& a, int b) {
  return big_integer(a) >>= b;
}

bool operator==(const big_integer& a, const big_integer& b) {
  return a.sign == b.sign && a.size() == b.size() && std::equal(a.number.begin(), a.number.end(), b.number.begin());
}

bool operator!=(const big_integer& a, const big_integer& b) {
  return !(a == b);
}

bool operator<(const big_integer& a, const big_integer& b) {
  if (a.sign) {
    if (b.sign) {
      return -a >= -b;
    } else {
      return true;
    }
  } else if (b.sign) {
    return false;
  }
  if (a.size() != b.size()) {
    return a.size() < b.size();
  }
  for (size_t i = b.size(); i != 0; --i) {
    if (a.number[i - 1] < b.number[i - 1]) {
      return true;
    } else if (a.number[i - 1] > b.number[i - 1]) {
      return false;
    }
  }
  return false;
}

bool operator>(const big_integer& a, const big_integer& b) {
  return (b < a);
}

bool operator<=(const big_integer& a, const big_integer& b) {
  return !(a > b);
}

bool operator>=(const big_integer& a, const big_integer& b) {
  return !(a < b);
}

std::string to_string(const big_integer& a) {
  if (a.compareToZero() == 0) {
    return "0";
  }
  big_integer copy1 = a, copy2;
  if (a.sign) {
    copy1 = -copy1;
  }
  std::string res, local_res;
  while (copy1.compareToZero()) {
    local_res = std::to_string(copy1.modDivide(a.tens_in_radix));
    std::reverse(local_res.begin(), local_res.end());
    if (copy1.compareToZero()) {
      local_res += std::string(9 - local_res.size(), '0');
    }
    res += local_res;
  }
  if (a.sign) {
    res += '-';
  }
  std::reverse(res.begin(), res.end());
  return res;
}

std::ostream& operator<<(std::ostream& out, const big_integer& a) {
  return out << to_string(a);
}

void big_integer::swap(big_integer& b) {
  std::swap(sign, b.sign);
  std::swap(number, b.number);
}

void big_integer::reserve(size_t a) {
  assert(a >= number.size());
  while (number.size() < a) {
    number.push_back(neutral_value());
  }
}

void big_integer::shrink() {
  while (!number.empty() && number.back() == neutral_value()) {
    number.pop_back();
  }
}

uint32_t big_integer::neutral_value() const {
  return (sign ? MAX_VALUE : 0);
}

size_t big_integer::size() const {
  return number.size();
}

void big_integer::negate() {
  if (*this == 0) {
    return;
  }
  for (size_t i = 0; i != number.size(); ++i) {
    number[i] = ~number[i];
  }
  sign = !sign;
  ++*this;
}

int32_t big_integer::modDivide(uint32_t rhs) {
  assert(rhs != 0);
  uint64_t carry = 0, result;
  for (size_t i = size(); i != 0; --i) {
    result = (carry << big_integer::degree_radix) + number[i - 1];
    carry = result % rhs;
    number[i - 1] = result / rhs;
  }
  shrink();
  return carry;
}

big_integer big_integer::modDivide(const big_integer& rhs) {
  assert(rhs != 0);
  if (rhs == -1) {
    negate();
    return 0;
  }
  bool signRes = false, signA = sign;
  if (sign) {
    negate();
    signRes = !signRes;
  }
  big_integer deliver(rhs);
  if (deliver.sign) {
    deliver.negate();
    signRes = !signRes;
  }
  if (*this < deliver) {
    if (signA) {
      negate();
    }
    big_integer res;
    res.swap(*this);
    return res;
  }
  big_integer res;
  size_t m = size() - deliver.size();
  size_t n = deliver.size();
  uint32_t coefNorm = (static_cast<uint64_t>(1) << degree_radix) / (deliver.number.back() + 1);
  big_integer coefNormBig = coefNorm;
  *this *= coefNormBig;
  deliver *= coefNormBig;
  res.reserve(size());
  for (size_t i = m + 1; i != 0; --i) {
    uint32_t q_ = ((static_cast<uint64_t>((i + n - 1 < size() ? number[i + n - 1] : 0)) << big_integer::degree_radix) +
                   (i + n - 2 < size() ? number[i + n - 2] : 0)) /
                  deliver.number.back();
    deliver <<= (big_integer::degree_radix * (i - 1));
    *this -= deliver * q_;
    while (compareToZero() == -1) {
      --q_;
      *this += deliver;
    }
    deliver >>= (big_integer::degree_radix * (i - 1));
    res.number[i - 1] = q_;
  }
  res.shrink();
  if (signRes) {
    res.negate();
  }
  modDivide(coefNorm);
  if (signA) {
    negate();
  }
  res.swap(*this);
  return res;
}

bool big_integer::getSign() const {
  return (number.empty() ? neutral_value() : number.back());
}

int16_t big_integer::compareToZero() const {
  if (sign) {
    return -1;
  }
  return (number.empty() ? 0 : 1);
}
