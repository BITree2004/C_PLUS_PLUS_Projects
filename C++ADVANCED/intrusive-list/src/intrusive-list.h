#pragma once

#include <algorithm>

namespace intrusive {

struct default_tag;

template <typename T, typename Tag>
class list;

class list_element_base {
private:
  list_element_base* prev;
  list_element_base* next;
  template <typename T, typename Tag>
  friend class list;

  void unlink() noexcept;
  static void link(list_element_base* left, list_element_base* right) noexcept;
  bool is_linked() const noexcept;

public:
  list_element_base() noexcept;
  list_element_base(const list_element_base&) noexcept;
  list_element_base(list_element_base&& other) noexcept;

  list_element_base& operator=(list_element_base&& other) noexcept;
  list_element_base& operator=(list_element_base& other) noexcept;

  ~list_element_base();
};

template <typename Tag = default_tag>
class list_element : private list_element_base {
private:
  template <typename T, typename Tag2>
  friend class list;
};

template <typename T, typename Tag = default_tag>
class list {
  static_assert(std::is_base_of_v<list_element<Tag>, T>, "T must derive from list_element");

private:
  template <typename U>
  class base_iterator;

  using node = list_element<Tag>;

  node sentinel;

public:
  using value_type = T;

  using reference = value_type&;
  using const_reference = const value_type&;

  using pointer = value_type*;
  using const_pointer = const value_type*;

  using iterator = base_iterator<T>;
  using const_iterator = base_iterator<const T>;

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
  template <typename U>
  class base_iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = U;
    using reference = value_type&;
    using pointer = value_type*;

    base_iterator() noexcept = default;

    base_iterator(const base_iterator& other) noexcept = default;

    base_iterator& operator=(const base_iterator& other) noexcept = default;

    operator base_iterator<const T>() const noexcept {
      return base_iterator<const T>(vertex);
    }

    reference operator*() const noexcept {
      return *static_cast<pointer>(vertex);
    }

    pointer operator->() const noexcept {
      return static_cast<pointer>(vertex);
    }

    base_iterator& operator++() noexcept {
      vertex = as_node(vertex->next);
      return *this;
    }

    base_iterator operator++(int) noexcept {
      base_iterator res = *this;
      ++*this;
      return res;
    }

    base_iterator& operator--() noexcept {
      vertex = as_node(vertex->prev);
      return *this;
    }

    base_iterator operator--(int) noexcept {
      base_iterator res = *this;
      --*this;
      return res;
    }

    friend bool operator==(const base_iterator& a, const base_iterator& b) noexcept {
      return a.vertex == b.vertex;
    }

    friend bool operator!=(const base_iterator& a, const base_iterator& b) noexcept {
      return !(a == b);
    }

  private:
    friend list;

    explicit base_iterator(node* vertex) noexcept : vertex(vertex) {}

    node* get_node() noexcept {
      return vertex;
    }

    node* vertex;
  };

public:
  // O(1)
  list() noexcept = default;

  // O(1)
  ~list() = default;

  list(const list&) = delete;
  list& operator=(const list&) = delete;

  // O(1)cons
  list(list&& other) noexcept = default;

  // O(1)
  list& operator=(list&& other) noexcept = default;

  // O(1)
  bool empty() const noexcept {
    return !sentinel.is_linked();
  }

  // O(n)
  size_t size() const noexcept {
    return std::distance(begin(), end());
  }

  // O(1)
  T& front() noexcept {
    return *begin();
  }

  // O(1)
  const T& front() const noexcept {
    return *begin();
  }

  // O(1)
  T& back() noexcept {
    return *--end();
  }

  // O(1)
  const T& back() const noexcept {
    return *--end();
  }

  // O(1)
  void push_front(T& value) noexcept {
    insert(begin(), value);
  }

  // O(1)
  void push_back(T& value) noexcept {
    insert(end(), value);
  }

  // O(1)
  void pop_front() noexcept {
    erase(begin());
  }

  // O(1)
  void pop_back() noexcept {
    erase(--end());
  }

  // O(1)
  void clear() noexcept {
    sentinel.unlink();
  }

  // O(1)
  iterator begin() noexcept {
    return iterator(as_node(sentinel.next));
  }

  // O(1)
  const_iterator begin() const noexcept {
    return const_iterator(as_node(sentinel.next));
  }

  // O(1)
  iterator end() noexcept {
    return iterator(&sentinel);
  }

  // O(1)
  const_iterator end() const noexcept {
    return const_iterator(const_cast<node*>(&sentinel));
  }

  // O(1)
  iterator insert(const_iterator pos, T& value) noexcept {
    node* node_value = as_node(value);
    node* before_node = pos.get_node();
    if (node_value == before_node) {
      return iterator(before_node);
    }
    node_value->unlink();
    node::link(before_node->prev, node_value);
    node::link(node_value, before_node);
    return iterator(node_value);
  }

  // O(1)
  iterator erase(const_iterator pos) noexcept {
    iterator copy(as_node(pos.get_node()->next));
    pos.get_node()->unlink();
    return copy;
  }

  // O(1)
  void splice(const_iterator pos, list&, const_iterator first, const_iterator last) noexcept {
    if (first == last) {
      return;
    }
    auto end = last.get_node()->prev;
    node::link(first.get_node()->prev, last.get_node());
    node::link(pos.get_node()->prev, first.get_node());
    node::link(end, pos.get_node());
  }

private:
  // O(1)
  static node* as_node(T& value) noexcept {
    return static_cast<node*>(&value);
  }

  // O(1)
  static node* as_node(list_element_base* value) noexcept {
    return static_cast<node*>(value);
  }
};

} // namespace intrusive
