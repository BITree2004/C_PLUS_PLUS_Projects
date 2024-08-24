#include "multiplication_algorithm.h"
#include <cmath>
#include <string>

struct opencl : multiplication_algorithm {
private:
    const cl_uint LOG_BANKS = 5;
    size_t TILE{};
    cl_ulong shift{};
    vector<size_t> local_size;
    vector<size_t> global_work_size;
    vector<cl_event> reading_events, execute_events;
    bool is_used_host_ptr_memory = false;

    [[nodiscard]] cl_ulong get_local_mem_size(cl_ulong tile) const {
        return (tile + ((tile) >> (LOG_BANKS + 1)));
    }

    void calc_tile(cl_device_id device) {
        size_t max_work_group;
        clGetDeviceInfo(device,
            CL_DEVICE_MAX_WORK_GROUP_SIZE,
            sizeof(size_t),
            &max_work_group,
            nullptr);
        if (realization == 2 || realization == 3) {
            max_work_group *= 2;
        }
        cl_ulong max_local_mem_size;
        clGetDeviceInfo(device,
            CL_DEVICE_LOCAL_MEM_SIZE,
            sizeof(cl_ulong),
            &max_local_mem_size,
            nullptr);
        for (int i = 1; i <= max_work_group; i *= 2) {
            if (4 * (get_local_mem_size(i) + 1) <= max_local_mem_size) {
                TILE = i;
            }
        }
    }

    void calc_size() {
        if (realization == 1) {
            local_size = { TILE };
            global_work_size = { TILE };
        }
        else if (realization == 2) {
            local_size = { TILE / 2 };
            global_work_size = { static_cast<size_t>(new_n / 2) };
        }
        else if (realization == 3) {
            local_size = { TILE / 2 };
        }
        else if (realization == 4) {
            local_size = { TILE };
        }
    }

    void calc_algo(cl_device_id device) {
        cl_ulong max_global_mem_size;
        clGetDeviceInfo(device,
            CL_DEVICE_MAX_MEM_ALLOC_SIZE,
            sizeof(cl_ulong),
            &max_global_mem_size,
            nullptr);
        if (new_n * 4 * sizeof(cl_float) > max_global_mem_size) {
            if (new_n * sizeof(cl_float) > max_global_mem_size) {
                // my minimal realization must have new_n 
                throw runtime_error("Global memory size's not enough! Expected: " + to_string(new_n * sizeof(cl_float)) + " bytes! But was: " + to_string(max_global_mem_size) + " !");
            }
            else {
                // This is how I save 2 * new_n bytes
                is_used_host_ptr_memory = true;
                if (new_n * 2 * sizeof(cl_float) > max_global_mem_size) {
                    cerr << "Sorry, but global memory's not enough! So program must use slowly algorithm." << endl;
                    realization = 1;
                    calc_tile(device);
                }
            }
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
        calc_algo(*devices.data(index));
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
        case 4:
            path_of_code = "program4.cl";
            break;
        default:
            throw runtime_error("Unsupported this realization " + to_string(realization));
        }
        string args;
        args = format("-D TILE={} -D PRE_TILE={} -D HIGH={} -D LOG_BANKS={}", TILE, TILE - 1, static_cast<size_t>(ceil(log2f(static_cast<float>(TILE)))), LOG_BANKS);
        program _program(_context.data(), devices.data(index), path_of_code, args);
        command_queue commandQueue(_context, devices[index]);
        kernel _kernel(_program.data(), "scanner");
        buffer<cl_float> mem1;
        if (realization <= 2) {
            new_n /= 2;
        }
        if (is_used_host_ptr_memory) {
            mem1 = buffer<cl_float>(_context, CL_MEM_USE_HOST_PTR, new_n * 2, a.data());
        } else {
            mem1 = buffer<cl_float>(_context, CL_MEM_READ_ONLY, new_n * 2, nullptr);
            reading_events.push_back(nullptr);
            clCheckError(
                clEnqueueWriteBuffer(commandQueue.data(),
                    mem1.data(),
                    CL_FALSE,
                    0,
                    n * sizeof(cl_float),
                    a.data(),
                    0,
                    nullptr,
                    &reading_events.back()
                ),
                "clEnqueueWriteBuffer"
            );
        }
        buffer<cl_float> mem2(_context, CL_MEM_WRITE_ONLY, new_n * 2, nullptr);
        if (realization <= 2) {
            new_n *= 2;
        }
        clCheckError(
            clSetKernelArg(_kernel.data(), 0, sizeof(cl_mem), mem1.pointer()),
            "clSetKernelArg 0"
        );
        clCheckError(
            clSetKernelArg(_kernel.data(), 1, sizeof(cl_mem), mem2.pointer()),
            "clSetKernelArg 1"
        );
        calc_size();

        if (realization == 1 || realization == 2) {
            normal_realization(commandQueue, _kernel);
        }
        else {
            specific_realization(_program, commandQueue, _kernel, mem2);
        }

        reading_events.push_back(nullptr);
        clCheckError(
            clEnqueueReadBuffer(commandQueue.data(),
                mem2.data(),
                CL_TRUE,
                0,
                n * sizeof(cl_float),
                res.data(),
                0,
                nullptr,
                &reading_events.back()
            ),
            "clEnqueueReadBuffer"
        );

        // calculate time of execution
        cl_ulong start;
        cl_ulong end;
        long double kernel_time = 0;
        for (const auto& x : execute_events) {
            clGetEventProfilingInfo(x, CL_PROFILING_COMMAND_START, sizeof(start), &start, nullptr);
            clGetEventProfilingInfo(x, CL_PROFILING_COMMAND_END, sizeof(end), &end, nullptr);
            kernel_time += static_cast<long double>(end - start) / 1e6;
        }
        long double all_time = kernel_time;
        for (const auto& x : reading_events) {
            clGetEventProfilingInfo(x, CL_PROFILING_COMMAND_START, sizeof(start), &start, nullptr);
            clGetEventProfilingInfo(x, CL_PROFILING_COMMAND_END, sizeof(end), &end, nullptr);
            all_time += static_cast<long double>(end - start) / 1e6;
        }
        cout << format("Time: {}\t{}", kernel_time, all_time) << endl;

        // write in output file
        cout << format("LOCAL_WORK_SIZE {}", local_size[0]) << endl;

        write();
        //check();
    }

    void normal_realization(
        command_queue& commandQueue,
        kernel& _kernel
    ) {
        clCheckError(
            clSetKernelArg(_kernel.data(), 2, sizeof(cl_uint), &new_n),
            "clSetKernelArg 2"
        );
        clCheckError(
            clSetKernelArg(_kernel.data(), 3, sizeof(cl_uint), &shift),
            "clSetKernelArg 3"
        );
        execute_events.push_back(nullptr);
        clCheckError(
            clEnqueueNDRangeKernel(commandQueue.data(),
                _kernel.data(),
                1,
                nullptr,
                global_work_size.data(),
                local_size.data(),
                0,
                nullptr,
                &execute_events.back()
            ),
            "clEnqueueNDRangeKernel"
        );
    }

    void specific_realization(
        program& _program,
        command_queue& commandQueue,
        kernel& _kernel,
        buffer<cl_float>& mem2
    ) {
        kernel back_kernel(_program.data(), "scanner_back");
        clCheckError(
            clSetKernelArg(back_kernel.data(), 0, sizeof(cl_mem), mem2.pointer()),
            "clSetKernelArg 0 in scanner_bask"
        );
        cl_uint current_n = new_n;
        vector<cl_uint> sizes = {};
        while (true) {
            sizes.push_back(current_n);
            global_work_size = { (realization == 3 ? current_n / 2 : current_n) };
            execute_events.push_back(nullptr);
            clCheckError(
                clSetKernelArg(_kernel.data(), 2, sizeof(cl_uint), &current_n),
                "clSetKernelArg 2"
            );
            clCheckError(
                clSetKernelArg(_kernel.data(), 3, sizeof(cl_uint), &shift),
                "clSetKernelArg 3"
            );
            clCheckError(
                clEnqueueNDRangeKernel(commandQueue.data(),
                    _kernel.data(),
                    1,
                    nullptr,
                    global_work_size.data(),
                    local_size.data(),
                    0,
                    nullptr,
                    &execute_events.back()
                ),
                "clEnqueueNDRangeKernel"
            );
            clFinish(commandQueue.data());
            if (current_n <= TILE) {
                break;
            }
            shift += current_n;
            current_n = make_multiple(current_n / TILE, TILE);
        }
        for (size_t i = sizes.size() - 1; i >= 1; --i) {
            shift -= sizes[i - 1];
            global_work_size = { (realization == 3 ? sizes[i - 1] / 2 : sizes[i - 1]) };
            execute_events.push_back(nullptr);
            clCheckError(
                clSetKernelArg(back_kernel.data(), 1, sizeof(cl_uint), &sizes[i - 1]),
                "clSetKernelArg 1 in scanner_bask"
            );
            clCheckError(
                clSetKernelArg(back_kernel.data(), 2, sizeof(cl_uint), &shift),
                "clSetKernelArg 2 in scanner_bask"
            );
            clCheckError(
                clEnqueueNDRangeKernel(commandQueue.data(),
                    back_kernel.data(),
                    1,
                    nullptr,
                    global_work_size.data(),
                    local_size.data(),
                    0,
                    nullptr,
                    &execute_events.back()
                ),
                "clEnqueueNDRangeKernel!"
            );
            clFinish(commandQueue.data());
        }
    }
};

