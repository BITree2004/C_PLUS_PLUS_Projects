#pragma once

#include <algorithm>
#include <cstddef>
#include <exception>
#include <utility>

struct bad_function_call : std::exception {
  const char* what() const noexcept {
    return "tried to call empty bitree/function";
  }
};

namespace details {

constexpr size_t BUFFER_ALIGNMENT = alignof(std::max_align_t);
constexpr size_t BUFFER_MIN_SIZE = std::max(sizeof(void*), BUFFER_ALIGNMENT);
constexpr size_t BUFFER_SIZE = std::max(BUFFER_MIN_SIZE, 2 * BUFFER_MIN_SIZE - sizeof(void*));
using buffer_t = std::aligned_storage<BUFFER_SIZE, BUFFER_ALIGNMENT>::type;

template <class T>
constexpr bool is_small_object =
    (sizeof(T) <= BUFFER_SIZE) && BUFFER_ALIGNMENT % alignof(T) == 0 && std::is_nothrow_move_constructible_v<T>;

template <typename R, typename... Args>
struct descriptor_base {
  virtual R call(buffer_t* buffer, Args... args) const = 0;

  virtual void destroy(buffer_t* src) const noexcept = 0;

  virtual void copy(buffer_t* out, const buffer_t* src) const = 0;

  virtual void move(buffer_t* out, buffer_t* src) const noexcept = 0;
};

template <typename F, typename R, typename... Args>
struct descriptor : descriptor_base<R, Args...> {
  R call(buffer_t* buffer, Args... args) const override {
    return (*get_object(buffer))(std::forward<Args>(args)...);
  }

  void destroy(buffer_t* src) const noexcept override {
    delete get_object(src);
  }

  void copy(buffer_t* out, const buffer_t* src) const override {
    new (out) F*(new F(*get_object(src)));
  }

  void move(buffer_t* out, buffer_t* src) const noexcept override {
    new (out) F*(get_object(src));
  }

  F* get_object(const buffer_t* buffer) const noexcept {
    return *std::launder(reinterpret_cast<F* const*>(buffer));
  }

  void create_object(buffer_t* buffer, F&& func) const {
    new (buffer) F*(new F(std::move(func)));
  }
};

template <typename F, typename R, typename... Args>
  requires is_small_object<F>
struct descriptor<F, R, Args...> : descriptor_base<R, Args...> {
  R call(buffer_t* buffer, Args... args) const override {
    return (*get_object(buffer))(std::forward<Args>(args)...);
  }

  void destroy(buffer_t* src) const noexcept override {
    get_object(src)->~F();
  }

  void copy(buffer_t* out, const buffer_t* src) const override {
    new (out) F(*get_object(src));
  }

  void move(buffer_t* out, buffer_t* src) const noexcept override {
    new (out) F(std::move(*get_object(src)));
    destroy(src);
  }

  F* get_object(buffer_t* buffer) const noexcept {
    return std::launder(reinterpret_cast<F*>(buffer));
  }

  const F* get_object(const buffer_t* buffer) const noexcept {
    return std::launder(reinterpret_cast<const F*>(buffer));
  }

  void create_object(buffer_t* buffer, F&& func) const noexcept {
    new (buffer) F(std::move(func));
  }
};

struct tag_empty {};

template <typename R, typename... Args>
struct descriptor<tag_empty, R, Args...> : descriptor_base<R, Args...> {
  R call(buffer_t*, Args...) const override {
    throw bad_function_call();
  }

  void destroy(buffer_t*) const noexcept override {}

  void copy(buffer_t*, const buffer_t*) const noexcept override {}

  void move(buffer_t*, buffer_t*) const noexcept override {}
};

} // namespace details
