#include "socow-vector.h"
#include "test-utils.h"

#include <gtest/gtest.h>

using std::as_const;

class vector_test : public base_test {};

TEST_F(vector_test, default_ctor) {
  container a;
  expect_empty_storage(a);
  element::assert_no_instances();
}

TEST_F(vector_test, push_back) {
  constexpr size_t N = 5000;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  EXPECT_EQ(N, a.size());
  EXPECT_LE(N, a.capacity());

  for (size_t i = 0; i < N; ++i) {
    ASSERT_EQ(2 * i + 1, a[i]);
  }
}

TEST_F(vector_test, push_back_from_self) {
  constexpr size_t N = 500;

  container a;
  a.push_back(42);
  for (size_t i = 1; i < N; ++i) {
    a.push_back(a[0]);
  }

  EXPECT_EQ(N, a.size());
  EXPECT_LE(N, a.capacity());

  for (size_t i = 0; i < N; ++i) {
    ASSERT_EQ(42, a[i]);
  }
}

TEST_F(vector_test, push_back_copies) {
  constexpr size_t N = 500;

  container a;
  a.reserve(N + 1);
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  element::reset_counters();
  a.push_back(N);
  EXPECT_GE(1, element::get_copy_counter());
  EXPECT_GE(0, element::get_swap_counter());
}

TEST_F(vector_test, push_back_reallocation_copies) {
  constexpr size_t N = 500;

  container a;
  a.reserve(N);
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  element::reset_counters();
  a.push_back(N);
  EXPECT_GE(N + 1, element::get_copy_counter() + element::get_swap_counter());
}

TEST_F(vector_test, push_back_throw) {
  constexpr size_t N = 500;

  container a;
  a.reserve(N + 1);
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  element::set_copy_throw_countdown(7);
  EXPECT_NO_THROW(a.push_back(42));
}

TEST_F(vector_test, push_back_reallocation_throw) {
  constexpr size_t N = 500;

  container a;
  a.reserve(N);
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  immutable_guard g(a);
  element::set_copy_throw_countdown(N - 1);
  EXPECT_THROW(a.push_back(42), std::runtime_error);
}

TEST_F(vector_test, push_back_reallocation_throw_last) {
  constexpr size_t N = 500;

  container a;
  a.reserve(N);
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  immutable_guard g(a);
  element::set_copy_throw_countdown(N + 1);
  EXPECT_THROW(a.push_back(42), std::runtime_error);
}

TEST_F(vector_test, subscript) {
  constexpr size_t N = 500, K = 100;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  for (size_t i = 0; i < K; ++i) {
    a[i] = 4 * i + 1;
  }

  for (size_t i = 0; i < K; ++i) {
    ASSERT_EQ(4 * i + 1, a[i]);
  }
  for (size_t i = K; i < N; ++i) {
    ASSERT_EQ(2 * i + 1, a[i]);
  }
}

TEST_F(vector_test, subscript_const) {
  constexpr size_t N = 500, K = 100;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  for (size_t i = 0; i < K; ++i) {
    a[i] = 4 * i + 1;
  }

  for (size_t i = 0; i < K; ++i) {
    ASSERT_EQ(4 * i + 1, as_const(a)[i]);
  }
  for (size_t i = K; i < N; ++i) {
    ASSERT_EQ(2 * i + 1, as_const(a)[i]);
  }
}

TEST_F(vector_test, data) {
  constexpr size_t N = 500;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  element* data = a.data();
  for (size_t i = 0; i < N; ++i) {
    ASSERT_EQ(2 * i + 1, data[i]);
    ASSERT_EQ(&a[i], data + i);
  }
}

TEST_F(vector_test, data_const) {
  constexpr size_t N = 500;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  const element* cdata = as_const(a).data();
  for (size_t i = 0; i < N; ++i) {
    ASSERT_EQ(2 * i + 1, cdata[i]);
    ASSERT_EQ(&as_const(a)[i], cdata + i);
  }
}

TEST_F(vector_test, front_back) {
  constexpr size_t N = 500;
  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  EXPECT_EQ(1, a.front());
  EXPECT_EQ(&a[0], &a.front());

  EXPECT_EQ(2 * N - 1, a.back());
  EXPECT_EQ(&a[N - 1], &a.back());
}

TEST_F(vector_test, front_back_const) {
  constexpr size_t N = 500;
  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  EXPECT_EQ(1, as_const(a).front());
  EXPECT_EQ(&a[0], &as_const(a).front());

  EXPECT_EQ(2 * N - 1, as_const(a).back());
  EXPECT_EQ(&a[N - 1], &as_const(a).back());
}

TEST_F(vector_test, reserve) {
  constexpr size_t N = 500, M = 100, K = 5000;

  container a;
  a.reserve(N);
  element::assert_no_instances();
  EXPECT_EQ(0, a.size());
  EXPECT_EQ(N, a.capacity());

  for (size_t i = 0; i < M; ++i) {
    a.push_back(2 * i + 1);
  }
  EXPECT_EQ(M, a.size());
  EXPECT_EQ(N, a.capacity());

  for (size_t i = 0; i < M; ++i) {
    ASSERT_EQ(2 * i + 1, a[i]);
  }

  a.reserve(K);
  EXPECT_EQ(M, a.size());
  EXPECT_EQ(K, a.capacity());

  for (size_t i = 0; i < M; ++i) {
    ASSERT_EQ(2 * i + 1, a[i]);
  }
}

TEST_F(vector_test, reserve_superfluous) {
  constexpr size_t N = 5000, M = 100, K = 500;

  container a;
  a.reserve(N);
  ASSERT_EQ(0, a.size());
  ASSERT_EQ(N, a.capacity());

  for (size_t i = 0; i < M; ++i) {
    a.push_back(2 * i + 1);
  }
  ASSERT_EQ(M, a.size());
  ASSERT_EQ(N, a.capacity());

  for (size_t i = 0; i < M; ++i) {
    ASSERT_EQ(2 * i + 1, a[i]);
  }

  element* old_data = a.data();

  a.reserve(K);
  EXPECT_EQ(M, a.size());
  EXPECT_EQ(N, a.capacity());
  EXPECT_EQ(old_data, a.data());

  for (size_t i = 0; i < M; ++i) {
    ASSERT_EQ(2 * i + 1, a[i]);
  }
}

TEST_F(vector_test, reserve_throw) {
  constexpr size_t N = 10, K = 7;

  container a;
  a.reserve(N);

  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  immutable_guard g(a);
  element::set_copy_throw_countdown(K);
  EXPECT_THROW(a.reserve(N + 1), std::runtime_error);
}

TEST_F(vector_test, shrink_to_fit) {
  constexpr size_t N = 500, M = 100;

  container a;
  a.reserve(N);
  ASSERT_EQ(0, a.size());
  ASSERT_EQ(N, a.capacity());

  for (size_t i = 0; i < M; ++i) {
    a.push_back(2 * i + 1);
  }
  ASSERT_EQ(M, a.size());
  ASSERT_EQ(N, a.capacity());

  for (size_t i = 0; i < M; ++i) {
    ASSERT_EQ(2 * i + 1, a[i]);
  }

  element::reset_counters();
  a.shrink_to_fit();
  EXPECT_GE(M, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  EXPECT_EQ(M, a.size());
  EXPECT_EQ(M, a.capacity());

  for (size_t i = 0; i < M; ++i) {
    ASSERT_EQ(2 * i + 1, a[i]);
  }
}

TEST_F(vector_test, shrink_to_fit_superfluous) {
  constexpr size_t N = 500;

  container a;
  a.reserve(N);
  ASSERT_EQ(0, a.size());
  ASSERT_EQ(N, a.capacity());

  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }
  ASSERT_EQ(N, a.size());

  size_t old_capacity = a.capacity();
  element* old_data = a.data();

  element::reset_counters();
  a.shrink_to_fit();
  EXPECT_EQ(0, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  EXPECT_EQ(N, a.size());
  EXPECT_EQ(old_capacity, a.capacity());
  EXPECT_EQ(old_data, a.data());

  for (size_t i = 0; i < N; ++i) {
    ASSERT_EQ(2 * i + 1, a[i]);
  }
}

TEST_F(vector_test, shrink_to_fit_empty) {
  container a;
  a.shrink_to_fit();
  expect_empty_storage(a);
  element::assert_no_instances();
}

TEST_F(vector_test, shrink_to_fit_throw) {
  constexpr size_t N = 10, K = 7;

  container a;
  a.reserve(N * 2);

  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  immutable_guard g(a);
  element::set_copy_throw_countdown(K);
  EXPECT_THROW(a.shrink_to_fit(), std::runtime_error);
}

TEST_F(vector_test, clear) {
  constexpr size_t N = 500;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }
  ASSERT_EQ(N, a.size());

  size_t old_capacity = a.capacity();
  element* old_data = a.data();

  a.clear();
  element::assert_no_instances();
  EXPECT_TRUE(a.empty());
  EXPECT_EQ(0, a.size());
  EXPECT_EQ(old_capacity, a.capacity());
  EXPECT_EQ(old_data, a.data());
}

TEST_F(vector_test, clear_throw) {
  constexpr size_t N = 500;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }
  ASSERT_EQ(N, a.size());

  element::set_copy_throw_countdown(1);
  element::set_swap_throw_countdown(1);
  EXPECT_NO_THROW(a.clear());
  EXPECT_TRUE(a.empty());
  element::assert_no_instances();
}

TEST_F(vector_test, swap_two_big) {
  container a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);

  container b;
  b.push_back(5);
  b.push_back(6);
  b.push_back(7);
  b.push_back(8);
  b.push_back(9);

  element::reset_counters();
  a.swap(b);
  EXPECT_EQ(0, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  EXPECT_EQ(5, a.size());
  EXPECT_EQ(4, b.size());
  EXPECT_EQ(6, a[1]);
  EXPECT_EQ(3, b[2]);

  element::reset_counters();
  a.swap(b);
  EXPECT_EQ(0, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  EXPECT_EQ(4, a.size());
  EXPECT_EQ(5, b.size());
  EXPECT_EQ(2, a[1]);
  EXPECT_EQ(7, b[2]);
}

TEST_F(vector_test, copy_ctor) {
  constexpr size_t N = 500;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  container b = a;
  EXPECT_EQ(a.size(), b.size());
  EXPECT_EQ(a.capacity(), b.capacity());

  for (size_t i = 0; i < N; ++i) {
    ASSERT_EQ(2 * i + 1, b[i]);
  }
}

TEST_F(vector_test, assignment_operator) {
  constexpr size_t N = 500;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  container b;
  b = a;
  EXPECT_EQ(a.size(), b.size());
  EXPECT_EQ(a.capacity(), b.capacity());

  container c;
  c.push_back(42);
  c = a;
  EXPECT_EQ(a.size(), c.size());
  EXPECT_EQ(a.capacity(), c.capacity());

  for (size_t i = 0; i < N; ++i) {
    ASSERT_EQ(2 * i + 1, a[i]);
    ASSERT_EQ(2 * i + 1, b[i]);
    ASSERT_EQ(2 * i + 1, c[i]);
  }
}

TEST_F(vector_test, self_assignment) {
  constexpr size_t N = 500;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  size_t old_capacity = a.capacity();
  element* old_data = a.data();

  a = a;
  EXPECT_EQ(N, a.size());
  EXPECT_EQ(old_capacity, a.capacity());
  EXPECT_EQ(old_data, a.data());

  for (size_t i = 0; i < N; ++i) {
    ASSERT_EQ(2 * i + 1, a[i]);
  }
}

TEST_F(vector_test, pop_back) {
  constexpr size_t N = 500;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  size_t old_capacity = a.capacity();
  element* old_data = a.data();

  for (size_t i = N; i > 0; --i) {
    ASSERT_EQ(2 * i - 1, a.back());
    ASSERT_EQ(i, a.size());
    a.pop_back();
  }
  element::assert_no_instances();
  EXPECT_TRUE(a.empty());
  EXPECT_EQ(0, a.size());
  EXPECT_EQ(old_capacity, a.capacity());
  EXPECT_EQ(old_data, a.data());
}

TEST_F(vector_test, insert_begin) {
  constexpr size_t N = 500;

  container a;
  for (size_t i = 0; i < N; ++i) {
    auto it = a.insert(a.begin(), 2 * i + 1);
    ASSERT_EQ(a.begin(), it);
    ASSERT_EQ(i + 1, a.size());
  }

  for (size_t i = 0; i < N; ++i) {
    ASSERT_EQ(2 * i + 1, a.back());
    a.pop_back();
  }

  element::assert_no_instances();
  ASSERT_TRUE(a.empty());
}

TEST_F(vector_test, insert_middle) {
  constexpr size_t N = 500, K = 100;

  container a;
  for (size_t i = 0; i < K; ++i) {
    a.push_back(2 * i + 1);
  }
  for (size_t i = K + N; i < N * 2; ++i) {
    a.push_back(2 * i + 1);
  }
  ASSERT_EQ(N, a.size());

  for (size_t i = K; i < K + N; ++i) {
    auto it = a.insert(a.begin() + i, 2 * i + 1);
    ASSERT_EQ(a.begin() + i, it);
  }

  for (size_t i = N * 2; i > 0; --i) {
    ASSERT_EQ(2 * i - 1, a.back());
    a.pop_back();
  }

  element::assert_no_instances();
  ASSERT_TRUE(a.empty());
}

TEST_F(vector_test, insert_end) {
  constexpr size_t N = 500;

  container a;
  for (size_t i = 0; i < N; ++i) {
    auto it = a.insert(a.end(), 2 * i + 1);
    ASSERT_EQ(a.end() - 1, it);
    ASSERT_EQ(i + 1, a.size());
  }

  for (size_t i = N; i > 0; --i) {
    ASSERT_EQ(2 * i - 1, a.back());
    a.pop_back();
  }

  element::assert_no_instances();
  ASSERT_TRUE(a.empty());
}

TEST_F(vector_test, insert_empty) {
  constexpr size_t N = 500;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(42);
  }
  for (size_t i = 0; i < N; ++i) {
    a.pop_back();
  }

  auto it = a.insert(a.begin(), 43);
  EXPECT_EQ(a.begin(), it);
  EXPECT_EQ(1, a.size());
  EXPECT_EQ(43, a[0]);
}

TEST_F(vector_test, insert_copies) {
  constexpr size_t N = 500, K = 100;

  container a;
  a.reserve(N + 1);
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }
  ASSERT_EQ(N, a.size());

  element::reset_counters();
  a.insert(a.begin() + K, 42);
  EXPECT_GE(N - K + 1, element::get_copy_counter() + element::get_swap_counter());
}

TEST_F(vector_test, insert_reallocation_copies) {
  constexpr size_t N = 500, K = 100;

  container a;
  a.reserve(N);
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }
  ASSERT_EQ(N, a.size());

  element::reset_counters();
  a.insert(a.begin() + K, 42);
  EXPECT_GE(N + 1, element::get_copy_counter() + element::get_swap_counter());
}

TEST_F(vector_test, insert_throw) {
  constexpr size_t N = 500, K = 100;

  container a;
  a.reserve(N + 2);
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }
  ASSERT_EQ(N, a.size());

  element::set_copy_throw_countdown(7);
  EXPECT_NO_THROW(a.insert(a.begin() + K, 42));

  element::set_copy_throw_countdown(0);
  element::set_swap_throw_countdown(7);
  EXPECT_THROW(a.insert(a.begin() + K, 42), std::runtime_error);
}

TEST_F(vector_test, insert_reallocation_throw) {
  constexpr size_t N = 500, K = 100;

  container a;
  a.reserve(N);
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }
  ASSERT_EQ(N, a.size());

  {
    immutable_guard g(a);
    element::set_copy_throw_countdown(N - 1);
    EXPECT_THROW(a.insert(a.begin() + K, 42), std::runtime_error);
  }
}

TEST_F(vector_test, insert_reallocation_throw_last) {
  constexpr size_t N = 500, K = 100;

  container a;
  a.reserve(N);
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }
  ASSERT_EQ(N, a.size());

  {
    immutable_guard g(a);
    element::set_copy_throw_countdown(N + 1);
    EXPECT_THROW(a.insert(a.begin() + K, 42), std::runtime_error);
  }
}

TEST_F(vector_test, erase_begin) {
  constexpr size_t N = 500;

  container a;
  for (size_t i = 0; i < N * 2; ++i) {
    a.push_back(2 * i + 1);
  }

  for (size_t i = 0; i < N; ++i) {
    auto it = a.erase(a.begin());
    ASSERT_EQ(a.begin(), it);
  }

  for (size_t i = 0; i < N; ++i) {
    ASSERT_EQ(2 * (i + N) + 1, a[i]);
  }
}

TEST_F(vector_test, erase_middle) {
  constexpr size_t N = 500, K = 200;

  container a;
  for (size_t i = 0; i < K; ++i) {
    a.push_back(2 * i + 1);
  }
  a.push_back(42);
  for (size_t i = K + 1; i < N + 1; ++i) {
    a.push_back(2 * i - 1);
  }

  size_t old_capacity = a.capacity();
  element* old_data = a.data();

  auto it = a.erase(a.begin() + K);
  ASSERT_EQ(a.begin() + K, it);
  ASSERT_EQ(N, a.size());
  ASSERT_EQ(old_capacity, a.capacity());
  ASSERT_EQ(old_data, a.data());

  for (size_t i = 0; i < N; ++i) {
    ASSERT_EQ(2 * i + 1, a[i]);
  }
}

TEST_F(vector_test, erase_end) {
  constexpr size_t N = 500;

  container a;
  for (size_t i = 0; i < N * 2; ++i) {
    a.push_back(2 * i + 1);
  }

  for (size_t i = 0; i < N; ++i) {
    auto it = a.erase(a.end() - 1);
    ASSERT_EQ(a.end(), it);
  }

  for (size_t i = 0; i < N; ++i) {
    ASSERT_EQ(2 * i + 1, a[i]);
  }
}

TEST_F(vector_test, erase_range_begin) {
  constexpr size_t N = 500, K = 100;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  size_t old_capacity = a.capacity();
  element* old_data = a.data();

  auto it = a.erase(a.begin(), a.begin() + K);
  EXPECT_EQ(a.begin(), it);
  EXPECT_EQ(N - K, a.size());
  EXPECT_EQ(old_capacity, a.capacity());
  EXPECT_EQ(old_data, a.data());

  for (size_t i = 0; i < N - K; ++i) {
    ASSERT_EQ(2 * (i + K) + 1, a[i]);
  }
}

TEST_F(vector_test, erase_range_middle) {
  constexpr size_t N = 500, K = 100;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  size_t old_capacity = a.capacity();
  element* old_data = a.data();

  auto it = a.erase(a.begin() + K, a.end() - K);
  EXPECT_EQ(a.begin() + K, it);
  EXPECT_EQ(K * 2, a.size());
  EXPECT_EQ(old_capacity, a.capacity());
  EXPECT_EQ(old_data, a.data());

  for (size_t i = 0; i < K; ++i) {
    ASSERT_EQ(2 * i + 1, a[i]);
  }
  for (size_t i = 0; i < K; ++i) {
    ASSERT_EQ(2 * (i + N - K) + 1, a[i + K]);
  }
}

TEST_F(vector_test, erase_range_end) {
  constexpr size_t N = 500, K = 100;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  size_t old_capacity = a.capacity();
  element* old_data = a.data();

  auto it = a.erase(a.end() - K, a.end());
  EXPECT_EQ(a.end(), it);
  EXPECT_EQ(N - K, a.size());
  EXPECT_EQ(old_capacity, a.capacity());
  EXPECT_EQ(old_data, a.data());

  for (size_t i = 0; i < N - K; ++i) {
    ASSERT_EQ(2 * i + 1, a[i]);
  }
}

TEST_F(vector_test, erase_range_all) {
  constexpr size_t N = 500;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  size_t old_capacity = a.capacity();
  element* old_data = a.data();

  auto it = a.erase(a.begin(), a.end());
  EXPECT_EQ(a.end(), it);

  element::assert_no_instances();
  EXPECT_TRUE(a.empty());
  EXPECT_EQ(0, a.size());
  EXPECT_EQ(old_capacity, a.capacity());
  EXPECT_EQ(old_data, a.data());
}

TEST_F(vector_test, erase_range_empty) {
  constexpr size_t N = 500;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  size_t old_capacity = a.capacity();
  element* old_data = a.data();

  element::reset_counters();

  auto it = a.begin() + 100;
  EXPECT_EQ(it, a.erase(it, it));

  EXPECT_EQ(0, element::get_copy_counter());
  EXPECT_EQ(0, element::get_swap_counter());

  EXPECT_EQ(N, a.size());
  EXPECT_EQ(old_capacity, a.capacity());
  EXPECT_EQ(old_data, a.data());
}

TEST_F(vector_test, erase_range_copies) {
  constexpr size_t N = 500, K = 100;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  element::reset_counters();
  a.erase(a.begin() + K, a.end() - K);
  EXPECT_GE(K, element::get_copy_counter() + element::get_swap_counter());
}

TEST_F(vector_test, erase_range_throw) {
  constexpr size_t N = 500, K = 100;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  element::set_copy_throw_countdown(7);
  EXPECT_NO_THROW(a.erase(a.begin() + K, a.end() - K));
}

// This test actually checks memory leak in pair with @valgrind
TEST_F(vector_test, copy_throw) {
  constexpr size_t N = 10;

  container a;
  a.reserve(N);
  ASSERT_EQ(N, a.capacity());

  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  element::set_copy_throw_countdown(1);
  element::set_swap_throw_countdown(1);
  EXPECT_NO_THROW(container{a});
}

TEST_F(vector_test, assign_throw) {
  constexpr size_t N = 10;

  container a;
  a.reserve(N);

  for (size_t i = 0; i < N; ++i) {
    a.push_back(2 * i + 1);
  }

  container b;
  b.push_back(0);

  element::set_copy_throw_countdown(1);
  element::set_swap_throw_countdown(1);
  EXPECT_NO_THROW(b = a);
}

TEST_F(vector_test, range_based_for) {
  constexpr size_t N = 500;

  container a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(42);
  }

  size_t i = 0;
  for (element& e : a) {
    e = i * 2 + 1;
    ++i;
  }

  i = 0;
  for (const element& e : a) {
    ASSERT_EQ(i * 2 + 1, e);
    ++i;
  }
}

TEST_F(vector_test, member_aliases) {
  EXPECT_TRUE((std::is_same<element, container::value_type>::value));
  EXPECT_TRUE((std::is_same<element&, container::reference>::value));
  EXPECT_TRUE((std::is_same<const element&, container::const_reference>::value));
  EXPECT_TRUE((std::is_same<element*, container::pointer>::value));
  EXPECT_TRUE((std::is_same<const element*, container::const_pointer>::value));
  EXPECT_TRUE((std::is_same<element*, container::iterator>::value));
  EXPECT_TRUE((std::is_same<const element*, container::const_iterator>::value));
}
