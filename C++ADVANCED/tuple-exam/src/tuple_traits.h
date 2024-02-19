#pragma once

#include <cstddef>
#include <type_traits>

template <typename... Types>
class tuple;

template <std::size_t N, typename T>
struct tuple_element;

template <std::size_t N, typename Head, typename... Tail>
struct tuple_element<N, tuple<Head, Tail...>> : tuple_element<N - 1, tuple<Tail...>> {};

template <typename Head, typename... Tail>
struct tuple_element<0, tuple<Head, Tail...>> : std::type_identity<Head> {};

template <std::size_t N, typename T>
using tuple_element_t = typename tuple_element<N, T>::type;

template <typename T>
struct tuple_size;

template <class... Types>
struct tuple_size<tuple<Types...>> : std::integral_constant<std::size_t, sizeof...(Types)> {};

template <typename T>
inline constexpr std::size_t tuple_size_v = tuple_size<T>::value;

namespace details {

template <std::size_t N, typename... Ts>
struct tuple_index;

template <std::size_t N, typename Type, typename H, typename... Ts>
struct tuple_index<N, Type, H, Ts...>
    : std::conditional_t<std::is_same_v<Type, H>, std::integral_constant<std::size_t, N>,
                         tuple_index<N + 1, Type, Ts...>> {};

template <typename... Ts>
inline constexpr std::size_t tuple_index_v = tuple_index<0, Ts...>::value;

} // namespace details
