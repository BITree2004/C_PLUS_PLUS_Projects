#pragma once

#include <cstddef>

namespace tests::util {

struct move_counter {
  move_counter() = default;

  move_counter(move_counter&& other) noexcept : moves(other.moves + 1) {}

  move_counter& operator=(move_counter&&) = default;

  std::size_t get_moves() const {
    return moves;
  }

private:
  std::size_t moves = 0;
};

struct copy_counter {
  copy_counter() = default;

  copy_counter(const copy_counter& other) : copies(other.copies + 1) {}

  std::size_t get_copies() const {
    return copies;
  }

private:
  std::size_t copies = 0;
};

struct non_copyable {
  explicit non_copyable(int value) : value(value) {}

  non_copyable(non_copyable&&) = default;
  non_copyable& operator=(non_copyable&&) = default;

  int get_value() const {
    return value;
  }

private:
  non_copyable(const non_copyable&) = delete;
  non_copyable& operator=(const non_copyable&) = delete;

  int value;
};

} // namespace tests::util
