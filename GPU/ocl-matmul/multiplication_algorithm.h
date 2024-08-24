#ifndef MULTIPLICATION_ALGORITHM_H
#define MULTIPLICATION_ALGORITHM_H

#include <iostream>
#include <fstream>
#include <utility>
#include "utils/buffer.h"
#include "utils/matrix.h"
#include "utils/reader.h"

using namespace std;

struct multiplication_algorithm {
private:
    string input;
    string output;
protected:
    // data
    cl_uint n{}, k{}, m{}, new_n{}, new_m{}, new_k{};
    matrix<cl_float> a, a_transpose, b, res;
    // properties
    string type;
    int index;
    int realization;

public:
    explicit multiplication_algorithm(
            string input,
            string output,
            string type,
            int index,
            int realization
    ) : input(std::move(input)), output(std::move(output)), type(std::move(type)), index(index),
        realization(realization) {
    }


protected:
    void read(size_t TILE) {
        read_matrix(TILE);
        res = matrix<cl_float>(new_m, new_n);
    }

    void write() {
        write_matrix();
    }

    void check() {
        //check(n, k, m, a, b, res); // for checking
    }

    static size_t make_multiple(size_t a, size_t b) {
        return (a + b - 1) / b * b;
    }

private:
    constexpr static cl_float EPS = 1e-5;

    static void check(
            cl_uint n,
            cl_uint k,
            cl_uint m,
            const matrix<cl_float> &a,
            const matrix<cl_float> &b,
            const matrix<cl_float> &c
    ) {
        for (size_t i = 0; i < min(100u, m); ++i) {
            for (size_t j = 0; j < min(100u, n); ++j) {
                cl_float res = 0;
                for (size_t z = 0; z < k; ++z) {
                    res += a(i, z) * b(z, j);
                }
                if (abs((res - c(i, j)) / (res + 1e-9)) > EPS || c(i, j) != c(i, j)) {
                    throw runtime_error(
                            "Not correct multiply in position " +
                            to_string(i) +
                            " " +
                            to_string(j) +
                            "! Expected: " +
                            to_string(res) +
                            " but was: " +
                            to_string(c(i, j))
                    );
                }
            }
        }
    }

    template<typename T>
    T read_token(char *&begin, function<T(char *begin, char **end)> func) {
        errno = 0;
        char *end;
        T x = func(begin, &end);
        if (ERANGE == errno || end == begin) {
            throw runtime_error(format("Error read from file [{}]!", input));
        }
        begin = end;
        return x;
    }

    void read_matrix(size_t TILE) {
        string file = read_file(input);
        char *begin = file.data();
        auto func_str_to_int = [](auto x, auto y) -> cl_uint { return strtol(x, y, 10); };
        n = read_token<cl_uint>(begin, func_str_to_int);
        k = read_token<cl_uint>(begin, func_str_to_int);
        m = read_token<cl_uint>(begin, func_str_to_int);
        new_n = make_multiple(n, TILE);
        new_m = make_multiple(m, TILE);
        new_k = make_multiple(k, TILE);
        a = matrix<cl_float>(new_m, new_k);
        a_transpose = matrix<cl_float>(new_k, new_m);
        b = matrix<cl_float>(new_k, new_n);
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < k; ++j) {
                a(i, j) = read_token<cl_float>(begin, strtof);
                a_transpose(j, i) = a(i, j);
            }
        }
        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < n; ++j) {
                b(i, j) = read_token<cl_float>(begin, strtof);
            }
        }
    }

    void write_matrix() {
        FILE *output_file = fopen(output.data(), "w");
        if (output_file == nullptr) {
            throw runtime_error(format("File with name [{}] doesn't open!", output));
        }
        fprintf(output_file, "%i %i\n", n, m);
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                fprintf(output_file, "%f ", res(i, j));
            }
            fprintf(output_file, "\n");
        }
        fclose(output_file);
    }
};

#endif
