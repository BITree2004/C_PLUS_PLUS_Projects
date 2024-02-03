#include "element.h"
#include "fault-injection.h"
#include "list.h"
#include "test-utils.h"

#include <gtest/gtest.h>

static_assert(!std::constructible_from<container::iterator, std::nullptr_t>,
              "iterator should not be constructible from nullptr");
static_assert(!std::constructible_from<container::const_iterator, std::nullptr_t>,
              "const_iterator should not be constructible from nullptr");
static_assert(!std::constructible_from<container::reverse_iterator, std::nullptr_t>,
              "reverse_iterator should not be constructible from nullptr");
static_assert(!std::constructible_from<container::const_reverse_iterator, std::nullptr_t>,
              "const_reverse_iterator should not be constructible from nullptr");

static_assert(std::bidirectional_iterator<container::iterator>);
static_assert(std::bidirectional_iterator<container::const_iterator>);
static_assert(std::bidirectional_iterator<container::reverse_iterator>);
static_assert(std::bidirectional_iterator<container::const_reverse_iterator>);

namespace {
class correctness_test : public base_test {};

class exception_safety_test : public base_test {};

class performance_test : public base_test {};

struct non_default_constructible {
  non_default_constructible() = delete;
};

void magic(element& c) {
  c = 42;
}

void magic([[maybe_unused]] const element& c) {}

} // namespace

TEST_F(correctness_test, non_default_type) {
  list<non_default_constructible> c;
  expect_empty(c);
}

TEST_F(correctness_test, default_ctor) {
  container c;
  instances_guard.expect_no_instances();
}

TEST_F(correctness_test, end_iterator) {
  container c;
  container::iterator i = c.end();

  EXPECT_EQ(c.begin(), i);
  c.push_back(5);
  --i;
  EXPECT_EQ(5, *i);
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

TEST_F(correctness_test, iterator_pre_inc) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6});

  container::iterator i = std::next(c.begin(), 3);
  ++ ++i;
  EXPECT_EQ(6, *i);
}

TEST_F(correctness_test, const_iterator_pre_inc) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5, 6});

  container::const_iterator i = std::next(c.begin(), 3);
  ++ ++i;
  EXPECT_EQ(6, *i);
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

TEST_F(correctness_test, push_back) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});
  expect_eq(c, {1, 2, 3, 4});
}

TEST_F(correctness_test, range_ctor) {
  auto range = {1, 2, 3, 4};
  container c(range.begin(), range.end());
  expect_eq(c, range);
}

TEST_F(correctness_test, range_ctor_empty) {
  auto int_ptr = static_cast<const int*>(nullptr);
  container c(int_ptr, int_ptr);
  expect_empty(c);
}

TEST_F(correctness_test, copy_ctor) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  container c2 = c;
  expect_eq(c2, {1, 2, 3, 4});
}

TEST_F(correctness_test, copy_ctor_empty) {
  container c;
  container c2 = c;
  expect_empty(c2);
}

TEST_F(correctness_test, copy_assignment) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  container c2;
  mass_push_back(c2, {5, 6, 7, 8});

  c2 = c;
  expect_eq(c2, {1, 2, 3, 4});
}

TEST_F(correctness_test, copy_assignment_self) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  c = c;
  expect_eq(c, {1, 2, 3, 4});
}

TEST_F(correctness_test, pop_back) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});
  c.pop_back();
  expect_eq(c, {1, 2, 3});
  c.pop_back();
  expect_eq(c, {1, 2});
  c.pop_back();
  expect_eq(c, {1});
  c.pop_back();
  expect_empty(c);
}

TEST_F(correctness_test, push_front) {
  container c;
  mass_push_front(c, {1, 2, 3, 4});
  expect_eq(c, {4, 3, 2, 1});
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

TEST_F(correctness_test, reverse_iterators) {
  container c;
  mass_push_front(c, {1, 2, 3, 4});
  expect_eq(reverse_view(c), {1, 2, 3, 4});

  EXPECT_EQ(1, *c.rbegin());
  EXPECT_EQ(2, *std::next(c.rbegin()));
  EXPECT_EQ(4, *std::prev(c.rend()));
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

TEST_F(correctness_test, iterators_postfix) {
  container s;
  mass_push_back(s, {1, 2, 3});

  container::iterator i = s.begin();
  EXPECT_EQ(1, *i);
  container::iterator j = i++;
  EXPECT_EQ(2, *i);
  EXPECT_EQ(1, *j);
  j = i++;
  EXPECT_EQ(3, *i);
  EXPECT_EQ(2, *j);
  j = i++;
  EXPECT_EQ(s.end(), i);
  EXPECT_EQ(3, *j);
  j = i--;
  EXPECT_EQ(3, *i);
  EXPECT_EQ(s.end(), j);
}

TEST_F(correctness_test, const_iterators_postfix) {
  container s;
  mass_push_back(s, {1, 2, 3});

  container::const_iterator i = s.begin();
  EXPECT_EQ(1, *i);
  container::const_iterator j = i++;
  EXPECT_EQ(2, *i);
  EXPECT_EQ(1, *j);
  j = i++;
  EXPECT_EQ(3, *i);
  EXPECT_EQ(2, *j);
  j = i++;
  EXPECT_TRUE(i == s.end());
  EXPECT_EQ(3, *j);
  j = i--;
  EXPECT_EQ(3, *i);
  EXPECT_TRUE(j == s.end());
}

TEST_F(correctness_test, insert_begin) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  c.insert(c.begin(), 0);
  expect_eq(c, {0, 1, 2, 3, 4});
}

TEST_F(correctness_test, insert_middle) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  c.insert(std::next(c.begin(), 2), 5);
  expect_eq(c, {1, 2, 5, 3, 4});
}

TEST_F(correctness_test, insert_end) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  c.insert(c.end(), 5);
  expect_eq(c, {1, 2, 3, 4, 5});
}

TEST_F(correctness_test, insert_iterators) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  container::iterator i2 = c.begin();
  ++i2;
  container::iterator i3 = i2;
  ++i3;

  c.insert(i3, 5);

  EXPECT_EQ(5, *--i3);
  EXPECT_EQ(2, *--i3);
  EXPECT_EQ(5, *++i2);
  EXPECT_EQ(3, *++i2);
}

TEST_F(correctness_test, insert_return_value) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  container::iterator i = c.insert(std::next(c.begin(), 2), 5);
  EXPECT_EQ(5, *i);
  EXPECT_EQ(2, *std::prev(i));
  EXPECT_EQ(3, *std::next(i));
}

TEST_F(correctness_test, insert_range_empty) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  auto int_ptr = static_cast<const int*>(nullptr);
  c.insert(std::next(c.begin(), 2), int_ptr, int_ptr);
  expect_eq(c, {1, 2, 3, 4});
}

TEST_F(correctness_test, insert_range_empty_return_value) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  auto int_ptr = static_cast<const int*>(nullptr);
  container::iterator i = c.insert(std::next(c.begin(), 2), int_ptr, int_ptr);
  EXPECT_EQ(std::next(c.begin(), 2), i);
  EXPECT_EQ(3, *i);
}

TEST_F(correctness_test, insert_range_whole) {
  container c;

  auto range = {1, 2, 3, 4};
  c.insert(c.begin(), range.begin(), range.end());
  expect_eq(c, range);
}

TEST_F(correctness_test, insert_range_whole_return_value) {
  container c;

  auto range = {1, 2, 3, 4};
  container::iterator i = c.insert(c.begin(), range.begin(), range.end());
  EXPECT_EQ(c.begin(), i);
  EXPECT_EQ(1, *i);
}

TEST_F(correctness_test, insert_range_whole_empty) {
  container c;

  auto int_ptr = static_cast<const int*>(nullptr);
  c.insert(c.begin(), int_ptr, int_ptr);
  expect_empty(c);
}

TEST_F(correctness_test, insert_range_whole_empty_return_value) {
  container c;

  auto int_ptr = static_cast<const int*>(nullptr);
  container::iterator i = c.insert(c.begin(), int_ptr, int_ptr);
  EXPECT_EQ(c.end(), i);
}

TEST_F(correctness_test, insert_range_begin) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  auto range = {5, 6, 7, 8};
  c.insert(c.begin(), range.begin(), range.end());
  expect_eq(c, {5, 6, 7, 8, 1, 2, 3, 4});
}

TEST_F(correctness_test, insert_range_begin_return_value) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  auto range = {5, 6, 7, 8};
  container::iterator i = c.insert(c.begin(), range.begin(), range.end());
  EXPECT_EQ(c.begin(), i);
  EXPECT_EQ(5, *i);
}

TEST_F(correctness_test, insert_range_middle) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  auto range = {5, 6, 7, 8};
  c.insert(std::next(c.begin(), 2), range.begin(), range.end());
  expect_eq(c, {1, 2, 5, 6, 7, 8, 3, 4});
}

TEST_F(correctness_test, insert_range_middle_return_value) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  auto range = {5, 6, 7, 8};
  container::iterator i = c.insert(std::next(c.begin(), 2), range.begin(), range.end());
  EXPECT_EQ(std::next(c.begin(), 2), i);
  EXPECT_EQ(5, *i);
}

TEST_F(correctness_test, insert_range_end) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  auto range = {5, 6, 7, 8};
  c.insert(c.end(), range.begin(), range.end());
  expect_eq(c, {1, 2, 3, 4, 5, 6, 7, 8});
}

TEST_F(correctness_test, insert_range_end_return_value) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  auto range = {5, 6, 7, 8};
  container::iterator i = c.insert(c.end(), range.begin(), range.end());
  EXPECT_EQ(std::next(c.begin(), 4), i);
  EXPECT_EQ(5, *i);
}

TEST_F(correctness_test, insert_range_iterators) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  container::iterator i2 = c.begin();
  ++i2;
  container::iterator i3 = i2;
  ++i3;

  auto range = {5, 6};
  c.insert(i3, range.begin(), range.end());

  EXPECT_EQ(6, *--i3);
  EXPECT_EQ(5, *--i3);
  EXPECT_EQ(2, *--i3);
  EXPECT_EQ(5, *++i2);
  EXPECT_EQ(6, *++i2);
  EXPECT_EQ(3, *++i2);
}

TEST_F(correctness_test, erase_begin) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  c.erase(c.begin());
  expect_eq(c, {2, 3, 4});
}

TEST_F(correctness_test, erase_middle) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  c.erase(std::next(c.begin(), 2));
  expect_eq(c, {1, 2, 4});
}

TEST_F(correctness_test, erase_end) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  c.erase(std::prev(c.end()));
  expect_eq(c, {1, 2, 3});
}

TEST_F(correctness_test, erase_return_value) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  container::iterator i = c.erase(std::next(std::as_const(c).begin()));
  EXPECT_EQ(3, *i);
  i = c.erase(i);
  EXPECT_EQ(4, *i);
}

TEST_F(correctness_test, erase_iterators) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  container::iterator i2 = c.begin();
  ++i2;
  container::iterator i3 = i2;
  ++i3;
  container::iterator i4 = i3;
  ++i4;

  c.erase(i3);

  EXPECT_EQ(2, *--i4);
  EXPECT_EQ(4, *++i2);
}

TEST_F(correctness_test, erase_range) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  c.erase(std::next(std::as_const(c).begin()), std::next(std::as_const(c).begin(), 3));
  expect_eq(c, {1, 4, 5});
}

TEST_F(correctness_test, erase_range_return_value) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::iterator i = c.erase(std::next(std::as_const(c).begin()), std::next(std::as_const(c).begin(), 3));
  EXPECT_EQ(4, *i);
  i = c.erase(i);
  EXPECT_EQ(5, *i);
}

TEST_F(correctness_test, erase_range_whole) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  c.erase(c.begin(), c.end());
  expect_empty(c);
}

TEST_F(correctness_test, erase_range_whole_return_value) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  container::iterator i = c.erase(c.begin(), c.end());
  EXPECT_EQ(c.end(), i);
}

TEST_F(correctness_test, erase_range_empty_begin) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  c.erase(c.begin(), c.begin());
  expect_eq(c, {1, 2, 3, 4});
}

TEST_F(correctness_test, erase_range_empty_begin_return_value) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  container::iterator i = c.erase(c.begin(), c.begin());
  EXPECT_EQ(c.begin(), i);
  EXPECT_EQ(1, *i);
}

TEST_F(correctness_test, erase_range_empty_end) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  c.erase(c.end(), c.end());
  expect_eq(c, {1, 2, 3, 4});
}

TEST_F(correctness_test, erase_range_empty_end_return_value) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  container::iterator i = c.erase(c.end(), c.end());
  EXPECT_EQ(c.end(), i);
  EXPECT_EQ(4, *--i);
}

TEST_F(correctness_test, erase_upto_end) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  c.erase(std::next(std::as_const(c).begin(), 2), std::as_const(c).end());
  expect_eq(c, {1, 2});
}

TEST_F(correctness_test, erase_upto_end_return_value) {
  container c;
  mass_push_back(c, {1, 2, 3, 4, 5});

  container::iterator i = c.erase(std::next(std::as_const(c).begin(), 2), std::as_const(c).end());
  EXPECT_EQ(c.end(), i);
  EXPECT_EQ(2, *--i);
}

TEST_F(correctness_test, splice_begin_begin) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(c1.begin(), c2, c2.begin(), std::next(c2.begin(), 2));
  expect_eq(c1, {5, 6, 1, 2, 3, 4});
  expect_eq(c2, {7, 8});
}

TEST_F(correctness_test, splice_begin_middle) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(c1.begin(), c2, std::next(c2.begin()), std::next(c2.begin(), 2));
  expect_eq(c1, {6, 1, 2, 3, 4});
  expect_eq(c2, {5, 7, 8});
}

TEST_F(correctness_test, splice_begin_end) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(c1.begin(), c2, std::next(c2.begin(), 2), c2.end());
  expect_eq(c1, {7, 8, 1, 2, 3, 4});
  expect_eq(c2, {5, 6});
}

TEST_F(correctness_test, splice_begin_whole) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(c1.begin(), c2, c2.begin(), c2.end());
  expect_eq(c1, {5, 6, 7, 8, 1, 2, 3, 4});
  expect_empty(c2);
}

TEST_F(correctness_test, splice_begin_empty) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(c1.begin(), c2, std::next(c2.begin(), 2), std::next(c2.begin(), 2));
  expect_eq(c1, {1, 2, 3, 4});
  expect_eq(c2, {5, 6, 7, 8});
}

TEST_F(correctness_test, splice_middle_begin) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(std::next(c1.begin(), 2), c2, c2.begin(), std::next(c2.begin(), 2));
  expect_eq(c1, {1, 2, 5, 6, 3, 4});
  expect_eq(c2, {7, 8});
}

TEST_F(correctness_test, splice_middle_middle) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(std::next(c1.begin(), 2), c2, std::next(c2.begin()), std::next(c2.begin(), 3));
  expect_eq(c1, {1, 2, 6, 7, 3, 4});
  expect_eq(c2, {5, 8});
}

TEST_F(correctness_test, splice_middle_end) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(std::next(c1.begin(), 2), c2, std::next(c2.begin(), 2), c2.end());
  expect_eq(c1, {1, 2, 7, 8, 3, 4});
  expect_eq(c2, {5, 6});
}

TEST_F(correctness_test, splice_middle_whole) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(std::next(c1.begin(), 2), c2, c2.begin(), c2.end());
  expect_eq(c1, {1, 2, 5, 6, 7, 8, 3, 4});
  EXPECT_TRUE(c2.empty());
}

TEST_F(correctness_test, splice_middle_empty) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(std::next(c1.begin(), 2), c2, std::next(c2.begin(), 2), std::next(c2.begin(), 2));
  expect_eq(c1, {1, 2, 3, 4});
  expect_eq(c2, {5, 6, 7, 8});
}

TEST_F(correctness_test, splice_end_begin) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(c1.end(), c2, c2.begin(), std::next(c2.begin(), 2));
  expect_eq(c1, {1, 2, 3, 4, 5, 6});
  expect_eq(c2, {7, 8});
}

TEST_F(correctness_test, splice_end_middle) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(c1.end(), c2, std::next(c2.begin()), std::next(c2.begin(), 3));
  expect_eq(c1, {1, 2, 3, 4, 6, 7});
  expect_eq(c2, {5, 8});
}

TEST_F(correctness_test, splice_end_end) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(c1.end(), c2, std::next(c2.begin(), 2), c2.end());
  expect_eq(c1, {1, 2, 3, 4, 7, 8});
  expect_eq(c2, {5, 6});
}

TEST_F(correctness_test, splice_end_whole) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(c1.end(), c2, c2.begin(), c2.end());
  expect_eq(c1, {1, 2, 3, 4, 5, 6, 7, 8});
  expect_empty(c2);
}

TEST_F(correctness_test, splice_end_empty) {
  container c1, c2;
  mass_push_back(c1, {1, 2, 3, 4});
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(c1.end(), c2, std::next(c2.begin(), 2), std::next(c2.begin(), 2));
  expect_eq(c1, {1, 2, 3, 4});
  expect_eq(c2, {5, 6, 7, 8});
}

TEST_F(correctness_test, splice_empty_begin) {
  container c1, c2;
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(c1.end(), c2, c2.begin(), std::next(c2.begin(), 2));
  expect_eq(c1, {5, 6});
  expect_eq(c2, {7, 8});
}

TEST_F(correctness_test, splice_empty_middle) {
  container c1, c2;
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(c1.end(), c2, std::next(c2.begin(), 1), std::next(c2.begin(), 3));
  expect_eq(c1, {6, 7});
  expect_eq(c2, {5, 8});
}

TEST_F(correctness_test, splice_empty_end) {
  container c1, c2;
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(c1.end(), c2, std::next(c2.begin(), 2), c2.end());
  expect_eq(c1, {7, 8});
  expect_eq(c2, {5, 6});
}

TEST_F(correctness_test, splice_empty_whole) {
  container c1, c2;
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(c1.end(), c2, c2.begin(), c2.end());
  expect_eq(c1, {5, 6, 7, 8});
  expect_empty(c2);
}

TEST_F(correctness_test, splice_empty_empty) {
  container c1, c2;
  mass_push_back(c2, {5, 6, 7, 8});

  c1.splice(c1.end(), c2, c2.begin(), c2.begin());
  expect_empty(c1);
  expect_eq(c2, {5, 6, 7, 8});
}

TEST_F(correctness_test, splice_self) {
  container c1;
  mass_push_back(c1, {1, 2, 3, 4, 5});

  c1.splice(std::next(c1.begin()), c1, std::next(c1.begin(), 2), std::prev(c1.end()));
  expect_eq(c1, {1, 3, 4, 2, 5});
}

TEST_F(correctness_test, splice_iterators) {
  container c1;
  container c2;
  mass_push_back(c1, {1, 2, 3, 4});
  mass_push_back(c2, {5, 6, 7, 8});

  container::const_iterator i = std::next(c1.begin(), 2);
  container::const_iterator j = std::next(c2.begin());
  container::const_iterator k = std::prev(c2.end());

  c1.splice(i, c2, j, k);
  expect_eq(c1, {1, 2, 6, 7, 3, 4});
  expect_eq(c2, {5, 8});

  EXPECT_EQ(3, *i);
  EXPECT_EQ(6, *j);
  EXPECT_EQ(8, *k);

  EXPECT_EQ(7, *std::prev(i));
  EXPECT_EQ(2, *std::prev(j));
  EXPECT_EQ(5, *std::prev(k));
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
  container c1;
  mass_push_back(c1, {1, 2, 3, 4});

  swap(c1, c1);
  expect_eq(c1, {1, 2, 3, 4});
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
}

TEST_F(correctness_test, swap_empty_self) {
  container c1;
  swap(c1, c1);
}

TEST_F(correctness_test, clear_empty) {
  container c;
  c.clear();
  expect_empty(c);
  c.clear();
  expect_empty(c);
  c.clear();
  expect_empty(c);
}

TEST_F(correctness_test, clear) {
  container c;
  mass_push_back(c, {1, 2, 3, 4});

  c.clear();
  expect_empty(c);

  mass_push_back(c, {5, 6, 7, 8});
  expect_eq(c, {5, 6, 7, 8});
}

TEST_F(exception_safety_test, push_back) {
  faulty_run([] {
    container c;

    auto range = {1, 2, 3, 4};
    for (int e : range) {
      strong_exception_safety_guard sg(c);
      c.push_back(e);
    }
    expect_eq(c, range);
  });
}

TEST_F(exception_safety_test, push_front) {
  faulty_run([] {
    container c;

    auto range = {1, 2, 3, 4};
    for (int e : reverse_view(range)) {
      strong_exception_safety_guard sg(c);
      c.push_front(e);
    }
    expect_eq(c, range);
  });
}

TEST_F(exception_safety_test, insert_range) {
  faulty_run([] {
    container c;
    mass_push_back(c, {1, 2, 3, 4});
    strong_exception_safety_guard sg(c);

    auto range = {5, 6, 7, 8};
    c.insert(std::next(c.begin(), 2), range.begin(), range.end());
    expect_eq(c, {1, 2, 5, 6, 7, 8, 3, 4});
  });
}

TEST_F(exception_safety_test, range_ctor) {
  faulty_run([] {
    auto range = {1, 2, 3, 4};
    container c(range.begin(), range.end());
    expect_eq(c, {1, 2, 3, 4});
  });
}

TEST_F(exception_safety_test, copy_ctor) {
  faulty_run([] {
    container c;
    mass_push_back(c, {1, 2, 3, 4});

    container c2 = c;
    expect_eq(c2, {1, 2, 3, 4});
  });
}

TEST_F(exception_safety_test, copy_assignment) {
  faulty_run([] {
    container c;
    mass_push_back(c, {1, 2, 3, 4});

    container c2;
    mass_push_back(c2, {5, 6, 7, 8});
    strong_exception_safety_guard sg(c2);

    c2 = c;
    expect_eq(c2, {1, 2, 3, 4});
  });
}

TEST_F(performance_test, size) {
  constexpr size_t N = 100'000;
  constexpr size_t K = 1'000'000;

  container c;

  for (size_t i = 0; i < N; ++i) {
    EXPECT_EQ(i, c.size());
    c.push_back(i);
  }

  for (size_t i = 0; i < K; ++i) {
    EXPECT_EQ(N, c.size());
  }
}

TEST_F(performance_test, splice) {
  constexpr size_t N = 100'000;
  constexpr size_t K = 1'000'000;

  container c1;
  container c2;

  for (size_t i = 0; i < N; ++i) {
    c1.push_back(i);
  }

  for (size_t i = 0; i < K; ++i) {
    if (i % 2 == 0) {
      c2.splice(c2.begin(), c1, c1.begin(), c1.end());
    } else {
      c1.splice(c1.begin(), c2, c2.begin(), c2.end());
    }
  }
}

TEST_F(performance_test, splice_self) {
  constexpr size_t N = 100'000;
  constexpr size_t K = 1'000'000;

  container c;

  for (size_t i = 0; i < N; ++i) {
    c.push_back(i);
  }

  for (size_t i = 0; i < K; ++i) {
    c.splice(c.begin(), c, std::next(c.begin()), std::prev(c.end()));
  }
}

TEST_F(performance_test, swap) {
  constexpr size_t N = 50'000;
  constexpr size_t K = 1'000'000;

  container c1;
  container c2;

  for (size_t i = 0; i < N; ++i) {
    c1.push_back(i * 2);
    c2.push_back(i * 2 + 1);
  }

  for (size_t i = 0; i < K; ++i) {
    swap(c1, c2);
  }
}
