#include "circular-buffer.h"
#include "element.h"
#include "test-utils.h"

#include <gtest/gtest.h>

namespace {

class performance_test : public base_test {};

} // namespace

TEST_F(performance_test, insert_begin) {
  constexpr size_t N = 200'000;

  container c;
  c.reserve(N);

  for (size_t i = 0; i < N; ++i) {
    c.insert(c.begin(), 42);
  }
}

TEST_F(performance_test, insert_end) {
  constexpr size_t N = 200'000;

  container c;
  c.reserve(N);

  for (size_t i = 0; i < N; ++i) {
    c.insert(c.end(), 42);
  }
}

TEST_F(performance_test, insert_middle_1) {
  constexpr size_t N = 200'000;

  container c;
  c.reserve(N);

  mass_push_back(c, {1, 2});

  for (size_t i = 0; i < N; ++i) {
    c.insert(c.begin() + 2, 42);
  }
}

TEST_F(performance_test, insert_middle_2) {
  constexpr size_t N = 200'000;

  container c;
  c.reserve(N);

  mass_push_back(c, {1, 2});

  for (size_t i = 0; i < N; ++i) {
    c.insert(c.end() - 2, 42);
  }
}

TEST_F(performance_test, erase_begin) {
  constexpr size_t N = 200'000;

  container c;
  c.reserve(N);

  for (size_t i = 0; i < N; ++i) {
    c.push_back(42);
  }

  for (size_t i = 0; i < N; ++i) {
    c.erase(c.begin());
  }
}

TEST_F(performance_test, erase_end) {
  constexpr size_t N = 200'000;

  container c;
  c.reserve(N);

  for (size_t i = 0; i < N; ++i) {
    c.push_back(42);
  }

  for (size_t i = 0; i < N; ++i) {
    c.erase(c.end() - 1);
  }
}

TEST_F(performance_test, erase_middle_1) {
  constexpr size_t N = 200'000;

  container c;
  c.reserve(N);

  for (size_t i = 0; i < N; ++i) {
    c.push_back(42);
  }

  for (size_t i = 2; i < N; ++i) {
    c.erase(c.begin() + 2);
  }
}

TEST_F(performance_test, erase_middle_2) {
  constexpr size_t N = 200'000;

  container c;
  c.reserve(N);

  for (size_t i = 0; i < N; ++i) {
    c.push_back(42);
  }

  for (size_t i = 2; i < N; ++i) {
    c.erase(c.end() - 2);
  }
}

TEST_F(performance_test, erase_range_1) {
  constexpr size_t N = 200'000;
  constexpr size_t K = 1'000;

  container c;
  c.reserve(N);

  for (size_t i = 0; i < N; ++i) {
    c.push_back(42);
  }

  for (size_t i = K + 2; i < N; i += K) {
    c.erase(c.begin() + 2, c.begin() + 2 + K);
  }
}

TEST_F(performance_test, erase_range_2) {
  constexpr size_t N = 200'000;
  constexpr size_t K = 1'000;

  container c;
  c.reserve(N);

  for (size_t i = 0; i < N; ++i) {
    c.push_back(42);
  }

  for (size_t i = K + 2; i < N; i += K) {
    c.erase(c.end() - 2 - K, c.end() - 2);
  }
}

TEST_F(performance_test, erase_range_3) {
  constexpr size_t N = 200'000;

  container c;
  c.reserve(N);

  for (size_t i = 0; i < N; ++i) {
    c.push_back(42);
  }

  for (size_t i = 4; i < N; ++i) {
    c.erase(c.begin() + 2, c.end() - 2);
  }
}
