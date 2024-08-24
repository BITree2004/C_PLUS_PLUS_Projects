	.text
	.amdgcn_target "amdgcn-amd-amdhsa--gfx1010"
	.protected	mul             ; -- Begin function mul
	.globl	mul
	.p2align	8
	.type	mul,@function
mul:                                    ; @mul
; %bb.0:
	v_mov_b32_e32 v2, s4
	v_mov_b32_e32 v3, s5
	s_load_dword s4, s[4:5], 0x4
	s_load_dwordx4 s[0:3], s[6:7], 0x10
	s_waitcnt lgkmcnt(0)
	s_load_dwordx2 s[2:3], s[6:7], 0x18
	s_load_dwordx4 s[12:15], s[6:7], 0x28
	global_load_ushort v3, v[2:3], off offset:6
	v_mov_b32_e32 v4, 0
	s_mov_b32 s11, s10
	s_mov_b32 s5, 0
                                        ; implicit-def: $vcc_hi
	s_mov_b32 s32, s11
	s_and_b32 s4, s4, 0xffff
	s_waitcnt lgkmcnt(0)
	s_cmp_lg_u32 s3, 0
	s_mul_i32 s8, s8, s4
	v_add_nc_u32_e32 v2, s8, v0
	s_waitcnt vmcnt(0)
	v_mul_lo_u32 v3, s9, v3
	v_add_nc_u32_e32 v3, v3, v1
	v_add_co_u32_e64 v3, vcc_lo, s14, v3
	v_add_co_ci_u32_e32 v4, vcc_lo, s15, v4, vcc_lo
	s_cbranch_scc0 BB0_3
; %bb.1:
	v_mad_u64_u32 v[11:12], s4, v1, s2, s[12:13]
	v_mul_lo_u32 v8, v4, s3
	v_mul_hi_u32 v9, v3, s3
	v_mul_lo_u32 v10, v3, s3
	s_load_dwordx4 s[28:31], s[6:7], 0x0
	v_lshlrev_b32_e32 v14, 6, v1
	s_mov_b32 s4, s3
	s_mov_b32 s3, s5
	v_lshlrev_b32_e32 v5, 2, v0
	s_mov_b64 s[8:9], 0
	v_add_nc_u32_e32 v43, 0x400, v14
	s_lshl_b64 s[6:7], s[2:3], 6
	v_add_co_u32_e64 v6, vcc_lo, v11, v2
	v_add_nc_u32_e32 v1, v9, v8
	v_add_co_ci_u32_e32 v7, vcc_lo, 0, v12, vcc_lo
	v_add_co_u32_e64 v8, vcc_lo, v10, v0
	v_mov_b32_e32 v0, 0
	v_lshlrev_b64 v[10:11], 2, v[6:7]
	v_add_co_ci_u32_e32 v9, vcc_lo, 0, v1, vcc_lo
	v_add_nc_u32_e32 v6, v14, v5
	v_add_nc_u32_e32 v7, v43, v5
	v_lshlrev_b64 v[12:13], 2, v[8:9]
	s_waitcnt lgkmcnt(0)
	v_add_co_u32_e64 v8, vcc_lo, s30, v10
	v_add_co_ci_u32_e32 v9, vcc_lo, s31, v11, vcc_lo
	v_add_co_u32_e64 v10, vcc_lo, s28, v12
	v_add_co_ci_u32_e32 v11, vcc_lo, s29, v13, vcc_lo
BB0_2:                                  ; =>This Inner Loop Header: Depth=1
	global_load_dword v12, v[10:11], off
	global_load_dword v13, v[8:9], off
	s_add_u32 s8, s8, 16
	s_addc_u32 s9, s9, 0
	v_add_co_u32_e64 v10, vcc_lo, v10, 64
	v_cmp_lt_u64_e64 s3, s[8:9], s[4:5]
	v_add_co_ci_u32_e32 v11, vcc_lo, 0, v11, vcc_lo
	v_add_co_u32_e64 v8, vcc_lo, v8, s6
	v_add_co_ci_u32_e32 v9, vcc_lo, s7, v9, vcc_lo
	s_and_b32 vcc_lo, exec_lo, s3
	s_waitcnt vmcnt(1)
	ds_write_b32 v7, v12
	s_waitcnt vmcnt(0)
	ds_write_b32 v6, v13
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	ds_read2_b32 v[12:13], v43 offset1:1
	ds_read2_b32 v[14:15], v5 offset1:16
	ds_read2_b32 v[16:17], v43 offset0:2 offset1:3
	ds_read2_b32 v[18:19], v5 offset0:32 offset1:48
	ds_read2_b32 v[20:21], v43 offset0:4 offset1:5
	ds_read2_b32 v[22:23], v5 offset0:64 offset1:80
	ds_read2_b32 v[24:25], v5 offset0:96 offset1:112
	ds_read2_b32 v[26:27], v43 offset0:6 offset1:7
	ds_read2_b32 v[28:29], v43 offset0:8 offset1:9
	ds_read2_b32 v[30:31], v43 offset0:10 offset1:11
	ds_read2_b32 v[34:35], v5 offset0:128 offset1:144
	ds_read2_b32 v[44:45], v5 offset0:160 offset1:176
	ds_read2_b32 v[36:37], v43 offset0:12 offset1:13
	ds_read2_b32 v[38:39], v43 offset0:14 offset1:15
	ds_read2_b32 v[46:47], v5 offset0:192 offset1:208
	ds_read2_b32 v[41:42], v5 offset0:224 offset1:240
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	v_fmac_f32_e32 v0, v12, v14
	v_fmac_f32_e32 v0, v13, v15
	v_fmac_f32_e32 v0, v16, v18
	v_fmac_f32_e32 v0, v17, v19
	v_fmac_f32_e32 v0, v20, v22
	v_fmac_f32_e32 v0, v21, v23
	v_fmac_f32_e32 v0, v26, v24
	v_fmac_f32_e32 v0, v27, v25
	v_fmac_f32_e32 v0, v28, v34
	v_fmac_f32_e32 v0, v29, v35
	v_fmac_f32_e32 v0, v30, v44
	v_fmac_f32_e32 v0, v31, v45
	v_fmac_f32_e32 v0, v36, v46
	v_fmac_f32_e32 v0, v37, v47
	v_fmac_f32_e32 v0, v38, v41
	v_fmac_f32_e32 v0, v39, v42
	s_cbranch_vccnz BB0_2
	s_branch BB0_4
BB0_3:
	v_mov_b32_e32 v0, 0
BB0_4:                                  ; %.loopexit
	v_mul_lo_u32 v1, s2, v4
	v_mul_hi_u32 v4, s2, v3
	v_mul_lo_u32 v3, s2, v3
	v_mov_b32_e32 v5, 0
	v_add_co_u32_e64 v2, vcc_lo, s12, v2
	v_add_co_ci_u32_e32 v5, vcc_lo, s13, v5, vcc_lo
	v_add_nc_u32_e32 v4, v4, v1
	v_add_co_u32_e64 v1, vcc_lo, v3, v2
	v_add_co_ci_u32_e32 v2, vcc_lo, v4, v5, vcc_lo
	v_lshlrev_b64 v[1:2], 2, v[1:2]
	v_add_co_u32_e64 v1, vcc_lo, s0, v1
	v_add_co_ci_u32_e32 v2, vcc_lo, s1, v2, vcc_lo
	global_store_dword v[1:2], v0, off
	s_endpgm
	.section	.rodata,#alloc
	.p2align	6
	.amdhsa_kernel mul
		.amdhsa_group_segment_fixed_size 2048
		.amdhsa_private_segment_fixed_size 0
		.amdhsa_user_sgpr_private_segment_buffer 1
		.amdhsa_user_sgpr_dispatch_ptr 1
		.amdhsa_user_sgpr_queue_ptr 0
		.amdhsa_user_sgpr_kernarg_segment_ptr 1
		.amdhsa_user_sgpr_dispatch_id 0
		.amdhsa_user_sgpr_flat_scratch_init 0
		.amdhsa_user_sgpr_private_segment_size 0
		.amdhsa_wavefront_size32 1
		.amdhsa_system_sgpr_private_segment_wavefront_offset 0
		.amdhsa_system_sgpr_workgroup_id_x 1
		.amdhsa_system_sgpr_workgroup_id_y 1
		.amdhsa_system_sgpr_workgroup_id_z 0
		.amdhsa_system_sgpr_workgroup_info 0
		.amdhsa_system_vgpr_workitem_id 1
		.amdhsa_next_free_vgpr 48
		.amdhsa_next_free_sgpr 33
		.amdhsa_reserve_flat_scratch 0
		.amdhsa_float_round_mode_32 0
		.amdhsa_float_round_mode_16_64 0
		.amdhsa_float_denorm_mode_32 3
		.amdhsa_float_denorm_mode_16_64 3
		.amdhsa_dx10_clamp 1
		.amdhsa_ieee_mode 1
		.amdhsa_fp16_overflow 0
		.amdhsa_workgroup_processor_mode 1
		.amdhsa_memory_ordered 1
		.amdhsa_forward_progress 0
		.amdhsa_exception_fp_ieee_invalid_op 0
		.amdhsa_exception_fp_denorm_src 0
		.amdhsa_exception_fp_ieee_div_zero 0
		.amdhsa_exception_fp_ieee_overflow 0
		.amdhsa_exception_fp_ieee_underflow 0
		.amdhsa_exception_fp_ieee_inexact 0
		.amdhsa_exception_int_div_zero 0
	.end_amdhsa_kernel
	.text
.Lfunc_end0:
	.size	mul, .Lfunc_end0-mul
                                        ; -- End function
	.section	.AMDGPU.csdata
; Kernel info:
; codeLenInByte = 712
; NumSgprs: 35
; NumVgprs: 48
; ScratchSize: 0
; MemoryBound: 0
; FloatMode: 240
; IeeeMode: 1
; LDSByteSize: 2048 bytes/workgroup (compile time only)
; SGPRBlocks: 4
; VGPRBlocks: 5
; NumSGPRsForWavesPerEU: 35
; NumVGPRsForWavesPerEU: 48
; Occupancy: 20
; WaveLimiterHint : 1
; COMPUTE_PGM_RSRC2:USER_SGPR: 8
; COMPUTE_PGM_RSRC2:TRAP_HANDLER: 0
; COMPUTE_PGM_RSRC2:TGID_X_EN: 1
; COMPUTE_PGM_RSRC2:TGID_Y_EN: 1
; COMPUTE_PGM_RSRC2:TGID_Z_EN: 0
; COMPUTE_PGM_RSRC2:TIDIG_COMP_CNT: 1
	.text
	.p2alignl 6, 3214868480
	.fill 48, 4, 3214868480

	.ident	"clang version 8.0 "
	.section	".note.GNU-stack"
	.addrsig
	.amdgpu_metadata
---
amdhsa.kernels:
  - .args:
      - .address_space:  global
        .is_const:       true
        .name:           A
        .offset:         0
        .size:           8
        .type_name:      'float*'
        .value_kind:     global_buffer
        .value_type:     f32
      - .address_space:  global
        .is_const:       true
        .name:           B
        .offset:         8
        .size:           8
        .type_name:      'float*'
        .value_kind:     global_buffer
        .value_type:     f32
      - .address_space:  global
        .name:           c
        .offset:         16
        .size:           8
        .type_name:      'float*'
        .value_kind:     global_buffer
        .value_type:     f32
      - .name:           N
        .offset:         24
        .size:           4
        .type_name:      uint
        .value_kind:     by_value
        .value_type:     u32
      - .name:           K
        .offset:         28
        .size:           4
        .type_name:      uint
        .value_kind:     by_value
        .value_type:     u32
      - .name:           M
        .offset:         32
        .size:           4
        .type_name:      uint
        .value_kind:     by_value
        .value_type:     u32
      - .offset:         40
        .size:           8
        .value_kind:     hidden_global_offset_x
        .value_type:     i64
      - .offset:         48
        .size:           8
        .value_kind:     hidden_global_offset_y
        .value_type:     i64
      - .offset:         56
        .size:           8
        .value_kind:     hidden_global_offset_z
        .value_type:     i64
      - .address_space:  global
        .offset:         64
        .size:           8
        .value_kind:     hidden_none
        .value_type:     i8
      - .address_space:  global
        .offset:         72
        .size:           8
        .value_kind:     hidden_none
        .value_type:     i8
      - .address_space:  global
        .offset:         80
        .size:           8
        .value_kind:     hidden_none
        .value_type:     i8
      - .address_space:  global
        .offset:         88
        .size:           8
        .value_kind:     hidden_multigrid_sync_arg
        .value_type:     i8
    .group_segment_fixed_size: 2048
    .kernarg_segment_align: 8
    .kernarg_segment_size: 96
    .language:       OpenCL C
    .language_version:
      - 1
      - 2
    .max_flat_workgroup_size: 256
    .name:           mul
    .private_segment_fixed_size: 0
    .sgpr_count:     35
    .sgpr_spill_count: 0
    .symbol:         mul.kd
    .vgpr_count:     48
    .vgpr_spill_count: 0
    .wavefront_size: 32
amdhsa.version:
  - 1
  - 0
...

	.end_amdgpu_metadata
