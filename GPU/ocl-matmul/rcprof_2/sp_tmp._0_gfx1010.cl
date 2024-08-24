/* Compiler options:
-c -emit-llvm -target amdgcn-amd-amdhsa -x cl -cl-kernel-arg-info -DTILE=16 -DPER_THREAD=1 -DtypeX=float1 -DTILE_K=16 -O3 -D__OPENCL_VERSION__=200 -D__IMAGE_SUPPORT__=1 -Xclang -cl-ext=+cl_khr_fp64,+cl_khr_global_int32_base_atomics,+cl_khr_global_int32_extended_atomics,+cl_khr_local_int32_base_atomics,+cl_khr_local_int32_extended_atomics,+cl_khr_int64_base_atomics,+cl_khr_int64_extended_atomics,+cl_khr_3d_image_writes,+cl_khr_byte_addressable_store,+cl_khr_fp16,+cl_khr_gl_sharing,+cl_khr_gl_depth_images,+cl_amd_device_attribute_query,+cl_amd_media_ops,+cl_amd_media_ops2,+cl_khr_d3d10_sharing,+cl_khr_d3d11_sharing,+cl_khr_dx9_media_sharing,+cl_khr_image2d_from_buffer,+cl_khr_subgroups,+cl_khr_gl_event,+cl_khr_mipmap_image,+cl_khr_mipmap_image_writes,+cl_amd_liquid_flash,+cl_amd_copy_buffer_p2p,+cl_amd_planar_yuv -mllvm -amdgpu-prelink  -include opencl-c.h 
*/

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