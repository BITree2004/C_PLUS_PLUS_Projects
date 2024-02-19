#include "optional.h"

#include <utility>

namespace {

struct cvalue {
  constexpr cvalue() : value(0) {}

  constexpr cvalue(int value) : value(value) {}

  constexpr cvalue(const cvalue& other) : value(other.value) {}

  constexpr cvalue& operator=(const cvalue& other) {
    value = other.value + 1;
    return *this;
  }

  constexpr int get() const {
    return value;
  }

private:
  int value;
};

} // namespace

static_assert(
    [] {
      optional<cvalue> a;
      return !static_cast<bool>(a);
    }(),
    "default ctor");

static_assert(
    [] {
      struct cvalue_with_nontrivial_dtor : cvalue {
        using cvalue::cvalue;

        constexpr ~cvalue_with_nontrivial_dtor() {}
      };

      optional<cvalue_with_nontrivial_dtor> a;
      return !static_cast<bool>(a);
    }(),
    "non-trivial dtor");

static_assert(
    [] {
      optional<cvalue> a(nullopt);
      return !static_cast<bool>(a);
    }(),
    "nullopt ctor");

static_assert(
    [] {
      optional<cvalue> a(42);
      return (*a).get() == 42;
    }(),
    "value ctor");

static_assert(
    [] {
      optional<cvalue> a(in_place, 42);
      return (*a).get() == 42;
    }(),
    "in_place ctor");

static_assert(
    [] {
      optional<cvalue> a(42);
      return (*std::as_const(a)).get() == 42;
    }(),
    "const indirection");

static_assert(
    [] {
      optional<cvalue> a(42);
      return a->get() == 42;
    }(),
    "member access");

static_assert(
    [] {
      optional<cvalue> a(42);
      return std::as_const(a)->get() == 42;
    }(),
    "member access const");

static_assert(
    [] {
      optional<int> a(42);
      return a == a;
    }(),
    "equals");

static_assert(
    [] {
      optional<int> a(42), b(43);
      return a != b;
    }(),
    "not equals");

static_assert(
    [] {
      optional<int> a(42), b(43);
      return a < b;
    }(),
    "less");

static_assert(
    [] {
      optional<int> a(42), b(43);
      return a <= b;
    }(),
    "less or equals");

static_assert(
    [] {
      optional<int> a(43), b(42);
      return a > b;
    }(),
    "greater");

static_assert(
    [] {
      optional<int> a(43), b(42);
      return a >= b;
    }(),
    "greater or equals");

static_assert(
    [] {
      optional<int> a(43);
      optional<int> b(a);
      return a == b;
    }(),
    "copy ctor");

static_assert(
    [] {
      optional<int> a(43);
      optional<int> b(std::move(a));
      return b && *b == 43;
    }(),
    "move ctor");

static_assert(
    [] {
      optional<int> a(43);
      optional<int> b(42);
      a = b;
      return a == b;
    }(),
    "copy assign");

static_assert(
    [] {
      optional<int> a(43);
      optional<int> b(42);
      a = std::move(b);
      return *a == 42;
    }(),
    "move assign");

static_assert(
    [] {
      optional<int> a(43);
      optional<int> b(42);
      a = std::move(b);
      return *a == 42;
    }(),
    "move assign");

static_assert(
    [] {
      optional<int> a(43);
      optional<int> b(42);
      swap(a, b);
      return *a == 42 && *b == 43;
    }(),
    "swap");

static_assert(
    [] {
      optional<int> a(43);
      a.reset();
      return !static_cast<bool>(a);
    }(),
    "reset");

static_assert(
    [] {
      optional<int> a(43);
      a.emplace(42);
      return *a == 42;
    }(),
    "emplace");
