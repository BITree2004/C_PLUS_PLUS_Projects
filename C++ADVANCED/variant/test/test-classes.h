#pragma once

#include <cassert>
#include <cstddef>
#include <exception>
#include <vector>

template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
overload(Ts...) -> overload<Ts...>;

struct trivial_t {};

struct no_default_t {
  no_default_t() = delete;
};

struct throwing_default_t {
  throwing_default_t() {
    throw std::exception();
  }
};

struct throwing_move_operator_t {
  inline static size_t swap_called = 0;

  throwing_move_operator_t() = default;

  throwing_move_operator_t(throwing_move_operator_t&&) noexcept(false) {
    throw std::exception();
  }

  throwing_move_operator_t& operator=(throwing_move_operator_t&&) = default;
};

void swap(throwing_move_operator_t&, throwing_move_operator_t&) noexcept {
  throwing_move_operator_t::swap_called += 1;
}

struct no_copy_t {
  no_copy_t(const no_copy_t&) = delete;
};

struct no_move_t {
  no_move_t(no_move_t&&) = delete;
};

struct non_trivial_copy_t {
  explicit non_trivial_copy_t(int x) noexcept : x{x} {}

  non_trivial_copy_t(const non_trivial_copy_t& other) noexcept : x{other.x + 1} {}

  int x;
};

struct non_trivial_copy_assignment_t {
  static constexpr int CTOR_DELTA = 5;
  static constexpr int ASSIGN_DELTA = 6;

  explicit non_trivial_copy_assignment_t(int x) noexcept : x{x} {}

  non_trivial_copy_assignment_t(const non_trivial_copy_assignment_t& other) noexcept : x(other.x + CTOR_DELTA) {}

  non_trivial_copy_assignment_t& operator=(const non_trivial_copy_assignment_t& other) noexcept {
    if (this != &other) {
      x = other.x + ASSIGN_DELTA;
    }
    return *this;
  }

  int x;
};

struct non_trivial_int_wrapper_t {
  non_trivial_int_wrapper_t(int x) : x{x} {}

  non_trivial_int_wrapper_t& operator=(int i) {
    x = i + 1;
    return *this;
  }

  friend constexpr bool operator==(const non_trivial_int_wrapper_t& lhs,
                                   const non_trivial_int_wrapper_t& rhs) noexcept {
    return lhs.x == rhs.x;
  }

  friend constexpr bool operator!=(const non_trivial_int_wrapper_t& lhs,
                                   const non_trivial_int_wrapper_t& rhs) noexcept {
    return lhs.x != rhs.x;
  }

  friend constexpr bool operator<(const non_trivial_int_wrapper_t& lhs, const non_trivial_int_wrapper_t& rhs) noexcept {
    return lhs.x < rhs.x;
  }

  friend constexpr bool operator<=(const non_trivial_int_wrapper_t& lhs,
                                   const non_trivial_int_wrapper_t& rhs) noexcept {
    return lhs.x <= rhs.x;
  }

  friend constexpr bool operator>(const non_trivial_int_wrapper_t& lhs, const non_trivial_int_wrapper_t& rhs) noexcept {
    return lhs.x > rhs.x;
  }

  friend constexpr bool operator>=(const non_trivial_int_wrapper_t& lhs,
                                   const non_trivial_int_wrapper_t& rhs) noexcept {
    return lhs.x >= rhs.x;
  }

  friend constexpr auto operator<=>(const non_trivial_int_wrapper_t& lhs,
                                    const non_trivial_int_wrapper_t& rhs) noexcept {
    return lhs.x <=> rhs.x;
  }

  int x;
};

struct no_move_assignment_t {
  no_move_assignment_t& operator=(no_move_assignment_t&&) = delete;
};

struct no_copy_assignment_t {
  no_copy_assignment_t& operator=(const no_copy_assignment_t&) = delete;
};

struct throwing_move_assignment_t {
  throwing_move_assignment_t(throwing_move_assignment_t&&) = default;

  throwing_move_assignment_t& operator=(throwing_move_assignment_t&&) noexcept(false) {
    throw std::exception();
  }
};

struct only_movable {
  inline static size_t move_assignment_called = 0;

  constexpr only_movable() = default;

  constexpr only_movable(only_movable&& other) noexcept {
    assert(other.coin && "Move of moved value?");
    coin = true;
    other.coin = false;
  }

  constexpr only_movable& operator=(only_movable&& other) noexcept {
    if (this != &other) {
      assert(other.coin && "Move of moved value?");
      move_assignment_called += 1;
      coin = true;
      other.coin = false;
    }
    return *this;
  }

  [[nodiscard]] constexpr bool has_coin() const noexcept {
    return coin;
  }

  only_movable(const only_movable&) = delete;
  only_movable& operator=(const only_movable&) = delete;

private:
  bool coin{true};
};

struct yac_coin {
  constexpr operator int() noexcept {
    return 42;
  }
};

struct coin_wrapper {
  constexpr coin_wrapper() noexcept = default;

  constexpr coin_wrapper(coin_wrapper&& other) noexcept {
    assert(other.coin && "Move of moved value?");
    coin = other.coin;
    other.coin = 0;
  }

  constexpr coin_wrapper& operator=(coin_wrapper&& other) noexcept {
    if (this != &other) {
      assert((other.coin != 0) && "Move of moved value?");
      coin = other.coin;
      other.coin = 0;
    }
    return *this;
  }

  [[nodiscard]] constexpr auto has_coins() const noexcept {
    return coin;
  }

  constexpr explicit coin_wrapper(yac_coin) noexcept : coin{17} {}

  constexpr coin_wrapper(const coin_wrapper& other) noexcept : coin(other.coin + 1) {}

  constexpr coin_wrapper& operator=(const coin_wrapper& other) noexcept {
    if (this != &other) {
      coin = other.coin + 1;
    }
    return *this;
  }

private:
  int coin{1};
};

struct sqr_sum_visitor {
  template <typename... Args>
  constexpr long operator()(Args... args) const noexcept {
    return ((args * args) + ...);
  }
};

struct strange_visitor {
  strange_visitor() = default;

  strange_visitor(const strange_visitor&) = delete;
  strange_visitor(strange_visitor&&) = delete;
  strange_visitor& operator=(const strange_visitor&) = delete;
  strange_visitor& operator=(strange_visitor&&) = delete;

  constexpr int operator()(int value) & {
    return value;
  }

  constexpr int operator()(int value) && {
    return value + 1;
  }

  constexpr int operator()(int value) const& {
    return value + 2;
  }

  constexpr int operator()(int value) const&& {
    return value + 3;
  }
};

struct broken_address {
  broken_address* operator&() {
    return nullptr;
  }

  const broken_address* operator&() const {
    return nullptr;
  }

  std::vector<int> x;
};

struct empty_comparable {
  empty_comparable() = default;

  empty_comparable(empty_comparable&&) {
    throw std::exception();
  }

  empty_comparable& operator=(empty_comparable&&) {
    throw std::exception();
  }

  bool operator==(const empty_comparable&) const {
    throw std::exception();
  }

  bool operator!=(const empty_comparable&) const {
    throw std::exception();
  }

  bool operator<(const empty_comparable&) const {
    throw std::exception();
  }

  bool operator<=(const empty_comparable&) const {
    throw std::exception();
  }

  bool operator>(const empty_comparable&) const {
    throw std::exception();
  }

  bool operator>=(const empty_comparable&) const {
    throw std::exception();
  }
};

struct comparison_counters {
  size_t equal = 0;
  size_t not_equal = 0;
  size_t less = 0;
  size_t less_equal = 0;
  size_t greater = 0;
  size_t greater_equal = 0;
  size_t spaceship = 0;
};

struct custom_comparison {
  custom_comparison(int value, comparison_counters* counters) : value(value), counters(counters) {}

  bool operator==(const custom_comparison& other) const {
    ++counters->equal;
    return this->value == other.value;
  }

  bool operator!=(const custom_comparison& other) const {
    ++counters->not_equal;
    return this->value != other.value;
  }

  bool operator<(const custom_comparison& other) const {
    ++counters->less;
    return this->value < other.value;
  }

  bool operator<=(const custom_comparison& other) const {
    ++counters->less_equal;
    return this->value <= other.value;
  }

  bool operator>(const custom_comparison& other) const {
    ++counters->greater;
    return this->value > other.value;
  }

  bool operator>=(const custom_comparison& other) const {
    ++counters->greater_equal;
    return this->value >= other.value;
  }

  auto operator<=>(const custom_comparison& other) const {
    ++counters->spaceship;
    return this->value <=> other.value;
  }

private:
  int value;
  comparison_counters* counters;
};

template <typename Ordering>
struct custom_ordered {
  bool operator==(const custom_ordered&) const {
    return true;
  }

  Ordering operator<=>(const custom_ordered&) const noexcept {
    return Ordering::equivalent;
  }
};

using partially_ordered = custom_ordered<std::partial_ordering>;
using weak_ordered = custom_ordered<std::weak_ordering>;
using strong_ordered = custom_ordered<std::strong_ordering>;

struct throwing_members_params {
  bool throwing_copy = false;
  bool throwing_move = false;
  bool throwing_copy_assignment = false;
  bool throwing_move_assignment = false;
};

struct throwing_members_construct_tag {};

template <throwing_members_params Params>
struct throwing_members {
  inline static size_t copy_ctor_calls;
  inline static size_t move_ctor_calls;
  inline static size_t copy_assignment_calls;
  inline static size_t move_assignment_calls;

  static void reset_counters() {
    copy_ctor_calls = 0;
    move_ctor_calls = 0;
    copy_assignment_calls = 0;
    move_assignment_calls = 0;
  }

  static size_t copy_calls() {
    return copy_ctor_calls + copy_assignment_calls;
  }

  static size_t move_calls() {
    return move_ctor_calls + move_assignment_calls;
  }

  throwing_members(throwing_members_construct_tag) noexcept {}

  throwing_members(const throwing_members&) noexcept {
    ++copy_ctor_calls;
  }

  throwing_members(const throwing_members&)
    requires(Params.throwing_copy)
  {
    ++copy_ctor_calls;
    throw std::exception();
  }

  throwing_members(throwing_members&&) noexcept {
    ++move_ctor_calls;
  }

  throwing_members(throwing_members&&)
    requires(Params.throwing_move)
  {
    ++move_ctor_calls;
    throw std::exception();
  }

  throwing_members& operator=(const throwing_members&) noexcept {
    ++copy_assignment_calls;
    return *this;
  }

  throwing_members& operator=(const throwing_members&)
    requires(Params.throwing_copy || Params.throwing_copy_assignment)
  {
    ++copy_assignment_calls;
    throw std::exception();
  }

  throwing_members& operator=(throwing_members&&) noexcept {
    ++move_assignment_calls;
    return *this;
  }

  throwing_members& operator=(throwing_members&&)
    requires(Params.throwing_move || Params.throwing_move_assignment)
  {
    ++move_assignment_calls;
    throw std::exception();
  }
};
