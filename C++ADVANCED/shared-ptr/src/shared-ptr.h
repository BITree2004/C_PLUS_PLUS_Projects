#pragma once

#include "control-block.h"
#include "weak-ptr.h"

template <typename T>
class shared_ptr {
public:
  shared_ptr() noexcept = default;

  shared_ptr(std::nullptr_t) noexcept : shared_ptr() {}

  template <typename Y>
  explicit shared_ptr(Y* ptr) : shared_ptr(ptr, std::default_delete<Y>()) {}

  template <typename Y, typename Deleter>
  shared_ptr(Y* ptr, Deleter deleter) : ptr(ptr) {
    try {
      cb = new detail::control_object_pointer_t<Y, Deleter>(ptr, std::move(deleter));
    } catch (...) {
      deleter(ptr);
      throw;
    }
  }

  template <typename Y>
  shared_ptr(const shared_ptr<Y>& other, T* ptr) noexcept : ptr(ptr),
                                                            cb(other.cb) {
    link();
  }

  template <typename Y>
  shared_ptr(shared_ptr<Y>&& other, T* ptr) noexcept : ptr(ptr),
                                                       cb(other.cb) {
    other.clear();
  }

  shared_ptr(const shared_ptr& other) noexcept : ptr(other.ptr), cb(other.cb) {
    link();
  }

  template <typename Y>
  shared_ptr(const shared_ptr<Y>& other) noexcept : ptr(other.ptr),
                                                    cb(other.cb) {
    link();
  }

  template <typename Y>
  shared_ptr(shared_ptr<Y>&& other) noexcept : ptr(other.ptr),
                                               cb(other.cb) {
    other.clear();
  }

  template <typename Y>
  explicit shared_ptr(const weak_ptr<Y>& other) : ptr(other.ptr),
                                                  cb(other.cb) {
    if (other.expired()) {
      throw bad_weak_ptr();
    }
    link();
  }

  shared_ptr& operator=(const shared_ptr& other) noexcept {
    if (this != &other) {
      swap_trick(other);
    }
    return *this;
  }

  template <typename Y>
  shared_ptr& operator=(const shared_ptr<Y>& other) noexcept {
    swap_trick(other);
    return *this;
  }

  template <typename Y>
  shared_ptr& operator=(shared_ptr<Y>&& other) noexcept {
    swap_trick(std::move(other));
    return *this;
  }

  T* get() const noexcept {
    return ptr;
  }

  operator bool() const noexcept {
    return ptr;
  }

  T& operator*() const noexcept {
    return *get();
  }

  T* operator->() const noexcept {
    return get();
  }

  std::size_t use_count() const noexcept {
    return (cb ? cb->strong_ref_cnt : 0);
  }

  void reset() noexcept {
    if (cb) {
      cb->dec_strong();
      clear();
    }
  }

  template <typename Y>
  void reset(Y* new_ptr) {
    reset(new_ptr, std::default_delete<Y>());
  }

  template <typename Y, typename Deleter>
  void reset(Y* new_ptr, Deleter deleter) {
    *this = shared_ptr<Y>(new_ptr, deleter);
  }

  template <typename V>
  friend bool operator==(const shared_ptr& lhs, const shared_ptr<V>& rhs) noexcept {
    return lhs.get() == rhs.get();
  }

  template <typename V>
  friend bool operator!=(const shared_ptr& lhs, const shared_ptr<V>& rhs) noexcept {
    return !(lhs == rhs);
  }

  ~shared_ptr() {
    reset();
  }

private:
  T* ptr{nullptr};
  detail::control_block_t* cb{nullptr};

  explicit shared_ptr(detail::control_block_object_t<T>* _ptr) noexcept : ptr(_ptr->get()), cb(_ptr) {}

  void swap_trick(shared_ptr other) noexcept {
    std::swap(cb, other.cb);
    std::swap(ptr, other.ptr);
  }

  void clear() noexcept {
    ptr = nullptr;
    cb = nullptr;
  }

  void link() const noexcept {
    if (cb) {
      cb->inc_strong();
    }
  }

  template <typename Y>
  friend class shared_ptr;

  template <typename Y>
  friend class weak_ptr;

  template <typename Y, typename... Args>
  friend shared_ptr<Y> make_shared(Args&&... args);
};

template <typename Y, typename... Args>
shared_ptr<Y> make_shared(Args&&... args) {
  return shared_ptr<Y>(new detail::control_block_object_t<Y>(std::forward<Args>(args)...));
}
