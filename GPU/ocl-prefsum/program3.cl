#define GET_INDEX(index) \
    (index + ((index) >> (LOG_BANKS + 1)))

kernel void scanner(
    global float *a,
    global float *b,
    const uint n,
    const uint shift
    ) {
    local float psum[GET_INDEX(TILE) + 1];
    const uint x1 = get_local_id(0);
    uint offset = 1;
    psum[GET_INDEX(x1)] = a[shift + get_group_id(0) * TILE + x1];
    psum[GET_INDEX(x1 + (TILE / 2))] = a[shift + get_group_id(0) * TILE + x1 + (TILE / 2)];
    #pragma unroll
    for (uint d = TILE>>1; d > 0; d >>= 1) {
        barrier(CLK_LOCAL_MEM_FENCE);
        if (x1 < d) {
            uint left_child = GET_INDEX(offset*(2 * x1 + 1) - 1);
            uint right_child = GET_INDEX(offset*(2 * x1 + 2) - 1);
            psum[right_child] += psum[left_child];
        }
        offset *= 2;
    }
    if (x1 == 0) {
        psum[GET_INDEX(TILE)] = psum[GET_INDEX(TILE - 1)];
        psum[GET_INDEX(TILE - 1)] = 0;
    }
    #pragma unroll
    for (uint d = 1; d < TILE; d *= 2) {
        offset >>= 1;
        barrier(CLK_LOCAL_MEM_FENCE);
        if (x1 < d)
        {
            uint left_child = GET_INDEX(offset*(2 * x1 + 1) - 1);
            uint right_child = GET_INDEX(offset*(2 * x1 + 2) - 1);
            float t = psum[left_child];
            psum[left_child] = psum[right_child];
            psum[right_child] += t;
        }
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    b[shift + get_group_id(0) * TILE + x1] = psum[GET_INDEX(x1 + 1)];
    b[shift + get_group_id(0) * TILE + x1 + (TILE / 2)] = psum[GET_INDEX(x1 + (TILE / 2) + 1)];
    if (n > TILE && x1 == 0) {
        a[shift + n + get_group_id(0)] = psum[GET_INDEX(TILE)];
    }
}

kernel void scanner_back(
    global float *b,
    uint n,
    const uint shift
    ) {
    const uint x1 = get_local_id(0);
    const uint x2 = x1 + (TILE / 2);
    const uint group_x = get_group_id(0);
    if (group_x != 0){ // maybe without
        b[shift + group_x * TILE + x1] += b[shift + n + group_x - 1];
        b[shift + group_x * TILE + x2] += b[shift + n + group_x - 1];
    }
}
