#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <utility>

template <typename T, size_t SMALL_SIZE>
class socow_vector {
public:
  using value_type = T;

  using reference = T&;
  using const_reference = const T&;

  using pointer = T*;
  using const_pointer = const T*;

  using iterator = pointer;
  using const_iterator = const_pointer;

public:
  socow_vector() noexcept : _size(0), _is_small_object(true) {} // O(1) nothrow

  socow_vector(const socow_vector& other) : socow_vector() {
    *this = other;
  } // O(SMALL_SIZE)

  socow_vector& operator=(const socow_vector& other) {
    if (&other != this) {
      if (other._is_small_object) {
        copy_to_small_object(other);
      } else {
        clear_memory();
        _is_small_object = false;
        _size = other.size();
        _dynamic_data = other._dynamic_data;
        _dynamic_data->_refs++;
      }
    }
    return *this;
  } // O(SMALL_SIZE)

  ~socow_vector() noexcept {
    clear_memory();
  } // O(N) nothrow

  reference operator[](size_t index) {
    return data()[index];
  } // O(N)

  const_reference operator[](size_t index) const noexcept {
    return data()[index];
  } // O(1) nothrow

  pointer data() {
    unshape(capacity());
    return (_is_small_object ? _static_data : _dynamic_data->_data);
  } // O(N)

  const_pointer data() const noexcept {
    return (_is_small_object ? _static_data : _dynamic_data->_data);
  } // O(1) nothrow

  size_t size() const noexcept {
    return _size;
  } // O(1) nothrow

  reference front() {
    return *begin();
  } // O(N)

  const_reference front() const noexcept {
    return *begin();
  } // O(1) nothrow

  reference back() {
    return *(end() - 1);
  } // O(N)

  const_reference back() const noexcept {
    return *(end() - 1);
  } // O(1) nothrow

  void push_back(const T& value) {
    insert(cend(), value);
  } // O(1)*

  void pop_back() {
    erase(cend() - 1);
  } // O(1)*

  bool empty() const noexcept {
    return size() == 0;
  } // O(1) nothrow

  size_t capacity() const noexcept {
    return (_is_small_object ? SMALL_SIZE : _dynamic_data->_capacity);
  } // O(1) nothrow

  void reserve(size_t new_capacity) {
    if (new_capacity <= size()) {
      return;
    }
    if (new_capacity > capacity() || is_shared_data()) {
      reallocate(new_capacity);
    }
  } // O(N)

  void shrink_to_fit() {
    if (size() == capacity()) {
      return;
    }
    reallocate(size());
  } // O(N)

  void clear() noexcept {
    if (is_shared_data()) {
      clear_memory();
      _is_small_object = true;
    } else {
      std::destroy(begin(), end());
    }
    _size = 0;
  } // O(N) noexcept

  void swap(socow_vector& other) {
    if (this == &other) {
      return;
    }
    if (_is_small_object) {
      if (other._is_small_object) {
        socow_vector& src = (size() > other.size() ? *this : other);
        socow_vector& dest = (size() <= other.size() ? *this : other);
        size_t dif = src.size() - dest.size();
        copy(src.begin() + dest.size(), dif, dest.begin() + dest.size());
        std::destroy(src.begin() + dest.size(), src.end());
        std::swap(src._size, dest._size);
        std::swap_ranges(src.begin(), src.end(), dest.begin());
      } else {
        other.swap(*this);
      }
    } else {
      socow_vector tmp(*this);
      *this = other;
      other = tmp;
    }
  } // O(SMALL_SIZE)

  iterator begin() {
    return data();
  } // O(1)

  iterator end() {
    return data() + size();
  } // O(1)

  const_iterator begin() const {
    return data();
  } // O(1)

  const_iterator end() const {
    return data() + size();
  } // O(1)

  iterator insert(const_iterator pos, const T& value) {
    size_t index = pos - cbegin();
    if (capacity() == size() || is_shared_data()) {
      size_t new_capacity = std::max(size() * 2, capacity());
      socow_vector tmp(socow_vector(), 0, new_capacity);
      copy(cbegin(), index, tmp.begin());
      tmp._size = index;
      tmp.push_back(value);
      copy(cbegin() + index, size() - index, tmp.begin() + index + 1);
      tmp._size = size() + 1;
      *this = tmp;
    } else {
      new (data() + size()) T(value);
      _size++;
      std::rotate(begin() + index, end() - 1, end());
    }
    return begin() + index;
  } // O(N)

  iterator erase(const_iterator pos) {
    return erase(pos, pos + 1);
  } // O(N)

  iterator erase(const_iterator first, const_iterator last) {
    size_t count_removed = last - first;
    assert(count_removed <= size());
    size_t start = first - cbegin();
    if (first == last) {
      return begin() + start;
    }
    if (is_shared_data()) {
      socow_vector tmp(socow_vector(), 0, capacity());
      copy(cbegin(), start, tmp.begin());
      tmp._size = start;
      copy(cbegin() + start + count_removed, size() - count_removed - start, tmp.begin() + start);
      tmp._size = size() - count_removed;
      *this = tmp;
    } else {
      for (size_t i = start + count_removed; i != size(); ++i) {
        std::swap(data()[i], data()[i - count_removed]);
      }
      std::destroy_n(end() - count_removed, count_removed);
      _size -= count_removed;
    }
    return begin() + start;
  } // O(N)

private:
  void clear_memory() noexcept {
    if (_is_small_object) {
      std::destroy(begin(), end());
    } else {
      if (_dynamic_data->_refs == 1) {
        std::destroy(begin(), end());
        _dynamic_data->~buffer();
        operator delete(_dynamic_data);
      } else {
        _dynamic_data->_refs--;
      }
    }
  }

  static void copy(const const_iterator source, const size_t src_size, iterator data) {
    std::uninitialized_copy_n(source, src_size, data);
  }

  void unshape(size_t new_capacity) {
    if (!is_shared_data()) {
      return;
    }
    reallocate(new_capacity);
  }

  void reallocate(size_t new_capacity) {
    if (_is_small_object && new_capacity <= SMALL_SIZE) {
      return;
    }
    assert(new_capacity >= size());
    if (new_capacity <= SMALL_SIZE) {
      copy_to_small_object(socow_vector(*this));
    } else {
      socow_vector tmp;
      tmp._dynamic_data = new_buffer(new_capacity);
      tmp._is_small_object = false;
      copy(cbegin(), size(), tmp.begin());
      tmp._size = _size;
      *this = tmp;
    }
    assert(_is_small_object || capacity() == new_capacity);
  }

  const_iterator cbegin() const noexcept {
    return begin();
  } // O(1) nothrow

  const_iterator cend() const noexcept {
    return end();
  } // O(1) nothrow

  bool is_shared_data() const noexcept {
    return !_is_small_object && _dynamic_data->_refs != 1;
  }

  socow_vector(const socow_vector& other, size_t count_copy, size_t capacity) : socow_vector() {
    assert(count_copy <= other.size());
    reserve(capacity);
    copy(other.begin(), count_copy, begin());
    _size = count_copy;
  }

  void copy_to_small_object(const socow_vector& other) {
    if (_is_small_object) {
      assert(other._is_small_object);
      if (size() < other.size()) {
        copy_to_static_data_in_small_object(other);
      } else {
        socow_vector(other, other.size(), SMALL_SIZE).swap(*this);
      }
    } else {
      socow_vector cp(*this);
      clear();
      try {
        copy_to_static_data_in_small_object(other);
      } catch (...) {
        *this = cp;
        throw;
      }
    }
    _is_small_object = true;
    _size = other.size();
  }

  void copy_to_static_data_in_small_object(const socow_vector& other) {
    assert(_is_small_object);
    socow_vector tmp(other, size(), SMALL_SIZE);
    copy(other.begin() + size(), other.size() - size(), end());
    try {
      tmp.swap(*this);
    } catch (...) {
      std::destroy_n(end(), other.size() - size());
      throw;
    }
  }

private:
  struct buffer {
    explicit buffer(size_t new_capacity) : _capacity(new_capacity) {}

    size_t _capacity{};
    size_t _refs{1};
    T _data[0];
  };

  union {
    T _static_data[SMALL_SIZE];
    buffer* _dynamic_data;
  };

  size_t _size;
  bool _is_small_object;

private:
  static buffer* new_buffer(size_t new_capacity) {
    return new (static_cast<buffer*>(operator new(sizeof(buffer) + new_capacity * sizeof(value_type))))
        buffer(new_capacity);
  }
};
