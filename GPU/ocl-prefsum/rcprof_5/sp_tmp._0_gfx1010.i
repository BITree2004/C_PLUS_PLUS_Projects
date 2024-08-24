# 1 "C:\\Users\\OCL_US~1\\AppData\\Local\\Temp\\comgr-f29532\\input\\CompileCLSource"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 372 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Users\\OCL_US~1\\AppData\\Local\\Temp\\comgr-f29532\\input\\CompileCLSource" 2



kernel void scanner(
    global float *a,
    global float *b,
    const uint n,
    const uint shift
    ) {
    local float psum[(512 + ((512) >> (5 + 1))) + 1];
    const uint x1 = get_local_id(0);
    uint offset = 1;
    psum[(x1 + ((x1) >> (5 + 1)))] = a[shift + get_group_id(0) * 512 + x1];
    psum[(x1 + (512 / 2) + ((x1 + (512 / 2)) >> (5 + 1)))] = a[shift + get_group_id(0) * 512 + x1 + (512 / 2)];
#pragma unroll
 for (uint d = 512>>1; d > 0; d >>= 1) {
        barrier(CLK_LOCAL_MEM_FENCE);
        if (x1 < d) {
            uint left_child = (offset*(2 * x1 + 1) - 1 + ((offset*(2 * x1 + 1) - 1) >> (5 + 1)));
            uint right_child = (offset*(2 * x1 + 2) - 1 + ((offset*(2 * x1 + 2) - 1) >> (5 + 1)));
            psum[right_child] += psum[left_child];
        }
        offset *= 2;
    }
    if (x1 == 0) {
        psum[(512 + ((512) >> (5 + 1)))] = psum[(512 - 1 + ((512 - 1) >> (5 + 1)))];
        psum[(512 - 1 + ((512 - 1) >> (5 + 1)))] = 0;
    }
#pragma unroll
 for (uint d = 1; d < 512; d *= 2) {
        offset >>= 1;
        barrier(CLK_LOCAL_MEM_FENCE);
        if (x1 < d)
        {
            uint left_child = (offset*(2 * x1 + 1) - 1 + ((offset*(2 * x1 + 1) - 1) >> (5 + 1)));
            uint right_child = (offset*(2 * x1 + 2) - 1 + ((offset*(2 * x1 + 2) - 1) >> (5 + 1)));
            float t = psum[left_child];
            psum[left_child] = psum[right_child];
            psum[right_child] += t;
        }
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    b[shift + get_group_id(0) * 512 + x1] = psum[(x1 + 1 + ((x1 + 1) >> (5 + 1)))];
    b[shift + get_group_id(0) * 512 + x1 + (512 / 2)] = psum[(x1 + (512 / 2) + 1 + ((x1 + (512 / 2) + 1) >> (5 + 1)))];
    if (n > 512 && x1 == 0) {
        a[shift + n + get_group_id(0)] = psum[(512 + ((512) >> (5 + 1)))];
    }
}

kernel void scanner_back(
    global float *b,
    uint n,
    const uint shift
    ) {
    const uint x1 = get_local_id(0);
    const uint x2 = x1 + (512 / 2);
    const uint group_x = get_group_id(0);
    if (group_x != 0){
        b[shift + group_x * 512 + x1] += b[shift + n + group_x - 1];
        b[shift + group_x * 512 + x2] += b[shift + n + group_x - 1];
    }
}
