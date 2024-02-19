#pragma once

#include "variant_visit.h"

#include <memory>

template <typename... Types>
class variant {
private:
  constexpr void destroy() {
    if (!valueless_by_exception()) {
      visit([]<typename T>(T& t) { t.~T(); }, *this);
      index_ = variant_npos;
    }
  }

  size_t index_{variant_npos};
  storage::VariadicUnion<Types...> storage_;

  template <size_t N, typename... Ts>
  friend constexpr variant_alternative_t<N, variant<Ts...>>& get(variant<Ts...>&);

  template <typename T, typename... Ts>
  friend constexpr bool holds_alternative(const variant<Ts...>& v);

public:
  // constructors
  constexpr variant()
    requires(!utility::is_default_constructible_v<Types...>)
  = delete;

  constexpr variant() noexcept(std::is_nothrow_default_constructible_v<variant_alternative_t<0, variant<Types...>>>)
    requires(utility::is_default_constructible_v<Types...>)
  {
    emplace<0>();
  }

  constexpr variant(const variant& other)
    requires(!utility::is_copy_constructible_v<Types...>)
  = delete;

  constexpr variant(const variant& other)
    requires(utility::is_copy_constructible_v<Types...>)
  {
    if (!other.valueless_by_exception()) {
      variant_visit::visit_ind(
          [this, &other](auto N) {
            if constexpr (N != variant_npos) {
              this->emplace<N>(get<N>(other));
            }
          },
          other);
    }
  }

  constexpr variant(const variant& other) noexcept
    requires(utility::is_trivially_copy_constructible_v<Types...>)
  = default;

  constexpr variant(variant&& other)
    requires(!utility::is_move_constructible_v<Types...>)
  = delete;

  constexpr variant(variant&& other) noexcept((std::is_nothrow_move_constructible_v<Types> && ...))
    requires(utility::is_move_constructible_v<Types...>)
  {
    if (!other.valueless_by_exception()) {
      variant_visit::visit_ind(
          [this, &other](auto N) {
            if constexpr (N != variant_npos) {
              this->emplace<N>(std::move(get<N>(other)));
            }
          },
          other);
    }
  }

  constexpr variant(variant&& other) noexcept
    requires(utility::is_trivially_move_constructible_v<Types...>)
  = default;

  template <std::size_t I, typename... Args>
    requires(I < sizeof...(Types) && std::is_constructible_v<variant_alternative_t<I, variant<Types...>>, Args...>)
  constexpr explicit variant(in_place_index_t<I>, Args&&... args) {
    emplace<I>(std::forward<Args>(args)...);
  }

  template <typename T, typename T_j = variant_alternative_t<utility::overload<T&&, Types...>::value, variant>>
  constexpr variant(T&& t) noexcept(std::is_nothrow_constructible_v<T_j, T>)
      : variant(in_place_type<T_j>, std::forward<T>(t)) {}

  template <typename T, typename... Args, std::size_t I = utility::get_index_by_type_v<T, Types...>>
    requires(std::is_constructible_v<T, Args...> && I < sizeof...(Types))
  constexpr explicit variant(in_place_type_t<T>, Args&&... args)
      : variant(in_place_index<I>, std::forward<Args>(args)...) {}

  constexpr variant& operator=(const variant&)
    requires(!utility::is_copy_assignable_v<Types...>)
  = delete;

  constexpr variant& operator=(const variant& other)
    requires(utility::is_copy_assignable_v<Types...>)
  {
    if (other.valueless_by_exception()) {
      destroy();
      return *this;
    }
    variant_visit::visit_ind(
        [this, &other](auto l, auto r) {
          if constexpr (l == r) {
            get<r>(*this) = get<r>(std::move(other));
          } else {
            using right_type = variant_alternative_t<r, variant<Types...>>;
            if constexpr (std::is_nothrow_copy_constructible_v<right_type> ||
                          !std::is_nothrow_move_constructible_v<right_type>) {
              emplace<r>(get<r>(other));
            } else {
              *this = variant<Types...>(other);
            }
          }
        },
        *this, other);
    index_ = other.index();
    return *this;
  }

  constexpr variant& operator=(const variant&)
    requires(utility::is_trivially_copy_assignable_v<Types...>)
  = default;

  constexpr variant& operator=(variant&&)
    requires(!utility::is_move_assignable_v<Types...>)
  = delete;

  constexpr variant& operator=(variant&& other) noexcept(
      ((std::is_nothrow_move_constructible_v<Types> && std::is_nothrow_move_assignable_v<Types>)&&...))
    requires(utility::is_move_assignable_v<Types...>)
  {
    if (other.valueless_by_exception()) {
      destroy();
    } else if (index() == other.index()) {
      variant_visit::visit_ind([this, &other](auto r) { get<r>(*this) = get<r>(std::move(other)); }, other);
    } else {
      variant_visit::visit_ind([this, &other](auto r) { this->emplace<r>(get<r>(std::move(other))); }, other);
    }
    index_ = other.index();
    return *this;
  }

  constexpr variant& operator=(variant&&)
    requires(utility::is_trivially_move_assignable_v<Types...>)
  = default;

  template <class T, std::size_t I = utility::overload<T&&, Types...>::value,
            typename T_j = variant_alternative_t<I, variant>>
  constexpr variant& operator=(T&& t) noexcept(std::is_nothrow_assignable_v<T_j&, T> &&
                                               std::is_nothrow_constructible_v<T_j, T>) {
    if (index() == I) {
      storage::assigment<I>(storage_, std::forward<T>(t));
    } else {
      if constexpr (std::is_nothrow_constructible_v<T_j, T> || !std::is_nothrow_move_constructible_v<T_j>) {
        emplace<I>(std::forward<T>(t));
      } else {
        emplace<I>(T_j(std::forward<T>(t)));
      }
    }
    return *this;
  }

  ~variant() noexcept
    requires(utility::is_trivially_destructible_v<Types...>)
  = default;

  ~variant() noexcept
    requires(!utility::is_trivially_destructible_v<Types...>)
  {
    destroy();
  }

  // other functions
  template <std::size_t I, class... Args>
  constexpr variant_alternative_t<I, variant<Types...>>& emplace(Args&&... args)
    requires(std::is_constructible_v<variant_alternative_t<I, variant<Types...>>, Args...> && I < sizeof...(Types))
  {
    destroy();
    std::construct_at(std::addressof(storage_), in_place_index<I>, std::forward<Args>(args)...);
    index_ = I;
    return get<I>(*this);
  }

  template <class T, class... Args, std::size_t I = utility::get_index_by_type_v<T, Types...>>
  constexpr T& emplace(Args&&... args)
    requires(std::is_constructible_v<T, Args...> && I < sizeof...(Types))
  {
    return emplace<I>(std::forward<Args>(args)...);
  }

  constexpr std::size_t index() const noexcept {
    return index_;
  }

  constexpr bool valueless_by_exception() const noexcept {
    return index() == variant_npos;
  }

  constexpr void swap(variant& other) noexcept(
      ((std::is_nothrow_move_constructible_v<Types> && std::is_nothrow_swappable_v<Types>)&&...)) {
    if (index() == other.index()) {
      if (valueless_by_exception()) {
        return;
      }
      variant_visit::visit_ind(
          [this, &other](auto N) {
            if constexpr (N != variant_npos) {
              using std::swap;
              swap(get<N>(*this), get<N>(other));
            }
          },
          other);
    } else {
      if (valueless_by_exception()) {
        other.swap(*this);
      } else if (other.valueless_by_exception()) {
        other = std::move(*this);
        destroy();
      } else {
        std::swap(*this, other);
      }
    }
  }
};

template <typename T, typename... Types>
constexpr bool holds_alternative(const variant<Types...>& v) {
  return utility::get_index_by_type_v<T, Types...> == v.index();
}

template <size_t N, typename... Ts>
constexpr variant_alternative_t<N, variant<Ts...>>& get(variant<Ts...>& v) {
  if (N == v.index()) {
    return storage::get_union<N>(v.storage_, in_place_index<N>);
  }
  throw bad_variant_access();
}

template <size_t N, typename... Ts>
constexpr const variant_alternative_t<N, variant<Ts...>>& get(const variant<Ts...>& v) {
  return get<N>(const_cast<variant<Ts...>&>(v));
}

template <size_t N, typename... Ts>
constexpr variant_alternative_t<N, variant<Ts...>>&& get(variant<Ts...>&& v) {
  return std::move(get<N>(v));
}

template <size_t N, typename... Ts>
constexpr const variant_alternative_t<N, variant<Ts...>>&& get(const variant<Ts...>&& v) {
  return std::move(get<N>(v));
}

template <typename T, typename... Ts>
constexpr T& get(variant<Ts...>& v) {
  return get<utility::get_index_by_type_v<T, Ts...>>(v);
}

template <typename T, typename... Ts>
constexpr T&& get(variant<Ts...>&& v) {
  return std::move(get<T>(v));
}

template <typename T, typename... Ts>
constexpr const T& get(const variant<Ts...>& v) {
  return get<T>(const_cast<variant<Ts...>&>(v));
}

template <typename T, typename... Ts>
constexpr const T&& get(const variant<Ts...>&& v) {
  return std::move(get<T>(v));
}

template <std::size_t I, class... Ts>
constexpr std::add_pointer_t<variant_alternative_t<I, variant<Ts...>>> get_if(variant<Ts...>* pv) noexcept {
  return const_cast<std::add_pointer_t<variant_alternative_t<I, variant<Ts...>>>>(
      get_if<I>(const_cast<const variant<Ts...>*>(pv)));
}

template <std::size_t I, class... Ts>
constexpr std::add_pointer_t<const variant_alternative_t<I, variant<Ts...>>> get_if(const variant<Ts...>* pv) noexcept {
  static_assert(I < sizeof...(Ts));
  if (pv && pv->index() == I) {
    return std::addressof(get<I>(*pv));
  }
  return nullptr;
}

template <class T, class... Types, std::size_t I = utility::get_index_by_type_v<T, Types...>>
constexpr std::add_pointer_t<T> get_if(variant<Types...>* pv) noexcept {
  return get_if<I>(pv);
}

template <class T, class... Types, std::size_t I = utility::get_index_by_type_v<T, Types...>>
constexpr std::add_pointer_t<const T> get_if(const variant<Types...>* pv) noexcept {
  return get_if<I>(pv);
}

template <class... Types>
constexpr bool operator==(const variant<Types...>& v, const variant<Types...>& w) {
  if (v.index() != w.index()) {
    return false;
  } else if (v.valueless_by_exception()) {
    return true;
  }
  return variant_visit::visit_ind([&](auto N) { return get<N>(v) == get<N>(w); }, v);
}

template <class... Types>
constexpr bool operator!=(const variant<Types...>& v, const variant<Types...>& w) {
  if (v.index() != w.index()) {
    return true;
  } else if (v.valueless_by_exception()) {
    return false;
  }
  return variant_visit::visit_ind([&](auto N) { return get<N>(v) != get<N>(w); }, v);
}

template <typename... Types>
constexpr bool operator<(const variant<Types...>& v, const variant<Types...>& w) {
  if (w.valueless_by_exception()) {
    return false;
  } else if (v.valueless_by_exception()) {
    return true;
  } else if (v.index() != w.index()) {
    return v.index() < w.index();
  }
  return variant_visit::visit_ind([&](auto N) { return get<N>(v) < get<N>(w); }, v);
}

template <typename... Types>
constexpr bool operator>(const variant<Types...>& v, const variant<Types...>& w) {
  if (v.valueless_by_exception()) {
    return false;
  } else if (w.valueless_by_exception()) {
    return true;
  } else if (v.index() != w.index()) {
    return v.index() > w.index();
  }
  return variant_visit::visit_ind([&](auto N) { return get<N>(v) > get<N>(w); }, v);
}

template <class... Types>
constexpr bool operator<=(const variant<Types...>& v, const variant<Types...>& w) {
  if (v.valueless_by_exception()) {
    return true;
  } else if (w.valueless_by_exception()) {
    return false;
  } else if (v.index() != w.index()) {
    return v.index() < w.index();
  }
  return variant_visit::visit_ind([&](auto N) { return get<N>(v) <= get<N>(w); }, v);
}

template <class... Types>
constexpr bool operator>=(const variant<Types...>& v, const variant<Types...>& w) {
  if (w.valueless_by_exception()) {
    return true;
  } else if (v.valueless_by_exception()) {
    return false;
  } else if (v.index() != w.index()) {
    return v.index() > w.index();
  }
  return variant_visit::visit_ind([&](auto N) { return get<N>(v) >= get<N>(w); }, v);
}

template <class... Types>
constexpr std::common_comparison_category_t<std::compare_three_way_result_t<Types>...> operator<=>(
    const variant<Types...>& v, const variant<Types...>& w) {
  if (v.valueless_by_exception() && w.valueless_by_exception()) {
    return std::strong_ordering::equal;
  } else if (v.valueless_by_exception()) {
    return std::strong_ordering::less;
  } else if (w.valueless_by_exception()) {
    return std::strong_ordering::greater;
  } else if (v.index() != w.index()) {
    return v.index() <=> w.index();
  }
  return visit(
      [](auto& x, auto& y) -> std::common_comparison_category_t<std::compare_three_way_result_t<Types>...> {
        return x <=> y;
      },
      v, w);
}
