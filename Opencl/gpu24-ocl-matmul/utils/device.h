#include "platform.h"
#include <string>

struct device {
private:
    struct device_id {
        cl_device_id id;
        size_t number_of_platform;
    };
    vector<device_id> actual;
    platform _platform;

public:
    explicit device(platform _platform, const string &type) : _platform(_platform) {
        vector<device_id> dgpu;
        vector<device_id> igpu;
        vector<device_id> gpu;
        vector<device_id> cpu;
        vector<device_id> all;
        vector<cl_device_id> devices;
        cl_device_type device_type;
        cl_bool is_integrated;
        cl_uint count_devices;
        for (size_t i = 0; i < _platform.size(); ++i) {
            if (clGetDeviceIDs(
                    _platform[i],
                    CL_DEVICE_TYPE_ALL,
                    0,
                    nullptr,
                    &count_devices
            ) != CL_SUCCESS) {
                throw runtime_error(format("ERROR with clGetDeviceIDs {}!", i * 2));
            }
            for (size_t j = 0; j < count_devices; ++j) {
                devices.push_back(nullptr);
            }
            if (clGetDeviceIDs(
                    _platform[i],
                    CL_DEVICE_TYPE_ALL,
                    count_devices,
                    devices.data() + devices.size() - count_devices,
                    &count_devices
            ) != CL_SUCCESS) {
                throw runtime_error(format("ERROR with clGetDeviceIDs {}!", i * 2 + 1));
            }
            // split by group
            for (size_t j = devices.size() - count_devices; j < devices.size(); ++j) {
                if (clGetDeviceInfo(
                        devices[j],
                        CL_DEVICE_TYPE,
                        sizeof(device_type),
                        &device_type,
                        nullptr
                ) != CL_SUCCESS) {
                    throw runtime_error("ERROR with clGetDeviceInfo!");
                }
                all.push_back({devices[j], i});
                if (device_type == CL_DEVICE_TYPE_GPU) {
                    gpu.push_back({devices[j], i});
                    if (clGetDeviceInfo(
                            devices[i],
                            CL_DEVICE_HOST_UNIFIED_MEMORY,
                            sizeof(is_integrated),
                            &is_integrated,
                            nullptr
                    ) != CL_SUCCESS) {
                        throw runtime_error("ERROR with clGetDeviceInfo 2!");
                    }
                    if (is_integrated) {
                        igpu.push_back({devices[j], i});
                    } else {
                        dgpu.push_back({devices[j], i});
                    }
                } else {
                    cpu.push_back({devices[j], i});
                }
            }
        }
        if (type == "dgpu") {
            actual = std::move(dgpu);
        } else if (type == "igpu") {
            actual = std::move(igpu);
        } else if (type == "gpu") {
            actual = std::move(gpu);
        } else if (type == "cpu") {
            actual = std::move(cpu);
        } else if (type == "all") {
            actual = std::move(all);
        } else {
            throw runtime_error(format("Unsupported this --device-type: {} !", type));
        }
        if (actual.empty()) {
            throw runtime_error(format("Doesn't found this --device-type: {} !", type));
        }
    }

    cl_device_id *data(int index) {
        if (index < 0 || index >= actual.size()) {
            index = 0;
        }
        return &actual[index].id;
    }

    string get_name(int i) {
        if (i < 0 || actual.size() <= i) {
            i = 0;
        }
        size_t size;
        if (clGetDeviceInfo(
                actual[i].id,
                CL_DEVICE_NAME,
                0,
                nullptr,
                &size
        ) != CL_SUCCESS) {
            throw runtime_error(format("ERROR with clGetDeviceInfo {}!", i));
        }
        string res;
        res.resize(size);
        if (clGetDeviceInfo(
                actual[i].id,
                CL_DEVICE_NAME,
                size,
                res.data(),
                &size
        ) != CL_SUCCESS) {
            throw runtime_error(format("ERROR with clGetDeviceInfo {}!", i));
        }
        assert(size == res.size());
        return format("Device: {}\t", res) + _platform.get_name(actual[i].number_of_platform);
    }

    cl_device_id& operator[](int index) {
        return actual[index].id;
    };
};
