#pragma once

#include "storage.h"

template <typename F>
class function;

template <typename R, typename... Args>
class function<R(Args...)> {
public:
  function() noexcept : _descriptor(&empty_descriptor) {}

  template <typename F>
  function(F func) : _descriptor(&descriptor_v<F>) {
    descriptor_v<F>.create_object(&_buffer, std::move(func));
  }

  function(const function& other) {
    _descriptor = other._descriptor;             // algo
    _descriptor->copy(&_buffer, &other._buffer); // copy data
  }

  function(function&& other) noexcept : function() {
    *this = std::move(other);
  }

  function& operator=(const function& other) {
    if (this != &other) {
      *this = function(other);
    }
    return *this;
  }

  function& operator=(function&& other) noexcept {
    if (this != &other) {
      clear();
      _descriptor = other._descriptor;             // algo
      _descriptor->move(&_buffer, &other._buffer); // move data
      other._descriptor = &empty_descriptor;       // clear algo
    }
    return *this;
  }

  void clear() noexcept {
    _descriptor->destroy(&_buffer);
    _descriptor = &empty_descriptor;
  }

  ~function() {
    clear();
  }

  explicit operator bool() const noexcept {
    return _descriptor != &empty_descriptor;
  }

  R operator()(Args... args) const {
    return _descriptor->call(&_buffer, std::forward<Args>(args)...);
  }

  template <typename F>
  F* target() noexcept {
    return const_cast<F*>(get_object<F>());
  }

  template <typename F>
  const F* target() const noexcept {
    return get_object<F>();
  }

private:
  template <typename F>
  static constexpr details::descriptor<F, R, Args...> descriptor_v = details::descriptor<F, R, Args...>();

  static constexpr details::descriptor<details::tag_empty, R, Args...> empty_descriptor =
      details::descriptor<details::tag_empty, R, Args...>(); // = &descriptor_v<details::empty>;

  template <typename F>
  F* get_object() const noexcept {
    if (_descriptor == &descriptor_v<F>) {
      return descriptor_v<F>.get_object(&_buffer);
    }
    return nullptr;
  }

  mutable details::buffer_t _buffer;
  const details::descriptor_base<R, Args...>* _descriptor;
};
