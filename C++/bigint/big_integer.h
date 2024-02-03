#pragma once

#include <iosfwd>
#include <limits>
#include <string>
#include <vector>

struct big_integer {
  big_integer();
  big_integer(const big_integer& other);
  big_integer(int a);
  big_integer(unsigned int a);
  big_integer(long int a);
  big_integer(long unsigned int a);
  big_integer(long long a);
  big_integer(unsigned long long a);
  explicit big_integer(const std::string& str);
  ~big_integer();

  big_integer& operator=(const big_integer& other);

  big_integer& operator+=(const big_integer& rhs);
  big_integer& operator+=(const int32_t rhs);
  big_integer& operator-=(const int32_t rhs);
  big_integer& operator-=(const big_integer& rhs);
  big_integer& operator*=(const int32_t rhs);
  big_integer& operator*=(const big_integer& rhs);
  big_integer& operator/=(const big_integer& rhs);
  big_integer& operator%=(const big_integer& rhs);

  big_integer& operator&=(const big_integer& rhs);
  big_integer& operator|=(const big_integer& rhs);
  big_integer& operator^=(const big_integer& rhs);

  big_integer& operator<<=(int rhs);
  big_integer& operator>>=(int rhs);

  big_integer operator+() const;
  big_integer operator-() const;
  big_integer operator~() const;

  big_integer& operator++();
  big_integer operator++(int);

  big_integer& operator--();
  big_integer operator--(int);

  friend bool operator==(const big_integer& a, const big_integer& b);
  friend bool operator!=(const big_integer& a, const big_integer& b);
  friend bool operator<(const big_integer& a, const big_integer& b);
  friend bool operator>(const big_integer& a, const big_integer& b);
  friend bool operator<=(const big_integer& a, const big_integer& b);
  friend bool operator>=(const big_integer& a, const big_integer& b);

  friend std::string to_string(const big_integer& a);

  void swap(big_integer& b);

  void reserve(size_t a);

  void shrink();

  void negate();

  size_t size() const;

private:
  int16_t compareToZero() const;

  bool getSign() const;

  uint32_t neutral_value() const;

  int32_t modDivide(uint32_t rhs);

  big_integer modDivide(const big_integer& b);

private:
  std::vector<uint32_t> number;
  bool sign{false};
  static const uint32_t degree_radix = 32;
  static const uint32_t MAX_VALUE = std::numeric_limits<uint32_t>::max();
  static const uint32_t tens_in_radix = 1000'000'000;
};

big_integer operator+(const big_integer& a, const big_integer& b);
big_integer operator-(const big_integer& a, const big_integer& b);
big_integer operator*(const big_integer& a, const big_integer& b);
big_integer operator/(const big_integer& a, const big_integer& b);
big_integer operator%(const big_integer& a, const big_integer& b);

big_integer operator&(const big_integer& a, const big_integer& b);
big_integer operator|(const big_integer& a, const big_integer& b);
big_integer operator^(const big_integer& a, const big_integer& b);

big_integer operator<<(const big_integer& a, int b);
big_integer operator>>(const big_integer& a, int b);

bool operator==(const big_integer& a, const big_integer& b);
bool operator!=(const big_integer& a, const big_integer& b);
bool operator<(const big_integer& a, const big_integer& b);
bool operator>(const big_integer& a, const big_integer& b);
bool operator<=(const big_integer& a, const big_integer& b);
bool operator>=(const big_integer& a, const big_integer& b);

std::string to_string(const big_integer& a);
std::ostream& operator<<(std::ostream& out, const big_integer& a);
