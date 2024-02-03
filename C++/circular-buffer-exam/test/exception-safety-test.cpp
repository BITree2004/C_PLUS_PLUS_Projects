#include "circular-buffer.h"
#include "element.h"
#include "fault-injection.h"
#include "test-utils.h"

#include <gtest/gtest.h>

namespace {

class exception_safety_test : public base_test {};

} // namespace

TEST_F(exception_safety_test, default_ctor_no_throw) {
  faulty_run([] {
    try {
      container c;
    } catch (...) {
      fault_injection_disable dg;
      ADD_FAILURE() << "default constructor should not throw";
      throw;
    }
  });
}

TEST_F(exception_safety_test, push_back) {
  faulty_run([] {
    container c;
    mass_push_back(c, {1, 2, 3, 4});

    strong_exception_safety_guard sg(c);
    c.push_back(42);
    expect_eq(c, {1, 2, 3, 4, 42});
  });
}

TEST_F(exception_safety_test, push_front) {
  faulty_run([] {
    container c;
    mass_push_front(c, {1, 2, 3, 4});

    strong_exception_safety_guard sg(c);
    c.push_front(42);
    expect_eq(c, {42, 4, 3, 2, 1});
  });
}

TEST_F(exception_safety_test, insert_1) {
  faulty_run([] {
    container c;
    mass_push_back(c, {1, 2, 3, 4, 5});

    c.insert(c.begin() + 2, 42);
    expect_eq(c, {1, 2, 42, 3, 4, 5});
  });
}

TEST_F(exception_safety_test, insert_2) {
  faulty_run([] {
    container c;
    mass_push_back(c, {1, 2, 3, 4, 5});

    c.insert(c.begin() + 3, 42);
    expect_eq(c, {1, 2, 3, 42, 4, 5});
  });
}

TEST_F(exception_safety_test, erase_1) {
  faulty_run([] {
    container c;
    mass_push_back(c, {1, 2, 3, 4, 5, 6});

    c.erase(c.begin() + 2);
    expect_eq(c, {1, 2, 4, 5, 6});
  });
}

TEST_F(exception_safety_test, erase_2) {
  faulty_run([] {
    container c;
    mass_push_back(c, {1, 2, 3, 4, 5, 6});

    c.erase(c.begin() + 3);
    expect_eq(c, {1, 2, 3, 5, 6});
  });
}

TEST_F(exception_safety_test, erase_range) {
  faulty_run([] {
    container c;
    mass_push_back(c, {1, 2, 3, 4, 5, 6, 7, 8});

    c.erase(c.begin() + 2, c.end() - 2);
    expect_eq(c, {1, 2, 7, 8});
  });
}

TEST_F(exception_safety_test, clear_no_throw) {
  faulty_run([] {
    container c;
    mass_push_back(c, {1, 2, 3, 4});
    try {
      c.clear();
    } catch (...) {
      fault_injection_disable dg;
      ADD_FAILURE() << "clear() should not throw";
      throw;
    }
  });
}

TEST_F(exception_safety_test, copy_ctor) {
  faulty_run([] {
    container c;
    mass_push_back(c, {1, 2, 3, 4, 5});

    container c2 = c;
    expect_eq(c, {1, 2, 3, 4, 5});
  });
}

TEST_F(exception_safety_test, copy_assignment) {
  faulty_run([] {
    container c1;
    mass_push_back(c1, {1, 2, 3, 4});

    container c2;
    mass_push_back(c2, {5, 6, 7, 8, 9});

    strong_exception_safety_guard sg(c1);
    c1 = c2;
    expect_eq(c1, {5, 6, 7, 8, 9});
  });
}
