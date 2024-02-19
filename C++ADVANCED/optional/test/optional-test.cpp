#include "optional.h"

#include "test-object.h"

#include <gtest/gtest.h>

namespace {

struct only_copy_constructible : test_object {
  using test_object::test_object;

  only_copy_constructible(const only_copy_constructible&) = default;
  only_copy_constructible& operator=(const only_copy_constructible&) = delete;

  only_copy_constructible(only_copy_constructible&& other) = delete;
  only_copy_constructible& operator=(only_copy_constructible&&) = delete;
};

struct only_move_constructible : test_object {
  using test_object::test_object;

  only_move_constructible(const only_move_constructible&) = delete;
  only_move_constructible& operator=(const only_move_constructible&) = delete;

  only_move_constructible(only_move_constructible&& other) = default;
  only_move_constructible& operator=(only_move_constructible&&) = delete;
};

struct only_copyable : test_object {
  using test_object::test_object;

  only_copyable(const only_copyable& other) : test_object(other) {
    ++copy_ctor_calls;
  }

  only_copyable& operator=(const only_copyable& other) {
    test_object::operator=(other);
    ++copy_assign_calls;
    return *this;
  }

  only_copyable(only_copyable&& other) = delete;
  only_copyable& operator=(only_copyable&& other) = delete;

  inline static size_t copy_ctor_calls = 0;
  inline static size_t copy_assign_calls = 0;
};

struct only_movable : test_object {
  using test_object::test_object;

  only_movable(const only_movable&) = delete;
  only_movable& operator=(const only_movable&) = delete;

  only_movable(only_movable&& other) : test_object(std::move(other)) {
    ++move_ctor_calls;
  }

  only_movable& operator=(only_movable&& other) {
    test_object::operator=(std::move(other));
    ++move_assign_calls;
    return *this;
  }

  inline static size_t move_ctor_calls = 0;
  inline static size_t move_assign_calls = 0;
};

class optional_test : public ::testing::Test {
protected:
  void SetUp() noexcept override {
    only_copyable::copy_ctor_calls = 0;
    only_copyable::copy_assign_calls = 0;
    only_movable::move_ctor_calls = 0;
    only_movable::move_ctor_calls = 0;
  }

  test_object::no_new_instances_guard instances_guard;
};

} // namespace

const nullopt_t* get_nullopt_ptr() noexcept {
  return &nullopt;
}

const in_place_t* get_in_place_ptr() noexcept {
  return &in_place;
}

TEST_F(optional_test, default_ctor) {
  optional<test_object> a;
  EXPECT_FALSE(static_cast<bool>(a));
}

TEST_F(optional_test, default_ctor_no_instances) {
  optional<test_object> a;
  EXPECT_FALSE(static_cast<bool>(a));
  instances_guard.expect_no_instances();
}

TEST_F(optional_test, value_ctor) {
  optional<int> a(42);
  EXPECT_TRUE(static_cast<bool>(a));
}

TEST_F(optional_test, dereference) {
  optional<int> a(42);
  EXPECT_EQ(42, *a);
  EXPECT_EQ(42, *std::as_const(a));
  EXPECT_EQ(42, *std::move(a));
  EXPECT_EQ(42, *std::move(std::as_const(a)));
}

TEST_F(optional_test, member_access) {
  optional<test_object> a(42);
  EXPECT_EQ(42, a->operator int());
  EXPECT_EQ(42, std::as_const(a)->operator int());
}

TEST_F(optional_test, reset) {
  optional<test_object> a(42);
  EXPECT_TRUE(static_cast<bool>(a));
  a.reset();
  EXPECT_FALSE(static_cast<bool>(a));
  instances_guard.expect_no_instances();
}

TEST_F(optional_test, dtor) {
  optional<test_object> a(42);
  EXPECT_TRUE(static_cast<bool>(a));
  EXPECT_EQ(42, *a);
}

TEST_F(optional_test, copy_ctor) {
  optional<only_copy_constructible> a(in_place, 42);
  optional<only_copy_constructible> b = a;
  EXPECT_TRUE(static_cast<bool>(b));
  EXPECT_EQ(42, *b);
}

TEST_F(optional_test, copy_ctor_empty) {
  optional<only_copy_constructible> a;
  optional<only_copy_constructible> b = a;
  EXPECT_FALSE(static_cast<bool>(b));
}

TEST_F(optional_test, move_ctor) {
  optional<only_move_constructible> a(in_place, 42);
  optional<only_move_constructible> b = std::move(a);
  EXPECT_TRUE(static_cast<bool>(b));
  EXPECT_EQ(42, *b);
  EXPECT_TRUE(static_cast<bool>(a));
  EXPECT_EQ(42, *a);
}

TEST_F(optional_test, move_ctor_empty) {
  optional<only_move_constructible> a;
  optional<only_move_constructible> b = std::move(a);
  EXPECT_FALSE(static_cast<bool>(b));
  EXPECT_FALSE(static_cast<bool>(a));
}

TEST_F(optional_test, copy_assignment_empty_empty) {
  optional<only_copyable> a, b;
  b = a;
  EXPECT_FALSE(static_cast<bool>(b));
  EXPECT_EQ(0, only_copyable::copy_ctor_calls);
  EXPECT_EQ(0, only_copyable::copy_assign_calls);
}

TEST_F(optional_test, copy_assignment_to_empty) {
  optional<only_copyable> a(in_place, 42), b;
  b = a;
  EXPECT_TRUE(static_cast<bool>(b));
  EXPECT_EQ(42, *b);
  EXPECT_EQ(1, only_copyable::copy_ctor_calls);
  EXPECT_EQ(0, only_copyable::copy_assign_calls);
}

TEST_F(optional_test, copy_assignment_from_empty) {
  optional<only_copyable> a, b(in_place, 42);
  b = a;
  EXPECT_FALSE(static_cast<bool>(b));
  EXPECT_EQ(0, only_copyable::copy_ctor_calls);
  EXPECT_EQ(0, only_copyable::copy_assign_calls);
}

TEST_F(optional_test, copy_assignment) {
  optional<only_copyable> a(in_place, 42), b(in_place, 41);
  b = a;
  EXPECT_TRUE(static_cast<bool>(b));
  EXPECT_EQ(42, *b);
  EXPECT_EQ(0, only_copyable::copy_ctor_calls);
  EXPECT_EQ(1, only_copyable::copy_assign_calls);
}

TEST_F(optional_test, move_assignment_empty_empty) {
  optional<only_movable> a, b;
  b = std::move(a);
  EXPECT_FALSE(static_cast<bool>(b));
  EXPECT_FALSE(static_cast<bool>(a));
  EXPECT_EQ(0, only_movable::move_ctor_calls);
  EXPECT_EQ(0, only_movable::move_assign_calls);
}

TEST_F(optional_test, move_assignment_to_empty) {
  optional<only_movable> a(in_place, 42), b;
  b = std::move(a);
  EXPECT_TRUE(static_cast<bool>(b));
  EXPECT_EQ(42, *b);
  EXPECT_TRUE(static_cast<bool>(a));
  EXPECT_EQ(42, *a);
  EXPECT_EQ(1, only_movable::move_ctor_calls);
  EXPECT_EQ(0, only_movable::move_assign_calls);
}

TEST_F(optional_test, move_assignment_from_empty) {
  optional<only_movable> a, b(in_place, 42);
  b = std::move(a);
  EXPECT_FALSE(static_cast<bool>(b));
  EXPECT_FALSE(static_cast<bool>(a));
  EXPECT_EQ(0, only_movable::move_ctor_calls);
  EXPECT_EQ(0, only_movable::move_assign_calls);
}

TEST_F(optional_test, move_assignment) {
  optional<only_movable> a(in_place, 42), b(in_place, 41);
  b = std::move(a);
  EXPECT_TRUE(static_cast<bool>(b));
  EXPECT_EQ(42, *b);
  EXPECT_TRUE(static_cast<bool>(a));
  EXPECT_EQ(42, *a);
  EXPECT_EQ(0, only_movable::move_ctor_calls);
  EXPECT_EQ(1, only_movable::move_assign_calls);
}

TEST_F(optional_test, nullopt_ctor) {
  optional<test_object> a = nullopt;
  EXPECT_FALSE(static_cast<bool>(a));
  instances_guard.expect_no_instances();
}

TEST_F(optional_test, nullopt_assignment) {
  optional<test_object> a(42);
  a = nullopt;
  EXPECT_FALSE(static_cast<bool>(a));
  EXPECT_TRUE(noexcept(a = nullopt));
  instances_guard.expect_no_instances();
}

TEST_F(optional_test, empty_ctor) {
  optional<test_object> a = {};
  EXPECT_FALSE(static_cast<bool>(a));
  instances_guard.expect_no_instances();
}

TEST_F(optional_test, empty_assignment) {
  optional<test_object> a(42);
  a = {};
  EXPECT_FALSE(static_cast<bool>(a));
  instances_guard.expect_no_instances();
}

TEST_F(optional_test, swap_non_empty) {
  optional<test_object> a(42);
  optional<test_object> b(55);

  swap(a, b);

  EXPECT_EQ(55, *a);
  EXPECT_EQ(42, *b);
}

TEST_F(optional_test, swap_empty_right) {
  optional<test_object> a(42);
  optional<test_object> b;

  swap(a, b);

  EXPECT_FALSE(a);
  EXPECT_EQ(42, *b);
}

TEST_F(optional_test, swap_empty_left) {
  optional<test_object> a;
  optional<test_object> b(55);

  swap(a, b);

  EXPECT_EQ(55, *a);
  EXPECT_FALSE(b);
}

TEST_F(optional_test, swap_empty_both) {
  optional<test_object> a;
  optional<test_object> b;

  swap(a, b);

  EXPECT_FALSE(a);
  EXPECT_FALSE(b);
}

namespace {

struct custom_swap {
  custom_swap(int value) noexcept : value(value) {}

  friend void swap(custom_swap& lhs, custom_swap& rhs) noexcept {
    std::swap(lhs.value, rhs.value);
    ++lhs.value;
    ++rhs.value;
  }

  int value;
};

} // namespace

TEST_F(optional_test, swap_custom) {
  optional<custom_swap> a(42);
  optional<custom_swap> b(55);

  swap(a, b);

  EXPECT_EQ(56, a->value);
  EXPECT_EQ(43, b->value);
}

TEST_F(optional_test, swap_empty_custom) {
  optional<custom_swap> a(42);
  optional<custom_swap> b;

  swap(a, b);

  EXPECT_FALSE(a);
  EXPECT_EQ(42, b->value);
}

namespace {

struct non_default_constructor {
  non_default_constructor(int, int, int, std::unique_ptr<int>) {}
};

} // namespace

TEST_F(optional_test, in_place_ctor) {
  optional<non_default_constructor> a(in_place, 1, 2, 3, std::unique_ptr<int>());
  EXPECT_TRUE(static_cast<bool>(a));
}

TEST_F(optional_test, emplace) {
  optional<non_default_constructor> a;
  a.emplace(1, 2, 3, std::unique_ptr<int>());
  EXPECT_TRUE(static_cast<bool>(a));
}

namespace {

struct throw_in_ctor {
  struct exception : std::exception {
    using std::exception::exception;
  };

  throw_in_ctor(int, int) {
    if (enable_throw) {
      throw exception();
    }
  }

  inline static bool enable_throw = false;
};

} // namespace

TEST_F(optional_test, emplace_throw) {
  optional<throw_in_ctor> a(in_place, 1, 2);
  throw_in_ctor::enable_throw = true;
  EXPECT_THROW(a.emplace(3, 4), throw_in_ctor::exception);
  EXPECT_FALSE(static_cast<bool>(a));
}

namespace {

struct comparison_counters {
  size_t equal = 0;
  size_t not_equal = 0;
  size_t less = 0;
  size_t less_equal = 0;
  size_t greater = 0;
  size_t greater_equal = 0;
};

struct custom_comparison {
  custom_comparison(int value, comparison_counters* counters) : value(value), counters(counters) {}

  bool operator==(const custom_comparison& other) const {
    ++counters->equal;
    return this->value == other.value;
  }

  bool operator!=(const custom_comparison& other) const {
    ++counters->not_equal;
    return this->value != other.value;
  }

  bool operator<(const custom_comparison& other) const {
    ++counters->less;
    return this->value < other.value;
  }

  bool operator<=(const custom_comparison& other) const {
    ++counters->less_equal;
    return this->value <= other.value;
  }

  bool operator>(const custom_comparison& other) const {
    ++counters->greater;
    return this->value > other.value;
  }

  bool operator>=(const custom_comparison& other) const {
    ++counters->greater_equal;
    return this->value >= other.value;
  }

private:
  int value;
  comparison_counters* counters;
};

} // namespace

TEST_F(optional_test, comparison_non_empty_and_non_empty) {
  comparison_counters ca, cb;
  optional<custom_comparison> a(in_place, 41, &ca);
  optional<custom_comparison> b(in_place, 42, &cb);

  EXPECT_FALSE(a == b);
  EXPECT_TRUE(a != b);
  EXPECT_TRUE(a < b);
  EXPECT_TRUE(a <= b);
  EXPECT_FALSE(a > b);
  EXPECT_FALSE(a >= b);

  EXPECT_TRUE(a == a);
  EXPECT_FALSE(a != a);
  EXPECT_FALSE(a < a);
  EXPECT_TRUE(a <= a);
  EXPECT_FALSE(a > a);
  EXPECT_TRUE(a >= a);

  EXPECT_FALSE(b == a);
  EXPECT_TRUE(b != a);
  EXPECT_FALSE(b < a);
  EXPECT_FALSE(b <= a);
  EXPECT_TRUE(b > a);
  EXPECT_TRUE(b >= a);

  EXPECT_EQ(2, ca.equal);
  EXPECT_EQ(2, ca.not_equal);
  EXPECT_EQ(2, ca.less);
  EXPECT_EQ(2, ca.less_equal);
  EXPECT_EQ(2, ca.greater);
  EXPECT_EQ(2, ca.greater_equal);

  EXPECT_EQ(1, cb.equal);
  EXPECT_EQ(1, cb.not_equal);
  EXPECT_EQ(1, cb.less);
  EXPECT_EQ(1, cb.less_equal);
  EXPECT_EQ(1, cb.greater);
  EXPECT_EQ(1, cb.greater_equal);
}

TEST_F(optional_test, comparison_non_empty_and_empty) {
  comparison_counters ca;
  optional<custom_comparison> a(in_place, 41, &ca);
  optional<custom_comparison> b;

  EXPECT_FALSE(a == b);
  EXPECT_TRUE(a != b);
  EXPECT_FALSE(a < b);
  EXPECT_FALSE(a <= b);
  EXPECT_TRUE(a > b);
  EXPECT_TRUE(a >= b);

  EXPECT_FALSE(b == a);
  EXPECT_TRUE(b != a);
  EXPECT_TRUE(b < a);
  EXPECT_TRUE(b <= a);
  EXPECT_FALSE(b > a);
  EXPECT_FALSE(b >= a);

  EXPECT_EQ(0, ca.equal);
  EXPECT_EQ(0, ca.not_equal);
  EXPECT_EQ(0, ca.less);
  EXPECT_EQ(0, ca.less_equal);
  EXPECT_EQ(0, ca.greater);
  EXPECT_EQ(0, ca.greater_equal);
}

TEST_F(optional_test, comparison_empty_and_empty) {
  optional<custom_comparison> a, b;

  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);
  EXPECT_FALSE(a < b);
  EXPECT_TRUE(a <= b);
  EXPECT_FALSE(a > b);
  EXPECT_TRUE(a >= b);

  EXPECT_TRUE(a == a);
  EXPECT_FALSE(a != a);
  EXPECT_FALSE(a < a);
  EXPECT_TRUE(a <= a);
  EXPECT_FALSE(a > a);
  EXPECT_TRUE(a >= a);

  EXPECT_TRUE(b == a);
  EXPECT_FALSE(b != a);
  EXPECT_FALSE(b < a);
  EXPECT_TRUE(b <= a);
  EXPECT_FALSE(b > a);
  EXPECT_TRUE(b >= a);
}
