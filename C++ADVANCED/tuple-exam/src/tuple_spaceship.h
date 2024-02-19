#pragma once

#include "tuple_storage.h"

namespace details {

template <typename T, typename U>
constexpr auto compare(const T& t, const U& u)
  requires requires {
    { t < u } -> std::convertible_to<bool>;
    { u < t } -> std::convertible_to<bool>;
  }
{
  if constexpr (std::three_way_comparable_with<T, U>) {
    return t <=> u;
  } else {
    if (t < u) {
      return std::weak_ordering::less;
    } else if (u < t) {
      return std::weak_ordering::greater;
    } else {
      return std::weak_ordering::equivalent;
    }
  }
}

template <typename T, typename U>
using ordering_t = decltype(compare(std::declval<T&>(), std::declval<U&>()));

template <typename R, typename T, typename U>
constexpr R compare(const T&, const U&, std::index_sequence<>) {
  return R::equivalent;
}

template <typename R, typename T, typename U, size_t Head, size_t... Tail>
constexpr R compare(const T& lhs, const U& rhs, std::index_sequence<Head, Tail...>) {
  auto res = compare(get<Head>(lhs), get<Head>(rhs));
  if (res != R::equivalent) {
    return res;
  }
  return compare<R>(lhs, rhs, std::index_sequence<Tail...>());
}

template <std::size_t N, typename... TTypes, typename... UTypes>
constexpr bool equal(const tuple<TTypes...>& lhs, const tuple<UTypes...>& rhs) {
  if (get<N>(lhs) == get<N>(rhs)) {
    if constexpr (N == 0) {
      return true;
    } else {
      return equal<N - 1>(lhs, rhs);
    }
  }
  return false;
}

} // namespace details
