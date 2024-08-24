#include "kernel.h"

struct command_queue {
private:
    cl_command_queue commandQueue;
public:
    command_queue(context &_context, const cl_device_id& _device) {
        cl_int err;
        commandQueue = clCreateCommandQueue(
                _context.data(),
                _device,
                CL_QUEUE_PROFILING_ENABLE,
                &err
                );
        clCheckError(err, "clCreateCommandQueue");
    }

    cl_command_queue data() {
        return commandQueue;
    }

    ~command_queue() {
        clReleaseCommandQueue(commandQueue);
    }
};
