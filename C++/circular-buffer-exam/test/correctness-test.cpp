#include "circular-buffer.h"
#include "element.h"
#include "fault-injection.h"
#include "test-utils.h"

#include <gtest/gtest.h>

#include <iterator>
#include <utility>

namespace {

class correctness_test : public base_test {};

[[maybe_unused]] void magic([[maybe_unused]] element& c) {
  c = 42;
}

void magic(const element&) {}

template <class It>
It advance(It it, typename std::iterator_traits<It>::difference_type n) {
  while (n > 0) {
    --n;
    ++it;
  }
  while (n < 0) {
    ++n;
    --it;
  }
  return it;
}

std::ptrdiff_t as_diff(std::size_t n) {
  return static_cast<std::ptrdiff_t>(n);
}

} // namespace

TEST_F(correctness_test, default_ctor) {
  container c;
  EXPECT_EQ(0, c.capacity());
  instances_guard.expect_no_instances();
}

TEST_F(correctness_test, push_back_single) {
  container c;
  c.push_back(42);
  expect_eq(c, {42});
}

TEST_F(correctness_test, push_back_many) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});
  expect_eq(c, {1, 2, 3, 4});
}

TEST_F(correctness_test, push_front_single) {
  container c;
  c.push_front(42);
  expect_eq(c, {42});
}

TEST_F(correctness_test, push_front_many) {
  container c;
  mass_push_front(c, {1, 2, 3, 4});
  expect_eq(c, {4, 3, 2, 1});
}

TEST_F(correctness_test, push_back_front) {
  container c;
  mass_push_back(c, {1, 2});
  mass_push_front(c, {3, 4});
  mass_push_back(c, {5});

  expect_eq(c, {4, 3, 1, 2, 5});
}

TEST_F(correctness_test, pop_back) {
  container c;
  mass_push_back(c, {1, 2, 3});

  c.pop_back();
  expect_eq(c, {1, 2});

  c.pop_back();
  expect_eq(c, {1});

  c.pop_back();
  expect_empty(c);
}

TEST_F(correctness_test, pop_front) {
  container c;
  mass_push_back(c, {1, 2, 3});

  c.pop_front();
  expect_eq(c, {2, 3});

  c.pop_front();
  expect_eq(c, {3});

  c.pop_front();
  expect_empty(c);
}

TEST_F(correctness_test, copy_ctor) {
  container c1;
  mass_push_back(c1, {1, 2, 3, 4});

  container c2 = c1;
  expect_eq(c2, {1, 2, 3, 4});
}

TEST_F(correctness_test, copy_ctor_empty) {
  container c;
  container c2 = c;
  expect_empty(c2);
}

TEST_F(correctness_test, copy_assignment) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});
  mass_push_back(c2, {5, 6, 7, 8, 9});

  c1 = c2;
  expect_eq(c1, {5, 6, 7, 8, 9});
}

TEST_F(correctness_test, copy_assignment_empty_to_empty) {
  container c1, c2;
  c1 = c2;
  expect_empty(c1);
}

TEST_F(correctness_test, copy_assignment_empty_to_nonempty) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});

  c1 = c2;
  expect_empty(c1);
}

TEST_F(correctness_test, copy_assignment_nonempty_to_empty) {
  container c1, c2;
  mass_push_back(c2, {5, 6, 7, 8});

  c1 = c2;
  expect_eq(c1, {5, 6, 7, 8});
}

TEST_F(correctness_test, copy_assignment_self) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  c = c;
  expect_eq(c, {1, 2, 3, 4});
}

TEST_F(correctness_test, assignment_self_empty) {
  container c;
  c = c;
  expect_empty(c);
}

TEST_F(correctness_test, swap) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});
  mass_push_back(c2, {5, 6, 7, 8, 9});

  swap(c1, c2);
  expect_eq(c1, {5, 6, 7, 8, 9});
  expect_eq(c2, {1, 2, 3, 4});
}

TEST_F(correctness_test, swap_self) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  swap(c, c);
  expect_eq(c, {1, 2, 3, 4});
}

TEST_F(correctness_test, swap_empty) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});

  swap(c1, c2);
  expect_empty(c1);
  expect_eq(c2, {1, 2, 3, 4});

  swap(c1, c2);
  expect_eq(c1, {1, 2, 3, 4});
  expect_empty(c2);
}

TEST_F(correctness_test, swap_empty_empty) {
  container c1, c2;
  swap(c1, c2);
  expect_empty(c1);
  expect_empty(c2);
}

TEST_F(correctness_test, swap_empty_self) {
  container c;
  swap(c, c);
  expect_empty(c);
}

TEST_F(correctness_test, empty) {
  container c;
  expect_empty(c);

  c.push_back(1);
  EXPECT_FALSE(c.empty());
  EXPECT_NE(0, c.size());
  EXPECT_NE(c.begin(), c.end());

  c.pop_front();
  expect_empty(c);
}

TEST_F(correctness_test, size) {
  container c;
  expect_empty(c);
  c.push_back(42);
  EXPECT_EQ(1, c.size());
  c.push_front(42);
  EXPECT_EQ(2, c.size());
  c.pop_back();
  EXPECT_EQ(1, c.size());
  c.pop_front();
  expect_empty(c);
}

TEST_F(correctness_test, subscript) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});
  EXPECT_EQ(1, c[0]);
  EXPECT_EQ(2, c[1]);
  EXPECT_EQ(3, c[2]);
  EXPECT_EQ(4, c[3]);
  c[2] = 5;
  EXPECT_EQ(5, c[2]);
}

TEST_F(correctness_test, subscript_const) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});
  EXPECT_EQ(3, std::as_const(c)[2]);
}

TEST_F(correctness_test, back_front) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  EXPECT_EQ(1, c.front());
  EXPECT_EQ(1, std::as_const(c).front());
  EXPECT_EQ(5, c.back());
  EXPECT_EQ(5, std::as_const(c).back());
}

TEST_F(correctness_test, back_front_ref) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  c.front() = 6;
  c.back() = 7;
  expect_eq(c, {6, 2, 3, 4, 7});
}

TEST_F(correctness_test, back_front_cref) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  EXPECT_TRUE(&c.front() == &std::as_const(c).front());
  EXPECT_TRUE(&c.back() == &std::as_const(c).back());
}

TEST_F(correctness_test, back_front_ncref) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  magic(std::as_const(c).front());
  magic(std::as_const(c).back());
  expect_eq(c, {1, 2, 3, 4, 5});
}

TEST_F(correctness_test, clear) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  c.clear();
  expect_empty(c);

  mass_push_back(c, {5, 6, 7, 8});
  expect_eq(c, {5, 6, 7, 8});
}

TEST_F(correctness_test, clear_empty) {
  container c;
  c.clear();
  expect_empty(c);
}

TEST_F(correctness_test, iterator_deref_1) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6});

  container::iterator i = std::next(c.begin(), 3);
  EXPECT_EQ(4, *i);
  magic(*i);
  expect_eq(c, {1, 2, 3, 42, 5, 6});

  container::const_iterator j = std::next(c.begin(), 2);
  EXPECT_EQ(3, *j);
  magic(*j);
  expect_eq(c, {1, 2, 3, 42, 5, 6});
}

TEST_F(correctness_test, iterator_deref_1c) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6});

  const container::iterator i = std::next(c.begin(), 3);
  EXPECT_EQ(4, *i);
  magic(*i);
  expect_eq(c, {1, 2, 3, 42, 5, 6});

  const container::const_iterator j = std::next(c.begin(), 2);
  EXPECT_EQ(3, *j);
  magic(*j);
  expect_eq(c, {1, 2, 3, 42, 5, 6});
}

TEST_F(correctness_test, iterator_deref_2) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6});

  container::iterator i = std::next(c.begin(), 3);
  magic(*i.operator->());
  expect_eq(c, {1, 2, 3, 42, 5, 6});

  container::const_iterator j = std::next(c.begin(), 2);
  magic(*j.operator->());
  expect_eq(c, {1, 2, 3, 42, 5, 6});
}

TEST_F(correctness_test, iterator_deref_2c) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6});

  const container::iterator i = std::next(c.begin(), 3);
  magic(*i.operator->());
  expect_eq(c, {1, 2, 3, 42, 5, 6});

  const container::const_iterator j = std::next(c.begin(), 2);
  EXPECT_EQ(3, *j);
  magic(*j.operator->());
  expect_eq(c, {1, 2, 3, 42, 5, 6});
}

TEST_F(correctness_test, iterator_increment_1) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::iterator i = c.begin();
  EXPECT_EQ(1, *i);
  EXPECT_EQ(2, *++i);
  EXPECT_EQ(3, *++i);
  EXPECT_EQ(4, *++i);
  EXPECT_EQ(5, *++i);
  EXPECT_EQ(c.end(), ++i);
}

TEST_F(correctness_test, iterator_increment_1c) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::const_iterator i = c.begin();
  EXPECT_EQ(1, *i);
  EXPECT_EQ(2, *++i);
  EXPECT_EQ(3, *++i);
  EXPECT_EQ(4, *++i);
  EXPECT_EQ(5, *++i);
  EXPECT_EQ(c.end(), ++i);
}

TEST_F(correctness_test, iterator_increment_2) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6});

  container::iterator i = std::next(c.begin(), 3);
  EXPECT_EQ(4, *i);
  ++ ++i;
  EXPECT_EQ(6, *i);
}

TEST_F(correctness_test, iterator_increment_2c) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6});

  container::const_iterator i = std::next(c.begin(), 3);
  EXPECT_EQ(4, *i);
  ++ ++i;
  EXPECT_EQ(6, *i);
}

TEST_F(correctness_test, iterator_increment_3) {
  container c;
  mass_push_back(c, {1, 2, 3});

  container::iterator i = c.begin();
  EXPECT_EQ(1, *i);
  container::iterator j = i++;
  EXPECT_EQ(2, *i);
  EXPECT_EQ(1, *j);
  j = i++;
  EXPECT_EQ(3, *i);
  EXPECT_EQ(2, *j);
  j = i++;
  EXPECT_EQ(c.end(), i);
  EXPECT_EQ(3, *j);
}

TEST_F(correctness_test, iterator_increment_3c) {
  container c;
  mass_push_back(c, {1, 2, 3});

  container::const_iterator i = std::as_const(c).begin();
  EXPECT_EQ(1, *i);
  container::const_iterator j = i++;
  EXPECT_EQ(2, *i);
  EXPECT_EQ(1, *j);
  j = i++;
  EXPECT_EQ(3, *i);
  EXPECT_EQ(2, *j);
  j = i++;
  EXPECT_EQ(std::as_const(c).end(), i);
  EXPECT_EQ(3, *j);
}

TEST_F(correctness_test, iterator_decrement_1) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::iterator i = c.end();
  EXPECT_EQ(5, *--i);
  EXPECT_EQ(4, *--i);
  EXPECT_EQ(3, *--i);
  EXPECT_EQ(2, *--i);
  EXPECT_EQ(1, *--i);
  EXPECT_EQ(c.begin(), i);
}

TEST_F(correctness_test, iterator_decrement_1c) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::const_iterator i = c.end();
  EXPECT_EQ(5, *--i);
  EXPECT_EQ(4, *--i);
  EXPECT_EQ(3, *--i);
  EXPECT_EQ(2, *--i);
  EXPECT_EQ(1, *--i);
  EXPECT_EQ(c.begin(), i);
}

TEST_F(correctness_test, iterator_decrement_2) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6});

  container::iterator i = std::prev(c.end(), 3);
  EXPECT_EQ(4, *i);
  -- --i;
  EXPECT_EQ(2, *i);
}

TEST_F(correctness_test, iterator_decrement_2c) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6});

  container::const_iterator i = std::prev(c.end(), 3);
  EXPECT_EQ(4, *i);
  -- --i;
  EXPECT_EQ(2, *i);
}

TEST_F(correctness_test, iterator_decrement_3) {
  container c;
  mass_push_back(c, {1, 2, 3});

  container::iterator i = c.end();
  container::iterator j = i--;
  EXPECT_EQ(3, *i);
  EXPECT_EQ(c.end(), j);
  j = i--;
  EXPECT_EQ(2, *i);
  EXPECT_EQ(3, *j);
  j = i--;
  EXPECT_EQ(1, *i);
  EXPECT_EQ(2, *j);
}

TEST_F(correctness_test, iterator_decrement_3c) {
  container c;
  mass_push_back(c, {1, 2, 3});

  container::const_iterator i = std::as_const(c).end();
  container::const_iterator j = i--;
  EXPECT_EQ(3, *i);
  EXPECT_EQ(c.end(), j);
  j = i--;
  EXPECT_EQ(2, *i);
  EXPECT_EQ(3, *j);
  j = i--;
  EXPECT_EQ(1, *i);
  EXPECT_EQ(2, *j);
}

TEST_F(correctness_test, iterator_default_ctor) {
  container::iterator i;
  container::const_iterator j;

  container c;
  mass_push_back(c, {1, 2, 3, 4});

  i = c.begin();
  j = c.begin();
  EXPECT_EQ(1, *i);
  EXPECT_EQ(1, *j);
}

TEST_F(correctness_test, iterator_swap) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3});
  mass_push_back(c2, {4, 5, 6});

  container::iterator i = std::next(c1.begin(), 2);
  container::iterator j = c2.begin();

  {
    using std::swap;
    swap(i, j);
  }

  c1.erase(j);
  c2.erase(i);
  expect_eq(c1, {1, 2});
  expect_eq(c2, {5, 6});
}

TEST_F(correctness_test, reverse_iterator) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});
  expect_eq(reverse_view(c), {4, 3, 2, 1});

  EXPECT_EQ(4, *c.rbegin());
  EXPECT_EQ(3, *std::next(c.rbegin()));
  EXPECT_EQ(1, *std::prev(c.rend()));
}

TEST_F(correctness_test, iterator_constness) {
  container c;
  mass_push_back(c, {1, 2, 3});

  magic(*std::as_const(c).begin());
  magic(*std::prev(std::as_const(c).end()));
  expect_eq(c, {1, 2, 3});
}

TEST_F(correctness_test, reverse_iterator_constness) {
  container c;
  mass_push_back(c, {1, 2, 3});

  magic(*std::as_const(c).rbegin());
  magic(*std::prev(std::as_const(c).rend()));
  expect_eq(c, {1, 2, 3});
}

TEST_F(correctness_test, iterator_value_type) {
  container c;
  mass_push_back(c, {1, 2, 3});

  container::iterator::value_type e = *c.begin();
  e = 42;
  expect_eq(c, {1, 2, 3});
}

TEST_F(correctness_test, iterator_value_type_const) {
  container c;
  mass_push_back(c, {1, 2, 3});

  container::const_iterator::value_type e = *std::as_const(c).begin();
  e = 42;
  expect_eq(c, {1, 2, 3});
}

TEST_F(correctness_test, iterator_conversions) {
  container c;
  container::const_iterator i1 = c.begin();
  container::iterator i2 = c.end();

  EXPECT_TRUE(i1 == i1);
  EXPECT_TRUE(i1 == i2);
  EXPECT_TRUE(i2 == i1);
  EXPECT_TRUE(i2 == i2);
  EXPECT_FALSE(i1 != i1);
  EXPECT_FALSE(i1 != i2);
  EXPECT_FALSE(i2 != i1);
  EXPECT_FALSE(i2 != i2);

  EXPECT_TRUE(std::as_const(i1) == i1);
  EXPECT_TRUE(std::as_const(i1) == i2);
  EXPECT_TRUE(std::as_const(i2) == i1);
  EXPECT_TRUE(std::as_const(i2) == i2);
  EXPECT_FALSE(std::as_const(i1) != i1);
  EXPECT_FALSE(std::as_const(i1) != i2);
  EXPECT_FALSE(std::as_const(i2) != i1);
  EXPECT_FALSE(std::as_const(i2) != i2);

  EXPECT_TRUE(i1 == std::as_const(i1));
  EXPECT_TRUE(i1 == std::as_const(i2));
  EXPECT_TRUE(i2 == std::as_const(i1));
  EXPECT_TRUE(i2 == std::as_const(i2));
  EXPECT_FALSE(i1 != std::as_const(i1));
  EXPECT_FALSE(i1 != std::as_const(i2));
  EXPECT_FALSE(i2 != std::as_const(i1));
  EXPECT_FALSE(i2 != std::as_const(i2));

  EXPECT_TRUE(std::as_const(i1) == std::as_const(i1));
  EXPECT_TRUE(std::as_const(i1) == std::as_const(i2));
  EXPECT_TRUE(std::as_const(i2) == std::as_const(i1));
  EXPECT_TRUE(std::as_const(i2) == std::as_const(i2));
  EXPECT_FALSE(std::as_const(i1) != std::as_const(i1));
  EXPECT_FALSE(std::as_const(i1) != std::as_const(i2));
  EXPECT_FALSE(std::as_const(i2) != std::as_const(i1));
  EXPECT_FALSE(std::as_const(i2) != std::as_const(i2));
}

TEST_F(correctness_test, iterator_compare_1) {
  container c;

  EXPECT_TRUE(c.begin() == c.end());
  EXPECT_FALSE(c.begin() != c.end());

  EXPECT_FALSE(c.begin() < c.end());
  EXPECT_FALSE(c.begin() > c.end());
  EXPECT_TRUE(c.begin() <= c.end());
  EXPECT_TRUE(c.begin() >= c.end());
}

TEST_F(correctness_test, iterator_compare_2) {
  container c;
  mass_push_back(c, {1, 2});

  container::const_iterator it1 = c.begin();
  container::const_iterator it2 = it1;

  EXPECT_EQ(it1, it2);
  EXPECT_LE(it1, it2);
  EXPECT_GE(it1, it2);

  EXPECT_FALSE(it1 < it2);
  EXPECT_FALSE(it1 > it2);

  ++it2;
  EXPECT_NE(it1, it2);
  EXPECT_LE(it1, it2);
  EXPECT_LT(it1, it2);
  EXPECT_GE(it2, it1);
  EXPECT_GT(it2, it1);

  container::const_iterator end = c.end();

  EXPECT_EQ(end, end);
  EXPECT_LE(end, end);
  EXPECT_GE(end, end);

  EXPECT_FALSE(end < end);
  EXPECT_FALSE(end > end);

  EXPECT_NE(it2, end);
  EXPECT_LT(it2, end);
  EXPECT_GT(end, it2);
}

TEST_F(correctness_test, iterator_operators) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  container::iterator it = c.begin();
  EXPECT_EQ(1, *it);
  EXPECT_EQ(2, *++it);
  EXPECT_EQ(2, *it++);
  EXPECT_EQ(3, *it);
  EXPECT_EQ(3, *it--);
  EXPECT_EQ(1, *--it);

  it += 3;
  EXPECT_EQ(4, *it);
  it -= 2;
  EXPECT_EQ(2, *it);
  EXPECT_EQ(3, *(it + 1));
  EXPECT_EQ(3, *(1 + it));
  EXPECT_EQ(1, *(it - 1));

  EXPECT_EQ(3, c.end() - it);

  EXPECT_EQ(3, it[1]);
  EXPECT_EQ(1, it[-1]);
}

TEST_F(correctness_test, iterator_add_positive) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::const_iterator it = c.begin();

  EXPECT_EQ(c.end(), it + c.size());

  for (std::ptrdiff_t i = 0; i <= c.size(); ++i) {
    EXPECT_EQ(advance(it, i), it + i);
    EXPECT_EQ(advance(it, i), i + it);

    auto it2 = it;
    it2 += i;
    EXPECT_EQ(advance(it, i), it2);
  }
}

TEST_F(correctness_test, iterator_sub_positive) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::const_iterator it = c.end();

  EXPECT_EQ(c.begin(), it - c.size());

  for (std::ptrdiff_t i = 0; i <= c.size(); ++i) {
    EXPECT_EQ(advance(it, -i), it - i);

    auto it2 = it;
    it2 -= i;
    EXPECT_EQ(advance(it, -i), it2);
  }
}

TEST_F(correctness_test, iterator_add_negative) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::const_iterator it = c.end();

  EXPECT_EQ(c.begin(), it + -as_diff(c.size()));

  for (std::ptrdiff_t i = 0; i <= c.size(); ++i) {
    EXPECT_EQ(advance(it, -i), it + -i);
    EXPECT_EQ(advance(it, -i), -i + it);

    auto it2 = it;
    it2 += -i;
    EXPECT_EQ(advance(it, -i), it2);
  }
}

TEST_F(correctness_test, iterator_sub_negative) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::const_iterator it = c.begin();

  EXPECT_EQ(c.end(), it - -as_diff(c.size()));

  for (std::ptrdiff_t i = 0; i <= c.size(); ++i) {
    EXPECT_EQ(advance(it, i), it - -i);

    auto it2 = it;
    it2 -= -i;
    EXPECT_EQ(advance(it, i), it2);
  }
}

TEST_F(correctness_test, iterator_diff) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::const_iterator it = c.begin();
  container::const_iterator end = c.end();

  EXPECT_EQ(0, it - it);
  EXPECT_EQ(0, end - end);
  EXPECT_EQ(as_diff(c.size()), end - it);
  EXPECT_EQ(-as_diff(c.size()), it - end);

  ++it;
  --end;

  EXPECT_EQ(as_diff(c.size() - 2), end - it);
  EXPECT_EQ(-as_diff(c.size() - 2), it - end);
}

TEST_F(correctness_test, iterator_subscript) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::const_iterator it = c.begin();

  EXPECT_EQ(c[0], it[0]);
  EXPECT_EQ(c[1], it[1]);

  EXPECT_EQ(&c[0], &it[0]);
  EXPECT_EQ(&c[1], &it[1]);

  ++it;

  EXPECT_EQ(c[0], it[-1]);
  EXPECT_EQ(c[1], it[0]);
  EXPECT_EQ(c[2], it[1]);

  EXPECT_EQ(&c[0], &it[-1]);
  EXPECT_EQ(&c[1], &it[0]);
  EXPECT_EQ(&c[2], &it[1]);
}

TEST_F(correctness_test, reserve) {
  container c;
  c.reserve(10);
  EXPECT_GE(c.capacity(), 10);
}

TEST_F(correctness_test, reserve_superfluous) {
  container c;
  c.reserve(10);
  c.reserve(5);
  EXPECT_GE(c.capacity(), 10);
}

TEST_F(correctness_test, insert_empty) {
  container c;

  container::iterator it = c.insert(c.begin(), 42);

  expect_eq(c, {42});
  EXPECT_EQ(c.begin(), it);
}

TEST_F(correctness_test, insert_singleton_begin) {
  container c;
  c.push_back(1);

  container::iterator it = c.insert(c.begin(), 42);

  expect_eq(c, {42, 1});
  EXPECT_EQ(c.begin(), it);
}

TEST_F(correctness_test, insert_singleton_end) {
  container c;
  c.push_back(1);

  container::iterator it = c.insert(c.begin() + 1, 42);

  expect_eq(c, {1, 42});
  EXPECT_EQ(c.begin() + 1, it);
}

TEST_F(correctness_test, insert_5_1) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::iterator it = c.insert(c.begin(), 42);

  expect_eq(c, {42, 1, 2, 3, 4, 5});
  EXPECT_EQ(c.begin(), it);
}

TEST_F(correctness_test, insert_5_2) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::iterator it = c.insert(c.begin() + 1, 42);

  expect_eq(c, {1, 42, 2, 3, 4, 5});
  EXPECT_EQ(c.begin() + 1, it);
}

TEST_F(correctness_test, insert_5_3) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::iterator it = c.insert(c.begin() + 2, 42);

  expect_eq(c, {1, 2, 42, 3, 4, 5});
  EXPECT_EQ(c.begin() + 2, it);
}

TEST_F(correctness_test, insert_5_4) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::iterator it = c.insert(c.begin() + 3, 42);

  expect_eq(c, {1, 2, 3, 42, 4, 5});
  EXPECT_EQ(c.begin() + 3, it);
}

TEST_F(correctness_test, insert_5_5) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::iterator it = c.insert(c.begin() + 4, 42);

  expect_eq(c, {1, 2, 3, 4, 42, 5});
  EXPECT_EQ(c.begin() + 4, it);
}

TEST_F(correctness_test, insert_5_6) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::iterator it = c.insert(c.begin() + 5, 42);

  expect_eq(c, {1, 2, 3, 4, 5, 42});
  EXPECT_EQ(c.begin() + 5, it);
}

TEST_F(correctness_test, erase_singleton) {
  container c;
  mass_push_back(c, {1});

  container::iterator it = c.erase(c.begin());

  expect_empty(c);
  EXPECT_EQ(c.begin(), it);
}

TEST_F(correctness_test, erase_pair_first) {
  container c;
  mass_push_back(c, {1, 2});

  container::iterator it = c.erase(c.begin());

  expect_eq(c, {2});
  EXPECT_EQ(c.begin(), it);
}

TEST_F(correctness_test, erase_pair_second) {
  container c;
  mass_push_back(c, {1, 2});

  container::iterator it = c.erase(c.begin() + 1);

  expect_eq(c, {1});
  EXPECT_EQ(c.begin() + 1, it);
}

TEST_F(correctness_test, erase_6_1) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6});

  container::iterator it = c.erase(c.begin());

  expect_eq(c, {2, 3, 4, 5, 6});
  EXPECT_EQ(c.begin(), it);
}

TEST_F(correctness_test, erase_6_2) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6});

  container::iterator it = c.erase(c.begin() + 1);

  expect_eq(c, {1, 3, 4, 5, 6});
  EXPECT_EQ(c.begin() + 1, it);
}

TEST_F(correctness_test, erase_6_3) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6});

  container::iterator it = c.erase(c.begin() + 2);

  expect_eq(c, {1, 2, 4, 5, 6});
  EXPECT_EQ(c.begin() + 2, it);
}

TEST_F(correctness_test, erase_6_4) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6});

  container::iterator it = c.erase(c.begin() + 3);

  expect_eq(c, {1, 2, 3, 5, 6});
  EXPECT_EQ(c.begin() + 3, it);
}

TEST_F(correctness_test, erase_6_5) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6});

  container::iterator it = c.erase(c.begin() + 4);

  expect_eq(c, {1, 2, 3, 4, 6});
  EXPECT_EQ(c.begin() + 4, it);
}

TEST_F(correctness_test, erase_6_6) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6});

  container::iterator it = c.erase(c.begin() + 5);

  expect_eq(c, {1, 2, 3, 4, 5});
  EXPECT_EQ(c.begin() + 5, it);
}

TEST_F(correctness_test, erase_range_short_prefix) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6, 7});

  container::iterator it = c.erase(c.begin(), c.begin() + 2);

  expect_eq(c, {3, 4, 5, 6, 7});
  EXPECT_EQ(c.begin(), it);
}

TEST_F(correctness_test, erase_range_long_prefix) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6, 7});

  container::iterator it = c.erase(c.begin(), c.begin() + 5);

  expect_eq(c, {6, 7});
  EXPECT_EQ(c.begin(), it);
}

TEST_F(correctness_test, erase_range_short_suffix) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6, 7});

  container::iterator it = c.erase(c.end() - 2, c.end());

  expect_eq(c, {1, 2, 3, 4, 5});
  EXPECT_EQ(c.end(), it);
}

TEST_F(correctness_test, erase_range_long_suffix) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6, 7});

  container::iterator it = c.erase(c.end() - 5, c.end());

  expect_eq(c, {1, 2});
  EXPECT_EQ(c.end(), it);
}

TEST_F(correctness_test, erase_range_middle_wide) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6, 7});

  container::iterator it = c.erase(c.begin() + 1, c.end() - 1);

  expect_eq(c, {1, 7});
  EXPECT_EQ(c.begin() + 1, it);
}

TEST_F(correctness_test, erase_range_middle_narrow_1) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6, 7});

  container::iterator it = c.erase(c.begin() + 2, c.end() - 3);

  expect_eq(c, {1, 2, 5, 6, 7});
  EXPECT_EQ(c.begin() + 2, it);
}

TEST_F(correctness_test, erase_range_middle_narrow_2) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6, 7});

  container::iterator it = c.erase(c.begin() + 3, c.end() - 2);

  expect_eq(c, {1, 2, 3, 6, 7});
  EXPECT_EQ(c.begin() + 3, it);
}

TEST_F(correctness_test, erase_range_middle_left) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6, 7, 8});

  container::iterator it = c.erase(c.begin() + 1, c.begin() + 3);

  expect_eq(c, {1, 4, 5, 6, 7, 8});
  EXPECT_EQ(c.begin() + 1, it);
}

TEST_F(correctness_test, erase_range_middle_right) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6, 7, 8});

  container::iterator it = c.erase(c.begin() + 5, c.begin() + 7);

  expect_eq(c, {1, 2, 3, 4, 5, 8});
  EXPECT_EQ(c.begin() + 5, it);
}

TEST_F(correctness_test, erase_range_whole) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::iterator it = c.erase(c.begin(), c.end());

  expect_empty(c);
  EXPECT_EQ(c.end(), it);
}

TEST_F(correctness_test, erase_range_null) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  for (container::iterator it = c.begin(); it != c.end(); ++it) {
    container::iterator res_it = c.erase(it, it);
    expect_eq(c, {1, 2, 3, 4, 5});
    EXPECT_EQ(it, res_it);
  }
}

TEST_F(correctness_test, erase_range_6) {
  container c;
  mass_push_back(c, {1, 2});
  container::iterator it = c.erase(c.begin(), c.end());

  EXPECT_TRUE(c.empty());
  EXPECT_TRUE(it == c.begin());
}
