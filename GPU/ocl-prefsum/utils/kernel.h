#include "program.h"

struct kernel {
private:
    cl_kernel _kernel;
    const string kernel_name;
public:
    explicit kernel(cl_program data, const string& kernel_name) : kernel_name(kernel_name) {
        cl_int err;
        _kernel = clCreateKernel(data, kernel_name.data(), &err);
        clCheckError(err,"clCreateKernel");
    }

    cl_kernel data() {
        return _kernel;
    }

    ~kernel() {
        clReleaseKernel(_kernel);
    }
};
