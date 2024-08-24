#include <algorithm>
#include <cstddef>
#include <functional>
#include <numeric>
#include <cstring>

template <class T>
class matrix {
public:
    using value_type = T;

    using reference = T&;
    using const_reference = const T&;

    using pointer = T*;
    using const_pointer = const T*;

    using iterator = T*;
    using const_iterator = const T*;

public:
    matrix() : _arr(nullptr), _rows(0), _cols(0) {}

    matrix(size_t rows, size_t cols) : _rows(rows), _cols(cols) {
        if (empty()) {
            _arr = nullptr;
            _rows = _cols = 0;
        } else {
            _arr = new T[rows * cols]{};
            memset(_arr, 0, sizeof(value_type) * rows * cols);
        }
    }


    matrix(const matrix& other) : _rows(other._rows), _cols(other._cols), is_transposed(other.is_transposed) {
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
        if (is_transposed) {
            return _arr[col * cols() + row];
        }
        return _arr[row * cols() + col];
    }

    const_reference operator()(size_t row, size_t col) const {
        if (is_transposed) {
            return _arr[col * cols() + row];
        }
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

    // other functions
    void swap(matrix& other) {
        std::swap(_cols, other._cols);
        std::swap(_rows, other._rows);
        std::swap(_arr, other._arr);
    }

    void transpose() {
        std::swap(_rows, _cols);
        is_transposed ^= 1;
    }
private:
    T* _arr;
    size_t _rows;
    size_t _cols;
    bool is_transposed{};
};
