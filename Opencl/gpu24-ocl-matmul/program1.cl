kernel void mul(
    global const float* A,
    global const float* B,
    global float* c,
    const uint N,
    const uint K,
    const uint M
    ) {
        uint x = get_global_id(0);
        uint y = get_global_id(1);
        float sum = 0;
        for (uint i = 0; i < K; i++) {
            sum += A[y * K + i] * B[i * N + x];
            //      [y][i]         [i][x]
        }
        c[y*N+x]=sum;
}