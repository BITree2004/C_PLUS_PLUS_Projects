kernel void scanner(
    global float *a,
    global float *b,
    const uint n,
    const uint shift
    ) {
    const uint x = get_local_id(0);
    const uint group_x = get_group_id(0);
    local float odd[TILE];
    local float even[TILE];
    local float* res;
    if (HIGH & 1) {
        res = odd;
    } else {
        res = even;
    }

    uint global_x = shift + group_x * TILE + x;
    even[x] = a[global_x];

    barrier(CLK_LOCAL_MEM_FENCE);
    uint d = 1;
    for (uint i = 0; i < HIGH; i++) {
        local const float* actual;
        local float* next;
        if (i & 1) {
            actual = odd;
            next = even;
        } else {
            actual = even;
            next = odd;
        }
        next[x] = actual[x] + ((x < d) ? 0 : actual[x - d]);
        d <<= 1;
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    b[global_x] = res[x];
    if (n > TILE && x == 0) {
        a[shift + n + group_x] = res[PRE_TILE];
    }
}

kernel void scanner_back(
    global float *b,
    uint n,
    const uint shift
    ) {
    const uint x = get_local_id(0);
    const uint group_x = get_group_id(0);
    if (group_x != 0){
        b[shift + group_x * TILE + x] += b[shift + n + group_x - 1];
    }
}
