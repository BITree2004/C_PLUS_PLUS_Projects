#pragma once

#include <gtest/gtest.h>

#include <ostream>
#include <unordered_set>

class element {
public:
  element(size_t val) : val(val) {
    add_instance();
  }

  element(const element& rhs) : val(rhs.val) {
    copy();
    add_instance();
  }

  element& operator=(const element& rhs) {
    assert_exists();
    rhs.assert_exists();
    copy();
    val = rhs.val;
    return *this;
  }

  ~element() {
    delete_instance();
  }

  void swap(element& other) {
    swap();
    std::swap(val, other.val);
  }

  static std::unordered_set<const element*>& instances() {
    static std::unordered_set<const element*> instances;
    return instances;
  }

  // TODO [maybe] return bool, assert in place
  static void assert_no_instances() {
    if (!instances().empty()) {
      instances().clear();
      FAIL() << "Not all instances are destroyed";
    }
  }

  static void reset_counters() {
    copy_counter = 0;
    swap_counter = 0;
  }

  static size_t get_copy_counter() {
    return copy_counter;
  }

  static size_t get_swap_counter() {
    return swap_counter;
  }

  static void set_copy_throw_countdown(size_t value) {
    copy_throw_countdown = value;
  }

  static void set_swap_throw_countdown(size_t value) {
    swap_throw_countdown = value;
  }

  friend bool operator==(const element& a, const element& b) {
    a.assert_exists();
    b.assert_exists();

    return a.val == b.val;
  }

  friend bool operator!=(const element& a, const element& b) {
    a.assert_exists();
    b.assert_exists();

    return a.val != b.val;
  }

  friend std::ostream& operator<<(std::ostream& out, const element& e) {
    return out << e.val;
  }

private:
  void add_instance() {
    auto p = instances().insert(this);
    if (!p.second) {
      FAIL() << "A new object is created at the address " << static_cast<void*>(this)
             << " while the previous object at this address was not destroyed";
    }
  }

  void delete_instance() {
    size_t erased = instances().erase(this);
    if (erased != 1) {
      FAIL() << "Attempt of destroying non-existing object at address " << static_cast<void*>(this);
    }
  }

  void assert_exists() const {
    const std::unordered_set<const element*>& inst = instances();
    bool exists = inst.find(this) != inst.end();
    if (!exists) {
      FAIL() << "Accessing an non-existsing object at address " << static_cast<const void*>(this);
    }
  }

  void copy() {
    if (copy_throw_countdown != 0) {
      if (--copy_throw_countdown == 0) {
        throw std::runtime_error("copy failed");
      }
    }
    ++copy_counter;
  }

  void swap() {
    if (swap_throw_countdown != 0) {
      if (--swap_throw_countdown == 0) {
        throw std::runtime_error("swap failed");
      }
    }
    ++swap_counter;
  }

private:
  size_t val;

  inline static size_t copy_throw_countdown = 0;
  inline static size_t swap_throw_countdown = 0;
  inline static size_t copy_counter = 0;
  inline static size_t swap_counter = 0;
};

template <>
inline void std::swap(element& left, element& right) {
  left.swap(right);
}
