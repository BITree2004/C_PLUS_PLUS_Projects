# 1 "C:\\Users\\OCL_US~1\\AppData\\Local\\Temp\\comgr-40beec\\input\\CompileCLSource"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 372 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Users\\OCL_US~1\\AppData\\Local\\Temp\\comgr-40beec\\input\\CompileCLSource" 2
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

        }
        c[y*N+x]=sum;
}
