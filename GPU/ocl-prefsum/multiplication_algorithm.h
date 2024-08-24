#ifndef MULTIPLICATION_ALGORITHM_H
#define MULTIPLICATION_ALGORITHM_H

#include <iostream>
#include <fstream>
#include <utility>
#include <functional>
#include <format>
#include "utils/buffer.h"
#include "utils/reader.h"

using namespace std;

struct multiplication_algorithm {
private:
    string input;
    string output;
protected:
    // data
    cl_ulong n{}, new_n{};
    vector<cl_float> a, res;
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
        read_array(TILE);
        res = vector<cl_float>(n);
    }

    void write() {
        write_array();
    }

    void check() {
        check(n, a, res); // for checking
    }

    static cl_ulong make_multiple(cl_ulong a, cl_ulong b) {
        return (a + b - 1) / b * b;
    }

private:
    constexpr static cl_float EPS = 1e-5;

    static void check(
            cl_ulong n,
            const vector<cl_float> &a,
            const vector<cl_float> &b
    ) {
        cl_float res = 0;
        for (cl_ulong i = 0; i < n; ++i) {
            res += a[i];
            if (abs((res - b[i]) / (res + 1e-9)) > EPS || b[i] != b[i]) {
                throw runtime_error(
                        "Not correct prefix sum in position " +
                        to_string(i) +
                        "! Expected: " +
                        to_string(res) +
                        " but was: " +
                        to_string(b[i])
                );
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

    void read_array(size_t TILE) {
        string file = read_file(input);
        char *begin = file.data();
        auto func_str_to_long = [](auto x, auto y) -> cl_ulong { return strtoll(x, y, 10); };
        n = read_token<cl_ulong>(begin, func_str_to_long);
        new_n = make_multiple(n, TILE);
        a = vector<cl_float>(new_n);
        for (int i = 0; i < n; ++i) {
            a[i] = read_token<cl_float>(begin, strtof);
        }
    }

    void gen_array(size_t TILE) {
        n = 16777472; // any const
        //cin >> n;
        new_n = make_multiple(n, TILE);
        a = vector<cl_float>(new_n);
        srand(time(nullptr));
        for (cl_ulong i = 0; i < n; ++i) {
            a[i] = rand() % 2;
        }
    }

    void write_array() {
        FILE *output_file = fopen(output.data(), "w");
        if (output_file == nullptr) {
            throw runtime_error(format("File with name [{}] doesn't open!", output));
        }
        for (cl_ulong i = 0; i < n; ++i) {
            fprintf(output_file, "%f ", res[i]);
        }
        fprintf(output_file, "\n");
        fclose(output_file);
    }

};

#endif
