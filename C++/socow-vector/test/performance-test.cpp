#include "socow-vector.h"
#include "test-utils.h"

#include <gtest/gtest.h>

using std::as_const;

class performance_test : public base_test {};

TEST_F(performance_test, insert) {
  constexpr size_t N = 10'000, M = 10'000;

  socow_vector<socow_vector<int, 3>, 3> a;
  for (size_t i = 0; i < N; ++i) {
    a.push_back(socow_vector<int, 3>());
    for (size_t j = 0; j < M; ++j) {
      a.back().push_back(2 * (i + 1) + 3 * j);
    }
  }

  socow_vector<int, 3> temp;
  for (size_t i = 0; i < M; ++i) {
    temp.push_back(3 * i);
  }

  a.insert(a.begin(), temp);

  for (size_t i = 0; i <= N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      ASSERT_EQ(2 * i + 3 * j, a[i][j]);
    }
  }
}

TEST_F(performance_test, erase) {
  constexpr size_t N = 10'000, M = 50'000, K = 100;

  socow_vector<int, 3> a;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      a.push_back(j);
    }
    a.erase(a.begin() + K, a.end() - K);
    ASSERT_EQ(K * 2, a.size());
    a.clear();
  }
}

TEST_F(performance_test, socow) {
  constexpr size_t N = 10'000'000, M = 10'000'000;

  socow_vector<socow_vector<int, 3>, 3> a;

  socow_vector<int, 3> temp;
  for (size_t i = 0; i < M; ++i) {
    temp.push_back(2 * i);
  }

  for (size_t i = 0; i < N; ++i) {
    a.push_back(temp);
  }

  for (size_t i = 0; i < N; ++i) {
    ASSERT_EQ(2 * i, as_const(a)[123'456][i]);
  }
}
