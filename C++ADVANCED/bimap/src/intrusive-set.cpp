#include "intrusive-set.h"

namespace intrusive::details {
bool is_left_child(set_element_base* parent, set_element_base* child) noexcept {
  return parent->left == child;
}

void replace_node(set_element_base* old_node, set_element_base* new_node) noexcept {
  set_element_base* parent = old_node->parent;
  if (is_left_child(parent, old_node)) {
    parent->left = new_node;
  } else {
    parent->right = new_node;
  }
  if (new_node) {
    new_node->parent = parent;
  }
}

void link_parent_left_child(set_element_base* parent, set_element_base* vertex) noexcept {
  vertex->parent = parent;
  parent->left = vertex;
}

void link_parent_right_child(set_element_base* parent, set_element_base* vertex) noexcept {
  vertex->parent = parent;
  parent->right = vertex;
}

set_element_base* get_next(set_element_base* vertex) noexcept {
  if (vertex->right) {
    vertex = vertex->right;
    while (vertex->left) {
      vertex = vertex->left;
    }
  } else {
    while (vertex->parent && !is_left_child(vertex->parent, vertex)) {
      vertex = vertex->parent;
    }
    vertex = vertex->parent;
  }
  return vertex;
}

set_element_base* get_prev(set_element_base* vertex) noexcept {
  if (vertex->left) {
    vertex = vertex->left;
    while (vertex->right) {
      vertex = vertex->right;
    }
  } else {
    while (vertex->parent && vertex->parent->left == vertex) {
      vertex = vertex->parent;
    }
    vertex = vertex->parent;
  }
  return vertex;
}
} // namespace intrusive::details
