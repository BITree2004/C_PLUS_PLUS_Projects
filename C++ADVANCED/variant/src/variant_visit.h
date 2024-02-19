#pragma once

#include "variant_storage.h"

#include <array>

namespace variant_visit {
template <typename T, std::size_t... Idx>
struct tensor {
  T t_;

  constexpr T getter() const {
    return t_;
  }
};

template <typename T, std::size_t First, std::size_t... Tail>
struct tensor<T, First, Tail...> {
  std::array<tensor<T, Tail...>, First> matrix_;

  constexpr T getter(std::size_t index, auto... indexs) const {
    return matrix_.at(index).getter(indexs...);
  }
};

template <std::size_t... Coordinates>
struct get_first_in_seq {
  static constexpr std::size_t value = 0;
};

template <std::size_t First, std::size_t... Tail>
struct get_first_in_seq<First, Tail...> {
  static constexpr std::size_t value = First;
};

template <std::size_t... Indexs>
using first_index_to_seq = std::make_index_sequence<get_first_in_seq<Indexs...>::value>;

// Coordinates + Tail_Dim - all list of Coordinates at tensor;
// Сurrents - current coordinates being added
template <typename First_in_seq, typename Сoordinates, typename Tail_Dim, typename Func>
struct table_impl;

template <std::size_t... Сurrents, std::size_t... Сoordinates, std::size_t First, std::size_t... Tail_Dim,
          typename Func>
struct table_impl<std::index_sequence<Сurrents...>, std::index_sequence<Сoordinates...>,
                  std::index_sequence<First, Tail_Dim...>, Func> {
  static constexpr tensor<Func, First, Tail_Dim...> create() {
    return {table_impl<first_index_to_seq<Tail_Dim...>, std::index_sequence<Сoordinates..., Сurrents>,
                       std::index_sequence<Tail_Dim...>, Func>::create()...};
  }
};

template <std::size_t... Сoordinates, typename Result, typename Visitor>
struct table_impl<std::index_sequence<>, std::index_sequence<Сoordinates...>, std::index_sequence<>,
                  Result (*)(Visitor)> {
  using Func = Result (*)(Visitor);

  static constexpr Func create() {
    return [](Visitor vis) -> Result {
      return std::forward<Visitor>(vis)(std::integral_constant<std::size_t, Сoordinates>()...);
    };
  }
};

template <typename>
using zero_holder = std::integral_constant<std::size_t, 0>;

template <typename Visitor, typename... Variants>
constexpr auto table_instance =
    table_impl<first_index_to_seq<variant_size_v<std::remove_reference_t<Variants>>...>, std::index_sequence<>,
               std::index_sequence<variant_size_v<std::remove_reference_t<Variants>>...>,
               std::invoke_result_t<Visitor, zero_holder<Variants>...> (*)(Visitor)>::create();

template <typename Visitor, typename... Variants>
constexpr auto visit_ind(Visitor&& vis, Variants&&... vars) -> std::invoke_result_t<Visitor, zero_holder<Variants>...> {
  return table_instance<Visitor, Variants...>.getter(vars.index()...)(std::forward<Visitor>(vis));
}

template <typename R, typename Visitor, typename... Variants>
constexpr R visit_impl(Visitor&& vis, Variants&&... vars) {
  if ((vars.valueless_by_exception() || ...)) {
    throw bad_variant_access();
  }
  return variant_visit::visit_ind(
      [&](auto... indexs) -> R { return std::forward<Visitor>(vis)(get<indexs>(std::forward<Variants>(vars))...); },
      std::forward<Variants>(vars)...);
}

} // namespace variant_visit

template <typename R, typename Visitor, typename... Variants>
constexpr R visit(Visitor&& vis, Variants&&... vars) {
  return variant_visit::visit_impl<R, Visitor, Variants...>(std::forward<Visitor>(vis),
                                                            std::forward<Variants>(vars)...);
}

template <typename Visitor, typename... Variants>
constexpr decltype(auto) visit(Visitor&& vis, Variants&&... vars) {
  return variant_visit::visit_impl<decltype(vis(get<0>(std::declval<Variants>())...)), Visitor, Variants...>(
      std::forward<Visitor>(vis), std::forward<Variants>(vars)...);
}
