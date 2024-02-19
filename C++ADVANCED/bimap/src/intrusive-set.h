#pragma once

#include <cassert>
#include <iterator>

namespace intrusive {

template <typename T, typename Key, typename Compare, typename Tag, typename Getter>
class set;

namespace details {
class set_element_base {
public:
  set_element_base* left{nullptr};
  set_element_base* right{nullptr};
  set_element_base* parent{nullptr};

  set_element_base() noexcept = default;
  ~set_element_base() = default;
};

template <typename T, typename Key>
class default_getter {
  static Key& get(T& _node) {
    return _node.key;
  }
};

bool is_left_child(set_element_base* parent, set_element_base* child) noexcept;

void replace_node(set_element_base* old_node, set_element_base* new_node) noexcept;

void link_parent_left_child(set_element_base* parent, set_element_base* vertex) noexcept;

void link_parent_right_child(set_element_base* parent, set_element_base* vertex) noexcept;

set_element_base* get_next(set_element_base* vertex) noexcept;

set_element_base* get_prev(set_element_base* vertex) noexcept;
} // namespace details

struct default_tag;
struct default_compare;

template <typename Tag = default_tag>
struct set_element : private details::set_element_base {
public:
  bool is_sentinel() const noexcept {
    return !parent;
  }

private:
  template <typename T, typename Key, typename Compare, typename Tag2, typename Getter>
  friend class set;
};

template <typename T, typename Key, typename Compare = default_compare, typename Tag = default_tag,
          typename Getter = details::default_getter<T, Key>>
class set {
  static_assert(std::is_base_of_v<set_element<Tag>, T>, "T must derive from list_element");

private:
  using node = set_element<Tag>;

  node* sentinel;
  [[no_unique_address]] Compare compare;

public:
  using value_type = T;

  using reference = value_type&;
  using pointer = value_type*;

  class iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Key;
    using reference = value_type&;
    using pointer = value_type*;

    iterator() noexcept = default;

    iterator(const iterator& other) noexcept = default;

    iterator(node* vertex) : vertex(vertex) {}

    iterator& operator=(const iterator& other) noexcept = default;

    reference operator*() const noexcept {
      return get_key(vertex);
    }

    pointer operator->() const noexcept {
      return &**this;
    }

    iterator& operator++() noexcept {
      vertex = as_node(details::get_next(vertex));
      return *this;
    }

    iterator operator++(int) noexcept {
      iterator res = *this;
      ++*this;
      return res;
    }

    iterator& operator--() noexcept {
      vertex = as_node(details::get_prev(vertex));
      return *this;
    }

    iterator operator--(int) noexcept {
      iterator res = *this;
      --*this;
      return res;
    }

    friend bool operator==(const iterator& a, const iterator& b) noexcept {
      return a.vertex == b.vertex;
    }

    friend bool operator!=(const iterator& a, const iterator& b) noexcept {
      return !(a == b);
    }

  protected:
    node* get_node() const noexcept {
      return vertex;
    }

    friend set;

    node* vertex;
  };

public:
  set(node& sentinel, const Compare& compare = Compare()) : sentinel(&sentinel), compare(compare) {}

  set(node& sentinel, Compare&& compare) : sentinel(&sentinel), compare(std::move(compare)) {}

  set(set&& other) = default;

  set() noexcept = default;

  ~set() noexcept = default;

  set(const set&) = delete;
  set& operator=(const set&) = delete;

  set& operator=(set&& other) noexcept = default;

  iterator begin() const noexcept {
    details::set_element_base* x = sentinel;
    while (x->left) {
      x = x->left;
    }
    return iterator(as_node(x));
  }

  iterator end() const noexcept {
    return iterator(sentinel);
  }

  iterator erase(iterator pos) noexcept {
    node* vertex = pos.get_node();
    node* next = as_node(details::get_next(vertex));
    if (vertex->left && vertex->right) {
      if (vertex->right != next) {
        details::replace_node(next, next->right);
        details::link_parent_right_child(next, vertex->right);
      }
      details::replace_node(vertex, next);
      details::link_parent_left_child(next, vertex->left);
    } else {
      details::replace_node(vertex, (vertex->right ? vertex->right : vertex->left));
    }
    vertex->left = vertex->right = vertex->parent = nullptr;
    return iterator(next);
  }

  iterator lower_bound(const Key& key) const {
    return iterator(lower_bound_on_tree(sentinel, key));
  }

  iterator upper_bound(const Key& key) const {
    iterator res = lower_bound(key);
    if (res == end() || cmp(key, res.get_node())) {
      return res;
    }
    return ++res;
  }

  iterator find(const Key& key) const {
    iterator res = lower_bound(key);
    if (res == end() || !cmp(key, res.get_node())) {
      return res;
    }
    return end();
  }

  const Compare& get_compare() const noexcept {
    return compare;
  }

  iterator insert_before(iterator pos, T& x) noexcept {
    node* new_node = static_cast<node*>(&x);
    node* p_node = pos.vertex;
    if (p_node->left) {
      node* left = as_node(p_node->left);
      details::replace_node(left, new_node);
      details::link_parent_left_child(new_node, left);
    } else {
      details::link_parent_left_child(p_node, new_node);
    }
    return iterator(new_node);
  }

  void swap(set& other) noexcept {
    using std::swap;
    swap(other.sentinel->left, sentinel->left);
    if (other.sentinel->left) {
      other.sentinel->left->parent = other.sentinel;
    }
    if (sentinel->left) {
      sentinel->left->parent = sentinel;
    }
    swap(compare, other.compare);
  }

private:
  static Key& get_key(node* a) noexcept {
    return Getter::get(*static_cast<T*>(a));
  }

  static node* as_node(details::set_element_base* value) noexcept {
    return static_cast<node*>(value);
  }

  node* lower_bound_on_tree(node* vertex, const Key& key) const noexcept {
    node* res = nullptr;
    while (vertex) {
      if (cmp(vertex, key)) {
        vertex = as_node(vertex->right);
      } else {
        res = vertex;
        vertex = as_node(vertex->left);
      }
    }
    return res;
  }

  bool cmp(const Key& key, node* vertex) const {
    return compare(key, get_key(vertex));
  }

  bool cmp(node* vertex, const Key& key) const {
    return !(vertex == sentinel) && compare(get_key(vertex), key);
  }

  bool cmp(node* left, node* right) const {
    assert(right != sentinel);
    return !(left == sentinel) && compare(get_key(left), get_key(right));
  }
};

} // namespace intrusive
