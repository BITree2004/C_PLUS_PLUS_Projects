#pragma once

#include "tuple_spaceship.h"

template <>
class tuple<> {};

template <typename... Types>
class tuple : public details::storage<Types...> {
private:
  using Base = details::storage<Types...>;

public:
  constexpr tuple()
    requires(std::conjunction_v<std::is_default_constructible<Types>...>)
  = default;

  constexpr tuple(const Types&... args) : Base(args...) {}

  tuple(const tuple&) = default;
  tuple(tuple&&) = default;

  tuple& operator=(const tuple&) = default;
  tuple& operator=(tuple&&) = default;

  template <typename... UTypes>
    requires(sizeof...(Types) == sizeof...(UTypes) && std::conjunction_v<std::is_constructible<Types, UTypes>...>)
  constexpr tuple(UTypes&&... args) : Base(std::forward<UTypes>(args)...) {}

  template <typename... UTypes>
    requires(sizeof...(Types) == sizeof...(UTypes) && std::conjunction_v<std::is_constructible<Types, UTypes>...>)
  constexpr tuple(const tuple<UTypes...>& other) : Base(other) {}

  template <typename... UTypes>
    requires(sizeof...(Types) == sizeof...(UTypes) && std::conjunction_v<std::is_constructible<Types, UTypes>...>)
  constexpr tuple(tuple<UTypes...>&& other) : Base(std::move(other)) {}
};

// deduction guide to aid CTAD
template <typename... Types>
tuple(Types...) -> tuple<Types...>;

template <typename... Types>
constexpr tuple<std::unwrap_ref_decay_t<Types>...> make_tuple(Types&&... args) {
  return tuple<std::unwrap_ref_decay_t<Types>...>(std::forward<Types>(args)...);
}

template <std::size_t N, typename... Types>
constexpr auto& get(tuple<Types...>& t) noexcept {
  return details::get<N>(t);
}

template <std::size_t N, typename... Types>
constexpr tuple_element_t<N, tuple<Types...>>&& get(tuple<Types...>&& t) noexcept {
  return std::move(get<N>(t));
}

template <std::size_t N, typename... Types>
constexpr const tuple_element_t<N, tuple<Types...>>& get(const tuple<Types...>& t) noexcept {
  return details::get<N>(t);
}

template <std::size_t N, typename... Types>
constexpr const tuple_element_t<N, tuple<Types...>>&& get(const tuple<Types...>&& t) noexcept {
  return std::move(get<N>(t));
}

template <typename T, typename... Types>
constexpr T& get(tuple<Types...>& t) noexcept {
  return get<details::tuple_index_v<T, Types...>>(t);
}

template <typename T, typename... Types>
constexpr T&& get(tuple<Types...>&& t) noexcept {
  return get<details::tuple_index_v<T, Types...>>(std::move(t));
}

template <typename T, typename... Types>
constexpr const T& get(const tuple<Types...>& t) noexcept {
  return get<details::tuple_index_v<T, Types...>>(t);
}

template <typename T, typename... Types>
constexpr const T&& get(const tuple<Types...>&& t) noexcept {
  return get<details::tuple_index_v<T, Types...>>(std::move(t));
}

template <typename... TTypes, typename... UTypes>
constexpr bool operator==(const tuple<TTypes...>& lhs, const tuple<UTypes...>& rhs) {
  return details::equal<sizeof...(TTypes) - 1>(lhs, rhs);
}

template <typename... TTypes, typename... UTypes>
constexpr auto operator<=>(const tuple<TTypes...>& lhs, const tuple<UTypes...>& rhs) {
  using R = std::common_comparison_category_t<details::ordering_t<TTypes, UTypes>...>;
  return compare<R>(lhs, rhs, std::index_sequence_for<TTypes...>());
}
