#pragma once

#include <utility>

namespace enable_function {

template <bool enable>
struct copy {};

template <>
struct copy<false> {
  constexpr copy() noexcept = default;
  constexpr copy(const copy&) noexcept = delete;
  constexpr copy(copy&&) noexcept = default;
  copy& operator=(const copy&) noexcept = default;
  copy& operator=(copy&&) noexcept = default;
};

template <typename T>
using copy_t = copy<std::is_copy_constructible_v<T>>;

template <bool enable>
struct move {};

template <>
struct move<false> {
  constexpr move() noexcept = default;
  constexpr move(const move&) noexcept = default;
  constexpr move(move&&) = delete;
  move& operator=(const move&) noexcept = default;
  move& operator=(move&&) noexcept = default;
};

template <typename T>
using move_t = move<std::is_move_constructible_v<T>>;

template <bool enable>
struct copy_assignment {};

template <>
struct copy_assignment<false> {
  constexpr copy_assignment() noexcept = default;
  constexpr copy_assignment(const copy_assignment&) noexcept = default;
  constexpr copy_assignment(copy_assignment&&) noexcept = default;
  copy_assignment& operator=(const copy_assignment&) noexcept = delete;
  copy_assignment& operator=(copy_assignment&&) noexcept = default;
};

template <typename T>
using assignment_t = copy_assignment<std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>>;

template <bool enable>
struct move_assignment {};

template <>
struct move_assignment<false> {
  constexpr move_assignment() noexcept = default;
  constexpr move_assignment(const move_assignment&) noexcept = default;
  constexpr move_assignment(move_assignment&&) noexcept = default;
  move_assignment& operator=(const move_assignment&) noexcept = default;
  move_assignment& operator=(move_assignment&&) noexcept = delete;
};

template <typename T>
using move_assignment_t = move_assignment<std::is_move_constructible_v<T> && std::is_move_assignable_v<T>>;

} // namespace enable_function
