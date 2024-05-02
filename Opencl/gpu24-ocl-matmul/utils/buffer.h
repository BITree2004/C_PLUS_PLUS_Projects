#include "command_queue.h"

template<typename T>
struct buffer {
private:
    cl_mem mem;

public:
    buffer(context &_context, const cl_mem_flags flag, const size_t size) {
        cl_int err;
        mem = clCreateBuffer(_context.data(), flag, size * sizeof(T), nullptr, &err);
        if (err != CL_SUCCESS) {
            throw runtime_error("Problem with clCreateBuffer" + to_string(err) + "!");
        }
    }

    cl_mem data() {
        return mem;
    }

    cl_mem *pointer() {
        return &mem;
    }

    ~buffer() {
        clReleaseMemObject(mem);
    }
};
