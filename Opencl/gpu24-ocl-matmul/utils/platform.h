#ifdef __APPLE__
#include <OpenCL/cl.h>
#else

#include <CL/cl.h>

#endif

#include <iostream>
#include <vector>
#include <cassert>
#include <format>

using namespace std;

struct platform {
private:
    cl_uint size_of_platforms{};
    vector<cl_platform_id> platforms;

public:
    platform() {
        if (clGetPlatformIDs(
                0,
                nullptr,
                &size_of_platforms
                ) != CL_SUCCESS) {
            throw runtime_error("ERROR with first clGetPlatformIDs");
        }
        for (size_t i = 0; i < size_of_platforms; ++i) {
            platforms.push_back(nullptr);
        }
        if (clGetPlatformIDs(
                size_of_platforms,
                platforms.data(),
                &size_of_platforms
                ) != CL_SUCCESS) {
            throw runtime_error("ERROR with second clGetPlatformIDs");
        }
        assert(size_of_platforms == platforms.size());
    }

    string get_name(size_t i) {
        assert(0 <= i && i < platforms.size());
        size_t size;
        if (clGetPlatformInfo(
                platforms[i],
                CL_PLATFORM_NAME,
                0,
                nullptr,
                &size
                )!= CL_SUCCESS) {
            throw runtime_error("ERROR with first clGetPlatformInfo");
        }
        string res;
        res.resize(size);
        if (clGetPlatformInfo(
                platforms[i],
                CL_PLATFORM_NAME,
                size,
                res.data(),
                &size
                ) != CL_SUCCESS) {
            throw runtime_error("ERROR with first clGetPlatformInfo");
        }
        assert(size == res.size());
        return format("Platform: {}", res);
    }

    size_t size() {
        return platforms.size();
    }

    cl_platform_id operator[](size_t index) {
        return platforms[index];
    };
};
