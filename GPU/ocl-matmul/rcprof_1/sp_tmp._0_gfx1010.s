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
	s_mov_b32 s11, s10
	v_mov_b32_e32 v2, 0
	v_mov_b32_e32 v14, 0
                                        ; implicit-def: $vcc_hi
	s_mov_b32 s32, s11
	s_and_b32 s4, s4, 0xffff
	s_waitcnt lgkmcnt(0)
	s_cmp_eq_u32 s3, 0
	s_mul_i32 s8, s8, s4
	v_add3_u32 v0, s12, s8, v0
	s_waitcnt vmcnt(0)
	v_mul_lo_u32 v3, s9, v3
	v_add3_u32 v1, s14, v3, v1
	s_cbranch_scc1 BB0_3
; %bb.1:
	s_load_dwordx4 s[4:7], s[6:7], 0x0
	v_mul_lo_u32 v4, v1, s3
	v_mov_b32_e32 v5, 0
	v_mov_b32_e32 v6, v0
	v_mov_b32_e32 v14, v5
BB0_2:                                  ; =>This Inner Loop Header: Depth=1
	v_lshlrev_b64 v[8:9], 2, v[4:5]
	v_mov_b32_e32 v7, v5
	s_add_i32 s3, s3, -1
	v_add_nc_u32_e32 v4, 1, v4
	v_lshlrev_b64 v[10:11], 2, v[6:7]
	v_add_nc_u32_e32 v6, s2, v6
	s_cmp_eq_u32 s3, 0
	s_waitcnt lgkmcnt(0)
	v_add_co_u32_e64 v7, vcc_lo, s4, v8
	v_add_co_ci_u32_e32 v8, vcc_lo, s5, v9, vcc_lo
	v_add_co_u32_e64 v9, vcc_lo, s6, v10
	v_add_co_ci_u32_e32 v10, vcc_lo, s7, v11, vcc_lo
	global_load_dword v9, v[9:10], off
	global_load_dword v7, v[7:8], off
	s_waitcnt vmcnt(0)
	v_fmac_f32_e32 v14, v7, v9
	s_cbranch_scc0 BB0_2
BB0_3:                                  ; %.loopexit
	v_mul_lo_u32 v1, v1, s2
	v_add_nc_u32_e32 v1, v1, v0
	v_lshlrev_b64 v[0:1], 2, v[1:2]
	v_add_co_u32_e64 v0, vcc_lo, s0, v0
	v_add_co_ci_u32_e32 v1, vcc_lo, s1, v1, vcc_lo
	global_store_dword v[0:1], v14, off
	s_endpgm
	.section	.rodata,#alloc
	.p2align	6
	.amdhsa_kernel mul
		.amdhsa_group_segment_fixed_size 0
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
		.amdhsa_next_free_vgpr 15
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
; codeLenInByte = 284
; NumSgprs: 35
; NumVgprs: 15
; ScratchSize: 0
; MemoryBound: 0
; FloatMode: 240
; IeeeMode: 1
; LDSByteSize: 0 bytes/workgroup (compile time only)
; SGPRBlocks: 4
; VGPRBlocks: 1
; NumSGPRsForWavesPerEU: 35
; NumVGPRsForWavesPerEU: 15
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
    .group_segment_fixed_size: 0
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
    .vgpr_count:     15
    .vgpr_spill_count: 0
    .wavefront_size: 32
amdhsa.version:
  - 1
  - 0
...

	.end_amdgpu_metadata
