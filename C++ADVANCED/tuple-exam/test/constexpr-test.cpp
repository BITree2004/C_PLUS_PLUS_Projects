#include "test-utils.h"
#include "tuple.h"

#include <string>
#include <utility>

using namespace tests;

static_assert([] {
  // tuple_size
  {
    using T0 = tuple<>;
    static_assert(tuple_size<T0>::value == 0);
    static_assert(tuple_size_v<T0> == 0);

    using T1 = tuple<int>;
    static_assert(tuple_size<T1>::value == 1);
    static_assert(tuple_size_v<T1> == 1);

    using T4 = tuple<int, double, int, int>;
    static_assert(tuple_size<T4>::value == 4);
    static_assert(tuple_size_v<T4> == 4);
  }

  // tuple_element
  {
    using T = tuple<int, double, std::string, int, int>;

    static_assert(std::is_same_v<tuple_element<0, T>::type, int>);
    static_assert(std::is_same_v<tuple_element_t<0, T>, int>);

    static_assert(std::is_same_v<tuple_element<1, T>::type, double>);
    static_assert(std::is_same_v<tuple_element_t<1, T>, double>);

    static_assert(std::is_same_v<tuple_element<2, T>::type, std::string>);
    static_assert(std::is_same_v<tuple_element_t<2, T>, std::string>);

    static_assert(std::is_same_v<tuple_element<3, T>::type, int>);
    static_assert(std::is_same_v<tuple_element_t<3, T>, int>);

    static_assert(std::is_same_v<tuple_element<4, T>::type, int>);
    static_assert(std::is_same_v<tuple_element_t<4, T>, int>);
  }

  // constructor traits
  {
    struct non_default_constructible {
      explicit non_default_constructible(int) {}
    };

    static_assert(std::is_default_constructible_v<tuple<int, double>>);
    static_assert(!std::is_default_constructible_v<tuple<int, non_default_constructible>>);

    static_assert(std::is_convertible_v<tuple<int, int>, tuple<long long, long long>>);
    static_assert(std::is_convertible_v<tuple<long long, long long>, tuple<int, int>>);

    static_assert(std::is_copy_constructible_v<tuple<int, int>>);

    static_assert(std::is_copy_constructible_v<tuple<std::string>>);
    static_assert(std::is_copy_assignable_v<tuple<std::string>>);

    static_assert(!std::is_copy_constructible_v<tuple<util::non_copyable>>);
    static_assert(std::is_move_constructible_v<tuple<util::non_copyable>>);
    static_assert(std::is_move_assignable_v<tuple<util::non_copyable>>);

    static_assert(!std::is_constructible_v<tuple<int, double>, tuple<int>>);
    static_assert(!std::is_constructible_v<tuple<int, double>, tuple<double>>);
    static_assert(!std::is_constructible_v<tuple<int, double>, tuple<int, double, int>>);
    static_assert(!std::is_constructible_v<tuple<int, double>, tuple<int, int, double>>);

    static_assert(!std::is_constructible_v<tuple<int>, std::string>);
    static_assert(!std::is_constructible_v<tuple<int>, tuple<std::string>>);
  }

  // triviality
  {
    static_assert(std::is_trivially_destructible_v<tuple<int, double>>);
    static_assert(std::is_trivially_copy_constructible_v<tuple<int, double>>);
    static_assert(std::is_trivially_copy_assignable_v<tuple<int, double>>);
    static_assert(std::is_trivially_move_constructible_v<tuple<int, double>>);
    static_assert(std::is_trivially_move_assignable_v<tuple<int, double>>);
  }

  // direct ctor
  {
    constexpr const char* str = "hello";
    constexpr tuple<int, double, const char*> t(42, 3.14, str);
    static_assert(get<0>(t) == 42);
    static_assert(get<1>(t) == 3.14);
    static_assert(get<2>(t) == str);
  }

  // make_tuple
  {
    constexpr const char* str = "hello";
    constexpr auto t = make_tuple(42, 3.14, str);
    static_assert(std::is_same_v<decltype(t), const tuple<int, double, const char*>>);
    static_assert(get<0>(t) == 42);
    static_assert(get<1>(t) == 3.14);
    static_assert(get<2>(t) == str);
  }

  // get
  {
    using T = tuple<int, double, float, int>;
    static_assert(requires(T t) {
      { get<1>(t) } -> std::same_as<double&>;
      { get<1>(std::as_const(t)) } -> std::same_as<const double&>;
      { get<1>(std::move(t)) } -> std::same_as<double&&>;
      { get<1>(std::move(std::as_const(t))) } -> std::same_as<const double&&>;
    });
    static_assert(requires(T t) {
      { get<double>(t) } -> std::same_as<double&>;
      { get<double>(std::as_const(t)) } -> std::same_as<const double&>;
      { get<double>(std::move(t)) } -> std::same_as<double&&>;
      { get<double>(std::move(std::as_const(t))) } -> std::same_as<const double&&>;
    });
  }

  // comparison
  {
    static_assert(tuple(1, 2, 3) == tuple(1, 2, 3));
    static_assert(tuple(1, 2, 3) < tuple(10, 2, 3));
  }

  // swap
  {
    constexpr tuple t = [] {
      tuple t1(42, 43);
      tuple t2(1337, 1338);

      using std::swap;
      swap(t1, t2);

      return tuple(get<0>(t1), get<1>(t1), get<0>(t2), get<1>(t2));
    }();
    static_assert(get<0>(t) == 1337);
    static_assert(get<1>(t) == 1338);
    static_assert(get<2>(t) == 42);
    static_assert(get<3>(t) == 43);
  }

  return true;
}());
