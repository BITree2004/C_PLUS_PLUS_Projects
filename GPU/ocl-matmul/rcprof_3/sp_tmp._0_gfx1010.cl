/* Compiler options:
-c -emit-llvm -target amdgcn-amd-amdhsa -x cl -cl-kernel-arg-info -DTILE=128 -DPER_THREAD=8 -DtypeX=float8 -DTILE_K=16 -O3 -D__OPENCL_VERSION__=200 -D__IMAGE_SUPPORT__=1 -Xclang -cl-ext=+cl_khr_fp64,+cl_khr_global_int32_base_atomics,+cl_khr_global_int32_extended_atomics,+cl_khr_local_int32_base_atomics,+cl_khr_local_int32_extended_atomics,+cl_khr_int64_base_atomics,+cl_khr_int64_extended_atomics,+cl_khr_3d_image_writes,+cl_khr_byte_addressable_store,+cl_khr_fp16,+cl_khr_gl_sharing,+cl_khr_gl_depth_images,+cl_amd_device_attribute_query,+cl_amd_media_ops,+cl_amd_media_ops2,+cl_khr_d3d10_sharing,+cl_khr_d3d11_sharing,+cl_khr_dx9_media_sharing,+cl_khr_image2d_from_buffer,+cl_khr_subgroups,+cl_khr_gl_event,+cl_khr_mipmap_image,+cl_khr_mipmap_image_writes,+cl_amd_liquid_flash,+cl_amd_copy_buffer_p2p,+cl_amd_planar_yuv -mllvm -amdgpu-prelink  -include opencl-c.h 
*/

#define TTILE TILE/PER_THREAD
kernel void mul(
        global const float* A,
        global const float* B,
        global float* c,
        const uint N,
        const uint K,
        const uint M
    ) {
    // both transposed array
    local float la[TILE_K][TILE];
    local float lb[TILE][TILE_K + 1]; // note
    const uint y2 = get_local_id(0);
    const uint x2 = get_local_id(1);
    const uint y = TILE * get_group_id(0);
    const uint x = TILE * get_group_id(1);

    union {
        float r[PER_THREAD][PER_THREAD];
        typeX v[PER_THREAD];
    } sum;
    union {
        float r[PER_THREAD];
        typeX v;
    } b;

    #pragma unroll
    for (uint i = 0; i < PER_THREAD; ++i) {
        sum.v[i] = 0;
    }
    // note
    for (uint k = 0; k < K; k += TILE_K) {
        #pragma unroll
        for (uint shift = 0; shift < TILE_K * TILE; shift += TTILE * TTILE) {
            uint index = shift + (x2 * TTILE + y2);
            uint c = index / TILE;
            uint r = index % TILE;
            uint k2 = k + c;
            la[c][r] /* diff banks */ = A[k2 * M /* because transposed A */ + y + r] /* colleas */;
            //la[c][r] /* diff banks */ = A[(y + r) * K + k2]; /* not colleas */
            lb[r][c] /* see lb */ = B[k2 * N + x + r] /* colleas */;
        }
        barrier(CLK_LOCAL_MEM_FENCE);

        #pragma unroll
        for (uint z=0; z < TILE_K; ++z) {
            #pragma unroll
            for (uint j = 0; j < PER_THREAD; ++j) {
                b.r[j] = lb[x2 + j * TTILE][z]; /* in one bank */
            }

            #pragma unroll
            for (uint i = 0; i < PER_THREAD; ++i) {
                uint row = y2 + i * TTILE;
                sum.v[i] += la[z][row] * b.v;
            }
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    #pragma unroll
    for (uint i = 0; i < PER_THREAD; ++i) {
        uint gy = y + y2 + i * TTILE;
        #pragma unroll
        for (uint j = 0; j < PER_THREAD; ++j) {
            uint gx = x + x2 + j * TTILE;
            //c[gx * M + gy] /*coleass*/ = sum.r[i][j]; // note for transposed
            c[gy * N + gx] /*not coleas*/ = sum.r[i][j];
        }
    }
}
