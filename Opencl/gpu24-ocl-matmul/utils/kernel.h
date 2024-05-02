#include "program.h"

struct kernel {
private:
    cl_kernel _kernel;
    const string kernel_name = "mul";
public:
    explicit kernel(cl_program data) {
        cl_int err;
        _kernel = clCreateKernel(data, kernel_name.data(), &err);
        if (err != CL_SUCCESS) {
            cerr << err << endl;
            throw runtime_error("ERROR with clCreateKernel");
        }
    }

    cl_kernel data() {
        return _kernel;
    }

    ~kernel() {
        clReleaseKernel(_kernel);
    }
};
