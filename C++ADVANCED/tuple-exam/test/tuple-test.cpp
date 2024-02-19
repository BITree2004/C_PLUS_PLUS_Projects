#include "tuple.h"

#include "test-utils.h"

#include <gtest/gtest.h>

#include <limits>
#include <string>

template class tuple<int>;
template class tuple<int, std::string, int>;

using namespace tests;

TEST(tuple, get_by_index) {
  tuple<int, double, float, int> t;

  get<0>(t) = 42;
  get<1>(t) = 3.14;
  get<2>(t) = 1.618;
  get<3>(t) = 100;

  EXPECT_EQ(get<0>(t), 42);
  EXPECT_EQ(get<1>(t), 3.14);
  EXPECT_EQ(get<2>(t), 1.618f);
  EXPECT_EQ(get<3>(t), 100);

  EXPECT_EQ(get<0>(std::as_const(t)), 42);
  EXPECT_EQ(get<0>(std::move(t)), 42);
  EXPECT_EQ(get<0>(std::move(std::as_const(t))), 42);
}

TEST(tuple, get_by_type) {
  tuple<int, double, float, int> t;

  get<double>(t) = 3.14;
  get<float>(t) = 1.618;

  EXPECT_EQ(get<double>(t), 3.14);
  EXPECT_EQ(get<float>(t), 1.618f);

  EXPECT_EQ(get<double>(std::as_const(t)), 3.14);
  EXPECT_EQ(get<double>(std::move(t)), 3.14);
  EXPECT_EQ(get<double>(std::move(std::as_const(t))), 3.14);
}

TEST(tuple, default_ctor) {
  tuple<int, double, std::string> t;
  EXPECT_EQ(get<0>(t), 0);
  EXPECT_EQ(get<1>(t), 0.0);
  EXPECT_EQ(get<2>(t), "");
}

TEST(tuple, direct_ctor) {
  tuple<int, double, std::string> t(42, 3.14, "hello");
  EXPECT_EQ(get<0>(t), 42);
  EXPECT_EQ(get<1>(t), 3.14);
  EXPECT_EQ(get<2>(t), "hello");
}

TEST(tuple, direct_ctor_references) {
  const int x = 42;
  double y = 3.14;
  tuple<const int&, const int&&, double&, double&&> t(x, std::move(x), y, std::move(y));

  EXPECT_EQ(get<0>(t), 42);
  EXPECT_EQ(get<1>(t), 42);
  EXPECT_EQ(get<2>(t), 3.14);
  EXPECT_EQ(get<3>(t), 3.14);

  get<2>(t) = 1.618;
  EXPECT_EQ(y, 1.618);

  get<3>(t) = 3.14;
  EXPECT_EQ(y, 3.14);
}

TEST(tuple, direct_ctor_reference_wrapper) {
  int x = 42;
  tuple<std::reference_wrapper<int>, std::reference_wrapper<const int>> t(std::ref(x), std::cref(x));

  EXPECT_EQ(get<0>(t), 42);
  EXPECT_EQ(get<1>(t), 42);

  get<0>(t).get() = 55;
  EXPECT_EQ(x, 55);
}

TEST(tuple, direct_ctor_const) {
  const std::vector<int> vec(3, 11);
  const util::copy_counter counter;
  tuple<std::vector<int>, util::copy_counter> t(vec, counter);

  EXPECT_EQ(get<0>(t).size(), 3);
  EXPECT_EQ(get<1>(t).get_copies(), 1);
  EXPECT_EQ(counter.get_copies(), 0);
}

TEST(tuple, direct_ctor_from_tuple) {
  tuple<tuple<int>> t(tuple<int>(42));
  EXPECT_EQ(get<0>(get<0>(t)), 42);
}

TEST(tuple, direct_ctor_initializer_lists_1) {
  tuple<tuple<int, double>, std::string> t({42, 3.14}, {'h', 'i'});
  EXPECT_EQ(get<0>(get<0>(t)), 42);
  EXPECT_EQ(get<1>(get<0>(t)), 3.14);
  EXPECT_EQ(get<1>(t), "hi");
}

TEST(tuple, direct_ctor_initializer_lists_2) {
  tuple<tuple<int, double>, std::string> t({}, {});
  EXPECT_EQ(get<0>(get<0>(t)), 0);
  EXPECT_EQ(get<1>(get<0>(t)), 0);
  EXPECT_EQ(get<1>(t), "");
}

TEST(tuple, ctad) {
  tuple t(42, 3.14, std::string("hello"));
  static_assert(std::same_as<decltype(t), tuple<int, double, std::string>>);

  EXPECT_EQ(get<0>(t), 42);
  EXPECT_EQ(get<1>(t), 3.14);
  EXPECT_EQ(get<2>(t), "hello");
}

TEST(tuple, ctad_references) {
  const int x = 42;
  double y = 3.14;
  tuple t(x, std::move(x), y, std::move(y));
  static_assert(std::is_same_v<decltype(t), tuple<int, int, double, double>>);

  EXPECT_EQ(get<0>(t), 42);
  EXPECT_EQ(get<1>(t), 42);
  EXPECT_EQ(get<2>(t), 3.14);
  EXPECT_EQ(get<3>(t), 3.14);
}

TEST(tuple, copy_ctor) {
  tuple<int, double, float> x(1, 2., 3.14);
  auto y(x);
  EXPECT_EQ(get<0>(x), get<0>(y));
  EXPECT_EQ(get<2>(x), get<2>(y));
}

TEST(tuple, move_ctor) {
  tuple<int, double, util::move_counter> x;
  get<1>(x) = 3.14;
  EXPECT_EQ(get<2>(x).get_moves(), 0);

  auto y(std::move(x));
  EXPECT_EQ(get<1>(y), 3.14);
  EXPECT_EQ(get<2>(y).get_moves(), 1);

  tuple<util::move_counter, int> z(std::move(get<2>(x)), 42);
  EXPECT_EQ(get<0>(z).get_moves(), 1);
  EXPECT_EQ(get<1>(z), 42);
}

TEST(tuple, converting_ctor_1) {
  tuple<int, double, float> x(1, 2., 3.14);
  tuple<int, float, double> z(x);
  EXPECT_EQ(get<0>(x), get<0>(z));
  EXPECT_FLOAT_EQ(get<2>(x), get<2>(z));
}

TEST(tuple, converting_ctor_2) {
  struct conv_to {
    int value = 0;
  };

  struct conv_from {
    explicit operator conv_to() {
      return conv_to{42};
    }
  };

  conv_from x;
  tuple<conv_to> y(x);
  EXPECT_EQ(get<0>(y).value, 42);
}

TEST(tuple, converting_ctor_move) {
  struct constructible_from_rvalue {
    explicit constructible_from_rvalue(util::move_counter&&) : value(42) {}

    int value = 0;
  };

  tuple<util::move_counter, float, float> x(util::move_counter(), 2., 3.14);
  tuple<constructible_from_rvalue, float, double> y(std::move(x));

  EXPECT_EQ(get<1>(y), 2.0f);
  EXPECT_EQ(get<2>(y), 3.14f);
  EXPECT_EQ(get<0>(x).get_moves(), 1);
  EXPECT_EQ(get<0>(y).value, 42);
}

TEST(tuple, converting_assignment) {
  tuple<int, double, float> x(1, 2., 3.14);
  tuple<int, float, double> z;
  z = x;
  EXPECT_EQ(get<0>(x), get<0>(z));
  EXPECT_FLOAT_EQ(get<2>(x), get<2>(z));
}

TEST(tuple, swap) {
  tuple<int, double, int> t1(42, 3.14, 1337);
  tuple<int, double, int> t2(69, 1.618, 1488);

  using std::swap;
  swap(t1, t2);
  EXPECT_EQ(get<0>(t1), 69);
  EXPECT_EQ(get<0>(t2), 42);
  EXPECT_EQ(get<1>(t1), 1.618);
  EXPECT_EQ(get<1>(t2), 3.14);
  EXPECT_EQ(get<2>(t1), 1488);
  EXPECT_EQ(get<2>(t2), 1337);
}

TEST(tuple, swap_move) {
  tuple t1(util::move_counter{}, util::non_copyable{42});
  tuple t2(util::move_counter{}, util::non_copyable{43});

  using std::swap;
  swap(t1, t2);
  EXPECT_LE(get<0>(t1).get_moves() + get<0>(t2).get_moves(), 3);
  EXPECT_EQ(get<1>(t1).get_value(), 43);
  EXPECT_EQ(get<1>(t2).get_value(), 42);
}

TEST(make_tuple, simple) {
  auto t = ::make_tuple(42, 3.14, std::string("hello"));
  static_assert(std::is_same_v<decltype(t), tuple<int, double, std::string>>);
  EXPECT_EQ(get<0>(t), 42);
  EXPECT_EQ(get<1>(t), 3.14);
  EXPECT_EQ(get<2>(t), "hello");
}

TEST(make_tuple, moves) {
  auto t = make_tuple(util::move_counter(), util::non_copyable(42));
  static_assert(std::is_same_v<decltype(t), tuple<util::move_counter, util::non_copyable>>);
  EXPECT_EQ(get<0>(t).get_moves(), 1);
  EXPECT_EQ(get<1>(t).get_value(), 42);
}

TEST(make_tuple, references) {
  const int x = 42;
  double y = 3.14;
  auto t = make_tuple(x, std::move(x), y, std::move(y));
  static_assert(std::is_same_v<decltype(t), tuple<int, int, double, double>>);

  EXPECT_EQ(get<0>(t), 42);
  EXPECT_EQ(get<1>(t), 42);
  EXPECT_EQ(get<2>(t), 3.14);
  EXPECT_EQ(get<3>(t), 3.14);

  get<0>(t) = 0;
  get<1>(t) = 0;
  get<2>(t) = 0;
  get<3>(t) = 0;
  EXPECT_EQ(x, 42);
  EXPECT_EQ(y, 3.14);
}

TEST(make_tuple, reference_wrapper) {
  int x = 42;
  auto t = ::make_tuple(std::ref(x), std::cref(x));
  static_assert(std::is_same_v<decltype(t), tuple<int&, const int&>>);

  EXPECT_EQ(get<0>(t), 42);
  EXPECT_EQ(get<1>(t), 42);

  get<0>(t) = 55;
  EXPECT_EQ(x, 55);
}

TEST(tuple_compare, three_way) {
  EXPECT_EQ(tuple(1, 2, 3) <=> tuple(1, 2, 3), std::partial_ordering::equivalent);
  EXPECT_EQ(tuple(100, 2, 3) <=> tuple(1, 2, 3), std::partial_ordering::greater);
  EXPECT_EQ(tuple(100, 2, 3) <=> tuple(100, 200, 3), std::partial_ordering::less);

  if constexpr (std::numeric_limits<double>::is_iec559) {
    double nan = std::numeric_limits<double>::quiet_NaN();
    EXPECT_EQ(tuple(1.0, 2.0) <=> tuple(2.0, nan), std::partial_ordering::less);
    EXPECT_EQ(tuple(1.0, 2.0) <=> tuple(1.0, nan), std::partial_ordering::unordered);
  }
}

TEST(tuple_compare, simple) {
  EXPECT_EQ(tuple(1, 2), tuple(1, 2));
  EXPECT_LE(tuple(1, 2), tuple(1, 2));
  EXPECT_GE(tuple(1, 2), tuple(1, 2));

  EXPECT_NE(tuple(1, 2), tuple(2, 2));
  EXPECT_LT(tuple(1, 2), tuple(2, 2));
  EXPECT_GT(tuple(2, 2), tuple(1, 2));

  EXPECT_NE(tuple(1, 2), tuple(1, 3));
  EXPECT_LT(tuple(1, 2), tuple(1, 3));
  EXPECT_GT(tuple(1, 3), tuple(1, 2));
}

TEST(tuple_compare, different_types) {
  EXPECT_EQ(tuple(1, 2U), tuple(1, 2U));
  EXPECT_LE(tuple(1, 2U), tuple(1, 2U));
  EXPECT_GE(tuple(1, 2U), tuple(1, 2U));

  EXPECT_NE(tuple(1, 2U), tuple(2, 2U));
  EXPECT_LT(tuple(1, 2U), tuple(2, 2U));
  EXPECT_GT(tuple(2, 2U), tuple(1, 2U));

  EXPECT_NE(tuple(1, 2U), tuple(1, 3U));
  EXPECT_LT(tuple(1, 2U), tuple(1, 3U));
  EXPECT_GT(tuple(1, 3U), tuple(1, 2U));
}
