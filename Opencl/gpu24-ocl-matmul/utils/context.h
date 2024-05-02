#include "device.h"

struct context {
private:
    cl_context _context;

public:
    explicit context(cl_device_id *data) {
        cl_int err;
        _context = clCreateContext(
                nullptr,
                1,
                data,
                nullptr,
                nullptr,
                &err
                );
        if (err != CL_SUCCESS) {
            throw runtime_error("ERROR with clCreateContext");
        }
    }

    cl_context data() {
        return _context;
    }

    ~context() {
        clReleaseContext(_context);
    }
};
