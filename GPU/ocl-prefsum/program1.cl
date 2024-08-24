kernel void scanner(
    global const float *a,
    global float *b,
    const uint n,
    const uint shift
    ) {
    local float odd[TILE];
    local float even[TILE];
    local float* res;
    if (HIGH & 1) {
        res = odd;
    } else {
        res = even;
    }

    const uint x = get_local_id(0);
    float sum = 0.0f;

    for (uint k = 0; k < n; k+= TILE){
        uint global_x = k + x;
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

        b[global_x] = sum + res[x];
        sum += res[PRE_TILE];
        barrier(CLK_LOCAL_MEM_FENCE);
    }
}
