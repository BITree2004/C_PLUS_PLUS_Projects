# 1 "C:\\Users\\OCL_US~1\\AppData\\Local\\Temp\\comgr-d23d0b\\input\\CompileCLSource"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 372 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Users\\OCL_US~1\\AppData\\Local\\Temp\\comgr-d23d0b\\input\\CompileCLSource" 2

kernel void mul(
        global const float* A,
        global const float* B,
        global float* c,
        const uint N,
        const uint K,
        const uint M
    ) {

    local float la[16][128];
    local float lb[128][16 + 1];
    const uint y2 = get_local_id(0);
    const uint x2 = get_local_id(1);
    const uint y = 128 * get_group_id(0);
    const uint x = 128 * get_group_id(1);

    union {
        float r[8][8];
        float8 v[8];
    } sum;
    union {
        float r[8];
        float8 v;
    } b;

#pragma unroll
 for (uint i = 0; i < 8; ++i) {
        sum.v[i] = 0;
    }

    for (uint k = 0; k < K; k += 16) {
#pragma unroll
 for (uint shift = 0; shift < 16 * 128; shift += 128/8 * 128/8) {
            uint index = shift + (x2 * 128/8 + y2);
            uint c = index / 128;
            uint r = index % 128;
            uint k2 = k + c;
            la[c][r] = A[k2 * M + y + r] ;

            lb[r][c] = B[k2 * N + x + r] ;
        }
        barrier(CLK_LOCAL_MEM_FENCE);

#pragma unroll
 for (uint z=0; z < 16; ++z) {
#pragma unroll
 for (uint j = 0; j < 8; ++j) {
                b.r[j] = lb[x2 + j * 128/8][z];
            }

#pragma unroll
 for (uint i = 0; i < 8; ++i) {
                uint row = y2 + i * 128/8;
                sum.v[i] += la[z][row] * b.v;
            }
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }

#pragma unroll
 for (uint i = 0; i < 8; ++i) {
        uint gy = y + y2 + i * 128/8;
#pragma unroll
 for (uint j = 0; j < 8; ++j) {
            uint gx = x + x2 + j * 128/8;

            c[gy * N + gx] = sum.r[i][j];
        }
    }
}
