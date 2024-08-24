#include <chrono>
#include <omp.h>
#include "multiplication_algorithm.h"
#include "utils/benchmark.h"

struct omp : multiplication_algorithm {
    template<typename... Args>
    explicit omp(Args &&... args) : multiplication_algorithm(args...) {}

    void run() {
        read(1);
        if (realization != 0) {
            throw runtime_error("Unsupported --realization token: " + to_string(realization) + " !");
        }
        begin();
        #pragma omp parallel for default(none) schedule(dynamic)
        for (int i = 0; i < m; ++i) {
            for (size_t j = 0; j < n; ++j) {
                cl_float sum = 0;
                for (size_t z = 0; z < k; ++z) {
                    sum += a(i, z) * b(z, j);
                }
                res(i, j) = sum;
            }
        }
        cout << format("Time: {}", finish());
        write();
        check();
    }
};
