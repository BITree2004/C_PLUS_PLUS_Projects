#pragma once

#include "circular-buffer.h"
#include "element.h"
#include "fault-injection.h"

#include <gtest/gtest.h>

#include <bit>
#include <initializer_list>
#include <ostream>

template class circular_buffer<element>;
using container = circular_buffer<element>;

template <typename F, typename = std::enable_if_t<std::is_invocable_v<F, std::ostream&>>>
decltype(auto) operator<<(std::ostream& out, const F& f) {
  return f(out);
}

template <typename C, typename T>
void mass_push_back(C& c, std::initializer_list<T> elems) {
  for (const T& e : elems) {
    c.push_back(e);
  }
}

template <typename C, typename T>
void mass_push_front(C& c, std::initializer_list<T> elems) {
  for (const T& e : elems) {
    c.push_front(e);
  }
}

template <class Actual, class Expected>
void expect_eq(const Actual& actual, const Expected& expected) {
  fault_injection_disable dg;

  EXPECT_EQ(expected.size(), actual.size());

  EXPECT_TRUE(std::equal(expected.begin(), expected.end(), actual.begin(), actual.end())) << [&](std::ostream& out) {
    out << '{';

    bool add_comma = false;
    for (const auto& e : expected) {
      if (add_comma) {
        out << ", ";
      }
      out << e;
      add_comma = true;
    }

    out << "} != {";

    add_comma = false;
    for (const auto& e : actual) {
      if (add_comma) {
        out << ", ";
      }
      out << e;
      add_comma = true;
    }

    out << "}\n";
  };
}

template <class Actual, class T>
void expect_eq(const Actual& actual, const std::initializer_list<T>& expected) {
  return expect_eq<Actual, std::initializer_list<T>>(actual, expected);
}

template <typename C>
void expect_empty(const C& c) {
  EXPECT_TRUE(c.empty());
  EXPECT_EQ(0, c.size());
  EXPECT_TRUE(c.begin() == c.end());
}

template <class It>
class reverse_view {
public:
  template <class R>
  reverse_view(const R& r) noexcept : reverse_view(r.begin(), r.end(), r.size()) {}

  reverse_view(It begin, It end, size_t size) noexcept : base_begin(begin), base_end(end), base_size(size) {}

  auto begin() const noexcept {
    return std::make_reverse_iterator(base_end);
  }

  auto end() const noexcept {
    return std::make_reverse_iterator(base_begin);
  }

  size_t size() const noexcept {
    return base_size;
  }

private:
  It base_begin;
  It base_end;
  size_t base_size;
};

template <class R>
reverse_view(const R& r) -> reverse_view<decltype(r.begin())>;

template <typename C>
class strong_exception_safety_guard {
public:
  explicit strong_exception_safety_guard(const C& c) noexcept : ref(c), expected((fault_injection_disable{}, c)) {}

  strong_exception_safety_guard(const strong_exception_safety_guard&) = delete;

  ~strong_exception_safety_guard() {
    if (std::uncaught_exceptions() > 0) {
      expect_eq(expected, ref);
    }
  }

private:
  const C& ref;
  C expected;
};

class base_test : public ::testing::Test {
protected:
  element::no_new_instances_guard instances_guard;
};
