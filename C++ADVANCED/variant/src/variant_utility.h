#pragma once

template <typename... Types>
class variant;

template <class>
struct in_place_type_t {
  explicit in_place_type_t() = default;
};

template <class T>
inline constexpr in_place_type_t<T> in_place_type{};

template <std::size_t>
struct in_place_index_t {
  explicit in_place_index_t() = default;
};

template <std::size_t I>
inline constexpr in_place_index_t<I> in_place_index{};

template <class T>
struct variant_size;

template <class... Types>
struct variant_size<variant<Types...>> : std::integral_constant<std::size_t, sizeof...(Types)> {};

template <class... Types>
struct variant_size<const variant<Types...>> : std::integral_constant<std::size_t, sizeof...(Types)> {};

template <class T>
inline constexpr std::size_t variant_size_v = variant_size<T>::value;

template <std::size_t I, class T>
struct variant_alternative;

template <std::size_t I, class Head, class... Types>
struct variant_alternative<I, variant<Head, Types...>> {
  using type = typename variant_alternative<I - 1, variant<Types...>>::type;
};

template <class Head, class... Types>
struct variant_alternative<0, variant<Head, Types...>> {
  using type = Head;
};

template <std::size_t I, class T>
struct variant_alternative<I, const T> {
  using type = const typename variant_alternative<I, T>::type;
};

template <size_t I, class T>
using variant_alternative_t = typename variant_alternative<I, T>::type;

class bad_variant_access : public std::exception {
public:
  bad_variant_access() noexcept = default;
  bad_variant_access(const bad_variant_access& other) noexcept = default;
  bad_variant_access& operator=(const bad_variant_access& other) noexcept = default;

  const char* what() const noexcept override {
    return "bitree/variant/bad_variant_access";
  }
};

constexpr size_t variant_npos = -1;

namespace utility {

template <typename T, typename... Args>
struct get_index_by_type {
  static constexpr size_t value = 1;
};

template <typename T, typename... Tail>
struct get_index_by_type<T, T, Tail...> {
  static constexpr size_t value = 0;
};

template <typename T, typename Head, typename... Tail>
struct get_index_by_type<T, Head, Tail...> {
  static constexpr size_t value = get_index_by_type<T, Tail...>::value + 1;
};

template <typename T, typename... Args>
constexpr size_t get_index_by_type_v = get_index_by_type<T, Args...>::value;

template <typename... Args>
concept is_trivially_destructible_v = (std::is_trivially_destructible_v<Args> && ...);

template <typename T, typename...>
constexpr bool is_default_constructible_v = std::is_default_constructible_v<T>;

template <typename... Args>
concept is_copy_constructible_v = (std::is_copy_constructible_v<Args> && ...);

template <typename... Args>
concept is_trivially_copy_constructible_v =
    (std::is_trivially_copy_constructible_v<Args> && ...) && is_copy_constructible_v<Args...>;

template <typename... Args>
concept is_move_constructible_v = (std::is_move_constructible_v<Args> && ...);

template <typename... Args>
concept is_trivially_move_constructible_v =
    (std::is_trivially_move_constructible_v<Args> && ...) && is_move_constructible_v<Args...>;

template <typename... Args>
concept is_copy_assignable_v = (std::is_copy_constructible_v<Args> && ...) && (std::is_copy_assignable_v<Args> && ...);

template <typename... Args>
concept is_trivially_copy_assignable_v =
    (std::is_trivially_copy_assignable_v<Args> && ...) && (std::is_trivially_copy_constructible_v<Args> && ...) &&
    (std::is_trivially_destructible_v<Args> && ...) && is_copy_assignable_v<Args...>;

template <typename... Args>
concept is_move_assignable_v = (std::is_move_constructible_v<Args> && ...) && (std::is_move_assignable_v<Args> && ...);

template <typename... Args>
concept is_trivially_move_assignable_v =
    (std::is_trivially_move_assignable_v<Args> && ...) && (std::is_trivially_move_constructible_v<Args> && ...) &&
    (std::is_trivially_destructible_v<Args> && ...) && is_move_assignable_v<Args...>;

template <typename Arg, typename Type>
concept overload_is_correct = requires(Arg&& t) { Type{std::forward<Arg>(t)}; };

template <typename Arg, std::size_t I, typename Type, typename... Types>
struct overloads : overloads<Arg, I + 1, Types...> {
  using overloads<Arg, I + 1, Types...>::getter;
  constexpr std::integral_constant<std::size_t, I> getter(Type)
    requires overload_is_correct<Arg, Type[]>;
};

template <typename Arg, std::size_t I, typename Type>
struct overloads<Arg, I, Type> {
  constexpr std::integral_constant<std::size_t, I> getter(Type)
    requires overload_is_correct<Arg, Type[]>;
};

template <typename Arg, typename... Types>
using overload = decltype(overloads<Arg, 0, Types...>().getter(std::declval<Arg>()));

} // namespace utility
