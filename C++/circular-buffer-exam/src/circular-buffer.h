#pragma once

#include <cassert>
#include <cstdlib>
#include <iterator>
#include <utility>

template <typename T>
class circular_buffer {
public:
  using value_type = T;

  using reference = T&;
  using const_reference = const T&;

  using pointer = T*;
  using const_pointer = const T*;

  using iterator = void;
  using const_iterator = void;

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
  // O(1), nothrow
  circular_buffer() noexcept;

  // O(n), strong
  circular_buffer(const circular_buffer&);

  // O(n), strong
  circular_buffer& operator=(const circular_buffer&);

  // O(n), nothrow
  ~circular_buffer();

  // O(1), nothrow
  size_t size() const noexcept;

  // O(1), nothrow
  bool empty() const noexcept;

  // O(1), nothrow
  size_t capacity() const noexcept;

  // O(1), nothrow
  iterator begin() noexcept;

  // O(1), nothrow
  const_iterator begin() const noexcept;

  // O(1), nothrow
  iterator end() noexcept;

  // O(1), nothrow
  const_iterator end() const noexcept;

  // O(1), nothrow
  reverse_iterator rbegin() noexcept;

  // O(1), nothrow
  const_reverse_iterator rbegin() const noexcept;

  // O(1), nothrow
  reverse_iterator rend() noexcept;

  // O(1), nothrow
  const_reverse_iterator rend() const noexcept;

  // O(1), nothrow
  T& operator[](size_t index);

  // O(1), nothrow
  const T& operator[](size_t index) const;

  // O(1), nothrow
  T& back();

  // O(1), nothrow
  const T& back() const;

  // O(1), nothrow
  T& front();

  // O(1), nothrow
  const T& front() const;

  // O(1), strong
  void push_back(const T& val);

  // O(1), strong
  void push_front(const T& val);

  // O(1), nothrow
  void pop_back();

  // O(1), nothrow
  void pop_front();

  // O(n), strong
  void reserve(size_t desired_capacity);

  // O(n), basic
  iterator insert(const_iterator pos, const T& val);

  // O(n), basic
  iterator erase(const_iterator pos);

  // O(n), basic
  iterator erase(const_iterator first, const_iterator last);

  // O(n), nothrow
  void clear() noexcept;

  // O(1), nothrow
  void swap(circular_buffer& other) noexcept;
};
