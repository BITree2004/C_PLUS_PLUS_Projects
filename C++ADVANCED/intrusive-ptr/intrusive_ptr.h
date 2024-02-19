#include <atomic>
#include <concepts>

template <typename T>
struct intrusive_ptr {
  using element_type = T;

  intrusive_ptr() noexcept = default;

  intrusive_ptr(T* ptr, bool add_ref = true) : pointer_(ptr) {
    if (add_ref && ptr) {
      intrusive_ptr_add_ref(ptr);
    }
  }

  intrusive_ptr(intrusive_ptr const& r) : intrusive_ptr(r.get()) {}

  template <class Y>
  intrusive_ptr(intrusive_ptr<Y> const& r) requires std::convertible_to<Y*, T*>
      : intrusive_ptr(r.get()) {}

  intrusive_ptr(intrusive_ptr&& r)
      : pointer_(std::exchange(r.pointer_, nullptr)) {}

  template <class Y>
  intrusive_ptr(intrusive_ptr<Y>&& r)
      : pointer_(std::exchange(r.pointer_, nullptr)) {}

  ~intrusive_ptr() {
    if (pointer_) {
      intrusive_ptr_release(pointer_);
    }
  }

  intrusive_ptr& operator=(intrusive_ptr const& r) {
    return operator= <T>(r);
  }

  template <class Y>
  requires std::convertible_to<Y*, T*> intrusive_ptr&
  operator=(intrusive_ptr<Y> const& r) {
    if (this == &r) {
      return *this;
    }
    return operator=(r.get());
  }

  intrusive_ptr& operator=(T* r) {
    intrusive_ptr(r).swap(*this);
    return *this;
  }

  intrusive_ptr& operator=(intrusive_ptr&& r) {
    return operator= <T>(std::move(r));
  }

  template <class Y>
  intrusive_ptr&
  operator=(intrusive_ptr<Y>&& r) requires std::convertible_to<Y*, T*> {
    intrusive_ptr(std::move(r)).swap(*this);
    return *this;
  }

  void reset() {
    intrusive_ptr().swap(*this);
  }

  void reset(T* r, bool add_ref = true) {
    intrusive_ptr(r, add_ref).swap(*this);
  }

  T& operator*() const noexcept {
    return *get();
  }

  T* operator->() const noexcept {
    return get();
  }

  T* get() const noexcept {
    return pointer_;
  }

  T* detach() noexcept {
    return std::exchange(pointer_, nullptr);
  }

  explicit operator bool() const noexcept {
    return get() != 0;
  }

  void swap(intrusive_ptr& b) noexcept {
    std::swap(pointer_, b.pointer_);
  }

private:
  element_type* pointer_ = nullptr;

  template <typename Y>
  friend struct intrusive_ptr;
};

template <class T, class U>
bool operator==(intrusive_ptr<T> const& a, intrusive_ptr<U> const& b) noexcept {
  return a.get() == b.get();
}

template <class T, class U>
bool operator!=(intrusive_ptr<T> const& a, intrusive_ptr<U> const& b) noexcept {
  return a.get() != b.get();
}

template <class T, class U>
bool operator==(intrusive_ptr<T> const& a, U* b) noexcept {
  return a.get() == b;
}

template <class T, class U>
bool operator!=(intrusive_ptr<T> const& a, U* b) noexcept {
  return a.get() != b;
}

template <class T, class U>
bool operator==(T* a, intrusive_ptr<U> const& b) noexcept {
  return a == b.get();
}

template <class T, class U>
bool operator!=(T* a, intrusive_ptr<U> const& b) noexcept {
  return a != b.get();
}

template <class T>
bool operator<(intrusive_ptr<T> const& a, intrusive_ptr<T> const& b) noexcept {
  return std::less(a.get(), b.get());
}

template <class T>
void swap(intrusive_ptr<T>& a, intrusive_ptr<T>& b) noexcept {
  a.swap(b);
}

template <typename T>
struct intrusive_ref_counter {
  intrusive_ref_counter() noexcept = default;
  intrusive_ref_counter(intrusive_ref_counter const& v) noexcept {}

  intrusive_ref_counter& operator=(intrusive_ref_counter const&) noexcept {
    return *this;
  }

  std::size_t use_count() const noexcept {
    return cnt_;
  }

  friend void intrusive_ptr_add_ref(intrusive_ref_counter const* p) noexcept {
    p->cnt_.fetch_add(1, std::memory_order_relaxed);
  }

  friend void intrusive_ptr_release(intrusive_ref_counter const* p) noexcept {
    if (p->cnt_.fetch_sub(1, std::memory_order_acq_rel) == 1)
      delete static_cast<const T*>(p);
  }

protected:
  ~intrusive_ref_counter() = default;

  std::tuple <int> x;
private:
  mutable std::atomic<std::size_t> cnt_ = 0;
};
