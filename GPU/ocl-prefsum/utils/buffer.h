#include "command_queue.h"

template<typename T>
struct buffer {
private:
    cl_mem mem = 0;

public:
    buffer() {}

    buffer(context &_context, const cl_mem_flags flag, const size_t size, void* host_ptr) {
        cl_int err;
        mem = clCreateBuffer(_context.data(), flag, size * sizeof(T), host_ptr, &err);
        clCheckError(err, "clCreateBuffer");
    }

    buffer& operator=(buffer&& other) {
        swap(mem, other.mem);
        return *this;
    }

    cl_mem data() {
        return mem;
    }

    cl_mem *pointer() {
        return &mem;
    }

    ~buffer() {
        if (mem != 0) {
            clReleaseMemObject(mem);
        }
    }
};
