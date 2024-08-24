#include "multiplication_algorithm.h"
#include <cmath>

struct opencl : multiplication_algorithm {
private:
    size_t TILE{};
    size_t PER_THREAD{};
    vector<size_t> local_size;
    vector<size_t> global_work_size;

    void calc_tile(cl_device_id device) {
        size_t max_work_group;
        clGetDeviceInfo(device,
                        CL_DEVICE_MAX_WORK_GROUP_SIZE,
                        sizeof(size_t),
                        &max_work_group,
                        nullptr);
        if (realization >= 3) {
            PER_THREAD = 8;
        } else {
            PER_THREAD = 1;
        }
        double sqrt_max_tile = sqrt(max_work_group * PER_THREAD * PER_THREAD);
        if (sqrt_max_tile != floor(sqrt_max_tile)) {
            sqrt_max_tile = sqrt(max_work_group * PER_THREAD * PER_THREAD / 2);
        }
        TILE = floor(sqrt_max_tile);
        TILE = min<size_t>(128, TILE);
        if (realization <= 2) {
            TILE = min<size_t>(16, TILE);
        }
        TILE = max<size_t>(PER_THREAD, TILE);
    }

    void calc_size() {
        if (realization >= 3) {
            global_work_size = {new_m / PER_THREAD, new_n / PER_THREAD};
            local_size = {TILE / PER_THREAD, TILE / PER_THREAD};
        } else {
            global_work_size = {new_n, new_m};
            local_size = {TILE, TILE};
        }
    }

public:
    template<typename... Args>
    explicit opencl(Args &&... args) : multiplication_algorithm(args...) {}

    void run() {
        platform _platform;
        device devices(_platform, type);
        context _context(devices.data(index));
        calc_tile(*devices.data(index));
        read(TILE);
        cout << devices.get_name(index) << endl;
        string path_of_code;
        switch (realization) {
            case 1:
                path_of_code = "program1.cl";
                break;
            case 2:
                path_of_code = "program2.cl";
                break;
            case 3:
                path_of_code = "program3.cl";
                break;
            default:
                throw runtime_error("Unsupported this realization " + to_string(realization));
        }
        string args;
        args = format(
                "-D TILE={} -D PER_THREAD={} -D typeX=float{} -D TILE_K={}",
                TILE,
                PER_THREAD,
                PER_THREAD,
                min<size_t>(TILE, 16)
        );
        program _program(_context.data(), devices.data(index), path_of_code, args);
        kernel _kernel(_program.data(), "mul");
        command_queue commandQueue(_context, devices[index]);

        buffer<cl_float> mem1(_context, CL_MEM_READ_ONLY, new_m * new_k);
        buffer<cl_float> mem2(_context, CL_MEM_READ_ONLY, new_k * new_n);
        buffer<cl_float> mem3(_context, CL_MEM_WRITE_ONLY, new_m * new_n);

        clCheckError(
                clSetKernelArg(_kernel.data(), 0, sizeof(cl_mem), mem1.pointer()),
                "clSetKernelArg 0"
        );

        clCheckError(
                clSetKernelArg(_kernel.data(), 1, sizeof(cl_mem), mem2.pointer()),
                "clSetKernelArg 1"
        );
        clCheckError(
                clSetKernelArg(_kernel.data(), 2, sizeof(cl_mem), mem3.pointer()),
                "clSetKernelArg 2"
        );
        clCheckError(
                clSetKernelArg(_kernel.data(), 3, sizeof(cl_uint), &new_n),
                "clSetKernelArg 3"
        );
        clCheckError(
                clSetKernelArg(_kernel.data(), 4, sizeof(cl_uint), &new_k),
                "clSetKernelArg 4"
        );
        clCheckError(
                clSetKernelArg(_kernel.data(), 5, sizeof(cl_uint), &new_m),
                "clSetKernelArg 5"
        );
        calc_size();
        cl_event kernel_event, a_event, b_event, c_event;
        clCheckError(
                clEnqueueWriteBuffer(commandQueue.data(),
                                     mem1.data(),
                                     CL_FALSE,
                                     0,
                                     new_m * new_k * sizeof(cl_float),
                                     (realization == 3 ? a_transpose.data() : a.data()),
                                     0,
                                     nullptr,
                                     &a_event
                ),
                "clEnqueueWriteBuffer"
        );
        clCheckError(
                clEnqueueWriteBuffer(commandQueue.data(),
                                     mem2.data(),
                                     CL_TRUE,
                                     0,
                                     new_k * new_n * sizeof(cl_float),
                                     b.data(),
                                     0,
                                     nullptr,
                                     &b_event
                ),
                "clEnqueueWriteBuffer"
        );
        clCheckError(
                clEnqueueNDRangeKernel(commandQueue.data(),
                                       _kernel.data(),
                                       2,
                                       nullptr,
                                       global_work_size.data(),
                                       local_size.data(),
                                       0,
                                       nullptr,
                                       &kernel_event
                ),
                "clEnqueueNDRangeKernel"
        );
        clCheckError(
                clEnqueueReadBuffer(commandQueue.data(),
                                    mem3.data(),
                                    CL_TRUE,
                                    0,
                                    new_m * new_n * sizeof(cl_float),
                                    res.data(),
                                    0,
                                    nullptr,
                                    &c_event
                ),
                "clEnqueueReadBuffer"
        );

        // calculate time of execution
        cl_ulong start;
        cl_ulong end;
        clGetEventProfilingInfo(kernel_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, nullptr);
        clGetEventProfilingInfo(kernel_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, nullptr);
        double kernel_time = static_cast<double>(end - start) / 1e6;
        clGetEventProfilingInfo(a_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, nullptr);
        clGetEventProfilingInfo(a_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, nullptr);
        double all_time = kernel_time + static_cast<double>(end - start) / 1e6;
        clGetEventProfilingInfo(b_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, nullptr);
        clGetEventProfilingInfo(b_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, nullptr);
        all_time += static_cast<double>(end - start) / 1e6;
        clGetEventProfilingInfo(c_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, nullptr);
        clGetEventProfilingInfo(c_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, nullptr);
        all_time += static_cast<double>(end - start) / 1e6;
        cout << format("Time: {}\t{}", kernel_time, all_time) << endl;

        // write in output file
        if (realization >= 2) {
            cout << format("LOCAL_WORK_SIZE [{}, {}]", local_size[0], local_size[1]) << endl;
        }
        if (realization >= 3) {
            cout << format("WI_WORK {}", PER_THREAD) << endl;
        }

        write();
        check();
    }
};
