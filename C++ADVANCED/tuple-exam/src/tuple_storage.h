#pragma once

#include "tuple_traits.h"

#include <utility>

namespace details {
template <typename... Types>
struct storage {};

template <typename Head, typename... Types>
struct storage<Head, Types...> : public storage<Types...> {
private:
  using Base = storage<Types...>;

public:
  Head _head{};
  constexpr storage() = default;

  template <typename H, typename... Ts>
  constexpr storage(const storage<H, Ts...>& other)
    requires(std::is_constructible_v<Head, H>)
      : Base(static_cast<const storage<Ts...>&>(other)),
        _head(other._head) {}

  template <typename H, typename... Ts>
  constexpr storage(storage<H, Ts...>&& other)
    requires(std::is_constructible_v<Head, H>)
      : Base(std::move(static_cast<storage<Ts...>&&>(other))),
        _head(std::move(other._head)) {}

  template <typename H, typename... Ts>
    requires(std::is_constructible_v<Head, H>)
  constexpr storage(H&& head, Ts&&... tail) : Base(std::forward<Ts>(tail)...),
                                              _head(std::forward<H>(head)) {}

  template <std::size_t N>
  constexpr auto& get() {
    if constexpr (N == 0) {
      return _head;
    } else {
      return Base::template get<N - 1>();
    }
  }

  template <std::size_t N>
  constexpr const auto& get() const {
    if constexpr (N == 0) {
      return _head;
    } else {
      return Base::template get<N - 1>();
    }
  }
};

template <std::size_t N, typename... Ts>
constexpr auto& get(storage<Ts...>& _storage) {
  return _storage.template get<N>();
}

template <std::size_t N, typename... Ts>
constexpr const auto& get(const storage<Ts...>& _storage) {
  return _storage.template get<N>();
}

} // namespace details
