#pragma once

template <typename T>
class shared_ptr;

class bad_weak_ptr : public std::exception {
public:
  explicit bad_weak_ptr() noexcept = default;

  virtual const char* what() const noexcept {
    return "bad_weak_ptr";
  }
};

template <typename T>
class weak_ptr {
public:
  weak_ptr() noexcept = default;

  template <typename Y>
  weak_ptr(const shared_ptr<Y>& other) noexcept : ptr(other.ptr),
                                                  cb(other.cb) {
    link();
  }

  weak_ptr(const weak_ptr& other) noexcept : ptr(other.ptr), cb(other.cb) {
    link();
  }

  template <typename Y>
  weak_ptr(const weak_ptr<Y>& other) noexcept : ptr(other.ptr),
                                                cb(other.cb) {
    link();
  }

  template <typename Y>
  weak_ptr(weak_ptr<Y>&& other) noexcept : ptr(other.ptr),
                                           cb(other.cb) {
    other.clear();
  }

  weak_ptr& operator=(const weak_ptr& other) noexcept {
    if (this != &other) {
      swap_trick(other);
    }
    return *this;
  }

  template <typename Y>
  weak_ptr& operator=(weak_ptr<Y>&& other) noexcept {
    swap_trick(std::move(other));
    return *this;
  }

  template <typename Y>
  weak_ptr& operator=(weak_ptr<Y>& other) noexcept {
    swap_trick(other);
    return *this;
  }

  bool expired() const noexcept {
    return !cb || cb->strong_ref_cnt == 0;
  }

  shared_ptr<T> lock() const {
    return expired() ? shared_ptr<T>() : shared_ptr<T>(*this);
  }

  void reset() noexcept {
    if (cb) {
      cb->dec_weak();
      clear();
    }
  }

  ~weak_ptr() {
    reset();
  }

private:
  T* ptr{nullptr};
  detail::control_block_t* cb{nullptr};

  friend class shared_ptr<T>;

  template <typename Y>
  friend class weak_ptr;

  void swap_trick(weak_ptr other) noexcept {
    std::swap(cb, other.cb);
    std::swap(ptr, other.ptr);
  }

  void clear() noexcept {
    ptr = nullptr;
    cb = nullptr;
  }

  void link() const noexcept {
    if (cb) {
      cb->inc_weak();
    }
  }
};
