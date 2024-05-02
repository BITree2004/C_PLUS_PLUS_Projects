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
        }
        else {
            PER_THREAD = 1;
        }
        double sqrt_max_tile = sqrt(max_work_group * PER_THREAD * PER_THREAD);
        if (sqrt_max_tile != floor(sqrt_max_tile)) {
            sqrt_max_tile = sqrt(max_work_group * PER_THREAD * PER_THREAD / 2);
        }
        TILE = floor(sqrt_max_tile);
        TILE = min<size_t>(128, TILE);
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
        args = format("-D TILE={} -D PER_THREAD={} -D typeX=float{} -D TILE_K={}", TILE, PER_THREAD, PER_THREAD, min<size_t>(TILE,16));
        program _program(_context.data(), devices.data(index), path_of_code, args);
        kernel _kernel(_program.data());
        command_queue commandQueue(_context, devices[index]);
	
        buffer<cl_float> mem1(_context, CL_MEM_READ_ONLY, new_m * new_k);
        buffer<cl_float> mem2(_context, CL_MEM_READ_ONLY, new_k * new_n);
        buffer<cl_float> mem3(_context, CL_MEM_WRITE_ONLY, new_m * new_n);
	
        if (clSetKernelArg(_kernel.data(), 0, sizeof(cl_mem), mem1.pointer()) != CL_SUCCESS) {
            throw runtime_error("Problem with clSetKernelArg 0");
        }
        if (clSetKernelArg(_kernel.data(), 1, sizeof(cl_mem), mem2.pointer()) != CL_SUCCESS) {
            throw runtime_error("Problem with clSetKernelArg 1");
        }
        if (clSetKernelArg(_kernel.data(), 2, sizeof(cl_mem), mem3.pointer()) != CL_SUCCESS) {
            throw runtime_error("Problem with clSetKernelArg 2");
        }
        if (clSetKernelArg(_kernel.data(), 3, sizeof(cl_uint), &new_n) != CL_SUCCESS) {
            throw runtime_error("Problem with clSetKernelArg 3");
        }
        if (clSetKernelArg(_kernel.data(), 4, sizeof(cl_uint), &new_k) != CL_SUCCESS) {
            throw runtime_error("Problem with clSetKernelArg 4");
        }
        if (clSetKernelArg(_kernel.data(), 5, sizeof(cl_uint), &new_m) != CL_SUCCESS) {
            throw runtime_error("Problem with clSetKernelArg 5");
        }
        calc_size();
        cl_event kernel_event, a_event, b_event, c_event;
        if (clEnqueueWriteBuffer(commandQueue.data(),
                                 mem1.data(),
                                 CL_FALSE,
                                 0,
                                 new_m * new_k * sizeof(cl_float),
                                 (realization == 3 ? a_transpose.data() : a.data()),
                                 0,
                                 nullptr,
                                 &a_event
        ) != CL_SUCCESS) {
            throw runtime_error("Problem with clEnqueueWriteBuffer");
        }
        if (clEnqueueWriteBuffer(commandQueue.data(),
                                 mem2.data(),
                                 CL_TRUE,
                                 0,
                                 new_k * new_n * sizeof(cl_float),
                                 b.data(),
                                 0,
                                 nullptr,
                                 &b_event
        ) != CL_SUCCESS) {
            throw runtime_error("Problem with clEnqueueWriteBuffer");
        }
        if (clEnqueueNDRangeKernel(commandQueue.data(),
                                            _kernel.data(),
                                            2,
                                            nullptr,
                                            global_work_size.data(),
                                            local_size.data(),
                                            0,
                                            nullptr,
                                            &kernel_event
        ) != CL_SUCCESS) {
            throw runtime_error("Problem with clEnqueueNDRangeKernel!");
        }
        if (clEnqueueReadBuffer(commandQueue.data(),
                                mem3.data(),
                                CL_TRUE,
                                0,
                                new_m * new_n * sizeof(cl_float),
                                res.data(),
                                0,
                                nullptr,
                                &c_event
        ) != CL_SUCCESS) {
            throw runtime_error("Problem with clEnqueueReadBuffer");
        }

        /*if (realization == 3) {
            res.transpose();
        }*/
        
        // calculate time of execution
        size_t start;
        size_t end;
        clGetEventProfilingInfo(kernel_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, nullptr);
        clGetEventProfilingInfo(kernel_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, nullptr);
        double kernel_time = (end - start) / 1e6;
        clGetEventProfilingInfo(a_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, nullptr);
        clGetEventProfilingInfo(a_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, nullptr);
        double all_time = kernel_time + (end - start) / 1e6;
        clGetEventProfilingInfo(b_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, nullptr);
        clGetEventProfilingInfo(b_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, nullptr);
        all_time += (end - start) / 1e6;
        clGetEventProfilingInfo(c_event, CL_PROFILING_COMMAND_START, sizeof(start), &start, nullptr);
        clGetEventProfilingInfo(c_event, CL_PROFILING_COMMAND_END, sizeof(end), &end, nullptr);
        all_time += (end - start) / 1e6;
        cout << format("Time: {}\t{}", kernel_time, all_time);
	
        // write in output file
        string added_info;
        if (realization >= 2) {
            added_info = format("LOCAL_WORK_SIZE [{}, {}]\n", local_size[0], local_size[1]);
        }
        if (realization >= 3) {
            added_info += format("WI_WORK {}\n", PER_THREAD);
        }
        
        write(added_info);
        check();
    }
};
