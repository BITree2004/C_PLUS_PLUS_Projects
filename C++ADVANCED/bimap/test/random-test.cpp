#include "bimap.h"
#include "test-classes.h"

#include <gtest/gtest.h>

#include <random>

namespace {

template <typename T>
std::vector<std::pair<T, T>> eliminate_same(std::vector<T>& lefts, std::vector<T>& rights, std::mt19937& e) {
  // std::sort(lefts.begin(), lefts.end());
  auto last = std::unique(lefts.begin(), lefts.end());
  lefts.erase(last, lefts.end());
  last = std::unique(rights.begin(), rights.end());
  rights.erase(last, rights.end());

  size_t min = std::min(lefts.size(), rights.size());
  lefts.resize(min);
  rights.resize(min);

  std::shuffle(lefts.begin(), lefts.end(), e);
  std::shuffle(rights.begin(), rights.end(), e);

  std::vector<std::pair<T, T>> res(min);
  for (size_t i = 0; i < min; i++) {
    res[i] = {lefts[i], rights[i]};
  }

  return res;
}

} // namespace

static constexpr uint32_t seed = 1488228;

TEST(bimap_randomized, comparison) {
  std::cout << "Seed used for randomized compare test is " << seed << std::endl;

  bimap<uint32_t, uint32_t> b1;
  bimap<uint32_t, uint32_t> b2;

  size_t total = 40000;
  std::mt19937 e(seed);
  std::vector<uint32_t> lefts(total), rights(total);
  for (size_t i = 0; i < total; i++) {
    lefts[i] = e();
    rights[i] = e();
  }
  auto future_insertions = eliminate_same(lefts, rights, e);

  std::shuffle(future_insertions.begin(), future_insertions.end(), e);
  for (auto p : future_insertions) {
    b1.insert(p.first, p.second);
  }

  std::shuffle(future_insertions.begin(), future_insertions.end(), e);
  for (auto p : future_insertions) {
    b2.insert(p.first, p.second);
  }

  EXPECT_EQ(b1.size(), b2.size());
  EXPECT_EQ(b1, b2);
}

TEST(bimap_randomized, invariant_check) {
  std::cout << "Seed used for randomized invariant test is " << seed << std::endl;
  bimap<int, int> b;

  std::mt19937 e(seed);
  size_t ins = 0, skip = 0, total = 50000;
  for (size_t i = 0; i < total; i++) {
    auto op = e() % 10;
    if (op > 2) {
      ins++;
      b.insert(e(), e());
    } else {
      if (b.empty()) {
        skip++;
        continue;
      }
      auto it = b.end_left();
      while (it == b.end_left()) {
        it = b.lower_bound_left(e());
      }
      b.erase_left(it);
    }
    if (i % 100 == 0) {
      int previous = *b.begin_left();
      for (auto it = ++b.begin_left(); it != b.end_left(); it++) {
        EXPECT_GT(*it, previous);
        previous = *it;
      }
      previous = *b.begin_right();
      for (auto it = ++b.begin_right(); it != b.end_right(); it++) {
        EXPECT_GT(*it, previous);
        previous = *it;
      }
    }
  }
  std::cout << "Invariant check stats:" << std::endl;
  std::cout << "Performed " << ins << " insertions and " << total - ins - skip << " erasures. " << skip << " skipped."
            << std::endl;
}

TEST(bimap_randomized, compare_to_two_maps) {
  std::cout << "Seed used for randomized cmp2map test is " << seed << std::endl;

  bimap<int, int> b;
  std::map<int, int> left_view, right_view;

  std::mt19937 e(seed);
  size_t ins = 0, skip = 0, total = 60000;
  for (size_t i = 0; i < total; i++) {
    unsigned int op = e() % 10;
    if (op > 2) {
      ins++;
      // insertion
      int l = e(), r = e();
      b.insert(l, r);
      left_view.insert({l, r});
      right_view.insert({r, l});
    } else {
      // erasure
      if (b.empty()) {
        skip++;
        continue;
      }
      auto it = b.end_left();
      while (it == b.end_left()) {
        it = b.lower_bound_left(e());
      }
      EXPECT_EQ(left_view.erase(*it), 1);
      EXPECT_EQ(right_view.erase(*it.flip()), 1);
      b.erase_left(it);
    }
    if (i % 100 == 0) {
      // check
      EXPECT_EQ(b.size(), left_view.size());
      EXPECT_EQ(b.size(), right_view.size());
      auto lit = b.begin_left();
      auto mlit = left_view.begin();
      for (; lit != b.end_left() && mlit != left_view.end(); lit++, mlit++) {
        EXPECT_EQ(*lit, mlit->first);
        EXPECT_EQ(*lit.flip(), mlit->second);
      }
    }
  }
  std::cout << "Comparing to maps stat:" << std::endl;
  std::cout << "Performed " << ins << " insertions and " << total - ins - skip << " erasures. " << skip << " skipped."
            << std::endl;
}
