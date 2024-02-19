#include "details.h"

template <typename T>
class optional : enable_function::move_assignment_t<T>, public details::move_assignment_t<T> {
public:
  constexpr optional(T value) : optional(in_place, std::move(value)) {}

  template <typename... Args>
  explicit constexpr optional(in_place_t, Args&&... args) {
    std::construct_at(&this->data, std::forward<Args>(args)...);
    this->is_present = true;
  }

  template <typename... Args>
  constexpr void emplace(Args&&... args) {
    this->reset();
    std::construct_at(&this->data, std::forward<Args>(args)...);
    this->is_present = true;
  }

  constexpr optional() noexcept = default;

  constexpr optional(nullopt_t) noexcept : optional() {}

  constexpr optional(const optional&) = default;
  constexpr optional(optional&&) = default;

  constexpr optional& operator=(const optional&) = default;
  constexpr optional& operator=(optional&&) = default;

  constexpr optional& operator=(nullopt_t) noexcept {
    this->reset();
    return *this;
  }

  constexpr explicit operator bool() const noexcept {
    return this->is_present;
  }

  constexpr T& operator*() & noexcept {
    return this->data;
  }

  constexpr const T& operator*() const& noexcept {
    return this->data;
  }

  constexpr T&& operator*() && noexcept {
    return std::move(**this);
  }

  constexpr const T&& operator*() const&& noexcept {
    return std::move(**this);
  }

  constexpr T* operator->() noexcept {
    return &**this;
  }

  constexpr const T* operator->() const noexcept {
    return &**this;
  }

  constexpr void swap(optional& other) {
    if (!this->is_present && !other.is_present) {
      return;
    }
    if (this->is_present && other.is_present) {
      using std::swap;
      swap(**this, *other);
      return;
    }
    if (this->is_present) {
      other.swap(*this);
      return;
    }
    *this = std::move(other);
    other.reset();
  }
};

template <typename T>
constexpr bool operator==(const optional<T>& a, const optional<T>& b) {
  if (a && b) {
    return *a == *b;
  } else {
    return static_cast<bool>(a) == static_cast<bool>(b);
  }
}

template <typename T>
constexpr bool operator!=(const optional<T>& a, const optional<T>& b) {
  if (a && b) {
    return *a != *b;
  } else {
    return static_cast<bool>(a) != static_cast<bool>(b);
  }
}

template <typename T>
constexpr bool operator<(const optional<T>& a, const optional<T>& b) {
  if (!a) {
    return static_cast<bool>(b);
  } else if (!b) {
    return false;
  } else {
    return *a < *b;
  }
}

template <typename T>
constexpr bool operator<=(const optional<T>& a, const optional<T>& b) {
  if (!a) {
    return true;
  } else if (!b) {
    return false;
  } else {
    return *a <= *b;
  }
}

template <typename T>
constexpr bool operator>(const optional<T>& a, const optional<T>& b) {
  if (!a) {
    return false;
  } else if (!b) {
    return true;
  } else {
    return *a > *b;
  }
}

template <typename T>
constexpr bool operator>=(const optional<T>& a, const optional<T>& b) {
  if (!a) {
    return !b;
  } else if (!b) {
    return true;
  } else {
    return *a >= *b;
  }
}

template <typename T>
constexpr void swap(optional<T>& lhs, optional<T>& rhs) {
  lhs.swap(rhs);
}
