#include "intrusive-list.h"

namespace intrusive {

list_element_base::list_element_base() noexcept : prev(this), next(this) {}

list_element_base::list_element_base(const list_element_base&) noexcept : list_element_base() {}

list_element_base& list_element_base::operator=(list_element_base& other) noexcept {
  if (this == &other) {
    return *this;
  }
  unlink();
  return *this;
}

list_element_base::list_element_base(list_element_base&& other) noexcept : list_element_base() {
  *this = std::move(other);
}

list_element_base& list_element_base::operator=(list_element_base&& other) noexcept {
  if (this == &other) {
    return *this;
  }
  unlink();
  if (other.is_linked()) {
    auto other_prev = other.prev;
    auto other_next = other.next;
    other.unlink();
    link(other_prev, this);
    link(this, other_next);
  }
  return *this;
}

list_element_base::~list_element_base() {
  unlink();
}

void list_element_base::unlink() noexcept {
  prev->next = next;
  next->prev = prev;
  prev = next = this;
}

void list_element_base::link(list_element_base* left, list_element_base* right) noexcept {
  left->next = right;
  right->prev = left;
}

bool list_element_base::is_linked() const noexcept {
  return prev != this;
}
} // namespace intrusive
