#include "socow-vector.h"
#include "test-utils.h"

#include <gtest/gtest.h>

using std::as_const;

class small_object_test : public base_test {};

TEST_F(small_object_test, push_back) {
  container a;

  for (size_t i = 0; i < 3; ++i) {
    a.push_back(i + 100);
    EXPECT_EQ(i + 1, a.size());
    expect_static_storage(a);
  }

  for (size_t i = 0; i < 3; ++i) {
    EXPECT_EQ(i + 100, a[i]);
  }
}

TEST_F(small_object_test, push_back_throw) {
  container a;

  for (size_t i = 0; i < 3; ++i) {
    {
      immutable_guard g(a);
      element::set_copy_throw_countdown(1);
      EXPECT_THROW(a.push_back(42), std::runtime_error);
    }
    a.push_back(i + 100);
    ASSERT_EQ(i + 1, a.size());
    expect_static_storage(a);
  }

  for (size_t i = 1; i <= 4; ++i) {
    immutable_guard g(a);
    element::set_copy_throw_countdown(i);
    EXPECT_THROW(a.push_back(42), std::runtime_error);
  }
}

TEST_F(small_object_test, push_back_copies) {
  container a;

  element::reset_counters();

  for (size_t i = 0; i < 3; ++i) {
    a.push_back(i + 100);
  }

  EXPECT_EQ(3, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());
}

TEST_F(small_object_test, push_back_small_into_big) {
  container a;

  for (size_t i = 0; i < 4; ++i) {
    a.push_back(i + 100);
  }

  ASSERT_EQ(4, a.size());
  ASSERT_LE(4, a.capacity());

  for (size_t i = 0; i < 4; ++i) {
    EXPECT_EQ(i + 100, a[i]);
  }
}

TEST_F(small_object_test, push_back_small_into_big_copies) {
  container a;

  for (size_t i = 0; i < 3; ++i) {
    a.push_back(i + 100);
  }

  expect_static_storage(a);

  element::reset_counters();

  a.push_back(103);

  ASSERT_EQ(4, a.size());
  ASSERT_LE(4, a.capacity());

  EXPECT_EQ(4, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  for (size_t i = 0; i < 4; ++i) {
    EXPECT_EQ(i + 100, a[i]);
  }
}

TEST_F(small_object_test, pop_back) {
  container a;
  a.push_back(3);
  a.push_back(7);

  a.pop_back();
  ASSERT_EQ(1, a.size());
  expect_static_storage(a);
  EXPECT_EQ(3, a[0]);

  a.pop_back();
  expect_empty_storage(a);
  element::assert_no_instances();
}

TEST_F(small_object_test, pop_back_big_into_small) {
  container a;
  for (size_t i = 0; i < 4; ++i) {
    a.push_back(i + 100);
  }
  ASSERT_EQ(4, a.size());

  container b = a;

  element::reset_counters();
  a.pop_back();
  EXPECT_GE(3, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  EXPECT_EQ(3, a.size());
}

TEST_F(small_object_test, pop_back_big_into_small_single_user) {
  container a;
  for (size_t i = 0; i < 4; ++i) {
    a.push_back(i + 100);
  }
  ASSERT_EQ(4, a.size());

  size_t old_capacity = a.capacity();
  const element* old_data = a.data();

  a.pop_back();
  EXPECT_EQ(3, a.size());
  EXPECT_EQ(old_capacity, a.capacity());
  EXPECT_EQ(old_data, a.data());
}

TEST_F(small_object_test, pop_back_big_into_small_throw) {
  container a;
  for (size_t i = 0; i < 4; ++i) {
    a.push_back(i + 100);
  }
  ASSERT_EQ(4, a.size());

  immutable_guard ga(a);

  for (size_t i = 1; i <= 3; ++i) {
    container b = a;
    immutable_guard gb(b);

    element::set_copy_throw_countdown(i);
    EXPECT_THROW(b.pop_back(), std::runtime_error);
  }
}

TEST_F(small_object_test, subscript) {
  container a;
  a.push_back(3);
  a.push_back(7);

  EXPECT_EQ(3, a[0]);
  EXPECT_EQ(7, a[1]);

  expect_static_storage(a);
}

TEST_F(small_object_test, subscript_const) {
  container a;
  a.push_back(3);
  a.push_back(7);

  EXPECT_EQ(3, as_const(a)[0]);
  EXPECT_EQ(7, as_const(a)[1]);

  expect_static_storage(a);
}

TEST_F(small_object_test, front_back) {
  container a;
  a.push_back(3);
  a.push_back(7);

  EXPECT_EQ(3, a.front());
  EXPECT_EQ(7, a.back());

  EXPECT_EQ(a.data() + 0, &a.front());
  EXPECT_EQ(a.data() + 1, &a.back());

  expect_static_storage(a);
}

TEST_F(small_object_test, front_back_const) {
  container a;
  a.push_back(3);
  a.push_back(7);

  EXPECT_EQ(3, as_const(a).front());
  EXPECT_EQ(7, as_const(a).back());

  EXPECT_EQ(as_const(a).data() + 0, &as_const(a).front());
  EXPECT_EQ(as_const(a).data() + 1, &as_const(a).back());

  expect_static_storage(a);
}

TEST_F(small_object_test, data) {
  container a;
  a.push_back(3);
  a.push_back(7);

  EXPECT_EQ(&a[0], a.data());
  EXPECT_EQ(&a[1], a.data() + 1);

  expect_static_storage(a);
}

TEST_F(small_object_test, data_const) {
  container a;
  a.push_back(3);
  a.push_back(7);

  EXPECT_EQ(&as_const(a)[0], as_const(a).data());
  EXPECT_EQ(&as_const(a)[1], as_const(a).data() + 1);

  expect_static_storage(a);
}

TEST_F(small_object_test, copy_empty) {
  container a;
  expect_empty_storage(a);

  container b = a;
  expect_empty_storage(b);

  a = b;
  expect_empty_storage(a);

  a = a;
  expect_empty_storage(a);

  element::assert_no_instances();
}

TEST_F(small_object_test, copy_ctor) {
  container a;
  a.push_back(3);
  a.push_back(7);

  container b = a;
  ASSERT_EQ(2, b.size());
  expect_static_storage(b);
  EXPECT_EQ(3, b[0]);
  EXPECT_EQ(7, b[1]);
}

TEST_F(small_object_test, copy_ctor_throw) {
  container a;
  a.push_back(3);
  a.push_back(7);

  {
    immutable_guard g(a);
    element::set_copy_throw_countdown(1);
    EXPECT_THROW(container{a}, std::runtime_error);
  }
  {
    immutable_guard g(a);
    element::set_copy_throw_countdown(2);
    EXPECT_THROW(container{a}, std::runtime_error);
  }
}

TEST_F(small_object_test, copy_assignment_small_to_small) {
  container a;
  a.push_back(3);
  a.push_back(5);
  a.push_back(7);

  container b;
  b.push_back(42);

  element::reset_counters();
  b = a;
  EXPECT_GE(4, element::get_copy_counter());
  EXPECT_GE(1, element::get_swap_counter());

  ASSERT_EQ(3, b.size());
  expect_static_storage(b);
  EXPECT_EQ(3, b[0]);
  EXPECT_EQ(5, b[1]);
  EXPECT_EQ(7, b[2]);
}

TEST_F(small_object_test, copy_assignment_small_to_small_throw) {
  container a;
  a.push_back(3);
  a.push_back(5);
  a.push_back(7);

  container b;
  b.push_back(42);

  immutable_guard ga(a);

  for (size_t i = 1; i <= 3; ++i) {
    immutable_guard gb(b);
    element::set_copy_throw_countdown(i);
    element::reset_counters();
    EXPECT_THROW(b = a, std::runtime_error);
    EXPECT_GE(i, element::get_copy_counter());
    EXPECT_EQ(0, element::get_swap_counter());
  }

  {
    container old_b = b;
    immutable_guard gb(b);
    element::set_copy_throw_countdown(4);
    element::reset_counters();
    try {
      b = a;
      element::set_copy_throw_countdown(0);
      b = old_b;
    } catch (const std::runtime_error&) {
      EXPECT_EQ(0, element::get_swap_counter());
    }
  }

  element::set_swap_throw_countdown(1);
  try {
    b = a;
  } catch (const std::runtime_error&) {}
}

TEST_F(small_object_test, copy_assignment_small_to_small_2) {
  container a;
  a.push_back(42);
  a.push_back(43);

  container b;
  b.push_back(3);
  b.push_back(5);
  b.push_back(7);

  element::reset_counters();
  b = a;
  EXPECT_GE(4, element::get_copy_counter());
  EXPECT_GE(2, element::get_swap_counter());

  ASSERT_EQ(2, b.size());
  expect_static_storage(b);
  EXPECT_EQ(42, b[0]);
  EXPECT_EQ(43, b[1]);
}

TEST_F(small_object_test, copy_assignment_small_to_small_2_throw) {
  container a;
  a.push_back(42);
  a.push_back(43);

  container b;
  b.push_back(3);
  b.push_back(5);
  b.push_back(7);

  immutable_guard ga(a);

  for (size_t i = 1; i <= 2; ++i) {
    immutable_guard gb(b);
    element::set_copy_throw_countdown(i);
    element::reset_counters();
    EXPECT_THROW(b = a, std::runtime_error);
    EXPECT_GE(i, element::get_copy_counter());
    EXPECT_EQ(0, element::get_swap_counter());
  }

  {
    container old_b = b;
    immutable_guard gb(b);
    element::set_copy_throw_countdown(4);
    element::reset_counters();
    try {
      b = a;
      element::set_copy_throw_countdown(0);
      b = old_b;
    } catch (const std::runtime_error&) {
      EXPECT_EQ(0, element::get_swap_counter());
    }
  }

  element::set_swap_throw_countdown(1);
  try {
    b = a;
  } catch (const std::runtime_error&) {}
}

TEST_F(small_object_test, copy_assignment_small_to_big) {
  container a;
  a.push_back(3);
  a.push_back(7);

  container b;
  for (size_t i = 0; i < 5; ++i) {
    b.push_back(i + 100);
  }

  element::reset_counters();
  b = a;
  EXPECT_GE(2, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  ASSERT_EQ(2, b.size());
  expect_static_storage(b);
  EXPECT_EQ(3, b[0]);
  EXPECT_EQ(7, b[1]);
}

TEST_F(small_object_test, copy_assignment_small_to_big_throw) {
  container a;
  a.push_back(3);
  a.push_back(7);

  container b;
  for (size_t i = 0; i < 5; ++i) {
    b.push_back(i + 100);
  }

  for (size_t i = 1; i <= 2; ++i) {
    immutable_guard g(a, b);
    element::set_copy_throw_countdown(i);
    element::reset_counters();
    EXPECT_THROW(b = a, std::runtime_error);
    EXPECT_GE(i, element::get_copy_counter());
    EXPECT_EQ(0, element::get_swap_counter());
  }
}

TEST_F(small_object_test, copy_assignment_big_to_small) {
  container a;
  a.reserve(10);
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b;
  b.push_back(42);

  element::reset_counters();
  b = a;
  EXPECT_EQ(0, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  EXPECT_EQ(5, b.size());
  EXPECT_EQ(10, b.capacity());
  EXPECT_EQ(as_const(a).data(), as_const(b).data());
}

TEST_F(small_object_test, self_assignment) {
  container a;
  a.push_back(3);
  a.push_back(7);

  element::reset_counters();
  a = a;
  EXPECT_EQ(0, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  ASSERT_EQ(2, a.size());
  expect_static_storage(a);
  EXPECT_EQ(3, a[0]);
  EXPECT_EQ(7, a[1]);
}

TEST_F(small_object_test, reserve_empty) {
  container a;

  for (size_t i = 0; i <= 3; ++i) {
    a.reserve(i);
    expect_empty_storage(a);
    element::assert_no_instances();
  }
}

TEST_F(small_object_test, reserve_small) {
  container a;

  for (size_t size = 1; size <= 3; ++size) {
    a.push_back(size + 100);

    for (size_t i = 0; i <= 3; ++i) {
      a.reserve(i);
      EXPECT_EQ(size, a.size());
      expect_static_storage(a);
    }
  }
}

TEST_F(small_object_test, reserve_small_into_big) {
  container a;
  a.push_back(3);
  a.push_back(7);
  a.reserve(5);

  ASSERT_EQ(2, a.size());
  EXPECT_EQ(5, a.capacity());
  EXPECT_EQ(3, a[0]);
  EXPECT_EQ(7, a[1]);
}

TEST_F(small_object_test, reserve_small_into_big_throw) {
  container a;
  a.push_back(3);
  a.push_back(7);
  a.push_back(9);

  for (size_t i = 1; i <= 2; ++i) {
    immutable_guard g(a);
    element::set_copy_throw_countdown(i);
    EXPECT_THROW(a.reserve(5), std::runtime_error);
  }
}

TEST_F(small_object_test, reserve_small_into_big_copies) {
  container a;
  a.push_back(3);
  a.push_back(7);

  element::reset_counters();
  a.reserve(5);
  EXPECT_GE(2, element::get_copy_counter() + element::get_swap_counter());
}

TEST_F(small_object_test, reserve_big_into_small) {
  container a;
  a.reserve(5);
  for (size_t i = 0; i < 2; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  a.reserve(3);

  ASSERT_EQ(2, a.size());
  expect_static_storage(a);
  ASSERT_EQ(2, b.size());
  ASSERT_EQ(5, b.capacity());

  for (size_t i = 0; i < 2; ++i) {
    ASSERT_EQ(i + 100, a[i]);
    ASSERT_EQ(i + 100, b[i]);
  }
}

TEST_F(small_object_test, reserve_big_into_small_throw) {
  container a;
  a.reserve(5);
  for (size_t i = 0; i < 2; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  immutable_guard g(a, b);
  element::set_copy_throw_countdown(2);
  EXPECT_THROW(a.reserve(3), std::runtime_error);
}

TEST_F(small_object_test, reserve_big_into_small_copies) {
  container a;
  a.reserve(5);
  for (size_t i = 0; i < 2; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  element::reset_counters();
  a.reserve(3);
  EXPECT_GE(2, element::get_copy_counter() + element::get_swap_counter());
}

TEST_F(small_object_test, shrink_to_fit_empty) {
  container a;
  a.shrink_to_fit();
  expect_empty_storage(a);
  element::assert_no_instances();
}

TEST_F(small_object_test, shrink_to_fit_small) {
  container a;

  for (size_t size = 1; size <= 3; ++size) {
    a.push_back(size + 100);

    for (size_t i = 0; i <= 3; ++i) {
      a.reserve(i);
      EXPECT_EQ(size, a.size());
      expect_static_storage(a);

      element::reset_counters();

      a.shrink_to_fit();
      EXPECT_EQ(size, a.size());
      expect_static_storage(a);

      EXPECT_GE(size, element::get_copy_counter());
      EXPECT_EQ(0, element::get_swap_counter());

      for (size_t j = 0; j < size; ++j) {
        ASSERT_EQ(j + 101, a[j]);
      }
    }
  }
}

TEST_F(small_object_test, shrink_to_fit_big_into_small) {
  container a;
  a.reserve(5);
  a.push_back(42);
  a.push_back(43);

  ASSERT_EQ(2, a.size());
  ASSERT_EQ(5, a.capacity());

  element::reset_counters();
  a.shrink_to_fit();
  EXPECT_GE(2, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  expect_static_storage(a);
  EXPECT_EQ(2, a.size());

  EXPECT_EQ(42, a[0]);
  EXPECT_EQ(43, a[1]);
}

TEST_F(small_object_test, shrink_to_fit_big_into_small_throw) {
  container a;
  a.reserve(5);
  a.push_back(42);
  a.push_back(43);

  ASSERT_EQ(2, a.size());
  ASSERT_EQ(5, a.capacity());

  immutable_guard g(a);
  element::set_copy_throw_countdown(2);
  EXPECT_THROW(a.shrink_to_fit(), std::runtime_error);
}

TEST_F(small_object_test, clear_empty) {
  container a;

  element::reset_counters();
  a.clear();
  EXPECT_EQ(0, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  expect_empty_storage(a);
  element::assert_no_instances();
}

TEST_F(small_object_test, clear_small) {
  container a;
  for (size_t size = 1; size <= 3; ++size) {
    for (size_t i = 0; i < size; ++i) {
      a.push_back(i + 100);
    }
    ASSERT_EQ(size, a.size());

    element::reset_counters();
    a.clear();
    EXPECT_EQ(0, element::get_copy_counter());
    EXPECT_EQ(0, element::get_swap_counter());

    expect_empty_storage(a);
    element::assert_no_instances();
  }
}

TEST_F(small_object_test, clear_small_throw) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }
  ASSERT_EQ(5, a.size());
  ASSERT_LE(5, a.capacity());

  element::set_copy_throw_countdown(1);
  element::set_swap_throw_countdown(1);
  EXPECT_NO_THROW(a.clear());
  EXPECT_TRUE(a.empty());
  element::assert_no_instances();
}

TEST_F(small_object_test, clear_big_into_small) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }
  ASSERT_EQ(5, a.size());
  ASSERT_LE(5, a.capacity());

  {
    container b = a;

    element::reset_counters();
    a.clear();
    EXPECT_EQ(0, element::get_copy_counter());
    EXPECT_EQ(0, element::get_swap_counter());

    EXPECT_TRUE(a.empty());
    EXPECT_EQ(0, a.size());
  }
  element::assert_no_instances();
}

TEST_F(small_object_test, clear_big_into_small_single_user) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }
  ASSERT_EQ(5, a.size());
  ASSERT_LE(5, a.capacity());

  size_t old_capacity = a.capacity();
  const element* old_data = a.data();

  element::reset_counters();
  a.clear();
  EXPECT_EQ(0, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  element::assert_no_instances();
  EXPECT_TRUE(a.empty());
  EXPECT_EQ(0, a.size());
  EXPECT_EQ(old_capacity, a.capacity());
  EXPECT_EQ(old_data, a.data());
}

TEST_F(small_object_test, swap_two_small) {
  container a;
  a.push_back(1);
  a.push_back(2);

  container b;
  b.push_back(3);

  element::reset_counters();
  a.swap(b);
  EXPECT_GE(1, element::get_copy_counter());

  ASSERT_EQ(1, a.size());
  ASSERT_EQ(2, b.size());
  EXPECT_EQ(1, b[0]);
  EXPECT_EQ(2, b[1]);
  EXPECT_EQ(3, a[0]);

  element::reset_counters();
  a.swap(b);
  EXPECT_GE(1, element::get_copy_counter());

  ASSERT_EQ(1, b.size());
  ASSERT_EQ(2, a.size());
  EXPECT_EQ(1, a[0]);
  EXPECT_EQ(2, a[1]);
  EXPECT_EQ(3, b[0]);
}

TEST_F(small_object_test, swap_self) {
  container a;
  a.push_back(1);
  a.push_back(2);

  immutable_guard g(a);

  element::reset_counters();
  a.swap(a);
  EXPECT_EQ(0, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());
}

TEST_F(small_object_test, swap_two_small_throw) {
  socow_vector<element, 6> a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);
  a.push_back(5);

  socow_vector<element, 6> b;
  b.push_back(6);
  b.push_back(7);
  b.push_back(8);

  {
    immutable_guard g(a, b);
    element::set_copy_throw_countdown(2);
    element::reset_counters();
    EXPECT_THROW(a.swap(b), std::runtime_error);
    EXPECT_GE(1, element::get_copy_counter());
    EXPECT_EQ(0, element::get_swap_counter());
  }
  {
    immutable_guard g(a, b);
    element::set_copy_throw_countdown(2);
    element::reset_counters();
    EXPECT_THROW(b.swap(a), std::runtime_error);
    EXPECT_GE(1, element::get_copy_counter());
    EXPECT_EQ(0, element::get_swap_counter());
  }

  element::set_copy_throw_countdown(3);
  EXPECT_NO_THROW(a.swap(b));

  element::set_copy_throw_countdown(3);
  EXPECT_NO_THROW(b.swap(a));

  element::set_copy_throw_countdown(0);
  element::set_swap_throw_countdown(1);
  EXPECT_THROW(a.swap(b), std::runtime_error);
}

TEST_F(small_object_test, swap_big_and_small) {
  container a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);

  container b;
  b.push_back(5);

  element::reset_counters();
  a.swap(b);
  EXPECT_GE(1, element::get_copy_counter());

  ASSERT_EQ(1, a.size());
  ASSERT_EQ(4, b.size());
  EXPECT_EQ(1, b[0]);
  EXPECT_EQ(2, b[1]);
  EXPECT_EQ(3, b[2]);
  EXPECT_EQ(4, b[3]);
  EXPECT_EQ(5, a[0]);

  element::reset_counters();
  a.swap(b);
  EXPECT_GE(1, element::get_copy_counter());

  ASSERT_EQ(1, b.size());
  ASSERT_EQ(4, a.size());
  EXPECT_EQ(1, a[0]);
  EXPECT_EQ(2, a[1]);
  EXPECT_EQ(3, a[2]);
  EXPECT_EQ(4, a[3]);
  EXPECT_EQ(5, b[0]);
}

TEST_F(small_object_test, swap_big_and_small_throw) {
  container a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);

  container b;
  b.push_back(5);
  b.push_back(6);

  {
    immutable_guard g(a, b);
    element::set_copy_throw_countdown(2);
    element::reset_counters();
    EXPECT_THROW(a.swap(b), std::runtime_error);
    EXPECT_GE(1, element::get_copy_counter());
    EXPECT_EQ(0, element::get_swap_counter());
  }
  {
    immutable_guard g(a, b);
    element::set_copy_throw_countdown(2);
    element::reset_counters();
    EXPECT_THROW(b.swap(a), std::runtime_error);
    EXPECT_GE(1, element::get_copy_counter());
    EXPECT_EQ(0, element::get_swap_counter());
  }
}

TEST_F(small_object_test, begin_end) {
  container a;
  a.push_back(1);
  a.push_back(2);

  auto it = a.begin();
  EXPECT_EQ(1, *it);
  ++it;
  EXPECT_EQ(2, *it);
  ++it;
  EXPECT_EQ(a.end(), it);
}

TEST_F(small_object_test, begin_end_const) {
  container a;
  a.push_back(1);
  a.push_back(2);

  auto it = as_const(a).begin();
  EXPECT_EQ(1, *it);
  ++it;
  EXPECT_EQ(2, *it);
  ++it;
  EXPECT_EQ(as_const(a).end(), it);
}

TEST_F(small_object_test, erase_empty) {
  container a;

  auto it = a.erase(a.begin(), a.end());
  EXPECT_EQ(a.begin(), it);
  expect_empty_storage(a);
  element::assert_no_instances();
}

TEST_F(small_object_test, erase_begin) {
  container a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);

  auto it = a.erase(a.begin());
  EXPECT_EQ(a.begin(), it);
  EXPECT_EQ(2, a.size());
  expect_static_storage(a);

  EXPECT_EQ(2, a[0]);
  EXPECT_EQ(3, a[1]);
}

TEST_F(small_object_test, erase_middle) {
  container a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);

  auto it = a.erase(a.begin() + 1, a.end() - 1);
  EXPECT_EQ(a.begin() + 1, it);
  EXPECT_EQ(a.end() - 1, it);
  EXPECT_EQ(2, a.size());
  expect_static_storage(a);
}

TEST_F(small_object_test, erase_end) {
  container a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);

  auto it = a.erase(a.end() - 1);
  EXPECT_EQ(a.end(), it);
  EXPECT_EQ(2, a.size());
  expect_static_storage(a);

  EXPECT_EQ(1, a[0]);
  EXPECT_EQ(2, a[1]);
}

TEST_F(small_object_test, erase_range_all) {
  container a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);

  auto it = a.erase(a.begin(), a.end());
  EXPECT_EQ(a.begin(), it);
  EXPECT_EQ(a.end(), it);
  expect_empty_storage(a);
  element::assert_no_instances();
}

TEST_F(small_object_test, erase_range_empty) {
  container a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);

  auto it = a.erase(a.begin(), a.begin());
  EXPECT_EQ(a.begin(), it);
  EXPECT_EQ(3, a.size());
  expect_static_storage(a);

  EXPECT_EQ(1, a[0]);
  EXPECT_EQ(2, a[1]);
  EXPECT_EQ(3, a[2]);
}

TEST_F(small_object_test, erase_range_copies) {
  container a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);

  element::reset_counters();
  a.erase(a.begin() + 1, a.end() - 1);
  EXPECT_GE(1, element::get_copy_counter() + element::get_swap_counter());
}

TEST_F(small_object_test, erase_range_throw) {
  container a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);

  element::set_copy_throw_countdown(1);
  EXPECT_NO_THROW(a.erase(a.begin() + 1, a.end() - 1));
}

TEST_F(small_object_test, erase_big_into_small) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  auto it = a.erase(as_const(a).begin() + 1, as_const(a).end() - 1);
  EXPECT_EQ(as_const(a).begin() + 1, it);
  EXPECT_EQ(2, a.size());
}

TEST_F(small_object_test, erase_big_into_small_single_user) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  size_t old_capacity = a.capacity();
  const element* old_data = a.data();

  auto it = a.erase(as_const(a).begin() + 1, as_const(a).end() - 1);
  EXPECT_EQ(as_const(a).begin() + 1, it);
  EXPECT_EQ(2, a.size());
  EXPECT_EQ(old_capacity, a.capacity());
  EXPECT_EQ(old_data, a.data());
}

TEST_F(small_object_test, erase_big_into_small_throw) {
  container a;
  for (size_t i = 0; i < 6; ++i) {
    a.push_back(i + 100);
  }

  immutable_guard ga(a);

  for (size_t i = 1; i <= 3; ++i) {
    container b = a;
    immutable_guard gb(b);

    element::set_copy_throw_countdown(i);
    EXPECT_THROW(b.erase(as_const(b).begin() + 2, as_const(b).end() - 1), std::runtime_error);
  }
}

TEST_F(small_object_test, erase_big_into_small_copies) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  element::reset_counters();
  a.erase(as_const(a).begin() + 2, as_const(a).end() - 1);
  EXPECT_GE(3, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());
}

TEST_F(small_object_test, insert_empty) {
  container a;
  auto it = a.insert(a.end(), 42);
  EXPECT_EQ(a.begin(), it);
  ASSERT_EQ(1, a.size());
  expect_static_storage(a);
  EXPECT_EQ(42, a[0]);
}

TEST_F(small_object_test, insert_begin) {
  container a;
  a.push_back(1);
  a.push_back(2);

  auto it = a.insert(a.begin(), 42);
  EXPECT_EQ(a.begin(), it);
  ASSERT_EQ(3, a.size());
  expect_static_storage(a);
  EXPECT_EQ(42, a[0]);
  EXPECT_EQ(1, a[1]);
  EXPECT_EQ(2, a[2]);
}

TEST_F(small_object_test, insert_middle) {
  container a;
  a.push_back(1);
  a.push_back(2);

  auto it = a.insert(a.begin() + 1, 42);
  EXPECT_EQ(a.begin() + 1, it);
  ASSERT_EQ(3, a.size());
  expect_static_storage(a);
  EXPECT_EQ(1, a[0]);
  EXPECT_EQ(42, a[1]);
  EXPECT_EQ(2, a[2]);
}

TEST_F(small_object_test, insert_end) {
  container a;
  a.push_back(1);
  a.push_back(2);

  auto it = a.insert(a.end(), 42);
  EXPECT_EQ(a.end() - 1, it);
  ASSERT_EQ(3, a.size());
  expect_static_storage(a);
  EXPECT_EQ(1, a[0]);
  EXPECT_EQ(2, a[1]);
  EXPECT_EQ(42, a[2]);
}

TEST_F(small_object_test, insert_copies) {
  container a;
  a.push_back(1);
  a.push_back(2);

  element::reset_counters();
  a.insert(a.begin() + 1, 42);
  EXPECT_GE(2, element::get_copy_counter() + element::get_swap_counter());
}

TEST_F(small_object_test, insert_throw) {
  container a;
  a.push_back(1);
  a.push_back(2);

  element::set_copy_throw_countdown(2);
  EXPECT_NO_THROW(a.insert(a.begin() + 1, 42));
}

TEST_F(small_object_test, insert_small_into_big_begin) {
  container a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);

  auto it = a.insert(a.begin(), 42);
  EXPECT_EQ(a.begin(), it);
  ASSERT_EQ(4, a.size());
  ASSERT_LE(4, a.capacity());
  EXPECT_EQ(42, a[0]);
  EXPECT_EQ(1, a[1]);
  EXPECT_EQ(2, a[2]);
  EXPECT_EQ(3, a[3]);
}

TEST_F(small_object_test, insert_small_into_big_middle) {
  container a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);

  auto it = a.insert(a.begin() + 1, 42);
  EXPECT_EQ(a.begin() + 1, it);
  ASSERT_EQ(4, a.size());
  ASSERT_LE(4, a.capacity());
  EXPECT_EQ(1, a[0]);
  EXPECT_EQ(42, a[1]);
  EXPECT_EQ(2, a[2]);
  EXPECT_EQ(3, a[3]);
}

TEST_F(small_object_test, insert_small_into_big_end) {
  container a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);

  auto it = a.insert(a.end(), 42);
  EXPECT_EQ(a.end() - 1, it);
  ASSERT_EQ(4, a.size());
  ASSERT_LE(4, a.capacity());
  EXPECT_EQ(1, a[0]);
  EXPECT_EQ(2, a[1]);
  EXPECT_EQ(3, a[2]);
  EXPECT_EQ(42, a[3]);
}

TEST_F(small_object_test, insert_small_into_big_throw) {
  container a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);

  for (size_t i = 1; i <= 4; ++i) {
    immutable_guard g(a);
    element::set_copy_throw_countdown(i);
    EXPECT_THROW(a.insert(a.begin() + 1, 42), std::runtime_error);
  }
}

TEST_F(small_object_test, insert_small_into_big_copies) {
  container a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);

  element::reset_counters();
  a.insert(a.begin() + 1, 42);
  EXPECT_EQ(4, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());
}
