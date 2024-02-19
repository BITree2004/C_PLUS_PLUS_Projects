#pragma once

#include "intrusive-set.h"

#include <algorithm>
#include <cstddef>
#include <stdexcept>

template <typename Left, typename Right, typename CompareLeft = std::less<Left>,
          typename CompareRight = std::less<Right>>
class bimap {
public:
  using left_t = Left;
  using right_t = Right;

private:
  struct left_tag;
  struct right_tag;

  struct node_t;

  template <typename T>
  struct base_iterator;

  struct right_key_info;

  struct left_key_info {
    using key = left_t;
    using flipped = right_key_info;
    using iterator = base_iterator<left_tag>;
    using set_node = intrusive::set_element<left_tag>;

    struct getter {
      static key& get(set_node& node) noexcept {
        return static_cast<node_t*>(&node)->l;
      }
    };

    using set = intrusive::set<set_node, key, CompareLeft, left_tag, getter>;
  };

  struct right_key_info {
    using key = right_t;
    using flipped = left_key_info;
    using iterator = base_iterator<right_tag>;
    using set_node = intrusive::set_element<right_tag>;

    struct getter {
      static key& get(set_node& node) noexcept {
        return static_cast<node_t*>(&node)->r;
      }
    };

    using set = intrusive::set<set_node, key, CompareRight, right_tag, getter>;
  };

  struct bimap_node : left_key_info::set_node, right_key_info::set_node {};

  struct node_t : bimap_node {
    Left l;
    Right r;

    template <typename L, typename R>
    node_t(L&& l, R&& r) : l(std::forward<L>(l)),
                           r(std::forward<R>(r)) {}
  };

  template <typename Tag>
  struct base_iterator
      : std::conditional_t<std::is_same_v<left_tag, Tag>, left_key_info, right_key_info>::set::iterator {
  private:
    using info = typename std::conditional_t<std::is_same_v<left_tag, Tag>, left_key_info, right_key_info>;
    using base = typename info::set::iterator;
    using flipped = typename info::flipped;
    using iterator = typename info::iterator;

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = typename info::key;
    using reference = const value_type&;
    using pointer = const value_type*;

    base_iterator() noexcept = default;

    base_iterator(const base_iterator& other) noexcept = default;

    base_iterator& operator=(const base_iterator& other) noexcept = default;

    // Элемент на который сейчас ссылается итератор.
    // Разыменование итератора end_left() не определено.
    // Разыменование невалидного итератора не определено.
    // reference operator*() const;
    // pointer operator->() const;

    // Переход к следующему по величине left'у.
    // Инкремент итератора end_left() не определен.
    // Инкремент невалидного итератора не определен.
    iterator& operator++() {
      base::operator++();
      return *this;
    }

    iterator operator++(int) {
      return iterator(base::operator++(0));
    }

    // Переход к предыдущему по величине left'у.
    // Декремент итератора begin_left() не определен.
    // Декремент невалидного итератора не определен.
    iterator& operator--() {
      base::operator--();
      return *this;
    }

    iterator operator--(int) {
      return iterator(base::operator--(0));
    }

    // left_iterator ссылается на левый элемент некоторой пары.
    // Эта функция возвращает итератор на правый элемент той же пары.
    // end_left().flip() возращает end_right().
    // end_right().flip() возвращает end_left().
    // flip() невалидного итератора не определен.
    typename flipped::iterator flip() const {
      bimap_node* res1 = get_node();
      auto* flipped_set_node = static_cast<typename flipped::set_node*>(res1);
      return typename flipped::iterator(flipped_set_node);
    }

  private:
    base_iterator(base other) noexcept : base(other) {}

    bimap_node* get_node() const {
      return static_cast<bimap_node*>(base::get_node());
    }

    friend bimap;
    friend typename flipped::iterator;
  };

  typename left_key_info::set left_set;
  typename right_key_info::set right_set;
  bimap_node sentinal{};
  size_t sz{};

public:
  using right_iterator = typename right_key_info::iterator; // По аналогии с left_iterator
  using left_iterator = typename left_key_info::iterator;

  // Создает bimap, не содержащий ни одной пары.
  bimap(CompareLeft compare_left = CompareLeft(), CompareRight compare_right = CompareRight())
      : left_set(sentinal, std::move(compare_left)),
        right_set(sentinal, std::move(compare_right)) {}

  // Конструкторы от других и присваивания
  bimap(const bimap& other)
      : left_set(sentinal, other.left_set.get_compare()),
        right_set(sentinal, other.right_set.get_compare()) {
    try {
      const auto end = other.end_left();
      for (auto x = other.begin_left(); x != end; ++x) {
        insert(*x, *x.flip());
      }
    } catch (...) {
      clear();
      throw;
    }
  }

  bimap(bimap&& other) noexcept : left_set(std::move(other.left_set)), right_set(std::move(other.right_set)) {
    std::swap(sz, other.sz);
  }

  bimap& operator=(const bimap& other) {
    if (this != &other) {
      bimap(other).swap(*this);
    }
    return *this;
  }

  bimap& operator=(bimap&& other) noexcept {
    if (this != &other) {
      bimap(std::move(other)).swap(*this);
    }
    return *this;
  }

  // Деструктор. Вызывается при удалении объектов bimap.
  // Инвалидирует все итераторы ссылающиеся на элементы этого bimap
  // (включая итераторы ссылающиеся на элементы следующие за последними).
  ~bimap() noexcept {
    clear();
  }

  friend void swap(bimap& lhs, bimap& rhs) noexcept {
    lhs.swap(rhs);
  }

  void swap(bimap& rhs) noexcept {
    left_set.swap(rhs.left_set);
    right_set.swap(rhs.right_set);
    std::swap(sz, rhs.sz);
  }

  // Вставка пары (left, right), возвращает итератор на left.
  // Если такой left или такой right уже присутствуют в bimap, вставка не
  // производится и возвращается end_left().
  left_iterator insert(const left_t& left, const right_t& right) {
    return perfect_forward_insert(left, right);
  }

  left_iterator insert(const left_t& left, right_t&& right) {
    return perfect_forward_insert(left, std::move(right));
  }

  left_iterator insert(left_t&& left, const right_t& right) {
    return perfect_forward_insert(std::move(left), right);
  }

  left_iterator insert(left_t&& left, right_t&& right) {
    return perfect_forward_insert(std::move(left), std::move(right));
  }

  // Удаляет элемент и соответствующий ему парный.
  // erase невалидного итератора не определен.
  // erase(end_left()) и erase(end_right()) не определены.
  // Пусть it ссылается на некоторый элемент e.
  // erase инвалидирует все итераторы ссылающиеся на e и на элемент парный к e.
  left_iterator erase_left(left_iterator it) noexcept {
    return erase(it, it.flip()).first;
  }

  // Аналогично erase, но по ключу, удаляет элемент если он присутствует, иначе
  // не делает ничего Возвращает была ли пара удалена
  bool erase_left(const left_t& left) {
    auto res = find_left(left);
    if (res == end_left()) {
      return false;
    }
    erase_left(res);
    return true;
  }

  right_iterator erase_right(right_iterator it) noexcept {
    return erase(it.flip(), it).second;
  }

  bool erase_right(const right_t& right) {
    auto res = find_right(right);
    if (res == end_right()) {
      return false;
    }
    erase_right(res);
    return true;
  }

  // erase от ренжа, удаляет [first, last), возвращает итератор на последний
  // элемент за удаленной последовательностью
  left_iterator erase_left(left_iterator first, left_iterator last) noexcept {
    while (first != last) {
      erase_left(first++);
    }
    return first;
  }

  right_iterator erase_right(right_iterator first, right_iterator last) noexcept {
    while (first != last) {
      erase_right(first++);
    }
    return first;
  }

  // Возвращает итератор по элементу. Если не найден - соответствующий end()
  left_iterator find_left(const left_t& left) const {
    return left_iterator(left_set.find(left));
  }

  right_iterator find_right(const right_t& right) const {
    return right_iterator(right_set.find(right));
  }

  // Возвращает противоположный элемент по элементу
  // Если элемента не существует -- бросает std::out_of_range
  const right_t& at_left(const left_t& key) const {
    auto res = find_left(key);
    if (res == end_left()) {
      throw std::out_of_range("bimap.h/at_left throws exception");
    }
    return *res.flip();
  }

  const left_t& at_right(const right_t& key) const {
    auto res = find_right(key);
    if (res == end_right()) {
      throw std::out_of_range("bimap.h/at_right throws exception");
    }
    return *res.flip();
  }

  // Возвращает противоположный элемент по элементу
  // Если элемента не существует, добавляет его в bimap и на противоположную
  // сторону кладет дефолтный элемент, ссылку на который и возвращает
  // Если дефолтный элемент уже лежит в противоположной паре - должен поменять
  // соответствующий ему элемент на запрашиваемый (смотри тесты)
  template <typename T = right_t>
  std::enable_if_t<std::is_default_constructible_v<T>, const T&> at_left_or_default(const left_t& key) {
    auto res = find_left(key);
    if (res == end_left()) {
      right_iterator old_it = find_right(right_t());
      if (old_it != end_right()) {
        erase_right(old_it);
      }
      return *insert(key, right_t()).flip();
    }
    return *res.flip();
  }

  template <typename T = left_t>
  std::enable_if_t<std::is_default_constructible_v<T>, const T&> at_right_or_default(const right_t& key) {
    auto res = find_right(key);
    if (res == end_right()) {
      left_iterator old_it = find_left(left_t());
      if (old_it != end_left()) {
        erase_left(old_it);
      }
      return *insert(left_t(), key);
    }
    return *res.flip();
  }

  // lower и upper bound'ы по каждой стороне
  // Возвращают итераторы на соответствующие элементы
  // Смотри std::lower_bound, std::upper_bound.
  left_iterator lower_bound_left(const left_t& left) const {
    return left_iterator(left_set.lower_bound(left));
  }

  left_iterator upper_bound_left(const left_t& left) const {
    return left_iterator(left_set.upper_bound(left));
  }

  right_iterator lower_bound_right(const right_t& right) const {
    return right_iterator(right_set.lower_bound(right));
  }

  right_iterator upper_bound_right(const right_t& right) const {
    return right_iterator(right_set.upper_bound(right));
  }

  // Возващает итератор на минимальный по порядку left.
  left_iterator begin_left() const {
    return left_iterator(left_set.begin());
  }

  // Возващает итератор на следующий за последним по порядку left.
  left_iterator end_left() const {
    return left_iterator(left_set.end());
  }

  // Возващает итератор на минимальный по порядку r
  right_iterator begin_right() const {
    return right_iterator(right_set.begin());
  }

  // Возващает итератор на следующий за последним по порядку right.
  right_iterator end_right() const {
    return right_iterator(right_set.end());
  }

  // Проверка на пустоту
  bool empty() const noexcept {
    return !size();
  }

  // Возвращает размер бимапы (кол-во пар)
  std::size_t size() const noexcept {
    return sz;
  }

  // Операторы сравнения
  friend bool operator==(const bimap& lhs, const bimap& rhs) {
    if (lhs.size() != rhs.size()) {
      return false;
    }
    const CompareLeft& left_compare = lhs.left_set.get_compare();
    const CompareRight& right_compare = lhs.right_set.get_compare();
    for (auto i = lhs.begin_left(), j = rhs.begin_left(); i != lhs.end_left(); ++i, ++j) {
      if (left_compare(*i, *j) || left_compare(*j, *i) || right_compare(*i.flip(), *j.flip()) ||
          right_compare(*j.flip(), *i.flip())) {
        return false;
      }
    }
    return true;
  }

  friend bool operator!=(const bimap& lhs, const bimap& rhs) {
    return !(lhs == rhs);
  }

  // O(1)
  void clear() noexcept {
    erase_left(begin_left(), end_left());
  }

private:
  template <typename Left_t, typename Right_t>
  left_iterator perfect_forward_insert(Left_t&& left, Right_t&& right) {
    auto left_it = lower_bound_left(left);
    auto right_it = lower_bound_right(right);
    if ((left_it != end_left() && *left_it == left) || (right_it != end_right() && *right_it == right)) {
      return end_left();
    }
    node_t* res = new node_t(std::forward<Left_t>(left), std::forward<Right_t>(right));
    ++sz;
    right_set.insert_before(right_it, *res);
    return left_set.insert_before(left_it, *res);
  }

  std::pair<left_iterator, right_iterator> erase(left_iterator left, right_iterator right) {
    node_t* x = static_cast<node_t*>(left.get_node());
    auto res =
        std::make_pair(left_iterator(left_set.erase(left.vertex)), right_iterator(right_set.erase(right.vertex)));
    delete x;
    --sz;
    return res;
  }
};
