#pragma once

#include <cstddef>
#include <iostream>
#include <iterator>
#include <vector>

template <typename T>
class list {
private:
  template <typename U>
  struct list_iterator;

public:
  using value_type = T;

  using reference = T&;
  using const_reference = const T&;

  using pointer = T*;
  using const_pointer = const T*;

  using iterator = list_iterator<T>;
  using const_iterator = list_iterator<const T>;

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
  struct base_list_node {
    base_list_node* prev = this;
    base_list_node* next = this;
    base_list_node() = default;

    base_list_node(base_list_node* prev, base_list_node* next) : prev(prev), next(next) {}
  };

  struct node : base_list_node {
    T value;

    node(const T& value) : base_list_node(), value(value) {}
  };

  template <typename U>
  struct list_iterator {
    base_list_node* vertex;

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using reference = U&;
    using pointer = U*;

  private:
    friend list;

    list_iterator(base_list_node* vertex) : vertex(vertex) {}

  public:
    list_iterator() = default;

    list_iterator(const list_iterator& other) {
      vertex = other.vertex;
    }

    list_iterator& operator=(const list_iterator& other) = default;

    operator list_iterator<const T>() const {
      return list_iterator<const T>(vertex);
    }

    reference operator*() const {
      return static_cast<node*>(vertex)->value;
    }

    pointer operator->() const {
      return &(static_cast<node*>(vertex)->value);
    }

    list_iterator& operator++() {
      vertex = vertex->next;
      return *this;
    }

    list_iterator operator++(int) {
      list_iterator res = *this;
      ++*this;
      return res;
    }

    list_iterator& operator--() {
      vertex = vertex->prev;
      return *this;
    }

    list_iterator operator--(int) {
      list_iterator res = *this;
      --*this;
      return res;
    }

    friend bool operator==(const list_iterator& a, const list_iterator& b) {
      return a.vertex == b.vertex;
    }

    friend bool operator!=(const list_iterator& a, const list_iterator& b) {
      return !(a == b);
    }
  };

  base_list_node tail;
  size_t _size = 0;

public:
  // O(1), nothrow
  list() noexcept {}

  // O(n), strong
  list(const list& other) : list(other.begin(), other.end()) {}

  // O(n), strong
  template <std::input_iterator InputIt>
  list(InputIt first, InputIt last) : list() {
    while (first != last) {
      push_back(*first++);
    }
  }

  // O(n), strong
  list& operator=(const list& other) {
    if (this != &other) {
      list copy(other);
      swap(copy, *this);
    }
    return *this;
  }

  // O(n), nothrow
  ~list() noexcept {
    clear();
  }

  // O(1), nothrow
  bool empty() const noexcept {
    return size() == 0;
  }

  // O(1), nothrow
  size_t size() const noexcept {
    return _size;
  }

  // O(1), nothrow
  T& front() {
    return *begin();
  }

  // O(1), nothrow
  const T& front() const {
    return *begin();
  }

  // O(1), nothrow
  T& back() {
    return *(--end());
  }

  // O(1), nothrow
  const T& back() const {
    return *(--end());
  }

  // O(1), strong
  void push_front(const T& value) {
    insert(begin(), value);
  }

  // O(1), strong
  void push_back(const T& value) {
    insert(end(), value);
  }

  // O(1), nothrowtail
  void pop_front() {
    erase(begin());
  }

  // O(1), nothrow
  void pop_back() {
    erase(--end());
  }

  // O(1), nothrow
  iterator begin() noexcept {
    return iterator(tail.next);
  }

  // O(1), nothrow
  const_iterator begin() const noexcept {
    return const_iterator(tail.next);
  }

  // O(1), nothrow
  iterator end() noexcept {
    return iterator(&tail);
  }

  // O(1), nothrow
  const_iterator end() const noexcept {
    return const_iterator(const_cast<base_list_node*>(&tail));
  }

  // O(1), nothrow
  reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }

  // O(1), nothrow
  const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }

  // O(1), nothrow
  reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }

  // O(1), nothrow
  const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }

  // O(n), nothrow
  void clear() noexcept {
    while (!empty()) {
      pop_back();
    }
  }

  // O(1), strong
  iterator insert(const_iterator pos, const T& value) {
    base_list_node* next_el = pos.vertex;
    node* vertex = new node(value);
    link(next_el->prev, vertex);
    link(vertex, next_el);
    _size++;
    return iterator(vertex);
  }

  // O(last - first), strong
  template <std::input_iterator InputIt>
  iterator insert(const_iterator pos, InputIt first, InputIt last) {
    const_iterator begin = pos.vertex->prev;
    list tmp(first, last);
    splice(pos, tmp, tmp.begin(), tmp.end());
    return iterator(begin.vertex->next);
  }

  // O(1), nothrow
  iterator erase(const_iterator pos) noexcept {
    node* vertex = static_cast<node*>(pos.vertex);
    iterator res(vertex->next);
    link(vertex->prev, vertex->next);
    vertex->value.~T();
    operator delete(vertex);
    _size--;
    return res;
  }

  // O(last - first), nothrow
  iterator erase(const_iterator first, const_iterator last) noexcept {
    list tmp;
    tmp.splice(tmp.begin(), *this, first, last);
    return iterator(last.vertex);
  }

  // O(last - first) in general but O(1) when possible, nothrow
  void splice(const_iterator pos, list& other, const_iterator first, const_iterator last) noexcept {
    size_t cnt = 0;
    if (this != &other) {
      if (first == other.begin() && last == other.end()) {
        cnt = other.size();
      } else {
        cnt = std::distance(first, last);
      }
    }
    splice(pos, other, first, last, cnt);
  }

  // O(1), nothrow
  friend void swap(list& left, list& right) noexcept {
    iterator old_end = --left.end();
    size_t size_left = left.size();
    left.splice(left.end(), right, right.begin(), right.end());
    right.splice(right.end(), left, left.begin(), ++old_end, size_left);
  }

private:
  // O(last - first) in general but O(1) when possible, nothrow
  void splice(const_iterator pos, list& other, const_iterator first, const_iterator last, size_t cnt) noexcept {
    if (first == last) {
      return;
    }
    other._size -= cnt;
    _size += cnt;
    base_list_node* end = last.vertex->prev;
    link(first.vertex->prev, last.vertex);
    link(pos.vertex->prev, first.vertex);
    link(end, pos.vertex);
  }

  static void link(base_list_node* a, base_list_node* b) {
    a->next = b;
    b->prev = a;
  }
};
