#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <numeric>

template <class T>
class matrix {
private:
  template <typename U>
  struct col_base_iterator {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using reference = U&;
    using pointer = U*;

  private:
    pointer _element;
    ptrdiff_t _col_count;
    size_t _offset;

    explicit col_base_iterator(pointer elem, size_t col_count, size_t offset)
        : _element(elem),
          _col_count(col_count),
          _offset(offset) {}

    friend matrix;

  public:
    col_base_iterator() = default;

    operator col_base_iterator<const T>() const {
      return col_base_iterator<const T>(_element, _col_count, _offset);
    }

    reference operator*() const {
      return _element[_offset];
    }

    pointer operator->() const {
      return _element + _offset;
    }

    col_base_iterator& operator++() {
      _element += _col_count;
      return *this;
    }

    col_base_iterator operator++(int) {
      col_base_iterator res = *this;
      ++*this;
      return res;
    }

    col_base_iterator& operator--() {
      _element -= _col_count;
      return *this;
    }

    col_base_iterator operator--(int) {
      col_base_iterator res = *this;
      --*this;
      return res;
    }

    col_base_iterator& operator+=(const difference_type n) {
      _element += n * static_cast<ptrdiff_t>(_col_count);
      return *this;
    }

    col_base_iterator& operator-=(const difference_type n) {
      return *this += (-n);
    }

    friend col_base_iterator operator+(const col_base_iterator& a, const difference_type b) {
      return col_base_iterator(a) += b;
    }

    friend col_base_iterator operator+(const difference_type b, const col_base_iterator& a) {
      return col_base_iterator(a) += b;
    }

    friend col_base_iterator operator-(const col_base_iterator& a, const difference_type b) {
      return col_base_iterator(a) -= b;
    }

    difference_type operator-(const col_base_iterator<const T>& other) const {
      return (_element - other._element) / static_cast<ptrdiff_t>(_col_count);
    }

    reference operator[](const ptrdiff_t n) const {
      return *(_element + n * _col_count + _offset);
    }

    friend bool operator==(const col_base_iterator& a, const col_base_iterator& b) {
      return a._element == b._element && a._offset == b._offset;
    }

    friend bool operator!=(const col_base_iterator& a, const col_base_iterator& b) {
      return !(a == b);
    }

    friend bool operator<(const col_base_iterator& a, const col_base_iterator& b) {
      return a._element < b._element || (a._element == b._element && a._offset < b._offset);
    }

    friend bool operator<=(const col_base_iterator& a, const col_base_iterator& b) {
      return a < b || a == b;
    }

    friend bool operator>(const col_base_iterator& a, const col_base_iterator& b) {
      return !(a <= b);
    }

    friend bool operator>=(const col_base_iterator& a, const col_base_iterator& b) {
      return !(a < b);
    }
  };

public:
  using value_type = T;

  using reference = T&;
  using const_reference = const T&;

  using pointer = T*;
  using const_pointer = const T*;

  using iterator = T*;
  using const_iterator = const T*;

  using row_iterator = T*;
  using const_row_iterator = const T*;

  using col_iterator = col_base_iterator<T>;
  using const_col_iterator = col_base_iterator<const T>;

public:
  matrix() : _arr(nullptr), _rows(0), _cols(0) {}

  matrix(size_t rows, size_t cols) : _rows(rows), _cols(cols) {
    if (empty()) {
      _arr = nullptr;
      _rows = _cols = 0;
    } else {
      _arr = new T[rows * cols]{};
    }
  }

  template <size_t Rows, size_t Cols>
  matrix(const T (&init)[Rows][Cols]) : _arr(new T[Rows * Cols]),
                                        _rows(Rows),
                                        _cols(Cols) {
    iterator it = begin();
    for (const T(&row)[Cols] : init) {
      it = std::copy_n(row, cols(), it);
    }
  }

  matrix(const matrix& other) : _rows(other._rows), _cols(other._cols) {
    if (other.empty()) {
      _arr = nullptr;
    } else {
      _arr = new T[other._rows * other._cols];
      std::copy(other.begin(), other.end(), begin());
    }
  }

  matrix& operator=(const matrix& other) {
    if (other != *this) {
      matrix(other).swap(*this);
    }
    return *this;
  }

  ~matrix() {
    delete[] _arr;
  }

  // Iterators

  iterator begin() {
    return data();
  }

  const_iterator begin() const {
    return data();
  }

  iterator end() {
    return begin() + size();
  }

  const_iterator end() const {
    return begin() + size();
  }

  row_iterator row_begin(size_t row) {
    return data() + row * cols();
  }

  const_row_iterator row_begin(size_t row) const {
    return data() + row * cols();
  }

  row_iterator row_end(size_t row) {
    return row_begin(row + 1);
  }

  const_row_iterator row_end(size_t row) const {
    return row_begin(row + 1);
  }

  col_iterator col_begin(size_t col) {
    return col_iterator(data(), cols(), col);
  }

  const_col_iterator col_begin(size_t col) const {
    return const_col_iterator(data(), cols(), col);
  }

  col_iterator col_end(size_t col) {
    return col_iterator(end(), cols(), col);
  }

  const_col_iterator col_end(size_t col) const {
    return const_col_iterator(end(), cols(), col);
  }

  // Size

  size_t rows() const {
    return _rows;
  }

  size_t cols() const {
    return _cols;
  }

  size_t size() const {
    return _rows * _cols;
  }

  bool empty() const {
    return size() == 0;
  }

  // Elements access

  reference operator()(size_t row, size_t col) {
    return _arr[row * cols() + col];
  }

  const_reference operator()(size_t row, size_t col) const {
    return _arr[row * cols() + col];
  }

  pointer data() {
    return _arr;
  }

  const_pointer data() const {
    return _arr;
  }

  // Comparison

  friend bool operator==(const matrix& left, const matrix& right) {
    return left.rows() == right.rows() && left.cols() == right.cols() &&
           std::equal(left.begin(), left.end(), right.begin());
  }

  friend bool operator!=(const matrix& left, const matrix& right) {
    return !operator==(left, right);
  }

  // Arithmetic operations

  matrix& operator+=(const matrix& other) {
    std::transform(begin(), end(), other.begin(), begin(), std::plus<T>());
    return *this;
  }

  matrix& operator-=(const matrix& other) {
    std::transform(begin(), end(), other.begin(), begin(), std::minus<T>());
    return *this;
  }

  matrix& operator*=(const matrix& other) {
    (*this * other).swap(*this);
    return *this;
  }

  matrix& operator*=(const_reference factor) {
    std::for_each(begin(), end(), [factor](T& x) { x *= factor; });
    return *this;
  }

  friend matrix operator+(const matrix& left, const matrix& right) {
    return matrix(left) += right;
  }

  friend matrix operator-(const matrix& left, const matrix& right) {
    return matrix(left) -= right;
  }

  friend matrix operator*(const matrix& left, const matrix& right) {
    matrix res(left._rows, right._cols);
    for (int i = 0; i < left._rows; i++) {
      for (int j = 0; j < right._cols; j++) {
        res(i, j) = std::inner_product(left.row_begin(i), left.row_end(i), right.col_begin(j), static_cast<T>(0));
      }
    }
    return res;
  }

  friend matrix operator*(const matrix& left, const_reference right) {
    return matrix(left) *= right;
  }

  friend matrix operator*(const_reference left, const matrix& right) {
    return matrix(right) *= left;
  }

  // other functions
  void swap(matrix& other) {
    std::swap(_cols, other._cols);
    std::swap(_rows, other._rows);
    std::swap(_arr, other._arr);
  }

private:
  T* _arr;
  size_t _rows;
  size_t _cols;
};
