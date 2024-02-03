#include "circular-buffer.h"
#include "fault-injection.h"
#include "test-utils.h"

#include <gtest/gtest.h>

#include <deque>
#include <random>
#include <vector>

namespace {

class random_test : public base_test {
protected:
  enum operation {
    PUSH_BACK,
    PUSH_FRONT,
    POP_BACK,
    POP_FRONT,
    INSERT,
    ERASE,
  };

  struct config {
    std::mt19937::result_type seed = std::mt19937::default_seed;
    std::vector<operation> operations;
    size_t iterations;
    size_t operations_per_iteration;
  };

  static void run_random_test(config cfg) {
    std::mt19937 rng(cfg.seed);
    std::uniform_int_distribution<int> value_dist(1, cfg.operations_per_iteration);
    std::uniform_int_distribution<size_t> op_dist(0, cfg.operations.size() - 1);

    circular_buffer<int> c;
    std::deque<int> d;

    for (size_t i = 0; i < cfg.iterations; ++i) {
      for (size_t j = 0; j < cfg.operations_per_iteration; ++j) {
        size_t op_idx = op_dist(rng);
        operation op = cfg.operations.begin()[op_idx];
        switch (op) {
        case PUSH_BACK: {
          int value = value_dist(rng);
          c.push_back(value);
          d.push_back(value);
          break;
        }
        case PUSH_FRONT: {
          int value = value_dist(rng);
          c.push_front(value);
          d.push_front(value);
          break;
        }
        case POP_BACK: {
          if (d.empty()) {
            continue;
          }
          c.pop_back();
          d.pop_back();
          break;
        }
        case POP_FRONT: {
          if (d.empty()) {
            continue;
          }
          c.pop_front();
          d.pop_front();
          break;
        }
        case INSERT: {
          std::uniform_int_distribution<size_t> pos_dist(0, d.size());
          int value = value_dist(rng);
          size_t pos = pos_dist(rng);
          c.insert(c.begin() + pos, value);
          d.insert(d.begin() + pos, value);
          break;
        }
        case ERASE: {
          if (d.empty()) {
            continue;
          }
          std::uniform_int_distribution<size_t> pos_dist(0, d.size() - 1);
          size_t pos = pos_dist(rng);
          c.erase(c.begin() + pos);
          d.erase(d.begin() + pos);
          break;
        }
        }
        ASSERT_EQ(d.size(), c.size());
      }
      expect_eq(c, d);
    }
  }
};

} // namespace

TEST_F(random_test, push_pop_back) {
  config cfg;
  cfg.seed = 1337;
  cfg.operations = {PUSH_BACK, POP_BACK};
  cfg.iterations = 100;
  cfg.operations_per_iteration = 100'000;
  run_random_test(cfg);
}

TEST_F(random_test, push_pop_front) {
  config cfg;
  cfg.seed = 1338;
  cfg.operations = {PUSH_FRONT, POP_FRONT};
  cfg.iterations = 100;
  cfg.operations_per_iteration = 100'000;
  run_random_test(cfg);
}

TEST_F(random_test, push_pop_back_front) {
  config cfg;
  cfg.seed = 1339;
  cfg.operations = {PUSH_BACK, PUSH_FRONT, POP_BACK, POP_FRONT};
  cfg.iterations = 100;
  cfg.operations_per_iteration = 100'000;
  run_random_test(cfg);
}

TEST_F(random_test, insert_erase) {
  config cfg;
  cfg.seed = 1340;
  cfg.operations = {INSERT, ERASE};
  cfg.iterations = 1'000;
  cfg.operations_per_iteration = 500;
  run_random_test(cfg);
}

TEST_F(random_test, all) {
  config cfg;
  cfg.seed = 1341;
  cfg.operations = {PUSH_BACK, PUSH_FRONT, POP_BACK, POP_FRONT, INSERT, ERASE};
  cfg.iterations = 1'000;
  cfg.operations_per_iteration = 1'500;
  run_random_test(cfg);
}
