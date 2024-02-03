#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>

template <typename T>
class vector {
public:
  using value_type = T;

  using reference = T&;
  using const_reference = const T&;

  using pointer = T*;
  using const_pointer = const T*;

  using iterator = pointer;
  using const_iterator = const_pointer;

public:
  vector() noexcept : _data(nullptr), _size(0), _capacity(0) {} // O(1) nothrow

  vector(const vector& other) : vector(other, other.size()) {} // O(N) strong

  vector& operator=(const vector& other) {
    if (&other != this) {
      vector(other).swap(*this);
    }
    return *this;
  } // O(N) strong

  ~vector() noexcept {
    clear();
    operator delete(_data);
  } // O(N) nothrow

  reference operator[](size_t index) {
    return _data[index];
  } // O(1) nothrow

  const_reference operator[](size_t index) const {
    return _data[index];
  } // O(1) nothrow

  pointer data() noexcept {
    return _data;
  } // O(1) nothrow

  const_pointer data() const noexcept {
    return _data;
  } // O(1) nothrow

  size_t size() const noexcept {
    return _size;
  } // O(1) nothrow

  reference front() {
    return *begin();
  } // O(1) nothrow

  const_reference front() const {
    return *begin();
  } // O(1) nothrow

  reference back() {
    return *(end() - 1);
  } // O(1) nothrow

  const_reference back() const {
    return *(end() - 1);
  } // O(1) nothrow

  void push_back(const T& value) {
    if (size() == capacity()) {
      vector tmp(*this, capacity() == 0 ? 1 : capacity() * 2);
      tmp.push_back(value);
      swap(tmp);
    } else {
      new (_data + size()) T(value);
      _size++;
    }
  } // O(1)* strong

  void pop_back() noexcept {
    assert(size() != 0);
    _data[--_size].~T();
  } // O(1) nothrow

  bool empty() const noexcept {
    return size() == 0;
  } // O(1) nothrow

  size_t capacity() const noexcept {
    return _capacity;
  } // O(1) nothrow

  void reserve(size_t new_capacity) {
    if (new_capacity > capacity()) {
      vector(*this, new_capacity).swap(*this);
    }
  } // O(N) strong

  void shrink_to_fit() {
    if (size() != capacity()) {
      vector(*this, size()).swap(*this);
    }
  } // O(N) strong

  void clear() noexcept {
    erase(begin(), end());
  } // O(N) nothrow

  void swap(vector& other) noexcept {
    std::swap(_data, other._data);
    std::swap(_size, other._size);
    std::swap(_capacity, other._capacity);
  } // O(1) nothrow

  iterator begin() noexcept {
    return data();
  } // O(1) nothrow

  iterator end() noexcept {
    return data() + size();
  } // O(1) nothrow

  const_iterator begin() const noexcept {
    return data();
  } // O(1) nothrow

  const_iterator end() const noexcept {
    return data() + size();
  } // O(1) nothrow

  iterator insert(const_iterator pos, const T& value) {
    size_t index = pos - data();
    push_back(value);
    for (size_t i = size() - 1; i != index; --i) {
      std::swap(_data[i], _data[i - 1]);
    }
    return data() + index;
  } // O(N) strong

  iterator erase(const_iterator pos) {
    return erase(pos, pos + 1);
  } // O(N) nothrow(swap)

  iterator erase(const_iterator first, const_iterator last) {
    size_t count_removed = last - first;
    size_t start = first - data();
    for (size_t i = start + count_removed; i != size(); ++i) {
      std::swap(_data[i], _data[i - count_removed]);
    }
    for (size_t i = 0; i != count_removed; ++i) {
      pop_back();
    }
    return data() + start;
  } // O(N) nothrow(swap)

private:
  vector(const vector& other, size_t capacity)
      : _data(capacity == 0 ? nullptr : static_cast<T*>(operator new(sizeof(value_type) * capacity))),
        _size(other.size()),
        _capacity(capacity) {
    assert(capacity >= other.size());

    for (size_t i = 0; i < size(); ++i) {
      try {
        new (_data + i) T(other._data[i]);
      } catch (...) {
        for (size_t j = i; j != 0; --j) {
          _data[j - 1].~T();
        }
        operator delete(_data);
        throw;
      }
    }
  }

private:
  T* _data;
  size_t _size;
  size_t _capacity;
};
