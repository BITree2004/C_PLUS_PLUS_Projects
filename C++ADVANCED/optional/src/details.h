#include "enable_function.h"

#include <memory>

struct nullopt_t {
  explicit constexpr nullopt_t(int32_t) {}
};

inline constexpr nullopt_t nullopt{0};

struct in_place_t {};

inline constexpr in_place_t in_place{};

namespace details {
template <typename T, bool triv>
struct destructor_base {
  constexpr destructor_base() noexcept {}

  constexpr destructor_base(const destructor_base&) noexcept = default;
  constexpr destructor_base(destructor_base&& other) noexcept = default;
  destructor_base& operator=(const destructor_base&) noexcept = default;
  destructor_base& operator=(destructor_base&&) noexcept = default;

  constexpr void reset() noexcept {
    if (is_present) {
      data.~T();
      is_present = false;
    }
  }

  constexpr ~destructor_base() noexcept {
    this->reset();
  }

protected:
  union {
    T data;
  };

  bool is_present = false;
};

template <typename T>
struct destructor_base<T, true> {

  constexpr destructor_base() noexcept {}

  constexpr destructor_base(const destructor_base&) noexcept = default;
  constexpr destructor_base(destructor_base&& other) noexcept = default;
  destructor_base& operator=(const destructor_base&) noexcept = default;
  destructor_base& operator=(destructor_base&&) noexcept = default;

  constexpr void reset() noexcept {
    if (is_present) {
      data.~T();
      is_present = false;
    }
  }

  constexpr ~destructor_base() noexcept = default;

protected:
  union {
    T data;
  };

  bool is_present = false;
};

template <typename T>
using destructor_base_t = destructor_base<T, std::is_trivially_destructible_v<T>>;

template <typename T, bool triv>
struct copy_base : destructor_base_t<T> {
private:
  using base = destructor_base_t<T>;

public:
  constexpr copy_base() noexcept = default;

  constexpr copy_base(const copy_base& other) : base() {
    if (other.is_present) {
      this->is_present = other.is_present;
      std::construct_at(&this->data, other.data);
    }
  }

  constexpr copy_base(copy_base&&) = default;
  copy_base& operator=(const copy_base& other) noexcept = default;
  copy_base& operator=(copy_base&& other) noexcept = default;
};

template <typename T>
struct copy_base<T, true> : destructor_base_t<T> {};

template <typename T>
using copy_t = copy_base<T, std::is_trivially_copy_constructible_v<T>>;

template <typename T, bool triv>
struct copy_assignment_base : copy_t<T>, enable_function::copy_t<T> {
public:
  constexpr copy_assignment_base() noexcept = default;
  constexpr copy_assignment_base(const copy_assignment_base&) = default;
  constexpr copy_assignment_base(copy_assignment_base&&) = default;

  copy_assignment_base& operator=(const copy_assignment_base& other) {
    if (!other.is_present) {
      this->reset();
      return *this;
    }
    if (this->is_present) {
      this->data = other.data;
    } else {
      std::construct_at(&this->data, other.data);
    }
    this->is_present = true;
    return *this;
  }

  copy_assignment_base& operator=(copy_assignment_base&&) noexcept = default;
};

template <typename T>
struct copy_assignment_base<T, true> : copy_base<T, true>, enable_function::copy_t<T> {};

template <typename T>
using assignment_t =
    copy_assignment_base<T, std::is_trivially_copy_constructible_v<T> && std::is_trivially_copy_assignable_v<T> &&
                                std::is_trivially_destructible_v<T>>;

template <typename T, bool triv>
struct move_base : assignment_t<T>, enable_function::assignment_t<T> {
public:
  constexpr move_base() noexcept = default;
  constexpr move_base(const move_base&) = default;

  constexpr move_base(move_base&& other) noexcept(std::is_nothrow_move_constructible_v<T>) {
    this->is_present = false;
    if (other.is_present) {
      this->is_present = true;
      std::construct_at(&this->data, std::move(other.data));
    }
  }

  move_base& operator=(const move_base&) noexcept = default;
  move_base& operator=(move_base&&) noexcept = default;
};

template <typename T>
struct move_base<T, true> : assignment_t<T>, enable_function::assignment_t<T> {};

template <typename T>
using move_t = move_base<T, std::is_trivially_move_constructible_v<T>>;

template <typename T, bool triv>
struct move_assignment_base : move_t<T>, enable_function::move_t<T> {
public:
  constexpr move_assignment_base() noexcept = default;
  constexpr move_assignment_base(const move_assignment_base&) = default;
  constexpr move_assignment_base(move_assignment_base&&) = default;
  move_assignment_base& operator=(const move_assignment_base& other) = default;

  move_assignment_base& operator=(move_assignment_base&& other) noexcept(
      std::is_nothrow_move_assignable_v<T>&& std::is_nothrow_move_constructible_v<T>) {
    if (!other.is_present) {
      this->reset();
      return *this;
    }
    if (this->is_present) {
      this->data = std::move(other.data);
    } else {
      std::construct_at(&this->data, std::move(other.data));
    }
    this->is_present = true;
    return *this;
  }
};

template <typename T>
struct move_assignment_base<T, true> : move_base<T, true>, enable_function::move_t<T> {};

template <typename T>
using move_assignment_t =
    move_assignment_base<T, std::is_trivially_move_assignable_v<T> && std::is_trivially_move_constructible_v<T> &&
                                std::is_trivially_destructible_v<T>>;

} // namespace details
