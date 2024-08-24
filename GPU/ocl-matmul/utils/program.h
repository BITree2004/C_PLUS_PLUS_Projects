#include <utility>
#include "context.h"
#include "reader.h"

struct program {
private:
    cl_program _program;

public:

    program(cl_context context_data, cl_device_id *device_data, string path_of_file, string args) {
        string file = read_file(std::move(path_of_file));
        char const *programs[1] = {file.data()};
        size_t size_programs[1] = {file.size()};
        cl_int err;
        cl_uint size_program = 1;
        _program = clCreateProgramWithSource(
                context_data,
                size_program,
                programs,
                size_programs,
                &err
        );
        clCheckError(err, "clCreateProgramWithSource");

        if (clBuildProgram(
                _program,
                1,
                device_data,
                args.data(),
                nullptr,
                nullptr
                ) != CL_SUCCESS) {
            size_t log_size = 0;
            clGetProgramBuildInfo(
                    _program,
                    device_data[0],
                    CL_PROGRAM_BUILD_LOG,
                    0, nullptr,
                    &log_size
            );
            string log;
            log.resize(log_size);
            clGetProgramBuildInfo(
                    _program,
                    device_data[0],
                    CL_PROGRAM_BUILD_LOG,
                    log_size,
                    log.data(),
                    nullptr
            );
            throw runtime_error(format("Problem with clBuildProgram: {}", log));
        }
    }

    cl_program data() {
        return _program;
    }

    ~program() {
        clReleaseProgram(_program);
    }
};
