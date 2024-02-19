#pragma once

#include "variant_utility.h"

namespace storage {

template <typename... Types>
union VariadicUnion {};

template <typename Head, typename... Tail>
  requires(utility::is_trivially_destructible_v<Head, Tail...>)
union VariadicUnion<Head, Tail...> {
  Head head_;
  VariadicUnion<Tail...> tail_;

  constexpr VariadicUnion() {}

  ~VariadicUnion() = default;

  template <std::size_t N, typename... Args>
  constexpr explicit VariadicUnion(in_place_index_t<N>, Args&&... args)
      : tail_(in_place_index<N - 1>, std::forward<Args>(args)...) {}

  template <typename... Args>
  constexpr explicit VariadicUnion(in_place_index_t<0>, Args&&... args) : head_(std::forward<Args>(args)...) {}
};

template <typename Head, typename... Tail>
union VariadicUnion<Head, Tail...> {
  Head head_;
  VariadicUnion<Tail...> tail_;

  constexpr VariadicUnion() {}

  ~VariadicUnion() {}

  template <std::size_t N, typename... Args>
  constexpr explicit VariadicUnion(in_place_index_t<N>, Args&&... args)
      : tail_(in_place_index<N - 1>, std::forward<Args>(args)...) {}

  template <typename... Args>
  constexpr explicit VariadicUnion(in_place_index_t<0>, Args&&... args) : head_(std::forward<Args>(args)...) {}
};

template <size_t Index, typename Head, typename... Tail>
constexpr auto& get_union(VariadicUnion<Head, Tail...>& v, in_place_index_t<Index>) {
  if constexpr (Index == 0) {
    return v.head_;
  } else {
    return get_union<Index - 1>(v.tail_, in_place_index<Index - 1>);
  }
}

template <size_t Index, typename Head, typename... Tail, typename T>
constexpr void assigment(VariadicUnion<Head, Tail...>& v, T&& value) {
  if constexpr (Index == 0) {
    v.head_ = std::forward<T>(value);
  } else {
    assigment<Index - 1>(v.tail_, std::forward<T>(value));
  }
}

} // namespace storage
