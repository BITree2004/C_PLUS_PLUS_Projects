#pragma once

#include "element.h"
#include "fault-injection.h"
#include "list.h"

#include <gtest/gtest.h>

#include <concepts>

template class list<element>;
using container = list<element>;

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

template <class Actual, class Expected = std::initializer_list<int>>
void expect_eq(const Actual& actual, const Expected& expected) {
  fault_injection_disable dg;

  EXPECT_EQ(expected.size(), actual.size());

  auto expected_first = expected.begin();
  auto expected_last = expected.end();
  auto actual_first = actual.begin();
  auto actual_last = actual.end();

  while (true) {
    if (expected_first == expected_last || actual_first == actual_last) {
      break;
    }

    EXPECT_EQ(*actual_first, *expected_first);
    ++expected_first;
    ++actual_first;
  }

  EXPECT_TRUE(expected_first == expected_last);
  EXPECT_TRUE(actual_first == actual_last);
}

template <typename C>
void expect_empty(const C& c) {
  EXPECT_TRUE(c.empty());
  EXPECT_EQ(0, c.size());
  EXPECT_TRUE(c.begin() == c.end());
}

// for old libc++ support
// for old libc++ support
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
