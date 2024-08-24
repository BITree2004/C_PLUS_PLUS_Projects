	.text
	.amdgcn_target "amdgcn-amd-amdhsa--gfx1010"
	.protected	scanner         ; -- Begin function scanner
	.globl	scanner
	.p2align	8
	.type	scanner,@function
scanner:                                ; @scanner
; %bb.0:
	s_load_dwordx4 s[28:31], s[4:5], 0x0
	s_load_dwordx2 s[12:13], s[4:5], 0x10
	s_mov_b32 s8, s7
	s_mov_b32 s7, 0
	v_lshrrev_b32_e32 v2, 6, v0
	v_mov_b32_e32 v3, 0
	v_or_b32_e32 v1, 0x100, v0
	v_lshlrev_b32_e32 v6, 1, v0
	s_lshl_b64 s[0:1], s[6:7], 9
	v_add_lshl_u32 v8, v2, v0, 2
	v_lshrrev_b32_e32 v4, 5, v0
	v_lshrrev_b32_e32 v7, 6, v1
	s_mov_b32 s32, s8
                                        ; implicit-def: $vcc_hi
	v_add_lshl_u32 v1, v6, v4, 2
	s_waitcnt lgkmcnt(0)
	s_add_u32 s0, s13, s0
	s_addc_u32 s1, 0, s1
	s_mov_b32 s2, s13
	v_add_co_u32_e64 v2, vcc_lo, s0, v0
	v_add_co_ci_u32_e32 v3, vcc_lo, s1, v3, vcc_lo
	v_lshlrev_b64 v[4:5], 2, v[2:3]
	v_add_co_u32_e64 v4, vcc_lo, s28, v4
	v_add_co_ci_u32_e32 v5, vcc_lo, s29, v5, vcc_lo
	v_cmp_gt_u32_e32 vcc_lo, 0x80, v0
	global_load_dword v9, v[4:5], off
	global_load_dword v4, v[4:5], off offset:1024
	v_add_lshl_u32 v5, v7, v0, 2
	s_waitcnt vmcnt(1)
	ds_write_b32 v8, v9
	s_waitcnt vmcnt(0)
	ds_write_b32 v5, v4 offset:1024
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	ds_read2_b32 v[4:5], v1 offset1:1
	s_waitcnt lgkmcnt(0)
	v_add_f32_e32 v7, v4, v5
	v_or_b32_e32 v4, 1, v6
	v_add_nc_u32_e32 v5, 2, v6
	ds_write_b32 v1, v7 offset:4
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	s_and_saveexec_b32 s0, vcc_lo
	; mask branch BB0_2
	s_cbranch_execz BB0_2
BB0_1:
	v_lshl_add_u32 v6, v4, 1, -1
	v_lshl_add_u32 v7, v5, 1, -1
	v_lshrrev_b32_e32 v8, 6, v6
	v_lshrrev_b32_e32 v9, 6, v7
	v_add_lshl_u32 v6, v8, v6, 2
	v_add_lshl_u32 v7, v9, v7, 2
	s_waitcnt_vscnt null, 0x0
	ds_read_b32 v6, v6
	ds_read_b32 v8, v7
	s_waitcnt lgkmcnt(0)
	v_add_f32_e32 v6, v6, v8
	ds_write_b32 v7, v6
BB0_2:
	v_nop
	s_or_b32 exec_lo, exec_lo, s0
	v_cmp_gt_u32_e64 s0, 64, v0
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	s_and_saveexec_b32 s1, s0
	; mask branch BB0_4
	s_cbranch_execz BB0_4
BB0_3:
	v_lshl_add_u32 v6, v4, 2, -1
	v_lshl_add_u32 v7, v5, 2, -1
	v_lshrrev_b32_e32 v8, 6, v6
	v_lshrrev_b32_e32 v9, 6, v7
	v_add_lshl_u32 v6, v8, v6, 2
	v_add_lshl_u32 v7, v9, v7, 2
	s_waitcnt_vscnt null, 0x0
	ds_read_b32 v6, v6
	ds_read_b32 v8, v7
	s_waitcnt lgkmcnt(0)
	v_add_f32_e32 v6, v6, v8
	ds_write_b32 v7, v6
BB0_4:
	v_nop
	s_or_b32 exec_lo, exec_lo, s1
	v_cmp_gt_u32_e64 s1, 32, v0
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	s_and_saveexec_b32 s2, s1
	; mask branch BB0_6
	s_cbranch_execz BB0_6
BB0_5:
	v_lshl_add_u32 v6, v4, 3, -1
	v_lshl_add_u32 v7, v5, 3, -1
	v_lshrrev_b32_e32 v8, 6, v6
	v_lshrrev_b32_e32 v9, 6, v7
	v_add_lshl_u32 v6, v8, v6, 2
	v_add_lshl_u32 v7, v9, v7, 2
	s_waitcnt_vscnt null, 0x0
	ds_read_b32 v6, v6
	ds_read_b32 v8, v7
	s_waitcnt lgkmcnt(0)
	v_add_f32_e32 v6, v6, v8
	ds_write_b32 v7, v6
BB0_6:
	v_nop
	s_or_b32 exec_lo, exec_lo, s2
	v_cmp_gt_u32_e64 s2, 16, v0
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	s_and_saveexec_b32 s3, s2
	; mask branch BB0_8
	s_cbranch_execz BB0_8
BB0_7:
	v_lshl_add_u32 v6, v4, 4, -1
	v_lshl_add_u32 v7, v5, 4, -1
	v_lshrrev_b32_e32 v8, 6, v6
	v_lshrrev_b32_e32 v9, 6, v7
	v_add_lshl_u32 v6, v8, v6, 2
	v_add_lshl_u32 v7, v9, v7, 2
	s_waitcnt_vscnt null, 0x0
	ds_read_b32 v6, v6
	ds_read_b32 v8, v7
	s_waitcnt lgkmcnt(0)
	v_add_f32_e32 v6, v6, v8
	ds_write_b32 v7, v6
BB0_8:
	v_nop
	s_or_b32 exec_lo, exec_lo, s3
	v_cmp_gt_u32_e64 s3, 8, v0
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	s_and_saveexec_b32 s4, s3
	; mask branch BB0_10
	s_cbranch_execz BB0_10
BB0_9:
	v_lshl_add_u32 v6, v4, 5, -1
	v_lshl_add_u32 v7, v5, 5, -1
	v_lshrrev_b32_e32 v8, 6, v6
	v_lshrrev_b32_e32 v9, 6, v7
	v_add_lshl_u32 v6, v8, v6, 2
	v_add_lshl_u32 v7, v9, v7, 2
	s_waitcnt_vscnt null, 0x0
	ds_read_b32 v6, v6
	ds_read_b32 v8, v7
	s_waitcnt lgkmcnt(0)
	v_add_f32_e32 v6, v6, v8
	ds_write_b32 v7, v6
BB0_10:
	v_nop
	s_or_b32 exec_lo, exec_lo, s4
	v_cmp_gt_u32_e64 s14, 4, v0
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	s_and_saveexec_b32 s4, s14
	; mask branch BB0_12
	s_cbranch_execz BB0_12
BB0_11:
	v_lshl_add_u32 v6, v4, 6, -1
	v_lshl_add_u32 v7, v5, 6, -1
	v_lshrrev_b32_e32 v8, 6, v6
	v_lshrrev_b32_e32 v9, 6, v7
	v_add_lshl_u32 v6, v8, v6, 2
	v_add_lshl_u32 v7, v9, v7, 2
	s_waitcnt_vscnt null, 0x0
	ds_read_b32 v6, v6
	ds_read_b32 v8, v7
	s_waitcnt lgkmcnt(0)
	v_add_f32_e32 v6, v6, v8
	ds_write_b32 v7, v6
BB0_12:
	v_nop
	s_or_b32 exec_lo, exec_lo, s4
	v_cmp_gt_u32_e64 s4, 2, v0
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	s_and_saveexec_b32 s5, s4
	; mask branch BB0_14
	s_cbranch_execz BB0_14
BB0_13:
	v_lshl_add_u32 v6, v4, 7, -1
	v_lshl_add_u32 v7, v5, 7, -1
	v_lshrrev_b32_e32 v8, 6, v6
	v_lshrrev_b32_e32 v9, 6, v7
	v_add_lshl_u32 v6, v8, v6, 2
	v_add_lshl_u32 v7, v9, v7, 2
	s_waitcnt_vscnt null, 0x0
	ds_read_b32 v6, v6
	ds_read_b32 v8, v7
	s_waitcnt lgkmcnt(0)
	v_add_f32_e32 v6, v6, v8
	ds_write_b32 v7, v6
BB0_14:
	v_nop
	s_or_b32 exec_lo, exec_lo, s5
	s_mov_b32 s5, 0
	v_cmp_eq_u32_e64 s15, 0, v0
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	s_and_saveexec_b32 s16, s15
	s_xor_b32 s16, exec_lo, s16
	; mask branch BB0_16
	s_cbranch_execz BB0_16
BB0_15:
	v_lshl_add_u32 v6, v4, 8, -1
	v_lshl_add_u32 v7, v5, 8, -1
	s_mov_b32 s5, exec_lo
	v_lshrrev_b32_e32 v8, 6, v6
	v_lshrrev_b32_e32 v9, 6, v7
	v_add_lshl_u32 v6, v8, v6, 2
	v_add_lshl_u32 v7, v9, v7, 2
	s_waitcnt_vscnt null, 0x0
	ds_read_b32 v6, v6
	ds_read_b32 v8, v7
	s_waitcnt lgkmcnt(0)
	v_add_f32_e32 v6, v6, v8
	v_mov_b32_e32 v8, 0
	ds_write_b32 v7, v6
	ds_read_b32 v6, v8 offset:2072
	v_add_nc_u32_e64 v7, 0x818, 0
	s_waitcnt lgkmcnt(0)
	ds_write2_b32 v7, v8, v6 offset1:2
BB0_16:
	v_nop
	s_or_b32 exec_lo, exec_lo, s16
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	s_and_saveexec_b32 s16, s15
	; mask branch BB0_18
	s_cbranch_execz BB0_18
BB0_17:
	v_lshl_add_u32 v6, v4, 8, -1
	v_lshl_add_u32 v7, v5, 8, -1
	v_lshrrev_b32_e32 v8, 6, v6
	v_lshrrev_b32_e32 v9, 6, v7
	v_add_lshl_u32 v6, v8, v6, 2
	v_add_lshl_u32 v7, v9, v7, 2
	s_waitcnt_vscnt null, 0x0
	ds_read_b32 v8, v6
	ds_read_b32 v9, v7
	s_waitcnt lgkmcnt(0)
	v_add_f32_e32 v8, v8, v9
	ds_write_b32 v6, v9
	ds_write_b32 v7, v8
BB0_18:
	v_nop
	s_or_b32 exec_lo, exec_lo, s16
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	s_and_saveexec_b32 s15, s4
	; mask branch BB0_20
	s_cbranch_execz BB0_20
BB0_19:
	v_lshl_add_u32 v6, v4, 7, -1
	v_lshl_add_u32 v7, v5, 7, -1
	v_lshrrev_b32_e32 v8, 6, v6
	v_lshrrev_b32_e32 v9, 6, v7
	v_add_lshl_u32 v6, v8, v6, 2
	v_add_lshl_u32 v7, v9, v7, 2
	s_waitcnt_vscnt null, 0x0
	ds_read_b32 v8, v6
	ds_read_b32 v9, v7
	s_waitcnt lgkmcnt(0)
	v_add_f32_e32 v8, v8, v9
	ds_write_b32 v6, v9
	ds_write_b32 v7, v8
BB0_20:
	v_nop
	s_or_b32 exec_lo, exec_lo, s15
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	s_and_saveexec_b32 s4, s14
	; mask branch BB0_22
	s_cbranch_execz BB0_22
BB0_21:
	v_lshl_add_u32 v6, v4, 6, -1
	v_lshl_add_u32 v7, v5, 6, -1
	v_lshrrev_b32_e32 v8, 6, v6
	v_lshrrev_b32_e32 v9, 6, v7
	v_add_lshl_u32 v6, v8, v6, 2
	v_add_lshl_u32 v7, v9, v7, 2
	s_waitcnt_vscnt null, 0x0
	ds_read_b32 v8, v6
	ds_read_b32 v9, v7
	s_waitcnt lgkmcnt(0)
	v_add_f32_e32 v8, v8, v9
	ds_write_b32 v6, v9
	ds_write_b32 v7, v8
BB0_22:
	v_nop
	s_or_b32 exec_lo, exec_lo, s4
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	s_and_saveexec_b32 s4, s3
	; mask branch BB0_24
	s_cbranch_execz BB0_24
BB0_23:
	v_lshl_add_u32 v6, v5, 5, -1
	v_lshl_add_u32 v8, v4, 5, -1
	v_lshrrev_b32_e32 v7, 6, v6
	v_lshrrev_b32_e32 v9, 6, v8
	v_add_lshl_u32 v6, v7, v6, 2
	v_add_lshl_u32 v8, v9, v8, 2
	s_waitcnt_vscnt null, 0x0
	ds_read_b32 v7, v6
	ds_read_b32 v9, v8
	s_waitcnt lgkmcnt(1)
	ds_write_b32 v8, v7
	ds_read_b32 v7, v6
	s_waitcnt lgkmcnt(0)
	v_add_f32_e32 v7, v9, v7
	ds_write_b32 v6, v7
BB0_24:
	v_nop
	s_or_b32 exec_lo, exec_lo, s4
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	s_and_saveexec_b32 s3, s2
	; mask branch BB0_26
	s_cbranch_execz BB0_26
BB0_25:
	v_lshl_add_u32 v6, v5, 4, -1
	v_lshl_add_u32 v8, v4, 4, -1
	v_lshrrev_b32_e32 v7, 6, v6
	v_lshrrev_b32_e32 v9, 6, v8
	v_add_lshl_u32 v6, v7, v6, 2
	v_add_lshl_u32 v8, v9, v8, 2
	s_waitcnt_vscnt null, 0x0
	ds_read_b32 v7, v6
	ds_read_b32 v9, v8
	s_waitcnt lgkmcnt(1)
	ds_write_b32 v8, v7
	ds_read_b32 v7, v6
	s_waitcnt lgkmcnt(0)
	v_add_f32_e32 v7, v9, v7
	ds_write_b32 v6, v7
BB0_26:
	v_nop
	s_or_b32 exec_lo, exec_lo, s3
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	s_and_saveexec_b32 s2, s1
	; mask branch BB0_28
	s_cbranch_execz BB0_28
BB0_27:
	v_lshl_add_u32 v6, v5, 3, -1
	v_lshl_add_u32 v8, v4, 3, -1
	v_lshrrev_b32_e32 v7, 6, v6
	v_lshrrev_b32_e32 v9, 6, v8
	v_add_lshl_u32 v6, v7, v6, 2
	v_add_lshl_u32 v8, v9, v8, 2
	s_waitcnt_vscnt null, 0x0
	ds_read_b32 v7, v6
	ds_read_b32 v9, v8
	s_waitcnt lgkmcnt(1)
	ds_write_b32 v8, v7
	ds_read_b32 v7, v6
	s_waitcnt lgkmcnt(0)
	v_add_f32_e32 v7, v9, v7
	ds_write_b32 v6, v7
BB0_28:
	v_nop
	s_or_b32 exec_lo, exec_lo, s2
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	s_and_saveexec_b32 s1, s0
	; mask branch BB0_30
	s_cbranch_execz BB0_30
BB0_29:
	v_lshl_add_u32 v6, v5, 2, -1
	v_lshl_add_u32 v8, v4, 2, -1
	v_lshrrev_b32_e32 v7, 6, v6
	v_lshrrev_b32_e32 v9, 6, v8
	v_add_lshl_u32 v6, v7, v6, 2
	v_add_lshl_u32 v8, v9, v8, 2
	s_waitcnt_vscnt null, 0x0
	ds_read_b32 v7, v6
	ds_read_b32 v9, v8
	s_waitcnt lgkmcnt(1)
	ds_write_b32 v8, v7
	ds_read_b32 v7, v6
	s_waitcnt lgkmcnt(0)
	v_add_f32_e32 v7, v9, v7
	ds_write_b32 v6, v7
BB0_30:
	v_nop
	s_or_b32 exec_lo, exec_lo, s1
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	s_and_saveexec_b32 s0, vcc_lo
	; mask branch BB0_32
	s_cbranch_execz BB0_32
BB0_31:
	v_lshl_add_u32 v5, v5, 1, -1
	v_lshl_add_u32 v4, v4, 1, -1
	v_lshrrev_b32_e32 v6, 6, v5
	v_lshrrev_b32_e32 v7, 6, v4
	v_add_lshl_u32 v5, v6, v5, 2
	v_add_lshl_u32 v4, v7, v4, 2
	s_waitcnt_vscnt null, 0x0
	ds_read_b32 v6, v5
	ds_read_b32 v7, v4
	s_waitcnt lgkmcnt(1)
	ds_write_b32 v4, v6
	ds_read_b32 v4, v5
	s_waitcnt lgkmcnt(0)
	v_add_f32_e32 v4, v7, v4
	ds_write_b32 v5, v4
BB0_32:
	v_nop
	s_or_b32 exec_lo, exec_lo, s0
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	ds_read2_b32 v[4:5], v1 offset1:1
	v_add_nc_u32_e32 v6, 1, v0
	v_cmp_lt_u32_e64 s0, 0x200, s12
	v_lshrrev_b32_e32 v6, 6, v6
	s_and_b32 s0, s0, s5
	s_waitcnt lgkmcnt(0)
	v_add_f32_e32 v7, v4, v5
	v_mov_b32_e32 v4, v5
	v_add_lshl_u32 v5, v6, v0, 2
	ds_write2_b32 v1, v4, v7 offset1:1
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	ds_read_b32 v4, v5 offset:4
	v_lshlrev_b64 v[1:2], 2, v[2:3]
	v_add_nc_u32_e32 v3, 0x101, v0
	v_lshrrev_b32_e32 v3, 6, v3
	v_add_co_u32_e64 v1, vcc_lo, s30, v1
	v_add_lshl_u32 v0, v3, v0, 2
	v_add_co_ci_u32_e32 v2, vcc_lo, s31, v2, vcc_lo
	s_waitcnt lgkmcnt(0)
	global_store_dword v[1:2], v4, off
	ds_read_b32 v0, v0 offset:1028
	s_waitcnt lgkmcnt(0)
	global_store_dword v[1:2], v0, off offset:1024
	s_and_saveexec_b32 s1, s0
	s_xor_b32 s1, exec_lo, s1
	; mask branch BB0_34
	s_cbranch_execz BB0_34
BB0_33:
	s_add_i32 s0, s13, s12
	v_mov_b32_e32 v0, 0
	s_add_u32 s0, s0, s6
	s_addc_u32 s1, 0, s7
	s_waitcnt_vscnt null, 0x0
	ds_read_b32 v2, v0 offset:2080
	s_lshl_b64 s[0:1], s[0:1], 2
	s_add_u32 s0, s28, s0
	s_addc_u32 s1, s29, s1
	v_mov_b32_e32 v0, s0
	v_mov_b32_e32 v1, s1
	s_waitcnt lgkmcnt(0)
	global_store_dword v[0:1], v2, off
BB0_34:
	s_endpgm
	.section	.rodata,#alloc
	.p2align	6
	.amdhsa_kernel scanner
		.amdhsa_group_segment_fixed_size 2084
		.amdhsa_private_segment_fixed_size 0
		.amdhsa_user_sgpr_private_segment_buffer 1
		.amdhsa_user_sgpr_dispatch_ptr 0
		.amdhsa_user_sgpr_queue_ptr 0
		.amdhsa_user_sgpr_kernarg_segment_ptr 1
		.amdhsa_user_sgpr_dispatch_id 0
		.amdhsa_user_sgpr_flat_scratch_init 0
		.amdhsa_user_sgpr_private_segment_size 0
		.amdhsa_wavefront_size32 1
		.amdhsa_system_sgpr_private_segment_wavefront_offset 0
		.amdhsa_system_sgpr_workgroup_id_x 1
		.amdhsa_system_sgpr_workgroup_id_y 0
		.amdhsa_system_sgpr_workgroup_id_z 0
		.amdhsa_system_sgpr_workgroup_info 0
		.amdhsa_system_vgpr_workitem_id 0
		.amdhsa_next_free_vgpr 10
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
	.size	scanner, .Lfunc_end0-scanner
                                        ; -- End function
	.section	.AMDGPU.csdata
; Kernel info:
; codeLenInByte = 2660
; NumSgprs: 35
; NumVgprs: 10
; ScratchSize: 0
; MemoryBound: 0
; FloatMode: 240
; IeeeMode: 1
; LDSByteSize: 2084 bytes/workgroup (compile time only)
; SGPRBlocks: 4
; VGPRBlocks: 1
; NumSGPRsForWavesPerEU: 35
; NumVGPRsForWavesPerEU: 10
; Occupancy: 20
; WaveLimiterHint : 1
; COMPUTE_PGM_RSRC2:USER_SGPR: 6
; COMPUTE_PGM_RSRC2:TRAP_HANDLER: 0
; COMPUTE_PGM_RSRC2:TGID_X_EN: 1
; COMPUTE_PGM_RSRC2:TGID_Y_EN: 0
; COMPUTE_PGM_RSRC2:TGID_Z_EN: 0
; COMPUTE_PGM_RSRC2:TIDIG_COMP_CNT: 0
	.text
	.protected	scanner_back    ; -- Begin function scanner_back
	.globl	scanner_back
	.p2align	8
	.type	scanner_back,@function
scanner_back:                           ; @scanner_back
; %bb.0:
	s_mov_b32 s8, s7
	s_cmp_eq_u32 s6, 0
	s_mov_b32 s1, 0
                                        ; implicit-def: $vcc_hi
	s_mov_b32 s32, s8
	s_cbranch_scc1 BB1_2
; %bb.1:
	s_load_dwordx2 s[2:3], s[4:5], 0x0
	s_load_dwordx2 s[4:5], s[4:5], 0x8
	v_mov_b32_e32 v1, 0
	s_waitcnt lgkmcnt(0)
	s_add_i32 s0, s6, s4
	s_add_i32 s0, s0, s5
	s_add_i32 s0, s0, -1
	s_lshl_b64 s[0:1], s[0:1], 2
	s_add_u32 s0, s2, s0
	s_addc_u32 s1, s3, s1
	s_lshl_b32 s4, s6, 9
	v_add3_u32 v0, s5, s4, v0
	s_load_dword s4, s[0:1], 0x0
	v_lshlrev_b64 v[2:3], 2, v[0:1]
	v_add_nc_u32_e32 v0, 0x100, v0
	v_lshlrev_b64 v[0:1], 2, v[0:1]
	v_add_co_u32_e64 v2, vcc_lo, s2, v2
	v_add_co_ci_u32_e32 v3, vcc_lo, s3, v3, vcc_lo
	v_add_co_u32_e64 v0, vcc_lo, s2, v0
	global_load_dword v4, v[2:3], off
	v_add_co_ci_u32_e32 v1, vcc_lo, s3, v1, vcc_lo
	s_waitcnt vmcnt(0) lgkmcnt(0)
	v_add_f32_e32 v4, s4, v4
	global_store_dword v[2:3], v4, off
	global_load_dword v2, v[0:1], off
	s_load_dword s0, s[0:1], 0x0
	s_waitcnt vmcnt(0) lgkmcnt(0)
	v_add_f32_e32 v2, s0, v2
	global_store_dword v[0:1], v2, off
BB1_2:
	s_endpgm
	.section	.rodata,#alloc
	.p2align	6
	.amdhsa_kernel scanner_back
		.amdhsa_group_segment_fixed_size 0
		.amdhsa_private_segment_fixed_size 0
		.amdhsa_user_sgpr_private_segment_buffer 1
		.amdhsa_user_sgpr_dispatch_ptr 0
		.amdhsa_user_sgpr_queue_ptr 0
		.amdhsa_user_sgpr_kernarg_segment_ptr 1
		.amdhsa_user_sgpr_dispatch_id 0
		.amdhsa_user_sgpr_flat_scratch_init 0
		.amdhsa_user_sgpr_private_segment_size 0
		.amdhsa_wavefront_size32 1
		.amdhsa_system_sgpr_private_segment_wavefront_offset 0
		.amdhsa_system_sgpr_workgroup_id_x 1
		.amdhsa_system_sgpr_workgroup_id_y 0
		.amdhsa_system_sgpr_workgroup_id_z 0
		.amdhsa_system_sgpr_workgroup_info 0
		.amdhsa_system_vgpr_workitem_id 0
		.amdhsa_next_free_vgpr 5
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
.Lfunc_end1:
	.size	scanner_back, .Lfunc_end1-scanner_back
                                        ; -- End function
	.section	.AMDGPU.csdata
; Kernel info:
; codeLenInByte = 196
; NumSgprs: 35
; NumVgprs: 5
; ScratchSize: 0
; MemoryBound: 0
; FloatMode: 240
; IeeeMode: 1
; LDSByteSize: 0 bytes/workgroup (compile time only)
; SGPRBlocks: 4
; VGPRBlocks: 0
; NumSGPRsForWavesPerEU: 35
; NumVGPRsForWavesPerEU: 5
; Occupancy: 20
; WaveLimiterHint : 1
; COMPUTE_PGM_RSRC2:USER_SGPR: 6
; COMPUTE_PGM_RSRC2:TRAP_HANDLER: 0
; COMPUTE_PGM_RSRC2:TGID_X_EN: 1
; COMPUTE_PGM_RSRC2:TGID_Y_EN: 0
; COMPUTE_PGM_RSRC2:TGID_Z_EN: 0
; COMPUTE_PGM_RSRC2:TIDIG_COMP_CNT: 0
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
        .name:           a
        .offset:         0
        .size:           8
        .type_name:      'float*'
        .value_kind:     global_buffer
        .value_type:     f32
      - .address_space:  global
        .name:           b
        .offset:         8
        .size:           8
        .type_name:      'float*'
        .value_kind:     global_buffer
        .value_type:     f32
      - .name:           n
        .offset:         16
        .size:           4
        .type_name:      uint
        .value_kind:     by_value
        .value_type:     u32
      - .name:           shift
        .offset:         20
        .size:           4
        .type_name:      uint
        .value_kind:     by_value
        .value_type:     u32
      - .offset:         24
        .size:           8
        .value_kind:     hidden_global_offset_x
        .value_type:     i64
      - .offset:         32
        .size:           8
        .value_kind:     hidden_global_offset_y
        .value_type:     i64
      - .offset:         40
        .size:           8
        .value_kind:     hidden_global_offset_z
        .value_type:     i64
      - .address_space:  global
        .offset:         48
        .size:           8
        .value_kind:     hidden_none
        .value_type:     i8
      - .address_space:  global
        .offset:         56
        .size:           8
        .value_kind:     hidden_none
        .value_type:     i8
      - .address_space:  global
        .offset:         64
        .size:           8
        .value_kind:     hidden_none
        .value_type:     i8
      - .address_space:  global
        .offset:         72
        .size:           8
        .value_kind:     hidden_multigrid_sync_arg
        .value_type:     i8
    .group_segment_fixed_size: 2084
    .kernarg_segment_align: 8
    .kernarg_segment_size: 80
    .language:       OpenCL C
    .language_version:
      - 1
      - 2
    .max_flat_workgroup_size: 256
    .name:           scanner
    .private_segment_fixed_size: 0
    .sgpr_count:     35
    .sgpr_spill_count: 0
    .symbol:         scanner.kd
    .vgpr_count:     10
    .vgpr_spill_count: 0
    .wavefront_size: 32
  - .args:
      - .address_space:  global
        .name:           b
        .offset:         0
        .size:           8
        .type_name:      'float*'
        .value_kind:     global_buffer
        .value_type:     f32
      - .name:           n
        .offset:         8
        .size:           4
        .type_name:      uint
        .value_kind:     by_value
        .value_type:     u32
      - .name:           shift
        .offset:         12
        .size:           4
        .type_name:      uint
        .value_kind:     by_value
        .value_type:     u32
      - .offset:         16
        .size:           8
        .value_kind:     hidden_global_offset_x
        .value_type:     i64
      - .offset:         24
        .size:           8
        .value_kind:     hidden_global_offset_y
        .value_type:     i64
      - .offset:         32
        .size:           8
        .value_kind:     hidden_global_offset_z
        .value_type:     i64
      - .address_space:  global
        .offset:         40
        .size:           8
        .value_kind:     hidden_none
        .value_type:     i8
      - .address_space:  global
        .offset:         48
        .size:           8
        .value_kind:     hidden_none
        .value_type:     i8
      - .address_space:  global
        .offset:         56
        .size:           8
        .value_kind:     hidden_none
        .value_type:     i8
      - .address_space:  global
        .offset:         64
        .size:           8
        .value_kind:     hidden_multigrid_sync_arg
        .value_type:     i8
    .group_segment_fixed_size: 0
    .kernarg_segment_align: 8
    .kernarg_segment_size: 72
    .language:       OpenCL C
    .language_version:
      - 1
      - 2
    .max_flat_workgroup_size: 256
    .name:           scanner_back
    .private_segment_fixed_size: 0
    .sgpr_count:     35
    .sgpr_spill_count: 0
    .symbol:         scanner_back.kd
    .vgpr_count:     5
    .vgpr_spill_count: 0
    .wavefront_size: 32
amdhsa.version:
  - 1
  - 0
...

	.end_amdgpu_metadata
