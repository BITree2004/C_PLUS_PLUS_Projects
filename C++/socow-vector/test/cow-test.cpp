#include "socow-vector.h"
#include "test-utils.h"

#include <gtest/gtest.h>

using std::as_const;

class cow_test : public base_test {};

TEST_F(cow_test, copy_ctor) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  element::reset_counters();

  container b = a;
  EXPECT_EQ(as_const(a).data(), as_const(b).data());
  EXPECT_EQ(a.capacity(), b.capacity());

  container c = a;
  EXPECT_EQ(as_const(a).data(), as_const(b).data());
  EXPECT_EQ(as_const(a).data(), as_const(c).data());
  EXPECT_EQ(a.capacity(), b.capacity());
  EXPECT_EQ(a.capacity(), c.capacity());

  EXPECT_EQ(0, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());
}

TEST_F(cow_test, copy_ctor_big_into_small) {
  container a;
  a.reserve(5);
  a.push_back(42);

  container b = a;
  EXPECT_EQ(as_const(a).data(), as_const(b).data());
  ASSERT_EQ(1, b.size());
  EXPECT_EQ(42, as_const(b)[0]);
}

TEST_F(cow_test, copy_assignment) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b;
  for (size_t i = 0; i < 10; ++i) {
    b.push_back(i + 200);
  }

  element::reset_counters();
  b = a;
  EXPECT_EQ(as_const(a).data(), as_const(b).data());
  EXPECT_EQ(0, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());
}

TEST_F(cow_test, copy_assignment_2) {
  container a;
  for (size_t i = 0; i < 10; ++i) {
    a.push_back(i + 100);
  }

  container b;
  for (size_t i = 0; i < 5; ++i) {
    b.push_back(i + 200);
  }

  element::reset_counters();
  b = a;
  EXPECT_EQ(as_const(a).data(), as_const(b).data());
  EXPECT_EQ(0, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());
}

TEST_F(cow_test, copy_assignment_big_to_small) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b;
  b.push_back(3);
  b.push_back(7);

  element::reset_counters();
  b = a;
  EXPECT_EQ(as_const(a).data(), as_const(b).data());
  EXPECT_EQ(0, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());
}

TEST_F(cow_test, self_assignment) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  element::reset_counters();
  a = a;
  EXPECT_EQ(as_const(a).data(), as_const(b).data());
  EXPECT_EQ(0, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());
}

TEST_F(cow_test, subscript) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  element::reset_counters();

  container b = a;
  EXPECT_EQ(0, element::get_copy_counter());

  b[3] = 42;
  EXPECT_EQ(103, as_const(a)[3]);

  EXPECT_EQ(6, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  EXPECT_EQ(103, as_const(a)[3]);
  EXPECT_EQ(42, as_const(b)[3]);

  for (size_t i = 0; i < 5; ++i) {
    if (i == 3) {
      continue;
    }
    EXPECT_EQ(i + 100, as_const(a)[i]);
    EXPECT_EQ(i + 100, as_const(b)[i]);
  }
}

TEST_F(cow_test, subscript_single_user) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  element::reset_counters();
  a[3] = 42;
  EXPECT_EQ(1, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  EXPECT_EQ(100, as_const(a)[0]);
  EXPECT_EQ(101, as_const(a)[1]);
  EXPECT_EQ(102, as_const(a)[2]);
  EXPECT_EQ(42, as_const(a)[3]);
  EXPECT_EQ(104, as_const(a)[4]);
}

TEST_F(cow_test, subscript_const) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  element::reset_counters();

  container b = a;
  as_const(b)[3];
  EXPECT_EQ(as_const(a).data(), as_const(b).data());

  EXPECT_EQ(0, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());
}

TEST_F(cow_test, data) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  element::reset_counters();

  container b = a;
  EXPECT_EQ(0, element::get_copy_counter());

  b.data()[3] = 42;
  EXPECT_EQ(103, as_const(a)[3]);

  EXPECT_EQ(6, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  EXPECT_EQ(103, as_const(a)[3]);
  EXPECT_EQ(42, as_const(b)[3]);

  for (size_t i = 0; i < 5; ++i) {
    if (i == 3) {
      continue;
    }
    EXPECT_EQ(i + 100, as_const(a)[i]);
    EXPECT_EQ(i + 100, as_const(b)[i]);
  }
}

TEST_F(cow_test, data_single_user) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  element::reset_counters();
  a.data()[3] = 42;
  EXPECT_EQ(1, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  EXPECT_EQ(100, as_const(a)[0]);
  EXPECT_EQ(101, as_const(a)[1]);
  EXPECT_EQ(102, as_const(a)[2]);
  EXPECT_EQ(42, as_const(a)[3]);
  EXPECT_EQ(104, as_const(a)[4]);
}

TEST_F(cow_test, data_const) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  element::reset_counters();

  container b = a;
  [[maybe_unused]] const element& e = as_const(b).data()[3];
  EXPECT_EQ(as_const(a).data(), as_const(b).data());

  EXPECT_EQ(0, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());
}

TEST_F(cow_test, front) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  element::reset_counters();

  container b = a;
  EXPECT_EQ(0, element::get_copy_counter());

  b.front() = 42;
  EXPECT_EQ(100, a.front());

  EXPECT_EQ(6, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  EXPECT_EQ(100, as_const(a)[0]);
  EXPECT_EQ(42, as_const(b)[0]);

  for (size_t i = 1; i < 5; ++i) {
    EXPECT_EQ(i + 100, as_const(a)[i]);
    EXPECT_EQ(i + 100, as_const(b)[i]);
  }
}

TEST_F(cow_test, front_single_user) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  element::reset_counters();
  a.front() = 42;
  EXPECT_EQ(1, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  EXPECT_EQ(42, as_const(a)[0]);
  EXPECT_EQ(101, as_const(a)[1]);
  EXPECT_EQ(102, as_const(a)[2]);
  EXPECT_EQ(103, as_const(a)[3]);
  EXPECT_EQ(104, as_const(a)[4]);
}

TEST_F(cow_test, front_const) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  element::reset_counters();

  container b = a;
  as_const(b).front();
  EXPECT_EQ(as_const(a).data(), as_const(b).data());

  EXPECT_EQ(0, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());
}

TEST_F(cow_test, back) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  element::reset_counters();

  container b = a;
  EXPECT_EQ(0, element::get_copy_counter());

  b.back() = 42;
  EXPECT_EQ(100, a.front());

  EXPECT_EQ(6, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  for (size_t i = 0; i < 4; ++i) {
    EXPECT_EQ(i + 100, as_const(a)[i]);
    EXPECT_EQ(i + 100, as_const(b)[i]);
  }

  EXPECT_EQ(104, as_const(a)[4]);
  EXPECT_EQ(42, as_const(b)[4]);
}

TEST_F(cow_test, back_single_user) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  element::reset_counters();
  a.back() = 42;
  EXPECT_EQ(1, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  EXPECT_EQ(100, as_const(a)[0]);
  EXPECT_EQ(101, as_const(a)[1]);
  EXPECT_EQ(102, as_const(a)[2]);
  EXPECT_EQ(103, as_const(a)[3]);
  EXPECT_EQ(42, as_const(a)[4]);
}

TEST_F(cow_test, back_const) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  element::reset_counters();

  container b = a;
  as_const(b).back();
  EXPECT_EQ(as_const(a).data(), as_const(b).data());

  EXPECT_EQ(0, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());
}

TEST_F(cow_test, push_back) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  a.push_back(1);
  b.push_back(2);

  EXPECT_EQ(1, a[5]);
  EXPECT_EQ(2, b[5]);
}

TEST_F(cow_test, push_back_throw) {
  container a;
  a.reserve(6);
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  for (size_t i = 1; i <= 6; ++i) {
    container b = a;
    immutable_guard g(a, b);

    element::set_copy_throw_countdown(i);
    EXPECT_THROW(b.push_back(42), std::runtime_error);
  }
}

TEST_F(cow_test, push_back_reallocation_throw) {
  container a;
  a.reserve(5);
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  for (size_t i = 1; i <= 6; ++i) {
    container b = a;
    immutable_guard g(a, b);

    element::set_copy_throw_countdown(i);
    EXPECT_THROW(b.push_back(42), std::runtime_error);
  }
}

TEST_F(cow_test, push_back_copies) {
  container a;
  a.reserve(6);
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  element::reset_counters();
  a.push_back(42);
  EXPECT_GE(6, element::get_copy_counter() + element::get_swap_counter());

  EXPECT_EQ(42, as_const(a).back());
}

TEST_F(cow_test, push_back_reallocation_copies) {
  container a;
  a.reserve(5);
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  element::reset_counters();
  a.push_back(42);
  EXPECT_GE(6, element::get_copy_counter() + element::get_swap_counter());

  EXPECT_EQ(42, as_const(a).back());
}

TEST_F(cow_test, pop_back) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  element::reset_counters();
  a.pop_back();
  EXPECT_GE(4, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  EXPECT_EQ(4, a.size());
  EXPECT_EQ(5, b.size());

  element t = b[4];
  EXPECT_EQ(104, t);
}

TEST_F(cow_test, pop_back_throw) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  immutable_guard g(a, b);
  element::set_copy_throw_countdown(2);
  EXPECT_THROW(b.pop_back(), std::runtime_error);
}

TEST_F(cow_test, reserve) {
  container a;
  a.reserve(5);
  for (size_t i = 0; i < 3; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  ASSERT_EQ(as_const(a).data(), as_const(b).data());

  b.reserve(10);
  EXPECT_EQ(5, a.capacity());
  EXPECT_EQ(10, b.capacity());
  EXPECT_NE(as_const(a).data(), as_const(b).data());

  for (size_t i = 0; i < 3; ++i) {
    EXPECT_EQ(i + 100, as_const(a)[i]);
    EXPECT_EQ(i + 100, as_const(b)[i]);
  }
}

TEST_F(cow_test, reserve_superfluous) {
  container a;
  a.reserve(10);
  for (size_t i = 0; i < 3; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  ASSERT_EQ(as_const(a).data(), as_const(b).data());

  b.reserve(5);
  EXPECT_EQ(5, b.capacity());
  EXPECT_EQ(3, b.size());
  EXPECT_NE(as_const(a).data(), as_const(b).data());

  const element* old_data = as_const(b).data();

  for (size_t i = 3; i < 5; ++i) {
    b.push_back(i + 100);
  }

  EXPECT_EQ(old_data, b.data());
}

TEST_F(cow_test, reserve_superfluous_single_user) {
  container a;
  a.reserve(10);
  for (size_t i = 0; i < 3; ++i) {
    a.push_back(i + 100);
  }

  const element* old_data = as_const(a).data();

  a.reserve(5);
  a.push_back(103);

  for (size_t i = 0; i < 4; ++i) {
    EXPECT_EQ(i + 100, a[i]);
  }

  EXPECT_EQ(old_data, a.data());
}

TEST_F(cow_test, reserve_superfluous_2) {
  container a;
  a.reserve(10);
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  ASSERT_EQ(as_const(a).data(), as_const(b).data());

  b.reserve(4);
  EXPECT_EQ(10, b.capacity());
  EXPECT_EQ(5, b.size());
  EXPECT_EQ(as_const(a).data(), as_const(b).data());

  for (size_t i = 0; i < 5; ++i) {
    EXPECT_EQ(i + 100, as_const(a)[i]);
    EXPECT_EQ(i + 100, as_const(b)[i]);
  }
}

TEST_F(cow_test, reserve_throw) {
  container a;
  a.reserve(5);
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  immutable_guard g(a, b);
  element::set_copy_throw_countdown(2);
  EXPECT_THROW(a.reserve(10), std::runtime_error);
}

TEST_F(cow_test, shrink_to_fit) {
  container a;
  a.reserve(10);
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  element::reset_counters();
  a.shrink_to_fit();
  EXPECT_GE(5, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  EXPECT_EQ(5, a.size());
  EXPECT_EQ(5, a.capacity());
  EXPECT_NE(as_const(a).data(), as_const(b).data());

  for (size_t i = 0; i < 5; ++i) {
    EXPECT_EQ(i + 100, as_const(a)[i]);
    EXPECT_EQ(i + 100, as_const(b)[i]);
  }
}

TEST_F(cow_test, shrink_to_fit_empty) {
  container a;
  a.reserve(5);

  container b = a;

  a.shrink_to_fit();
  expect_empty_storage(a);
  element::assert_no_instances();

  EXPECT_EQ(5, b.capacity());
}

TEST_F(cow_test, shrink_to_fit_into_big_throw) {
  container a;
  a.reserve(10);
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  ASSERT_EQ(5, a.size());
  ASSERT_EQ(10, a.capacity());

  container b = a;

  immutable_guard g(a, b);
  element::set_copy_throw_countdown(2);
  EXPECT_THROW(a.shrink_to_fit(), std::runtime_error);
}

TEST_F(cow_test, shrink_to_fit_into_small_throw) {
  container a;
  a.reserve(5);
  a.push_back(42);
  a.push_back(43);

  ASSERT_EQ(2, a.size());
  ASSERT_EQ(5, a.capacity());

  container b = a;

  immutable_guard g(a, b);
  element::set_copy_throw_countdown(2);
  EXPECT_THROW(b.shrink_to_fit(), std::runtime_error);
}

TEST_F(cow_test, clear) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  {
    container b = a;
    a.clear();

    EXPECT_EQ(0, a.size());
    EXPECT_TRUE(a.empty());

    for (size_t i = 0; i < 5; ++i) {
      EXPECT_EQ(i + 100, b[i]);
    }
  }
  element::assert_no_instances();
}

TEST_F(cow_test, clear_throw) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  {
    container b = a;
    immutable_guard gb(b);

    element::set_copy_throw_countdown(1);
    element::set_swap_throw_countdown(1);
    EXPECT_NO_THROW(a.clear());
    EXPECT_TRUE(a.empty());
  }
  element::assert_no_instances();
}

TEST_F(cow_test, begin) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  *b.begin() = 42;
  EXPECT_EQ(100, *a.begin());
}

TEST_F(cow_test, begin_single_user) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  element::reset_counters();
  *a.begin() = 42;
  EXPECT_EQ(1, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());
}

TEST_F(cow_test, begin_const) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  as_const(b).begin();
  EXPECT_EQ(as_const(a).data(), as_const(b).data());
}

TEST_F(cow_test, end) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  *std::prev(b.end()) = 42;
  EXPECT_EQ(104, *std::prev(a.end()));
}

TEST_F(cow_test, end_single_user) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  element::reset_counters();
  *std::prev(a.end()) = 42;
  EXPECT_EQ(1, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());
}

TEST_F(cow_test, end_const) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  as_const(b).end();
  EXPECT_EQ(as_const(a).data(), as_const(b).data());
}

TEST_F(cow_test, insert) {
  container a;
  a.reserve(5);
  a.push_back(100);
  a.push_back(101);
  a.push_back(103);
  a.push_back(104);

  container b = a;
  immutable_guard g(b);

  auto it = a.insert(as_const(a).begin() + 2, 102);
  EXPECT_EQ(as_const(a).begin() + 2, it);

  for (size_t i = 0; i < 5; ++i) {
    EXPECT_EQ(i + 100, as_const(a)[i]);
  }
}

TEST_F(cow_test, insert_single_user) {
  container a;
  a.reserve(10);
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  const element* old_data = as_const(a).data();
  auto it = a.insert(as_const(a).begin() + 2, 200);
  EXPECT_EQ(as_const(a).begin() + 2, it);
  EXPECT_EQ(old_data, as_const(a).data());
}

TEST_F(cow_test, insert_throw) {
  container a;
  a.reserve(6);
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  for (size_t i = 1; i <= 6; ++i) {
    container b = a;
    immutable_guard g(a, b);

    element::set_copy_throw_countdown(i);
    EXPECT_THROW(b.insert(as_const(b).begin() + 2, 42), std::runtime_error);
  }
}

TEST_F(cow_test, insert_reallocation_throw) {
  container a;
  a.reserve(5);
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  for (size_t i = 1; i <= 6; ++i) {
    container b = a;
    immutable_guard g(a, b);

    element::set_copy_throw_countdown(i);
    EXPECT_THROW(b.insert(as_const(b).begin() + 2, 42), std::runtime_error);
  }
}

TEST_F(cow_test, insert_copies) {
  container a;
  a.reserve(11);
  for (size_t i = 0; i < 10; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  element::reset_counters();
  a.insert(as_const(a).begin() + 2, 42);
  EXPECT_GE(11, element::get_copy_counter() + element::get_swap_counter());

  EXPECT_EQ(42, as_const(a)[2]);
}

TEST_F(cow_test, insert_reallocation_copies) {
  container a;
  a.reserve(10);
  for (size_t i = 0; i < 10; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  element::reset_counters();
  a.insert(as_const(a).begin() + 2, 42);
  EXPECT_GE(11, element::get_copy_counter() + element::get_swap_counter());

  EXPECT_EQ(42, as_const(a)[2]);
}

TEST_F(cow_test, erase) {
  container a;
  a.reserve(10);
  a.push_back(100);
  a.push_back(101);
  a.push_back(200);
  a.push_back(102);
  a.push_back(103);

  container b = a;
  immutable_guard g(b);

  auto it = a.erase(as_const(a).begin() + 2);
  EXPECT_EQ(as_const(a).begin() + 2, it);

  for (size_t i = 0; i < 4; ++i) {
    EXPECT_EQ(i + 100, as_const(a)[i]);
  }
}

TEST_F(cow_test, erase_single_user) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  const element* old_data = as_const(a).data();
  auto it = a.erase(as_const(a).begin() + 2);
  EXPECT_EQ(as_const(a).begin() + 2, it);
  EXPECT_EQ(old_data, as_const(a).data());
}

TEST_F(cow_test, erase_throw) {
  container a;
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  immutable_guard g(a, b);
  element::set_copy_throw_countdown(2);
  EXPECT_THROW(a.erase(as_const(a).begin() + 2), std::runtime_error);
}

TEST_F(cow_test, erase_copies) {
  container a;
  for (size_t i = 0; i < 10; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  element::reset_counters();
  a.erase(as_const(a).begin() + 3, as_const(a).end() - 1);
  EXPECT_GE(4, element::get_copy_counter());
  EXPECT_GE(0, element::get_swap_counter());
}

TEST_F(cow_test, unshare_throw) {
  container a;
  a.reserve(5);
  for (size_t i = 0; i < 5; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  immutable_guard g(a, b);
  element::set_copy_throw_countdown(3);
  EXPECT_THROW(a.data(), std::runtime_error);
}
