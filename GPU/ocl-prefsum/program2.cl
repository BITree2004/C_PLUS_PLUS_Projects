#define LOG_BANKS 5

#define GET_INDEX(index) \
    (index + ((index) >> (LOG_BANKS + 1)))

float prescan(
        global const float *a,
        global float *b,
        const uint shift,
        local float* psum,
        const uint x1,
        const uint x2
) {
    uint offset = 1;
    psum[GET_INDEX(x1)] = a[shift + x1];
    psum[GET_INDEX(x2)] = a[shift + x2];
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
    return psum[GET_INDEX(TILE)];
}

kernel void scanner(
    global const float *a,
    global float *b,
    const uint n,
    const uint shift
    ) {
    local float psum[3 * TILE + 1];

    const uint x1 = get_local_id(0);
    const uint x2 = x1 + (TILE / 2);
    float sum = 0.0f;
    float last;

    for (uint k = 0; k < n; k+= TILE){
        last = prescan(
            a,
            b,
            k,
            psum,
            x1,
            x2
        );
        b[k + x1] = sum + psum[GET_INDEX(x1 + 1)];
        b[k + x2] = sum + psum[GET_INDEX(x2 + 1)];
        sum += last;
        barrier(CLK_LOCAL_MEM_FENCE);
    }
}
