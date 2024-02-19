#pragma once

#include <memory>

namespace detail {

struct control_block_t {
  size_t strong_ref_cnt{1};
  size_t weak_ref_cnt{0};

  virtual void clear() noexcept = 0;
  virtual ~control_block_t() = default;

  void check_lifetime() noexcept {
    if (strong_ref_cnt + weak_ref_cnt == 0) {
      delete this;
    }
  }

  void dec_strong() noexcept {
    --strong_ref_cnt;
    if (strong_ref_cnt == 0) {
      clear();
    }
    check_lifetime();
  }

  void dec_weak() noexcept {
    --weak_ref_cnt;
    check_lifetime();
  }

  void inc_strong() noexcept {
    ++strong_ref_cnt;
  }

  void inc_weak() noexcept {
    ++weak_ref_cnt;
  }
};

template <typename T, typename D = std::default_delete<T>>
struct control_object_pointer_t : control_block_t {
  T* ptr;
  [[no_unique_address]] D deleter;

  control_object_pointer_t(T* _ptr, D&& del) : ptr(_ptr), deleter(std::move(del)) {}

  void clear() noexcept override {
    deleter(ptr);
  }
};

template <typename T>
struct control_block_object_t : control_block_t {
  template <typename... Args>
  explicit control_block_object_t(Args&&... args) {
    new (&data) T(std::forward<Args>(args)...);
  }

  alignas(T) std::byte data[sizeof(T)];

  void clear() noexcept override {
    get()->~T();
  }

  T* get() noexcept {
    return std::launder(reinterpret_cast<T*>(&data));
  }
};

} // namespace detail
