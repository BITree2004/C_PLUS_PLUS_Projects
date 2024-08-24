# 1 "C:\\Users\\OCL_US~1\\AppData\\Local\\Temp\\comgr-f66943\\input\\CompileCLSource"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 372 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Users\\OCL_US~1\\AppData\\Local\\Temp\\comgr-f66943\\input\\CompileCLSource" 2
kernel void mul(
    global const float* A,
    global const float* B,
    global float* c, const
    const uint N,
    const uint K,
    const uint M
    ) {
        local float la[16][16];
        local float lb[16][16];
        const size_t x2 = get_local_id(0);
        const size_t y2 = get_local_id(1);
        const size_t x = get_global_id(0);
        const size_t y = get_global_id(1);
        float sum = 0;
        for (size_t k = 0; k < K; k += 16) {
            la[y2][x2] =A[y*K + (k + x2)];
            lb[y2][x2] =B[(y2+k)*N+x];
            barrier(CLK_LOCAL_MEM_FENCE);
            for (size_t z = 0; z < 16; z++) {
                sum += la[y2][z] * lb[z][x2] ;
            }
            barrier(CLK_LOCAL_MEM_FENCE);
        }
        c[y*N+x] =sum;
}
