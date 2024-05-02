kernel void mul(
    global const float* A,
    global const float* B,
    global float* c, const
    const uint N,
    const uint K,
    const uint M
    ) {
        local float la[TILE][TILE];
        local float lb[TILE][TILE];
        const size_t x2 = get_local_id(0); // local column
        const size_t y2 = get_local_id(1); // local row
        const size_t x = get_global_id(0); // global column
        const size_t y = get_global_id(1); // global row
        float sum = 0;
        for (size_t k = 0; k < K; k += TILE) {
            la[y2][x2]/*diff banks*/=A[y*K + (k + x2)]; // colleas
            lb[y2][x2]/*diff banks*/=B[(y2+k)*N+x]; // colleas
            barrier(CLK_LOCAL_MEM_FENCE);
            for (size_t z = 0; z < TILE; z++) {
                sum += la[y2][z] /* in one bank */ * lb[z][x2] /*diff banks*/;
            }
            barrier(CLK_LOCAL_MEM_FENCE);
        }
        c[y*N+x]/*colleas*/=sum;
}