	.text
	.amdgcn_target "amdgcn-amd-amdhsa--gfx1010"
	.protected	mul             ; -- Begin function mul
	.globl	mul
	.p2align	8
	.type	mul,@function
mul:                                    ; @mul
; %bb.0:
	s_mov_b32 s20, s8
	s_load_dwordx4 s[8:11], s[4:5], 0x18
	s_load_dwordx4 s[0:3], s[4:5], 0x10
	s_lshl_b32 s6, s6, 7
	s_waitcnt lgkmcnt(0)
	s_lshl_b32 s3, s7, 7
	s_mov_b32 s12, 0
	s_mov_b32 s32, s20
                                        ; implicit-def: $vcc_hi
	s_cmp_lg_u32 s9, 0
	s_cbranch_scc0 BB0_4
; %bb.1:
	v_lshl_add_u32 v2, v1, 4, v0
	s_movk_i32 s7, 0x2200
	s_movk_i32 s2, 0x44
	s_load_dwordx4 s[16:19], s[4:5], 0x0
	v_mov_b32_e32 v99, 0
	v_and_b32_e32 v3, 0x7f, v2
	v_lshrrev_b32_e32 v19, 7, v2
	v_add_nc_u32_e32 v5, 0x100, v2
	v_add_nc_u32_e32 v6, 0x200, v2
	v_add_nc_u32_e32 v7, 0x300, v2
	v_lshlrev_b32_e32 v8, 2, v3
	v_lshlrev_b32_e32 v10, 2, v19
	v_lshrrev_b32_e32 v5, 7, v5
	v_lshrrev_b32_e32 v6, 7, v6
	v_lshrrev_b32_e32 v7, 7, v7
	v_lshl_or_b32 v9, v19, 9, v8
	v_mad_u32_u24 v70, v3, s2, v10
	v_lshl_or_b32 v11, v5, 9, v8
	v_lshlrev_b32_e32 v12, 2, v5
	v_lshl_or_b32 v13, v6, 9, v8
	v_add_nc_u32_e32 v69, s7, v9
	v_add_nc_u32_e32 v9, 0x400, v2
	v_add_nc_u32_e32 v71, s7, v11
	v_mad_u32_u24 v72, v3, s2, v12
	v_lshl_or_b32 v11, v7, 9, v8
	v_lshlrev_b32_e32 v12, 2, v7
	v_lshrrev_b32_e32 v9, 7, v9
	v_add_nc_u32_e32 v73, s7, v13
	v_lshlrev_b32_e32 v10, 2, v6
	v_add_nc_u32_e32 v75, s7, v11
	v_mad_u32_u24 v76, v3, s2, v12
	v_lshl_or_b32 v11, v9, 9, v8
	v_add_nc_u32_e32 v13, 0x500, v2
	v_add_nc_u32_e32 v12, 0x600, v2
	v_add_nc_u32_e32 v2, 0x700, v2
	v_mad_u32_u24 v74, v3, s2, v10
	v_add_nc_u32_e32 v77, s7, v11
	v_lshrrev_b32_e32 v10, 7, v13
	v_lshrrev_b32_e32 v11, 7, v12
	v_lshrrev_b32_e32 v2, 7, v2
	v_mul_lo_u32 v92, s8, v9
	v_mul_lo_u32 v93, s10, v9
	v_mul_lo_u32 v90, s8, v10
	v_mul_lo_u32 v88, s8, v11
	v_mul_lo_u32 v86, s8, v2
	v_mul_lo_u32 v87, s10, v2
	v_mul_lo_u32 v89, s10, v11
	v_mul_lo_u32 v91, s10, v10
	v_mul_lo_u32 v94, s8, v7
	v_mul_lo_u32 v95, s10, v7
	v_mul_lo_u32 v96, s8, v6
	v_mul_lo_u32 v97, s10, v6
	v_mul_lo_u32 v106, s8, v5
	v_mul_lo_u32 v107, s10, v5
	v_mul_lo_u32 v108, s8, v19
	v_mul_lo_u32 v109, s10, v19
	v_lshlrev_b32_e32 v13, 2, v9
	v_lshl_or_b32 v14, v10, 9, v8
	v_lshlrev_b32_e32 v12, 2, v10
	v_lshlrev_b32_e32 v18, 2, v2
	v_mov_b32_e32 v98, v99
	v_mad_u32_u24 v78, v3, s2, v13
	v_lshl_or_b32 v13, v11, 9, v8
	v_add_nc_u32_e32 v79, s7, v14
	v_lshlrev_b32_e32 v14, 2, v11
	v_lshl_or_b32 v8, v2, 9, v8
	v_mul_u32_u24_e32 v85, s2, v1
	v_mov_b32_e32 v100, v99
	v_mov_b32_e32 v101, v99
	v_mov_b32_e32 v102, v99
	v_mov_b32_e32 v103, v99
	v_mov_b32_e32 v104, v99
	v_mov_b32_e32 v105, v99
	v_mov_b32_e32 v26, v98
	v_mov_b32_e32 v34, v98
	v_mov_b32_e32 v42, v98
	v_mov_b32_e32 v50, v98
	v_mov_b32_e32 v59, v98
	v_mad_u32_u24 v84, v3, s2, v18
	v_mov_b32_e32 v18, v98
	v_or_b32_e32 v67, s6, v3
	v_or_b32_e32 v68, s3, v3
	v_mad_u32_u24 v80, v3, s2, v12
	v_mad_u32_u24 v82, v3, s2, v14
	v_add_nc_u32_e32 v81, s7, v13
	v_mov_b32_e32 v10, v98
	v_add_nc_u32_e32 v83, s7, v8
	v_mov_b32_e32 v2, v98
	v_lshl_add_u32 v58, v0, 2, s7
	s_lshl_b32 s2, s8, 4
	s_lshl_b32 s4, s10, 4
	v_add_nc_u32_e32 v110, 0x440, v85
	v_add_nc_u32_e32 v111, 0x448, v85
	v_add_nc_u32_e32 v112, 0x880, v85
	v_add_nc_u32_e32 v113, 0x888, v85
	v_add_nc_u32_e32 v114, 0xcc0, v85
	v_mov_b32_e32 v3, v99
	v_mov_b32_e32 v4, v100
	v_mov_b32_e32 v5, v101
	v_mov_b32_e32 v6, v102
	v_mov_b32_e32 v7, v103
	v_mov_b32_e32 v8, v104
	v_mov_b32_e32 v9, v105
	v_mov_b32_e32 v11, v99
	v_mov_b32_e32 v12, v100
	v_mov_b32_e32 v13, v101
	v_mov_b32_e32 v14, v102
	v_mov_b32_e32 v15, v103
	v_mov_b32_e32 v16, v104
	v_mov_b32_e32 v17, v105
	v_mov_b32_e32 v19, v99
	v_mov_b32_e32 v20, v100
	v_mov_b32_e32 v21, v101
	v_mov_b32_e32 v22, v102
	v_mov_b32_e32 v23, v103
	v_mov_b32_e32 v24, v104
	v_mov_b32_e32 v25, v105
	v_mov_b32_e32 v27, v99
	v_mov_b32_e32 v28, v100
	v_mov_b32_e32 v29, v101
	v_mov_b32_e32 v30, v102
	v_mov_b32_e32 v31, v103
	v_mov_b32_e32 v32, v104
	v_mov_b32_e32 v33, v105
	v_mov_b32_e32 v35, v99
	v_mov_b32_e32 v36, v100
	v_mov_b32_e32 v37, v101
	v_mov_b32_e32 v38, v102
	v_mov_b32_e32 v39, v103
	v_mov_b32_e32 v40, v104
	v_mov_b32_e32 v41, v105
	v_mov_b32_e32 v43, v99
	v_mov_b32_e32 v44, v100
	v_mov_b32_e32 v45, v101
	v_mov_b32_e32 v46, v102
	v_mov_b32_e32 v47, v103
	v_mov_b32_e32 v48, v104
	v_mov_b32_e32 v49, v105
	v_mov_b32_e32 v51, v99
	v_mov_b32_e32 v52, v100
	v_mov_b32_e32 v53, v101
	v_mov_b32_e32 v54, v102
	v_mov_b32_e32 v55, v103
	v_mov_b32_e32 v56, v104
	v_mov_b32_e32 v57, v105
	v_mov_b32_e32 v60, v99
	v_mov_b32_e32 v61, v100
	v_mov_b32_e32 v62, v101
	v_mov_b32_e32 v63, v102
	v_mov_b32_e32 v64, v103
	v_mov_b32_e32 v65, v104
	v_mov_b32_e32 v66, v105
BB0_2:                                  ; =>This Inner Loop Header: Depth=1
	v_add_nc_u32_e32 v116, v108, v68
	v_mov_b32_e32 v117, v99
	v_add_nc_u32_e32 v98, v109, v67
	v_add_nc_u32_e32 v162, v107, v67
	v_mov_b32_e32 v163, v99
	v_add_nc_u32_e32 v164, v106, v68
	v_lshlrev_b64 v[116:117], 2, v[116:117]
	v_lshlrev_b64 v[190:191], 2, v[98:99]
	v_mov_b32_e32 v165, v99
	v_lshlrev_b64 v[162:163], 2, v[162:163]
	v_add_nc_u32_e32 v166, v97, v67
	v_mov_b32_e32 v167, v99
	v_add_nc_u32_e32 v168, v96, v68
	v_lshlrev_b64 v[164:165], 2, v[164:165]
	v_mov_b32_e32 v169, v99
	s_waitcnt lgkmcnt(0)
	v_add_co_u32_e64 v192, vcc_lo, s18, v116
	v_lshlrev_b64 v[166:167], 2, v[166:167]
	v_add_nc_u32_e32 v170, v95, v67
	v_mov_b32_e32 v171, v99
	v_lshlrev_b64 v[168:169], 2, v[168:169]
	v_add_co_ci_u32_e32 v193, vcc_lo, s19, v117, vcc_lo
	v_add_co_u32_e64 v190, vcc_lo, s16, v190
	v_add_nc_u32_e32 v172, v94, v68
	v_lshlrev_b64 v[170:171], 2, v[170:171]
	v_mov_b32_e32 v173, v99
	v_add_nc_u32_e32 v174, v93, v67
	v_add_co_ci_u32_e32 v191, vcc_lo, s17, v191, vcc_lo
	v_add_co_u32_e64 v194, vcc_lo, s16, v162
	v_mov_b32_e32 v175, v99
	v_lshlrev_b64 v[172:173], 2, v[172:173]
	v_add_nc_u32_e32 v176, v92, v68
	v_mov_b32_e32 v177, v99
	v_add_co_ci_u32_e32 v195, vcc_lo, s17, v163, vcc_lo
	v_add_co_u32_e64 v164, vcc_lo, s18, v164
	v_lshlrev_b64 v[174:175], 2, v[174:175]
	v_add_nc_u32_e32 v178, v91, v67
	v_lshlrev_b64 v[176:177], 2, v[176:177]
	v_mov_b32_e32 v179, v99
	v_add_co_ci_u32_e32 v165, vcc_lo, s19, v165, vcc_lo
	v_add_co_u32_e64 v162, vcc_lo, s16, v166
	v_add_nc_u32_e32 v180, v90, v68
	v_mov_b32_e32 v181, v99
	v_lshlrev_b64 v[178:179], 2, v[178:179]
	v_add_nc_u32_e32 v182, v89, v67
	v_add_co_ci_u32_e32 v163, vcc_lo, s17, v167, vcc_lo
	v_add_co_u32_e64 v166, vcc_lo, s18, v168
	v_lshlrev_b64 v[180:181], 2, v[180:181]
	v_mov_b32_e32 v183, v99
	v_add_nc_u32_e32 v184, v88, v68
	v_mov_b32_e32 v185, v99
	v_add_co_ci_u32_e32 v167, vcc_lo, s19, v169, vcc_lo
	v_add_co_u32_e64 v168, vcc_lo, s16, v170
	v_lshlrev_b64 v[182:183], 2, v[182:183]
	v_add_nc_u32_e32 v186, v87, v67
	v_lshlrev_b64 v[184:185], 2, v[184:185]
	v_mov_b32_e32 v187, v99
	v_add_co_ci_u32_e32 v169, vcc_lo, s17, v171, vcc_lo
	v_add_co_u32_e64 v170, vcc_lo, s18, v172
	v_add_nc_u32_e32 v188, v86, v68
	v_mov_b32_e32 v189, v99
	v_lshlrev_b64 v[186:187], 2, v[186:187]
	global_load_dword v168, v[168:169], off
	v_add_co_ci_u32_e32 v171, vcc_lo, s19, v173, vcc_lo
	v_add_co_u32_e64 v172, vcc_lo, s16, v174
	v_lshlrev_b64 v[188:189], 2, v[188:189]
	v_add_nc_u32_e32 v158, 0x1100, v85
	v_add_nc_u32_e32 v159, 0x1540, v85
	global_load_dword v169, v[170:171], off
	v_add_co_ci_u32_e32 v173, vcc_lo, s17, v175, vcc_lo
	v_add_co_u32_e64 v174, vcc_lo, s18, v176
	v_add_nc_u32_e32 v160, 0x1980, v85
	v_add_nc_u32_e32 v161, 0x1dc0, v85
	v_add_nc_u32_e32 v155, 0xcc8, v85
	global_load_dword v170, v[172:173], off
	v_add_co_ci_u32_e32 v175, vcc_lo, s19, v177, vcc_lo
	v_add_co_u32_e64 v176, vcc_lo, s16, v178
	v_add_nc_u32_e32 v157, 0x1988, v85
	v_add_nc_u32_e32 v156, 0x1548, v85
	v_add_nc_u32_e32 v150, 0x1108, v85
	v_add_nc_u32_e32 v151, 0x1dc8, v85
	v_add_co_ci_u32_e32 v177, vcc_lo, s17, v179, vcc_lo
	v_add_co_u32_e64 v178, vcc_lo, s18, v180
	global_load_dword v171, v[174:175], off
	v_add_nc_u32_e32 v152, 0x400, v58
	v_add_nc_u32_e32 v153, 0x450, v85
	v_add_nc_u32_e32 v149, 0x480, v58
	v_add_co_ci_u32_e32 v179, vcc_lo, s19, v181, vcc_lo
	v_add_co_u32_e64 v180, vcc_lo, s16, v182
	global_load_dword v172, v[176:177], off
	v_add_nc_u32_e32 v148, 0x500, v58
	v_add_nc_u32_e32 v154, 0x458, v85
	v_add_nc_u32_e32 v147, 0x580, v58
	v_add_co_ci_u32_e32 v181, vcc_lo, s17, v183, vcc_lo
	v_add_co_u32_e64 v182, vcc_lo, s18, v184
	global_load_dword v173, v[178:179], off
	v_add_nc_u32_e32 v146, 0x600, v58
	v_add_nc_u32_e32 v145, 0x680, v58
	v_add_nc_u32_e32 v144, 0x700, v58
	v_add_co_ci_u32_e32 v183, vcc_lo, s19, v185, vcc_lo
	v_add_co_u32_e64 v184, vcc_lo, s16, v186
	global_load_dword v174, v[180:181], off
	v_add_nc_u32_e32 v143, 0x780, v58
	v_add_nc_u32_e32 v135, 0x890, v85
	v_add_nc_u32_e32 v139, 0xcd0, v85
	v_add_co_ci_u32_e32 v185, vcc_lo, s17, v187, vcc_lo
	v_add_co_u32_e64 v186, vcc_lo, s18, v188
	global_load_dword v188, v[190:191], off
	global_load_dword v190, v[194:195], off
	global_load_dword v191, v[164:165], off
	global_load_dword v175, v[182:183], off
	v_add_co_ci_u32_e32 v187, vcc_lo, s19, v189, vcc_lo
	global_load_dword v189, v[192:193], off
	global_load_dword v192, v[162:163], off
	global_load_dword v193, v[166:167], off
	global_load_dword v176, v[184:185], off
	global_load_dword v177, v[186:187], off
	v_add_nc_u32_e32 v136, 0x1110, v85
	v_add_nc_u32_e32 v137, 0x1550, v85
	v_add_nc_u32_e32 v134, 0x1990, v85
	v_add_nc_u32_e32 v138, 0x1dd0, v85
	v_add_nc_u32_e32 v142, 0x800, v58
	v_add_nc_u32_e32 v131, 0x880, v58
	v_add_nc_u32_e32 v130, 0x900, v58
	v_add_nc_u32_e32 v129, 0x980, v58
	v_add_nc_u32_e32 v128, 0xa00, v58
	v_add_nc_u32_e32 v127, 0xa80, v58
	v_add_nc_u32_e32 v126, 0xb00, v58
	v_add_nc_u32_e32 v125, 0xb80, v58
	v_add_nc_u32_e32 v132, 0x898, v85
	v_add_nc_u32_e32 v122, 0xcd8, v85
	v_add_nc_u32_e32 v133, 0x1558, v85
	v_add_nc_u32_e32 v123, 0x1998, v85
	v_add_nc_u32_e32 v140, 0x1118, v85
	v_add_nc_u32_e32 v141, 0x1dd8, v85
	v_add_nc_u32_e32 v124, 0xc00, v58
	v_add_nc_u32_e32 v121, 0xc80, v58
	v_add_nc_u32_e32 v119, 0xd00, v58
	v_add_nc_u32_e32 v105, 0xe00, v58
	v_add_nc_u32_e32 v115, 0xd80, v58
	v_add_nc_u32_e32 v104, 0xe80, v58
	v_add_nc_u32_e32 v103, 0xf00, v58
	v_add_nc_u32_e32 v102, 0xf80, v58
	v_add_nc_u32_e32 v120, 0x1000, v58
	v_add_nc_u32_e32 v100, 0x460, v85
	v_add_nc_u32_e32 v117, 0x8a0, v85
	v_add_nc_u32_e32 v116, 0xce0, v85
	v_add_nc_u32_e32 v165, 0x1120, v85
	v_add_nc_u32_e32 v163, 0x1560, v85
	v_add_nc_u32_e32 v164, 0x1128, v85
	v_add_nc_u32_e32 v166, 0x19a0, v85
	v_add_nc_u32_e32 v167, 0x1de0, v85
	v_add_nc_u32_e32 v101, 0x468, v85
	v_add_nc_u32_e32 v118, 0x8a8, v85
	v_add_nc_u32_e32 v98, 0xce8, v85
	v_add_nc_u32_e32 v162, 0x19a8, v85
	v_add_nc_u32_e32 v194, 0x1500, v58
	v_add_nc_u32_e32 v195, 0x1580, v58
	v_add_nc_u32_e32 v200, 0x1900, v58
	v_add_nc_u32_e32 v201, 0x1980, v58
	v_add_nc_u32_e32 v202, 0x1a00, v58
	v_add_nc_u32_e32 v203, 0x1a80, v58
	v_add_nc_u32_e32 v204, 0x1b00, v58
	v_add_nc_u32_e32 v205, 0x1b80, v58
	v_add_nc_u32_e32 v206, 0x1c00, v58
	v_add_nc_u32_e32 v207, 0x1df8, v85
	v_add_nc_u32_e32 v208, 0x1c80, v58
	v_add_nc_u32_e32 v209, 0x1d00, v58
	v_add_nc_u32_e32 v210, 0x1d80, v58
	v_add_nc_u32_e32 v211, 0x1e00, v58
	v_add_nc_u32_e32 v212, 0x1e80, v58
	v_add_nc_u32_e32 v213, 0x1f00, v58
	v_add_nc_u32_e32 v214, 0x1f80, v58
	s_add_i32 s12, s12, 16
	v_add_nc_u32_e32 v68, s2, v68
	v_add_nc_u32_e32 v67, s4, v67
	s_cmp_lt_u32 s12, s9
	s_waitcnt vmcnt(8)
	ds_write_b32 v69, v188
	s_waitcnt vmcnt(4)
	ds_write_b32 v70, v189
	ds_write_b32 v71, v190
	ds_write_b32 v72, v191
	s_waitcnt vmcnt(3)
	ds_write_b32 v73, v192
	s_waitcnt vmcnt(2)
	ds_write_b32 v74, v193
	ds_write_b32 v75, v168
	ds_write_b32 v76, v169
	ds_write_b32 v77, v170
	ds_write_b32 v78, v171
	ds_write_b32 v79, v172
	ds_write_b32 v80, v173
	ds_write_b32 v81, v174
	ds_write_b32 v82, v175
	s_waitcnt vmcnt(1)
	ds_write_b32 v83, v176
	s_waitcnt vmcnt(0)
	ds_write_b32 v84, v177
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	ds_read2_b32 v[174:175], v58 offset1:16
	ds_read2_b32 v[172:173], v85 offset1:1
	ds_read2_b32 v[184:185], v110 offset1:1
	ds_read2_b32 v[177:178], v112 offset1:1
	ds_read2_b32 v[170:171], v114 offset1:1
	ds_read2_b32 v[196:197], v158 offset1:1
	ds_read2_b32 v[180:181], v159 offset1:1
	ds_read2_b32 v[182:183], v160 offset1:1
	ds_read2_b32 v[168:169], v161 offset1:1
	ds_read2_b32 v[198:199], v155 offset1:1
	ds_read2_b32 v[186:187], v157 offset1:1
	ds_read2_b32 v[188:189], v85 offset0:2 offset1:3
	ds_read2_b32 v[157:158], v85 offset0:4 offset1:5
	ds_read2_b32 v[190:191], v113 offset1:1
	ds_read2_b32 v[192:193], v156 offset1:1
	ds_read2_b32 v[155:156], v58 offset0:128 offset1:144
	v_add_nc_u32_e32 v159, 0x1080, v58
	v_add_nc_u32_e32 v161, 0x1100, v58
	v_add_nc_u32_e32 v160, 0x1200, v58
	s_waitcnt lgkmcnt(14)
	v_fma_f32 v59, v174, v172, v59
	s_waitcnt lgkmcnt(13)
	v_fma_f32 v60, v174, v184, v60
	s_waitcnt lgkmcnt(12)
	v_fma_f32 v61, v174, v177, v61
	s_waitcnt lgkmcnt(11)
	v_fma_f32 v62, v174, v170, v62
	s_waitcnt lgkmcnt(10)
	v_fma_f32 v63, v174, v196, v63
	s_waitcnt lgkmcnt(9)
	v_fma_f32 v64, v174, v180, v64
	s_waitcnt lgkmcnt(8)
	v_fma_f32 v65, v174, v182, v65
	s_waitcnt lgkmcnt(7)
	v_fmac_f32_e32 v66, v174, v168
	v_fma_f32 v50, v175, v172, v50
	v_fma_f32 v51, v175, v184, v51
	v_fma_f32 v52, v175, v177, v52
	v_fma_f32 v53, v175, v170, v53
	v_fma_f32 v54, v175, v196, v54
	v_fma_f32 v55, v175, v180, v55
	v_fma_f32 v56, v175, v182, v56
	v_fmac_f32_e32 v57, v175, v168
	ds_read2_b32 v[174:175], v58 offset0:32 offset1:48
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v66, v155, v169
	v_fmac_f32_e32 v65, v155, v183
	v_fmac_f32_e32 v64, v155, v181
	v_fmac_f32_e32 v63, v155, v197
	v_fmac_f32_e32 v62, v155, v171
	v_fmac_f32_e32 v61, v155, v178
	v_fmac_f32_e32 v60, v155, v185
	v_fmac_f32_e32 v59, v155, v173
	v_fmac_f32_e32 v57, v156, v169
	v_fmac_f32_e32 v56, v156, v183
	v_fmac_f32_e32 v55, v156, v181
	v_fmac_f32_e32 v54, v156, v197
	v_fmac_f32_e32 v53, v156, v171
	v_fmac_f32_e32 v52, v156, v178
	v_fmac_f32_e32 v51, v156, v185
	v_fmac_f32_e32 v50, v156, v173
	ds_read2_b32 v[155:156], v58 offset0:160 offset1:176
	s_waitcnt lgkmcnt(1)
	v_fma_f32 v42, v174, v172, v42
	v_fma_f32 v43, v174, v184, v43
	v_fma_f32 v44, v174, v177, v44
	v_fma_f32 v45, v174, v170, v45
	v_fma_f32 v46, v174, v196, v46
	v_fma_f32 v47, v174, v180, v47
	v_fma_f32 v48, v174, v182, v48
	v_fmac_f32_e32 v49, v174, v168
	v_fma_f32 v34, v175, v172, v34
	v_fma_f32 v35, v175, v184, v35
	v_fma_f32 v36, v175, v177, v36
	v_fma_f32 v37, v175, v170, v37
	v_fma_f32 v38, v175, v196, v38
	v_fma_f32 v39, v175, v180, v39
	v_fma_f32 v40, v175, v182, v40
	v_fmac_f32_e32 v41, v175, v168
	ds_read2_b32 v[174:175], v58 offset0:64 offset1:80
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v49, v155, v169
	v_fmac_f32_e32 v48, v155, v183
	v_fmac_f32_e32 v47, v155, v181
	v_fmac_f32_e32 v46, v155, v197
	v_fmac_f32_e32 v45, v155, v171
	v_fmac_f32_e32 v44, v155, v178
	v_fmac_f32_e32 v43, v155, v185
	v_fmac_f32_e32 v42, v155, v173
	v_fmac_f32_e32 v41, v156, v169
	v_fmac_f32_e32 v40, v156, v183
	v_fmac_f32_e32 v39, v156, v181
	v_fmac_f32_e32 v38, v156, v197
	v_fmac_f32_e32 v37, v156, v171
	v_fmac_f32_e32 v36, v156, v178
	v_fmac_f32_e32 v35, v156, v185
	v_fmac_f32_e32 v34, v156, v173
	ds_read2_b32 v[155:156], v58 offset0:192 offset1:208
	s_waitcnt lgkmcnt(1)
	v_fma_f32 v26, v174, v172, v26
	v_fma_f32 v27, v174, v184, v27
	v_fma_f32 v28, v174, v177, v28
	v_fma_f32 v29, v174, v170, v29
	v_fma_f32 v30, v174, v196, v30
	v_fma_f32 v31, v174, v180, v31
	v_fma_f32 v32, v174, v182, v32
	v_fmac_f32_e32 v33, v174, v168
	v_fma_f32 v18, v175, v172, v18
	v_fma_f32 v19, v175, v184, v19
	v_fma_f32 v20, v175, v177, v20
	v_fma_f32 v21, v175, v170, v21
	v_fma_f32 v22, v175, v196, v22
	v_fma_f32 v23, v175, v180, v23
	v_fma_f32 v24, v175, v182, v24
	v_fmac_f32_e32 v25, v175, v168
	ds_read2_b32 v[174:175], v58 offset0:96 offset1:112
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v27, v155, v185
	v_fmac_f32_e32 v19, v156, v185
	v_fmac_f32_e32 v31, v155, v181
	v_fmac_f32_e32 v23, v156, v181
	v_fmac_f32_e32 v33, v155, v169
	v_fmac_f32_e32 v32, v155, v183
	v_fmac_f32_e32 v30, v155, v197
	v_fmac_f32_e32 v29, v155, v171
	v_fmac_f32_e32 v28, v155, v178
	v_fmac_f32_e32 v26, v155, v173
	v_fmac_f32_e32 v25, v156, v169
	v_fmac_f32_e32 v24, v156, v183
	v_fmac_f32_e32 v22, v156, v197
	v_fmac_f32_e32 v21, v156, v171
	v_fmac_f32_e32 v20, v156, v178
	v_fmac_f32_e32 v18, v156, v173
	v_add_nc_u32_e32 v156, 0x470, v85
	v_add_nc_u32_e32 v155, 0x1de8, v85
	s_waitcnt lgkmcnt(0)
	v_fma_f32 v15, v174, v180, v15
	v_fma_f32 v7, v175, v180, v7
	ds_read2_b32 v[179:180], v58 offset0:224 offset1:240
	v_fma_f32 v11, v174, v184, v11
	v_fma_f32 v3, v175, v184, v3
	v_fma_f32 v10, v174, v172, v10
	v_fma_f32 v2, v175, v172, v2
	v_fma_f32 v12, v174, v177, v12
	v_fma_f32 v4, v175, v177, v4
	ds_read2_b32 v[176:177], v152 offset1:16
	v_fma_f32 v13, v174, v170, v13
	v_fma_f32 v5, v175, v170, v5
	v_fma_f32 v14, v174, v196, v14
	v_fma_f32 v6, v175, v196, v6
	v_fma_f32 v16, v174, v182, v16
	v_fma_f32 v8, v175, v182, v8
	v_fmac_f32_e32 v17, v174, v168
	v_fmac_f32_e32 v9, v175, v168
	ds_read2_b32 v[174:175], v111 offset1:1
	v_add_nc_u32_e32 v170, 0x1180, v58
	v_add_nc_u32_e32 v168, 0x1280, v58
	v_add_nc_u32_e32 v182, 0x1568, v85
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v11, v179, v185
	v_fmac_f32_e32 v3, v180, v185
	ds_read2_b32 v[184:185], v150 offset1:1
	ds_read2_b32 v[150:151], v151 offset1:1
	v_fmac_f32_e32 v9, v180, v169
	v_fmac_f32_e32 v8, v180, v183
	v_fmac_f32_e32 v15, v179, v181
	v_fmac_f32_e32 v7, v180, v181
	v_fmac_f32_e32 v6, v180, v197
	v_fmac_f32_e32 v5, v180, v171
	v_fmac_f32_e32 v4, v180, v178
	v_fmac_f32_e32 v2, v180, v173
	ds_read2_b32 v[152:153], v153 offset1:1
	ds_read2_b32 v[180:181], v154 offset1:1
	s_waitcnt lgkmcnt(5)
	v_fmac_f32_e32 v59, v176, v188
	v_fmac_f32_e32 v61, v176, v190
	s_waitcnt lgkmcnt(4)
	v_fmac_f32_e32 v60, v176, v174
	v_fmac_f32_e32 v62, v176, v198
	v_fmac_f32_e32 v64, v176, v192
	v_fmac_f32_e32 v65, v176, v186
	v_fmac_f32_e32 v50, v177, v188
	v_fmac_f32_e32 v51, v177, v174
	s_waitcnt lgkmcnt(3)
	v_fmac_f32_e32 v63, v176, v184
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v66, v176, v150
	v_fmac_f32_e32 v52, v177, v190
	v_fmac_f32_e32 v53, v177, v198
	v_fmac_f32_e32 v54, v177, v184
	v_fmac_f32_e32 v55, v177, v192
	v_fmac_f32_e32 v56, v177, v186
	v_fmac_f32_e32 v57, v177, v150
	ds_read2_b32 v[176:177], v149 offset1:16
	ds_read2_b32 v[148:149], v148 offset1:16
	v_fmac_f32_e32 v17, v179, v169
	v_fmac_f32_e32 v12, v179, v178
	v_fmac_f32_e32 v16, v179, v183
	v_fmac_f32_e32 v14, v179, v197
	v_fmac_f32_e32 v13, v179, v171
	v_fmac_f32_e32 v10, v179, v173
	v_add_nc_u32_e32 v169, 0x1300, v58
	v_add_nc_u32_e32 v171, 0x1380, v58
	v_add_nc_u32_e32 v172, 0x1400, v58
	v_add_nc_u32_e32 v183, 0x1480, v58
	v_add_nc_u32_e32 v196, 0x8b0, v85
	v_add_nc_u32_e32 v197, 0xcf0, v85
	v_add_nc_u32_e32 v179, 0x1570, v85
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v49, v176, v150
	s_waitcnt lgkmcnt(0)
	v_fmac_f32_e32 v26, v148, v188
	v_fmac_f32_e32 v27, v148, v174
	v_fmac_f32_e32 v28, v148, v190
	v_fmac_f32_e32 v29, v148, v198
	v_fmac_f32_e32 v30, v148, v184
	v_fmac_f32_e32 v31, v148, v192
	v_fmac_f32_e32 v32, v148, v186
	v_fmac_f32_e32 v33, v148, v150
	ds_read2_b32 v[147:148], v147 offset1:16
	v_fmac_f32_e32 v41, v177, v150
	v_fmac_f32_e32 v18, v149, v188
	v_fmac_f32_e32 v19, v149, v174
	v_fmac_f32_e32 v20, v149, v190
	v_fmac_f32_e32 v21, v149, v198
	v_fmac_f32_e32 v22, v149, v184
	v_fmac_f32_e32 v23, v149, v192
	v_fmac_f32_e32 v24, v149, v186
	v_fmac_f32_e32 v25, v149, v150
	v_fmac_f32_e32 v42, v176, v188
	v_fmac_f32_e32 v43, v176, v174
	v_fmac_f32_e32 v44, v176, v190
	v_fmac_f32_e32 v45, v176, v198
	v_fmac_f32_e32 v46, v176, v184
	v_fmac_f32_e32 v47, v176, v192
	v_fmac_f32_e32 v48, v176, v186
	v_fmac_f32_e32 v34, v177, v188
	v_fmac_f32_e32 v35, v177, v174
	v_fmac_f32_e32 v36, v177, v190
	s_waitcnt lgkmcnt(0)
	v_fmac_f32_e32 v17, v147, v150
	v_fmac_f32_e32 v9, v148, v150
	ds_read2_b32 v[149:150], v146 offset1:16
	ds_read2_b32 v[145:146], v145 offset1:16
	v_fmac_f32_e32 v37, v177, v198
	v_fmac_f32_e32 v38, v177, v184
	v_fmac_f32_e32 v39, v177, v192
	v_fmac_f32_e32 v40, v177, v186
	ds_read2_b32 v[177:178], v143 offset1:16
	v_fmac_f32_e32 v11, v147, v174
	v_fmac_f32_e32 v3, v148, v174
	v_fmac_f32_e32 v10, v147, v188
	v_fmac_f32_e32 v2, v148, v188
	v_fmac_f32_e32 v12, v147, v190
	v_fmac_f32_e32 v4, v148, v190
	v_fmac_f32_e32 v13, v147, v198
	v_fmac_f32_e32 v5, v148, v198
	v_fmac_f32_e32 v14, v147, v184
	v_fmac_f32_e32 v6, v148, v184
	v_fmac_f32_e32 v15, v147, v192
	v_fmac_f32_e32 v7, v148, v192
	v_fmac_f32_e32 v16, v147, v186
	v_fmac_f32_e32 v8, v148, v186
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v49, v145, v151
	v_fmac_f32_e32 v48, v145, v187
	v_fmac_f32_e32 v47, v145, v193
	v_fmac_f32_e32 v46, v145, v185
	v_fmac_f32_e32 v45, v145, v199
	v_fmac_f32_e32 v44, v145, v191
	v_fmac_f32_e32 v43, v145, v175
	v_fmac_f32_e32 v42, v145, v189
	ds_read2_b32 v[144:145], v144 offset1:16
	v_fmac_f32_e32 v66, v149, v151
	v_fmac_f32_e32 v65, v149, v187
	v_fmac_f32_e32 v64, v149, v193
	v_fmac_f32_e32 v63, v149, v185
	v_fmac_f32_e32 v62, v149, v199
	v_fmac_f32_e32 v61, v149, v191
	v_fmac_f32_e32 v60, v149, v175
	v_fmac_f32_e32 v59, v149, v189
	v_fmac_f32_e32 v57, v150, v151
	v_fmac_f32_e32 v56, v150, v187
	v_fmac_f32_e32 v55, v150, v193
	v_fmac_f32_e32 v54, v150, v185
	v_fmac_f32_e32 v53, v150, v199
	v_fmac_f32_e32 v52, v150, v191
	v_fmac_f32_e32 v51, v150, v175
	v_fmac_f32_e32 v50, v150, v189
	ds_read2_b32 v[147:148], v135 offset1:1
	ds_read2_b32 v[149:150], v139 offset1:1
	ds_read2_b32 v[135:136], v136 offset1:1
	v_fmac_f32_e32 v41, v146, v151
	v_fmac_f32_e32 v40, v146, v187
	v_fmac_f32_e32 v39, v146, v193
	v_fmac_f32_e32 v38, v146, v185
	v_fmac_f32_e32 v37, v146, v199
	v_fmac_f32_e32 v36, v146, v191
	v_fmac_f32_e32 v35, v146, v175
	v_fmac_f32_e32 v34, v146, v189
	s_waitcnt lgkmcnt(3)
	v_fmac_f32_e32 v27, v144, v175
	v_fmac_f32_e32 v25, v145, v151
	v_fmac_f32_e32 v24, v145, v187
	v_fmac_f32_e32 v23, v145, v193
	v_fmac_f32_e32 v22, v145, v185
	v_fmac_f32_e32 v21, v145, v199
	v_fmac_f32_e32 v20, v145, v191
	v_fmac_f32_e32 v19, v145, v175
	v_fmac_f32_e32 v18, v145, v189
	ds_read2_b32 v[145:146], v142 offset1:16
	ds_read2_b32 v[173:174], v137 offset1:1
	v_fmac_f32_e32 v11, v177, v175
	v_fmac_f32_e32 v3, v178, v175
	ds_read2_b32 v[175:176], v134 offset1:1
	ds_read2_b32 v[137:138], v138 offset1:1
	v_fmac_f32_e32 v33, v144, v151
	v_fmac_f32_e32 v17, v177, v151
	v_fmac_f32_e32 v9, v178, v151
	v_fmac_f32_e32 v32, v144, v187
	v_fmac_f32_e32 v31, v144, v193
	v_fmac_f32_e32 v30, v144, v185
	v_fmac_f32_e32 v29, v144, v199
	v_fmac_f32_e32 v28, v144, v191
	v_fmac_f32_e32 v26, v144, v189
	v_fmac_f32_e32 v12, v177, v191
	v_fmac_f32_e32 v4, v178, v191
	v_fmac_f32_e32 v16, v177, v187
	v_fmac_f32_e32 v15, v177, v193
	v_fmac_f32_e32 v14, v177, v185
	s_waitcnt lgkmcnt(3)
	v_fmac_f32_e32 v59, v145, v157
	v_fmac_f32_e32 v60, v145, v152
	v_fmac_f32_e32 v61, v145, v147
	v_fmac_f32_e32 v62, v145, v149
	v_fmac_f32_e32 v63, v145, v135
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v64, v145, v173
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v65, v145, v175
	s_waitcnt lgkmcnt(0)
	v_fmac_f32_e32 v66, v145, v137
	v_fmac_f32_e32 v50, v146, v157
	v_fmac_f32_e32 v51, v146, v152
	v_fmac_f32_e32 v52, v146, v147
	v_fmac_f32_e32 v53, v146, v149
	v_fmac_f32_e32 v54, v146, v135
	v_fmac_f32_e32 v55, v146, v173
	v_fmac_f32_e32 v56, v146, v175
	v_fmac_f32_e32 v57, v146, v137
	ds_read2_b32 v[145:146], v131 offset1:16
	v_fmac_f32_e32 v13, v177, v199
	v_fmac_f32_e32 v5, v178, v199
	v_fmac_f32_e32 v10, v177, v189
	ds_read2_b32 v[142:143], v132 offset1:1
	v_fmac_f32_e32 v8, v178, v187
	v_fmac_f32_e32 v7, v178, v193
	v_fmac_f32_e32 v6, v178, v185
	v_fmac_f32_e32 v2, v178, v189
	ds_read2_b32 v[132:133], v133 offset1:1
	ds_read2_b32 v[139:140], v140 offset1:1
	ds_read2_b32 v[177:178], v141 offset1:1
	v_add_nc_u32_e32 v184, 0x1600, v58
	v_add_nc_u32_e32 v186, 0x1680, v58
	v_add_nc_u32_e32 v188, 0x1700, v58
	v_add_nc_u32_e32 v190, 0x1780, v58
	v_add_nc_u32_e32 v192, 0x1800, v58
	v_add_nc_u32_e32 v187, 0x8b8, v85
	v_add_nc_u32_e32 v193, 0x1130, v85
	v_add_nc_u32_e32 v185, 0x1578, v85
	s_waitcnt lgkmcnt(4)
	v_fmac_f32_e32 v42, v145, v157
	v_fmac_f32_e32 v43, v145, v152
	v_fmac_f32_e32 v44, v145, v147
	v_fmac_f32_e32 v45, v145, v149
	v_fmac_f32_e32 v46, v145, v135
	v_fmac_f32_e32 v47, v145, v173
	v_fmac_f32_e32 v48, v145, v175
	v_fmac_f32_e32 v49, v145, v137
	v_fmac_f32_e32 v34, v146, v157
	v_fmac_f32_e32 v35, v146, v152
	v_fmac_f32_e32 v36, v146, v147
	v_fmac_f32_e32 v37, v146, v149
	v_fmac_f32_e32 v38, v146, v135
	v_fmac_f32_e32 v39, v146, v173
	v_fmac_f32_e32 v40, v146, v175
	v_fmac_f32_e32 v41, v146, v137
	ds_read2_b32 v[145:146], v130 offset1:16
	ds_read2_b32 v[129:130], v129 offset1:16
	v_add_nc_u32_e32 v189, 0x19b0, v85
	v_add_nc_u32_e32 v191, 0x1df0, v85
	v_add_nc_u32_e32 v199, 0x1880, v58
	v_add_nc_u32_e32 v198, 0xcf8, v85
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v27, v145, v152
	v_fmac_f32_e32 v19, v146, v152
	s_waitcnt lgkmcnt(0)
	v_fmac_f32_e32 v11, v129, v152
	v_fmac_f32_e32 v3, v130, v152
	ds_read2_b32 v[151:152], v128 offset1:16
	ds_read2_b32 v[127:128], v127 offset1:16
	v_fmac_f32_e32 v26, v145, v157
	v_fmac_f32_e32 v28, v145, v147
	v_fmac_f32_e32 v29, v145, v149
	v_fmac_f32_e32 v30, v145, v135
	v_fmac_f32_e32 v31, v145, v173
	v_fmac_f32_e32 v32, v145, v175
	v_fmac_f32_e32 v33, v145, v137
	ds_read2_b32 v[144:145], v125 offset1:16
	v_fmac_f32_e32 v12, v129, v147
	v_fmac_f32_e32 v4, v130, v147
	v_fmac_f32_e32 v18, v146, v157
	v_fmac_f32_e32 v20, v146, v147
	v_fmac_f32_e32 v21, v146, v149
	v_fmac_f32_e32 v22, v146, v135
	v_fmac_f32_e32 v23, v146, v173
	v_fmac_f32_e32 v24, v146, v175
	v_fmac_f32_e32 v25, v146, v137
	v_fmac_f32_e32 v10, v129, v157
	v_fmac_f32_e32 v13, v129, v149
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v49, v127, v138
	v_fmac_f32_e32 v48, v127, v176
	v_fmac_f32_e32 v47, v127, v174
	v_fmac_f32_e32 v46, v127, v136
	v_fmac_f32_e32 v45, v127, v150
	v_fmac_f32_e32 v44, v127, v148
	v_fmac_f32_e32 v43, v127, v153
	v_fmac_f32_e32 v42, v127, v158
	v_fmac_f32_e32 v41, v128, v138
	v_fmac_f32_e32 v40, v128, v176
	v_fmac_f32_e32 v39, v128, v174
	v_fmac_f32_e32 v38, v128, v136
	v_fmac_f32_e32 v37, v128, v150
	v_fmac_f32_e32 v36, v128, v148
	v_fmac_f32_e32 v35, v128, v153
	v_fmac_f32_e32 v34, v128, v158
	ds_read2_b32 v[127:128], v126 offset1:16
	v_fmac_f32_e32 v14, v129, v135
	v_fmac_f32_e32 v15, v129, v173
	v_fmac_f32_e32 v16, v129, v175
	v_fmac_f32_e32 v17, v129, v137
	v_fmac_f32_e32 v5, v130, v149
	v_fmac_f32_e32 v66, v151, v138
	v_fmac_f32_e32 v65, v151, v176
	v_fmac_f32_e32 v64, v151, v174
	v_fmac_f32_e32 v63, v151, v136
	v_fmac_f32_e32 v62, v151, v150
	v_fmac_f32_e32 v61, v151, v148
	v_fmac_f32_e32 v60, v151, v153
	v_fmac_f32_e32 v59, v151, v158
	v_fmac_f32_e32 v57, v152, v138
	v_fmac_f32_e32 v56, v152, v176
	v_fmac_f32_e32 v55, v152, v174
	v_fmac_f32_e32 v54, v152, v136
	v_fmac_f32_e32 v53, v152, v150
	v_fmac_f32_e32 v52, v152, v148
	v_fmac_f32_e32 v51, v152, v153
	v_fmac_f32_e32 v50, v152, v158
	ds_read2_b32 v[151:152], v122 offset1:1
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v28, v127, v148
	v_fmac_f32_e32 v25, v128, v138
	v_fmac_f32_e32 v24, v128, v176
	v_fmac_f32_e32 v23, v128, v174
	v_fmac_f32_e32 v22, v128, v136
	v_fmac_f32_e32 v21, v128, v150
	v_fmac_f32_e32 v20, v128, v148
	v_fmac_f32_e32 v19, v128, v153
	v_fmac_f32_e32 v18, v128, v158
	ds_read2_b32 v[128:129], v124 offset1:16
	ds_read2_b32 v[122:123], v123 offset1:1
	v_fmac_f32_e32 v12, v144, v148
	v_fmac_f32_e32 v4, v145, v148
	ds_read2_b32 v[148:149], v85 offset0:6 offset1:7
	v_fmac_f32_e32 v6, v130, v135
	v_fmac_f32_e32 v7, v130, v173
	v_fmac_f32_e32 v8, v130, v175
	v_fmac_f32_e32 v2, v130, v157
	v_fmac_f32_e32 v9, v130, v137
	v_fmac_f32_e32 v33, v127, v138
	v_fmac_f32_e32 v32, v127, v176
	v_fmac_f32_e32 v31, v127, v174
	v_fmac_f32_e32 v30, v127, v136
	v_fmac_f32_e32 v29, v127, v150
	v_fmac_f32_e32 v27, v127, v153
	v_fmac_f32_e32 v26, v127, v158
	ds_read2_b32 v[126:127], v105 offset1:16
	v_fmac_f32_e32 v15, v144, v174
	v_fmac_f32_e32 v7, v145, v174
	s_waitcnt lgkmcnt(3)
	v_fmac_f32_e32 v51, v129, v180
	v_fmac_f32_e32 v52, v129, v142
	v_fmac_f32_e32 v53, v129, v151
	v_fmac_f32_e32 v54, v129, v139
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v50, v129, v148
	v_fmac_f32_e32 v55, v129, v132
	v_fmac_f32_e32 v56, v129, v122
	v_fmac_f32_e32 v57, v129, v177
	ds_read2_b32 v[129:130], v121 offset1:16
	v_fmac_f32_e32 v59, v128, v148
	v_fmac_f32_e32 v60, v128, v180
	v_fmac_f32_e32 v61, v128, v142
	v_fmac_f32_e32 v62, v128, v151
	v_fmac_f32_e32 v63, v128, v139
	v_fmac_f32_e32 v64, v128, v132
	v_fmac_f32_e32 v65, v128, v122
	v_fmac_f32_e32 v66, v128, v177
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v62, v126, v152
	v_fmac_f32_e32 v63, v126, v140
	v_fmac_f32_e32 v64, v126, v133
	v_fmac_f32_e32 v65, v126, v123
	v_fmac_f32_e32 v66, v126, v178
	v_fmac_f32_e32 v61, v126, v143
	v_fmac_f32_e32 v60, v126, v181
	v_fmac_f32_e32 v59, v126, v149
	v_fmac_f32_e32 v57, v127, v178
	v_fmac_f32_e32 v56, v127, v123
	v_fmac_f32_e32 v55, v127, v133
	s_waitcnt lgkmcnt(0)
	v_fmac_f32_e32 v42, v129, v148
	v_fmac_f32_e32 v43, v129, v180
	v_fmac_f32_e32 v44, v129, v142
	v_fmac_f32_e32 v45, v129, v151
	v_fmac_f32_e32 v46, v129, v139
	v_fmac_f32_e32 v47, v129, v132
	v_fmac_f32_e32 v48, v129, v122
	v_fmac_f32_e32 v49, v129, v177
	v_fmac_f32_e32 v34, v130, v148
	v_fmac_f32_e32 v35, v130, v180
	v_fmac_f32_e32 v36, v130, v142
	v_fmac_f32_e32 v37, v130, v151
	v_fmac_f32_e32 v38, v130, v139
	v_fmac_f32_e32 v39, v130, v132
	v_fmac_f32_e32 v40, v130, v122
	v_fmac_f32_e32 v41, v130, v177
	ds_read2_b32 v[129:130], v119 offset1:16
	v_fmac_f32_e32 v54, v127, v140
	v_fmac_f32_e32 v53, v127, v152
	v_fmac_f32_e32 v52, v127, v143
	v_fmac_f32_e32 v51, v127, v181
	v_fmac_f32_e32 v50, v127, v149
	ds_read2_b32 v[126:127], v104 offset1:16
	v_fmac_f32_e32 v17, v144, v138
	v_fmac_f32_e32 v9, v145, v138
	v_fmac_f32_e32 v16, v144, v176
	v_fmac_f32_e32 v8, v145, v176
	v_fmac_f32_e32 v14, v144, v136
	v_fmac_f32_e32 v6, v145, v136
	v_fmac_f32_e32 v13, v144, v150
	v_fmac_f32_e32 v5, v145, v150
	v_fmac_f32_e32 v11, v144, v153
	v_fmac_f32_e32 v3, v145, v153
	v_fmac_f32_e32 v10, v144, v158
	v_fmac_f32_e32 v2, v145, v158
	ds_read2_b32 v[134:135], v117 offset1:1
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v26, v129, v148
	v_fmac_f32_e32 v27, v129, v180
	v_fmac_f32_e32 v28, v129, v142
	v_fmac_f32_e32 v29, v129, v151
	v_fmac_f32_e32 v30, v129, v139
	v_fmac_f32_e32 v31, v129, v132
	v_fmac_f32_e32 v32, v129, v122
	v_fmac_f32_e32 v33, v129, v177
	v_fmac_f32_e32 v18, v130, v148
	v_fmac_f32_e32 v19, v130, v180
	v_fmac_f32_e32 v20, v130, v142
	v_fmac_f32_e32 v21, v130, v151
	v_fmac_f32_e32 v22, v130, v139
	v_fmac_f32_e32 v23, v130, v132
	v_fmac_f32_e32 v24, v130, v122
	v_fmac_f32_e32 v25, v130, v177
	ds_read2_b32 v[129:130], v115 offset1:16
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v49, v126, v178
	v_fmac_f32_e32 v48, v126, v123
	v_fmac_f32_e32 v47, v126, v133
	v_fmac_f32_e32 v46, v126, v140
	v_fmac_f32_e32 v45, v126, v152
	v_fmac_f32_e32 v44, v126, v143
	v_fmac_f32_e32 v43, v126, v181
	v_fmac_f32_e32 v42, v126, v149
	v_fmac_f32_e32 v41, v127, v178
	v_fmac_f32_e32 v40, v127, v123
	v_fmac_f32_e32 v39, v127, v133
	v_fmac_f32_e32 v38, v127, v140
	v_fmac_f32_e32 v37, v127, v152
	v_fmac_f32_e32 v36, v127, v143
	v_fmac_f32_e32 v35, v127, v181
	v_fmac_f32_e32 v34, v127, v149
	ds_read2_b32 v[126:127], v103 offset1:16
	ds_read2_b32 v[124:125], v85 offset0:8 offset1:9
	ds_read2_b32 v[136:137], v85 offset0:10 offset1:11
	s_waitcnt lgkmcnt(3)
	v_fmac_f32_e32 v15, v129, v132
	v_fmac_f32_e32 v7, v130, v132
	ds_read2_b32 v[102:103], v102 offset1:16
	v_fmac_f32_e32 v10, v129, v148
	v_fmac_f32_e32 v11, v129, v180
	v_fmac_f32_e32 v12, v129, v142
	v_fmac_f32_e32 v13, v129, v151
	v_fmac_f32_e32 v14, v129, v139
	v_fmac_f32_e32 v16, v129, v122
	v_fmac_f32_e32 v17, v129, v177
	v_fmac_f32_e32 v8, v130, v122
	v_fmac_f32_e32 v2, v130, v148
	v_fmac_f32_e32 v3, v130, v180
	v_fmac_f32_e32 v4, v130, v142
	v_fmac_f32_e32 v5, v130, v151
	v_fmac_f32_e32 v6, v130, v139
	v_fmac_f32_e32 v9, v130, v177
	ds_read2_b32 v[131:132], v120 offset1:16
	ds_read2_b32 v[115:116], v116 offset1:1
	ds_read2_b32 v[119:120], v165 offset1:1
	ds_read2_b32 v[128:129], v164 offset1:1
	ds_read2_b32 v[121:122], v163 offset1:1
	s_waitcnt lgkmcnt(8)
	v_fmac_f32_e32 v33, v126, v178
	v_fmac_f32_e32 v32, v126, v123
	v_fmac_f32_e32 v31, v126, v133
	v_fmac_f32_e32 v30, v126, v140
	v_fmac_f32_e32 v29, v126, v152
	v_fmac_f32_e32 v28, v126, v143
	v_fmac_f32_e32 v27, v126, v181
	v_fmac_f32_e32 v26, v126, v149
	v_fmac_f32_e32 v25, v127, v178
	v_fmac_f32_e32 v24, v127, v123
	v_fmac_f32_e32 v23, v127, v133
	v_fmac_f32_e32 v22, v127, v140
	v_fmac_f32_e32 v21, v127, v152
	v_fmac_f32_e32 v20, v127, v143
	v_fmac_f32_e32 v19, v127, v181
	v_fmac_f32_e32 v18, v127, v149
	ds_read2_b32 v[126:127], v166 offset1:1
	ds_read2_b32 v[138:139], v167 offset1:1
	s_waitcnt lgkmcnt(7)
	v_fmac_f32_e32 v17, v102, v178
	v_fmac_f32_e32 v9, v103, v178
	ds_read2_b32 v[177:178], v100 offset1:1
	v_fmac_f32_e32 v16, v102, v123
	v_fmac_f32_e32 v8, v103, v123
	v_fmac_f32_e32 v15, v102, v133
	v_fmac_f32_e32 v7, v103, v133
	v_fmac_f32_e32 v14, v102, v140
	v_fmac_f32_e32 v6, v103, v140
	v_fmac_f32_e32 v13, v102, v152
	v_fmac_f32_e32 v5, v103, v152
	v_fmac_f32_e32 v12, v102, v143
	v_fmac_f32_e32 v4, v103, v143
	v_fmac_f32_e32 v11, v102, v181
	v_fmac_f32_e32 v3, v103, v181
	v_fmac_f32_e32 v10, v102, v149
	v_fmac_f32_e32 v2, v103, v149
	ds_read2_b32 v[102:103], v156 offset1:1
	ds_read2_b32 v[148:149], v159 offset1:16
	ds_read2_b32 v[164:165], v161 offset1:16
	ds_read2_b32 v[152:153], v170 offset1:16
	ds_read2_b32 v[100:101], v101 offset1:1
	ds_read2_b32 v[117:118], v118 offset1:1
	ds_read2_b32 v[140:141], v182 offset1:1
	ds_read2_b32 v[144:145], v162 offset1:1
	ds_read2_b32 v[142:143], v98 offset1:1
	ds_read2_b32 v[146:147], v155 offset1:1
	ds_read2_b32 v[161:162], v169 offset1:16
	ds_read2_b32 v[181:182], v171 offset1:16
	s_waitcnt lgkmcnt(19)
	v_fmac_f32_e32 v59, v131, v124
	s_waitcnt lgkmcnt(13)
	v_fmac_f32_e32 v66, v131, v138
	v_fmac_f32_e32 v50, v132, v124
	v_fmac_f32_e32 v57, v132, v138
	v_add_nc_u32_e32 v173, 0x478, v85
	v_add_nc_u32_e32 v174, 0x1138, v85
	s_waitcnt lgkmcnt(12)
	v_fmac_f32_e32 v60, v131, v177
	v_fmac_f32_e32 v61, v131, v134
	v_fmac_f32_e32 v62, v131, v115
	s_waitcnt lgkmcnt(10)
	v_fmac_f32_e32 v42, v148, v124
	s_waitcnt lgkmcnt(9)
	v_fmac_f32_e32 v26, v164, v124
	v_fmac_f32_e32 v27, v164, v177
	v_fmac_f32_e32 v28, v164, v134
	v_fmac_f32_e32 v29, v164, v115
	v_fmac_f32_e32 v30, v164, v119
	v_fmac_f32_e32 v31, v164, v121
	v_fmac_f32_e32 v32, v164, v126
	v_fmac_f32_e32 v33, v164, v138
	v_fmac_f32_e32 v18, v165, v124
	v_fmac_f32_e32 v19, v165, v177
	v_fmac_f32_e32 v20, v165, v134
	v_fmac_f32_e32 v21, v165, v115
	v_fmac_f32_e32 v22, v165, v119
	v_fmac_f32_e32 v23, v165, v121
	v_fmac_f32_e32 v24, v165, v126
	v_fmac_f32_e32 v25, v165, v138
	ds_read2_b32 v[164:165], v160 offset1:16
	ds_read2_b32 v[157:158], v168 offset1:16
	v_fmac_f32_e32 v43, v148, v177
	v_fmac_f32_e32 v44, v148, v134
	v_fmac_f32_e32 v45, v148, v115
	v_fmac_f32_e32 v46, v148, v119
	v_fmac_f32_e32 v47, v148, v121
	v_fmac_f32_e32 v48, v148, v126
	v_fmac_f32_e32 v49, v148, v138
	v_fmac_f32_e32 v34, v149, v124
	v_fmac_f32_e32 v35, v149, v177
	v_fmac_f32_e32 v36, v149, v134
	v_fmac_f32_e32 v37, v149, v115
	v_fmac_f32_e32 v38, v149, v119
	v_fmac_f32_e32 v39, v149, v121
	v_fmac_f32_e32 v40, v149, v126
	v_fmac_f32_e32 v41, v149, v138
	s_waitcnt lgkmcnt(10)
	v_fmac_f32_e32 v10, v152, v124
	v_fmac_f32_e32 v11, v152, v177
	v_fmac_f32_e32 v12, v152, v134
	v_fmac_f32_e32 v13, v152, v115
	v_fmac_f32_e32 v14, v152, v119
	v_fmac_f32_e32 v15, v152, v121
	v_fmac_f32_e32 v16, v152, v126
	v_fmac_f32_e32 v17, v152, v138
	v_fmac_f32_e32 v2, v153, v124
	v_fmac_f32_e32 v3, v153, v177
	v_fmac_f32_e32 v4, v153, v134
	v_fmac_f32_e32 v5, v153, v115
	v_fmac_f32_e32 v6, v153, v119
	v_fmac_f32_e32 v7, v153, v121
	v_fmac_f32_e32 v8, v153, v126
	v_fmac_f32_e32 v9, v153, v138
	ds_read2_b32 v[150:151], v172 offset1:16
	ds_read2_b32 v[152:153], v183 offset1:16
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v49, v157, v139
	v_fmac_f32_e32 v48, v157, v127
	v_fmac_f32_e32 v47, v157, v122
	v_fmac_f32_e32 v46, v157, v120
	v_fmac_f32_e32 v45, v157, v116
	v_fmac_f32_e32 v44, v157, v135
	v_fmac_f32_e32 v43, v157, v178
	v_fmac_f32_e32 v42, v157, v125
	v_fmac_f32_e32 v41, v158, v139
	v_fmac_f32_e32 v40, v158, v127
	v_fmac_f32_e32 v39, v158, v122
	v_fmac_f32_e32 v38, v158, v120
	v_fmac_f32_e32 v37, v158, v116
	v_fmac_f32_e32 v36, v158, v135
	v_fmac_f32_e32 v35, v158, v178
	v_fmac_f32_e32 v34, v158, v125
	ds_read2_b32 v[154:155], v194 offset1:16
	ds_read2_b32 v[123:124], v184 offset1:16
	ds_read2_b32 v[156:157], v195 offset1:16
	v_fmac_f32_e32 v52, v132, v134
	v_fmac_f32_e32 v63, v131, v119
	v_fmac_f32_e32 v64, v131, v121
	v_fmac_f32_e32 v65, v131, v126
	v_fmac_f32_e32 v51, v132, v177
	v_fmac_f32_e32 v53, v132, v115
	v_fmac_f32_e32 v54, v132, v119
	v_fmac_f32_e32 v55, v132, v121
	v_fmac_f32_e32 v56, v132, v126
	v_fmac_f32_e32 v66, v164, v139
	v_fmac_f32_e32 v57, v165, v139
	v_fmac_f32_e32 v33, v161, v139
	v_fmac_f32_e32 v32, v161, v127
	v_fmac_f32_e32 v31, v161, v122
	v_fmac_f32_e32 v30, v161, v120
	v_fmac_f32_e32 v29, v161, v116
	v_fmac_f32_e32 v28, v161, v135
	v_fmac_f32_e32 v27, v161, v178
	v_fmac_f32_e32 v26, v161, v125
	v_fmac_f32_e32 v25, v162, v139
	v_fmac_f32_e32 v17, v181, v139
	v_fmac_f32_e32 v9, v182, v139
	ds_read2_b32 v[138:139], v188 offset1:16
	ds_read2_b32 v[158:159], v186 offset1:16
	ds_read2_b32 v[160:161], v190 offset1:16
	v_fmac_f32_e32 v65, v164, v127
	v_fmac_f32_e32 v64, v164, v122
	v_fmac_f32_e32 v63, v164, v120
	v_fmac_f32_e32 v62, v164, v116
	v_fmac_f32_e32 v61, v164, v135
	v_fmac_f32_e32 v60, v164, v178
	v_fmac_f32_e32 v59, v164, v125
	v_fmac_f32_e32 v56, v165, v127
	v_fmac_f32_e32 v55, v165, v122
	v_fmac_f32_e32 v54, v165, v120
	v_fmac_f32_e32 v53, v165, v116
	v_fmac_f32_e32 v52, v165, v135
	v_fmac_f32_e32 v51, v165, v178
	v_fmac_f32_e32 v50, v165, v125
	v_fmac_f32_e32 v24, v162, v127
	v_fmac_f32_e32 v23, v162, v122
	v_fmac_f32_e32 v22, v162, v120
	v_fmac_f32_e32 v21, v162, v116
	v_fmac_f32_e32 v20, v162, v135
	v_fmac_f32_e32 v19, v162, v178
	v_fmac_f32_e32 v18, v162, v125
	v_fmac_f32_e32 v16, v181, v127
	v_fmac_f32_e32 v8, v182, v127
	v_fmac_f32_e32 v15, v181, v122
	v_fmac_f32_e32 v14, v181, v120
	v_fmac_f32_e32 v6, v182, v120
	v_fmac_f32_e32 v13, v181, v116
	v_fmac_f32_e32 v5, v182, v116
	v_fmac_f32_e32 v12, v181, v135
	v_fmac_f32_e32 v11, v181, v178
	v_fmac_f32_e32 v10, v181, v125
	ds_read2_b32 v[133:134], v192 offset1:16
	ds_read2_b32 v[168:169], v173 offset1:1
	ds_read2_b32 v[164:165], v196 offset1:1
	ds_read2_b32 v[166:167], v187 offset1:1
	ds_read2_b32 v[126:127], v197 offset1:1
	ds_read2_b32 v[115:116], v193 offset1:1
	ds_read2_b32 v[170:171], v174 offset1:1
	v_add_nc_u32_e32 v175, 0x19b8, v85
	ds_read2_b32 v[120:121], v179 offset1:1
	ds_read2_b32 v[162:163], v189 offset1:1
	ds_read2_b32 v[104:105], v85 offset0:12 offset1:13
	ds_read2_b32 v[172:173], v185 offset1:1
	ds_read2_b32 v[131:132], v191 offset1:1
	ds_read2_b32 v[176:177], v199 offset1:16
	ds_read2_b32 v[184:185], v200 offset1:16
	ds_read2_b32 v[180:181], v201 offset1:16
	v_fmac_f32_e32 v3, v182, v178
	v_fmac_f32_e32 v7, v182, v122
	v_fmac_f32_e32 v4, v182, v135
	v_fmac_f32_e32 v2, v182, v125
	s_waitcnt lgkmcnt(22)
	v_fmac_f32_e32 v59, v150, v136
	v_fmac_f32_e32 v60, v150, v100
	v_fmac_f32_e32 v61, v150, v117
	v_fmac_f32_e32 v62, v150, v142
	v_fmac_f32_e32 v63, v150, v128
	v_fmac_f32_e32 v64, v150, v140
	v_fmac_f32_e32 v65, v150, v144
	v_fmac_f32_e32 v66, v150, v146
	v_fmac_f32_e32 v50, v151, v136
	v_fmac_f32_e32 v51, v151, v100
	v_fmac_f32_e32 v52, v151, v117
	v_fmac_f32_e32 v53, v151, v142
	v_fmac_f32_e32 v54, v151, v128
	v_fmac_f32_e32 v55, v151, v140
	v_fmac_f32_e32 v56, v151, v144
	v_fmac_f32_e32 v57, v151, v146
	ds_read2_b32 v[150:151], v202 offset1:16
	ds_read2_b32 v[191:192], v204 offset1:16
	ds_read2_b32 v[182:183], v203 offset1:16
	ds_read2_b32 v[178:179], v205 offset1:16
	s_waitcnt lgkmcnt(25)
	v_fmac_f32_e32 v42, v152, v136
	v_fmac_f32_e32 v43, v152, v100
	v_fmac_f32_e32 v44, v152, v117
	v_fmac_f32_e32 v45, v152, v142
	v_fmac_f32_e32 v46, v152, v128
	v_fmac_f32_e32 v47, v152, v140
	v_fmac_f32_e32 v48, v152, v144
	v_fmac_f32_e32 v49, v152, v146
	v_fmac_f32_e32 v34, v153, v136
	v_fmac_f32_e32 v35, v153, v100
	v_fmac_f32_e32 v36, v153, v117
	v_fmac_f32_e32 v37, v153, v142
	v_fmac_f32_e32 v38, v153, v128
	v_fmac_f32_e32 v39, v153, v140
	v_fmac_f32_e32 v40, v153, v144
	v_fmac_f32_e32 v41, v153, v146
	s_waitcnt lgkmcnt(24)
	v_fmac_f32_e32 v26, v154, v136
	v_fmac_f32_e32 v27, v154, v100
	v_fmac_f32_e32 v28, v154, v117
	v_fmac_f32_e32 v29, v154, v142
	v_fmac_f32_e32 v30, v154, v128
	v_fmac_f32_e32 v31, v154, v140
	v_fmac_f32_e32 v32, v154, v144
	v_fmac_f32_e32 v33, v154, v146
	v_fmac_f32_e32 v18, v155, v136
	v_fmac_f32_e32 v19, v155, v100
	v_fmac_f32_e32 v20, v155, v117
	v_fmac_f32_e32 v21, v155, v142
	v_fmac_f32_e32 v22, v155, v128
	v_fmac_f32_e32 v23, v155, v140
	v_fmac_f32_e32 v24, v155, v144
	v_fmac_f32_e32 v25, v155, v146
	s_waitcnt lgkmcnt(22)
	v_fmac_f32_e32 v10, v156, v136
	v_fmac_f32_e32 v11, v156, v100
	v_fmac_f32_e32 v12, v156, v117
	v_fmac_f32_e32 v13, v156, v142
	v_fmac_f32_e32 v14, v156, v128
	v_fmac_f32_e32 v15, v156, v140
	v_fmac_f32_e32 v16, v156, v144
	v_fmac_f32_e32 v17, v156, v146
	v_fmac_f32_e32 v2, v157, v136
	v_fmac_f32_e32 v3, v157, v100
	v_fmac_f32_e32 v4, v157, v117
	v_fmac_f32_e32 v5, v157, v142
	v_fmac_f32_e32 v6, v157, v128
	v_fmac_f32_e32 v7, v157, v140
	v_fmac_f32_e32 v8, v157, v144
	v_fmac_f32_e32 v9, v157, v146
	v_fmac_f32_e32 v57, v124, v147
	v_fmac_f32_e32 v55, v124, v141
	v_fmac_f32_e32 v54, v124, v129
	v_fmac_f32_e32 v52, v124, v118
	v_fmac_f32_e32 v51, v124, v101
	v_fmac_f32_e32 v50, v124, v137
	ds_read2_b32 v[154:155], v206 offset1:16
	ds_read2_b32 v[152:153], v208 offset1:16
	ds_read2_b32 v[148:149], v85 offset0:14 offset1:15
	ds_read2_b32 v[195:196], v198 offset1:1
	v_fmac_f32_e32 v53, v124, v143
	v_fmac_f32_e32 v56, v124, v145
	ds_read2_b32 v[174:175], v175 offset1:1
	ds_read2_b32 v[124:125], v210 offset1:16
	ds_read2_b32 v[187:188], v207 offset1:1
	ds_read2_b32 v[156:157], v209 offset1:16
	v_fmac_f32_e32 v66, v123, v147
	v_fmac_f32_e32 v65, v123, v145
	v_fmac_f32_e32 v64, v123, v141
	v_fmac_f32_e32 v63, v123, v129
	v_fmac_f32_e32 v62, v123, v143
	v_fmac_f32_e32 v61, v123, v118
	v_fmac_f32_e32 v60, v123, v101
	v_fmac_f32_e32 v59, v123, v137
	s_waitcnt lgkmcnt(28)
	v_fmac_f32_e32 v49, v158, v147
	v_fmac_f32_e32 v48, v158, v145
	v_fmac_f32_e32 v47, v158, v141
	v_fmac_f32_e32 v46, v158, v129
	v_fmac_f32_e32 v45, v158, v143
	v_fmac_f32_e32 v44, v158, v118
	v_fmac_f32_e32 v43, v158, v101
	v_fmac_f32_e32 v42, v158, v137
	v_fmac_f32_e32 v41, v159, v147
	v_fmac_f32_e32 v40, v159, v145
	v_fmac_f32_e32 v39, v159, v141
	v_fmac_f32_e32 v38, v159, v129
	v_fmac_f32_e32 v37, v159, v143
	v_fmac_f32_e32 v36, v159, v118
	v_fmac_f32_e32 v35, v159, v101
	v_fmac_f32_e32 v34, v159, v137
	v_fmac_f32_e32 v33, v138, v147
	v_fmac_f32_e32 v32, v138, v145
	v_fmac_f32_e32 v31, v138, v141
	v_fmac_f32_e32 v30, v138, v129
	v_fmac_f32_e32 v29, v138, v143
	v_fmac_f32_e32 v28, v138, v118
	v_fmac_f32_e32 v27, v138, v101
	v_fmac_f32_e32 v26, v138, v137
	v_fmac_f32_e32 v25, v139, v147
	v_fmac_f32_e32 v24, v139, v145
	v_fmac_f32_e32 v23, v139, v141
	v_fmac_f32_e32 v22, v139, v129
	v_fmac_f32_e32 v21, v139, v143
	v_fmac_f32_e32 v20, v139, v118
	v_fmac_f32_e32 v19, v139, v101
	v_fmac_f32_e32 v18, v139, v137
	s_waitcnt lgkmcnt(27)
	v_fmac_f32_e32 v17, v160, v147
	v_fmac_f32_e32 v9, v161, v147
	v_fmac_f32_e32 v16, v160, v145
	v_fmac_f32_e32 v8, v161, v145
	v_fmac_f32_e32 v15, v160, v141
	v_fmac_f32_e32 v7, v161, v141
	v_fmac_f32_e32 v14, v160, v129
	v_fmac_f32_e32 v6, v161, v129
	v_fmac_f32_e32 v13, v160, v143
	v_fmac_f32_e32 v5, v161, v143
	v_fmac_f32_e32 v12, v160, v118
	v_fmac_f32_e32 v4, v161, v118
	v_fmac_f32_e32 v11, v160, v101
	v_fmac_f32_e32 v3, v161, v101
	v_fmac_f32_e32 v10, v160, v137
	v_fmac_f32_e32 v2, v161, v137
	ds_read2_b32 v[158:159], v212 offset1:16
	ds_read2_b32 v[197:198], v211 offset1:16
	ds_read2_b32 v[189:190], v213 offset1:16
	ds_read2_b32 v[193:194], v214 offset1:16
	s_waitcnt lgkmcnt(21)
	v_fmac_f32_e32 v59, v133, v104
	v_fmac_f32_e32 v60, v133, v102
	v_fmac_f32_e32 v61, v133, v164
	v_fmac_f32_e32 v62, v133, v126
	v_fmac_f32_e32 v63, v133, v115
	v_fmac_f32_e32 v64, v133, v120
	v_fmac_f32_e32 v65, v133, v162
	s_waitcnt lgkmcnt(19)
	v_fmac_f32_e32 v66, v133, v131
	v_fmac_f32_e32 v50, v134, v104
	v_fmac_f32_e32 v51, v134, v102
	v_fmac_f32_e32 v52, v134, v164
	v_fmac_f32_e32 v53, v134, v126
	v_fmac_f32_e32 v54, v134, v115
	v_fmac_f32_e32 v55, v134, v120
	v_fmac_f32_e32 v56, v134, v162
	v_fmac_f32_e32 v57, v134, v131
	s_waitcnt lgkmcnt(18)
	v_fmac_f32_e32 v42, v176, v104
	v_fmac_f32_e32 v43, v176, v102
	v_fmac_f32_e32 v44, v176, v164
	v_fmac_f32_e32 v45, v176, v126
	v_fmac_f32_e32 v46, v176, v115
	v_fmac_f32_e32 v47, v176, v120
	v_fmac_f32_e32 v48, v176, v162
	v_fmac_f32_e32 v49, v176, v131
	v_fmac_f32_e32 v34, v177, v104
	v_fmac_f32_e32 v35, v177, v102
	v_fmac_f32_e32 v36, v177, v164
	v_fmac_f32_e32 v37, v177, v126
	v_fmac_f32_e32 v38, v177, v115
	v_fmac_f32_e32 v39, v177, v120
	v_fmac_f32_e32 v40, v177, v162
	v_fmac_f32_e32 v41, v177, v131
	s_waitcnt lgkmcnt(17)
	v_fmac_f32_e32 v26, v184, v104
	v_fmac_f32_e32 v27, v184, v102
	v_fmac_f32_e32 v28, v184, v164
	v_fmac_f32_e32 v29, v184, v126
	v_fmac_f32_e32 v30, v184, v115
	v_fmac_f32_e32 v31, v184, v120
	v_fmac_f32_e32 v32, v184, v162
	v_fmac_f32_e32 v33, v184, v131
	v_fmac_f32_e32 v18, v185, v104
	v_fmac_f32_e32 v19, v185, v102
	v_fmac_f32_e32 v20, v185, v164
	v_fmac_f32_e32 v21, v185, v126
	v_fmac_f32_e32 v22, v185, v115
	v_fmac_f32_e32 v23, v185, v120
	v_fmac_f32_e32 v24, v185, v162
	v_fmac_f32_e32 v25, v185, v131
	s_waitcnt lgkmcnt(16)
	v_fmac_f32_e32 v10, v180, v104
	v_fmac_f32_e32 v11, v180, v102
	v_fmac_f32_e32 v12, v180, v164
	v_fmac_f32_e32 v13, v180, v126
	v_fmac_f32_e32 v14, v180, v115
	v_fmac_f32_e32 v15, v180, v120
	v_fmac_f32_e32 v16, v180, v162
	v_fmac_f32_e32 v17, v180, v131
	v_fmac_f32_e32 v2, v181, v104
	v_fmac_f32_e32 v3, v181, v102
	v_fmac_f32_e32 v4, v181, v164
	v_fmac_f32_e32 v5, v181, v126
	v_fmac_f32_e32 v6, v181, v115
	v_fmac_f32_e32 v7, v181, v120
	v_fmac_f32_e32 v8, v181, v162
	v_fmac_f32_e32 v9, v181, v131
	s_waitcnt lgkmcnt(15)
	v_fmac_f32_e32 v66, v150, v132
	v_fmac_f32_e32 v65, v150, v163
	v_fmac_f32_e32 v64, v150, v121
	v_fmac_f32_e32 v63, v150, v116
	v_fmac_f32_e32 v62, v150, v127
	v_fmac_f32_e32 v61, v150, v165
	v_fmac_f32_e32 v60, v150, v103
	v_fmac_f32_e32 v59, v150, v105
	v_fmac_f32_e32 v57, v151, v132
	v_fmac_f32_e32 v56, v151, v163
	v_fmac_f32_e32 v55, v151, v121
	v_fmac_f32_e32 v54, v151, v116
	v_fmac_f32_e32 v53, v151, v127
	v_fmac_f32_e32 v52, v151, v165
	v_fmac_f32_e32 v51, v151, v103
	v_fmac_f32_e32 v50, v151, v105
	s_waitcnt lgkmcnt(13)
	v_fmac_f32_e32 v49, v182, v132
	v_fmac_f32_e32 v48, v182, v163
	v_fmac_f32_e32 v47, v182, v121
	v_fmac_f32_e32 v46, v182, v116
	v_fmac_f32_e32 v45, v182, v127
	v_fmac_f32_e32 v44, v182, v165
	v_fmac_f32_e32 v43, v182, v103
	v_fmac_f32_e32 v42, v182, v105
	v_fmac_f32_e32 v41, v183, v132
	v_fmac_f32_e32 v40, v183, v163
	v_fmac_f32_e32 v39, v183, v121
	v_fmac_f32_e32 v38, v183, v116
	v_fmac_f32_e32 v37, v183, v127
	v_fmac_f32_e32 v36, v183, v165
	v_fmac_f32_e32 v35, v183, v103
	v_fmac_f32_e32 v34, v183, v105
	v_fmac_f32_e32 v33, v191, v132
	v_fmac_f32_e32 v32, v191, v163
	v_fmac_f32_e32 v31, v191, v121
	v_fmac_f32_e32 v30, v191, v116
	v_fmac_f32_e32 v29, v191, v127
	v_fmac_f32_e32 v28, v191, v165
	v_fmac_f32_e32 v27, v191, v103
	v_fmac_f32_e32 v26, v191, v105
	v_fmac_f32_e32 v25, v192, v132
	v_fmac_f32_e32 v24, v192, v163
	v_fmac_f32_e32 v23, v192, v121
	v_fmac_f32_e32 v22, v192, v116
	v_fmac_f32_e32 v21, v192, v127
	v_fmac_f32_e32 v20, v192, v165
	v_fmac_f32_e32 v19, v192, v103
	v_fmac_f32_e32 v18, v192, v105
	s_waitcnt lgkmcnt(12)
	v_fmac_f32_e32 v17, v178, v132
	v_fmac_f32_e32 v9, v179, v132
	v_fmac_f32_e32 v16, v178, v163
	v_fmac_f32_e32 v8, v179, v163
	v_fmac_f32_e32 v15, v178, v121
	v_fmac_f32_e32 v7, v179, v121
	v_fmac_f32_e32 v14, v178, v116
	v_fmac_f32_e32 v6, v179, v116
	v_fmac_f32_e32 v13, v178, v127
	v_fmac_f32_e32 v5, v179, v127
	v_fmac_f32_e32 v12, v178, v165
	v_fmac_f32_e32 v4, v179, v165
	v_fmac_f32_e32 v11, v178, v103
	v_fmac_f32_e32 v3, v179, v103
	v_fmac_f32_e32 v10, v178, v105
	v_fmac_f32_e32 v2, v179, v105
	s_waitcnt lgkmcnt(9)
	v_fmac_f32_e32 v59, v154, v148
	v_fmac_f32_e32 v60, v154, v168
	v_fmac_f32_e32 v61, v154, v166
	s_waitcnt lgkmcnt(8)
	v_fmac_f32_e32 v62, v154, v195
	v_fmac_f32_e32 v63, v154, v170
	v_fmac_f32_e32 v64, v154, v172
	s_waitcnt lgkmcnt(7)
	v_fmac_f32_e32 v65, v154, v174
	s_waitcnt lgkmcnt(5)
	v_fmac_f32_e32 v66, v154, v187
	v_fmac_f32_e32 v50, v155, v148
	v_fmac_f32_e32 v51, v155, v168
	v_fmac_f32_e32 v52, v155, v166
	v_fmac_f32_e32 v53, v155, v195
	v_fmac_f32_e32 v54, v155, v170
	v_fmac_f32_e32 v55, v155, v172
	v_fmac_f32_e32 v56, v155, v174
	v_fmac_f32_e32 v57, v155, v187
	v_fmac_f32_e32 v42, v152, v148
	v_fmac_f32_e32 v43, v152, v168
	v_fmac_f32_e32 v44, v152, v166
	v_fmac_f32_e32 v45, v152, v195
	v_fmac_f32_e32 v46, v152, v170
	v_fmac_f32_e32 v47, v152, v172
	v_fmac_f32_e32 v48, v152, v174
	v_fmac_f32_e32 v49, v152, v187
	v_fmac_f32_e32 v34, v153, v148
	v_fmac_f32_e32 v35, v153, v168
	v_fmac_f32_e32 v36, v153, v166
	v_fmac_f32_e32 v37, v153, v195
	v_fmac_f32_e32 v38, v153, v170
	v_fmac_f32_e32 v39, v153, v172
	v_fmac_f32_e32 v40, v153, v174
	v_fmac_f32_e32 v41, v153, v187
	s_waitcnt lgkmcnt(4)
	v_fmac_f32_e32 v26, v156, v148
	v_fmac_f32_e32 v27, v156, v168
	v_fmac_f32_e32 v28, v156, v166
	v_fmac_f32_e32 v29, v156, v195
	v_fmac_f32_e32 v30, v156, v170
	v_fmac_f32_e32 v31, v156, v172
	v_fmac_f32_e32 v32, v156, v174
	v_fmac_f32_e32 v33, v156, v187
	v_fmac_f32_e32 v18, v157, v148
	v_fmac_f32_e32 v19, v157, v168
	v_fmac_f32_e32 v20, v157, v166
	v_fmac_f32_e32 v21, v157, v195
	v_fmac_f32_e32 v22, v157, v170
	v_fmac_f32_e32 v23, v157, v172
	v_fmac_f32_e32 v24, v157, v174
	v_fmac_f32_e32 v25, v157, v187
	v_fmac_f32_e32 v10, v124, v148
	v_fmac_f32_e32 v11, v124, v168
	v_fmac_f32_e32 v12, v124, v166
	v_fmac_f32_e32 v13, v124, v195
	v_fmac_f32_e32 v14, v124, v170
	v_fmac_f32_e32 v15, v124, v172
	v_fmac_f32_e32 v16, v124, v174
	v_fmac_f32_e32 v17, v124, v187
	v_fmac_f32_e32 v2, v125, v148
	v_fmac_f32_e32 v3, v125, v168
	v_fmac_f32_e32 v4, v125, v166
	v_fmac_f32_e32 v5, v125, v195
	v_fmac_f32_e32 v6, v125, v170
	v_fmac_f32_e32 v7, v125, v172
	v_fmac_f32_e32 v8, v125, v174
	v_fmac_f32_e32 v9, v125, v187
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v66, v197, v188
	v_fmac_f32_e32 v65, v197, v175
	v_fmac_f32_e32 v64, v197, v173
	v_fmac_f32_e32 v63, v197, v171
	v_fmac_f32_e32 v62, v197, v196
	v_fmac_f32_e32 v61, v197, v167
	v_fmac_f32_e32 v60, v197, v169
	v_fmac_f32_e32 v59, v197, v149
	v_fmac_f32_e32 v57, v198, v188
	v_fmac_f32_e32 v56, v198, v175
	v_fmac_f32_e32 v55, v198, v173
	v_fmac_f32_e32 v54, v198, v171
	v_fmac_f32_e32 v53, v198, v196
	v_fmac_f32_e32 v52, v198, v167
	v_fmac_f32_e32 v51, v198, v169
	v_fmac_f32_e32 v50, v198, v149
	v_fmac_f32_e32 v49, v158, v188
	v_fmac_f32_e32 v48, v158, v175
	v_fmac_f32_e32 v47, v158, v173
	v_fmac_f32_e32 v46, v158, v171
	v_fmac_f32_e32 v45, v158, v196
	v_fmac_f32_e32 v44, v158, v167
	v_fmac_f32_e32 v43, v158, v169
	v_fmac_f32_e32 v42, v158, v149
	v_fmac_f32_e32 v41, v159, v188
	v_fmac_f32_e32 v40, v159, v175
	v_fmac_f32_e32 v39, v159, v173
	v_fmac_f32_e32 v38, v159, v171
	v_fmac_f32_e32 v37, v159, v196
	v_fmac_f32_e32 v36, v159, v167
	v_fmac_f32_e32 v35, v159, v169
	v_fmac_f32_e32 v34, v159, v149
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v33, v189, v188
	v_fmac_f32_e32 v32, v189, v175
	v_fmac_f32_e32 v31, v189, v173
	v_fmac_f32_e32 v30, v189, v171
	v_fmac_f32_e32 v29, v189, v196
	v_fmac_f32_e32 v28, v189, v167
	v_fmac_f32_e32 v27, v189, v169
	v_fmac_f32_e32 v26, v189, v149
	v_fmac_f32_e32 v25, v190, v188
	v_fmac_f32_e32 v24, v190, v175
	v_fmac_f32_e32 v23, v190, v173
	v_fmac_f32_e32 v22, v190, v171
	v_fmac_f32_e32 v21, v190, v196
	v_fmac_f32_e32 v20, v190, v167
	v_fmac_f32_e32 v19, v190, v169
	v_fmac_f32_e32 v18, v190, v149
	s_waitcnt lgkmcnt(0)
	v_fmac_f32_e32 v17, v193, v188
	v_fmac_f32_e32 v9, v194, v188
	v_fmac_f32_e32 v16, v193, v175
	v_fmac_f32_e32 v8, v194, v175
	v_fmac_f32_e32 v15, v193, v173
	v_fmac_f32_e32 v7, v194, v173
	v_fmac_f32_e32 v14, v193, v171
	v_fmac_f32_e32 v6, v194, v171
	v_fmac_f32_e32 v13, v193, v196
	v_fmac_f32_e32 v5, v194, v196
	v_fmac_f32_e32 v12, v193, v167
	v_fmac_f32_e32 v4, v194, v167
	v_fmac_f32_e32 v11, v193, v169
	v_fmac_f32_e32 v3, v194, v169
	v_fmac_f32_e32 v10, v193, v149
	v_fmac_f32_e32 v2, v194, v149
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	s_barrier
	s_waitcnt vmcnt(0) lgkmcnt(0)
	s_waitcnt_vscnt null, 0x0
	buffer_gl0_inv
	s_cbranch_scc1 BB0_2
; %bb.3:
	v_mov_b32_e32 v58, v53
	v_mov_b32_e32 v67, v52
	v_mov_b32_e32 v83, v51
	v_mov_b32_e32 v87, v50
	s_branch BB0_5
BB0_4:                                  ; %._crit_edge
	s_mov_b32 s19, s12
	s_mov_b32 s13, s12
	s_mov_b32 s14, s12
	s_mov_b32 s15, s12
	s_mov_b32 s16, s12
	s_mov_b32 s17, s12
	s_mov_b32 s18, s12
	v_mov_b32_e32 v2, s12
	v_mov_b32_e32 v9, s19
	v_mov_b32_e32 v3, s13
	v_mov_b32_e32 v4, s14
	v_mov_b32_e32 v5, s15
	v_mov_b32_e32 v6, s16
	v_mov_b32_e32 v7, s17
	v_mov_b32_e32 v8, s18
	v_mov_b32_e32 v17, v9
	v_mov_b32_e32 v25, v9
	v_mov_b32_e32 v33, v9
	v_mov_b32_e32 v41, v9
	v_mov_b32_e32 v49, v9
	v_mov_b32_e32 v57, v9
	s_lshl_b32 s2, s8, 4
	v_mov_b32_e32 v58, 0
	v_mov_b32_e32 v16, v8
	v_mov_b32_e32 v15, v7
	v_mov_b32_e32 v14, v6
	v_mov_b32_e32 v13, v5
	v_mov_b32_e32 v12, v4
	v_mov_b32_e32 v11, v3
	v_mov_b32_e32 v10, v2
	v_mov_b32_e32 v24, v8
	v_mov_b32_e32 v23, v7
	v_mov_b32_e32 v22, v6
	v_mov_b32_e32 v21, v5
	v_mov_b32_e32 v20, v4
	v_mov_b32_e32 v19, v3
	v_mov_b32_e32 v18, v2
	v_mov_b32_e32 v32, v8
	v_mov_b32_e32 v31, v7
	v_mov_b32_e32 v30, v6
	v_mov_b32_e32 v29, v5
	v_mov_b32_e32 v28, v4
	v_mov_b32_e32 v27, v3
	v_mov_b32_e32 v26, v2
	v_mov_b32_e32 v40, v8
	v_mov_b32_e32 v39, v7
	v_mov_b32_e32 v38, v6
	v_mov_b32_e32 v37, v5
	v_mov_b32_e32 v36, v4
	v_mov_b32_e32 v35, v3
	v_mov_b32_e32 v34, v2
	v_mov_b32_e32 v48, v8
	v_mov_b32_e32 v47, v7
	v_mov_b32_e32 v46, v6
	v_mov_b32_e32 v45, v5
	v_mov_b32_e32 v44, v4
	v_mov_b32_e32 v43, v3
	v_mov_b32_e32 v42, v2
	v_mov_b32_e32 v67, 0
	v_mov_b32_e32 v83, 0
	v_mov_b32_e32 v87, 0
	v_mov_b32_e32 v66, 0
	v_mov_b32_e32 v65, 0
	v_mov_b32_e32 v64, 0
	v_mov_b32_e32 v63, 0
	v_mov_b32_e32 v62, 0
	v_mov_b32_e32 v61, 0
	v_mov_b32_e32 v60, 0
	v_mov_b32_e32 v59, 0
	v_mov_b32_e32 v56, v8
	v_mov_b32_e32 v55, v7
	v_mov_b32_e32 v54, v6
	v_mov_b32_e32 v53, v5
	v_mov_b32_e32 v52, v4
	v_mov_b32_e32 v51, v3
	v_mov_b32_e32 v50, v2
BB0_5:
	v_add_nc_u32_e32 v0, s6, v0
	v_mov_b32_e32 v51, 0
	s_movk_i32 s4, 0x50
	s_movk_i32 s5, 0x60
	s_movk_i32 s6, 0x70
	v_mul_lo_u32 v0, v0, s8
	v_mov_b32_e32 v53, v51
	v_mov_b32_e32 v71, v51
	v_mov_b32_e32 v73, v51
	v_mov_b32_e32 v75, v51
	v_mov_b32_e32 v77, v51
	v_add3_u32 v50, s3, v1, v0
	v_mov_b32_e32 v1, v51
	v_lshlrev_b64 v[78:79], 2, v[50:51]
	v_add_nc_u32_e32 v52, 16, v50
	v_add_nc_u32_e32 v70, 32, v50
	v_add_nc_u32_e32 v0, 48, v50
	v_add_nc_u32_e32 v72, 64, v50
	v_add_nc_u32_e32 v74, s4, v50
	v_lshlrev_b64 v[52:53], 2, v[52:53]
	v_lshlrev_b64 v[70:71], 2, v[70:71]
	v_lshlrev_b64 v[0:1], 2, v[0:1]
	v_add_co_u32_e64 v81, vcc_lo, s0, v78
	v_lshlrev_b64 v[72:73], 2, v[72:73]
	v_lshlrev_b64 v[74:75], 2, v[74:75]
	v_add_nc_u32_e32 v76, s5, v50
	v_add_nc_u32_e32 v78, s6, v50
	v_add_co_ci_u32_e32 v82, vcc_lo, s1, v79, vcc_lo
	v_add_co_u32_e64 v90, vcc_lo, s0, v52
	v_mov_b32_e32 v79, v51
	v_add_nc_u32_e32 v50, s2, v50
	global_store_dword v[81:82], v59, off
	v_add_co_ci_u32_e32 v91, vcc_lo, s1, v53, vcc_lo
	v_add_co_u32_e64 v70, vcc_lo, s0, v70
	v_lshlrev_b64 v[52:53], 2, v[76:77]
	global_store_dword v[90:91], v60, off
	v_add_co_ci_u32_e32 v71, vcc_lo, s1, v71, vcc_lo
	v_add_co_u32_e64 v0, vcc_lo, s0, v0
	v_lshlrev_b64 v[59:60], 2, v[78:79]
	global_store_dword v[70:71], v61, off
	v_add_co_ci_u32_e32 v1, vcc_lo, s1, v1, vcc_lo
	v_add_co_u32_e64 v72, vcc_lo, s0, v72
	v_add_co_ci_u32_e32 v73, vcc_lo, s1, v73, vcc_lo
	v_add_co_u32_e64 v70, vcc_lo, s0, v74
	global_store_dword v[0:1], v62, off
	v_add_co_ci_u32_e32 v71, vcc_lo, s1, v75, vcc_lo
	v_add_co_u32_e64 v79, vcc_lo, s0, v52
	v_lshlrev_b64 v[74:75], 2, v[50:51]
	global_store_dword v[72:73], v63, off
	global_store_dword v[70:71], v64, off
	v_add_co_ci_u32_e32 v80, vcc_lo, s1, v53, vcc_lo
	v_add_nc_u32_e32 v0, 16, v50
	v_mov_b32_e32 v1, v51
	v_add_co_u32_e64 v59, vcc_lo, s0, v59
	v_add_nc_u32_e32 v63, 32, v50
	v_mov_b32_e32 v64, v51
	v_add_nc_u32_e32 v70, 48, v50
	v_lshlrev_b64 v[0:1], 2, v[0:1]
	v_add_co_ci_u32_e32 v60, vcc_lo, s1, v60, vcc_lo
	v_add_co_u32_e64 v61, vcc_lo, s0, v74
	v_lshlrev_b64 v[52:53], 2, v[63:64]
	v_mov_b32_e32 v71, v51
	global_store_dword v[79:80], v65, off
	v_add_nc_u32_e32 v63, s4, v50
	v_add_co_ci_u32_e32 v62, vcc_lo, s1, v75, vcc_lo
	global_store_dword v[59:60], v66, off
	v_add_co_u32_e64 v0, vcc_lo, s0, v0
	v_add_nc_u32_e32 v65, s5, v50
	global_store_dword v[61:62], v87, off
	v_lshlrev_b64 v[59:60], 2, v[70:71]
	v_add_co_ci_u32_e32 v1, vcc_lo, s1, v1, vcc_lo
	v_add_co_u32_e64 v52, vcc_lo, s0, v52
	v_add_nc_u32_e32 v61, 64, v50
	v_mov_b32_e32 v62, v51
	v_mov_b32_e32 v66, v51
	v_add_co_ci_u32_e32 v53, vcc_lo, s1, v53, vcc_lo
	global_store_dword v[0:1], v83, off
	v_add_co_u32_e64 v59, vcc_lo, s0, v59
	v_lshlrev_b64 v[61:62], 2, v[61:62]
	v_add_co_ci_u32_e32 v60, vcc_lo, s1, v60, vcc_lo
	global_store_dword v[52:53], v67, off
	global_store_dword v[59:60], v58, off
	v_lshlrev_b64 v[0:1], 2, v[63:64]
	v_add_co_u32_e64 v60, vcc_lo, s0, v61
	v_lshlrev_b64 v[52:53], 2, v[65:66]
	v_add_nc_u32_e32 v58, s6, v50
	v_mov_b32_e32 v59, v51
	v_add_nc_u32_e32 v50, s2, v50
	v_add_co_ci_u32_e32 v61, vcc_lo, s1, v62, vcc_lo
	v_mov_b32_e32 v65, v51
	v_add_co_u32_e64 v0, vcc_lo, s0, v0
	v_lshlrev_b64 v[58:59], 2, v[58:59]
	v_lshlrev_b64 v[62:63], 2, v[50:51]
	global_store_dword v[60:61], v54, off
	v_add_nc_u32_e32 v60, 16, v50
	v_add_co_ci_u32_e32 v1, vcc_lo, s1, v1, vcc_lo
	v_add_co_u32_e64 v66, vcc_lo, s0, v52
	v_mov_b32_e32 v61, v51
	v_add_nc_u32_e32 v64, 32, v50
	global_store_dword v[0:1], v55, off
	v_add_co_ci_u32_e32 v67, vcc_lo, s1, v53, vcc_lo
	v_add_co_u32_e64 v58, vcc_lo, s0, v58
	v_lshlrev_b64 v[0:1], 2, v[60:61]
	v_lshlrev_b64 v[52:53], 2, v[64:65]
	v_add_nc_u32_e32 v60, s4, v50
	v_add_co_ci_u32_e32 v59, vcc_lo, s1, v59, vcc_lo
	global_store_dword v[66:67], v56, off
	global_store_dword v[58:59], v57, off
	v_add_co_u32_e64 v67, vcc_lo, s0, v62
	v_add_nc_u32_e32 v56, 48, v50
	v_mov_b32_e32 v57, v51
	v_add_nc_u32_e32 v58, 64, v50
	v_mov_b32_e32 v59, v51
	v_add_co_ci_u32_e32 v68, vcc_lo, s1, v63, vcc_lo
	v_add_co_u32_e64 v0, vcc_lo, s0, v0
	v_lshlrev_b64 v[56:57], 2, v[56:57]
	v_lshlrev_b64 v[54:55], 2, v[58:59]
	global_store_dword v[67:68], v42, off
	v_add_co_ci_u32_e32 v1, vcc_lo, s1, v1, vcc_lo
	v_add_co_u32_e64 v62, vcc_lo, s0, v52
	v_add_nc_u32_e32 v52, s5, v50
	v_add_co_ci_u32_e32 v63, vcc_lo, s1, v53, vcc_lo
	v_add_co_u32_e64 v42, vcc_lo, s0, v56
	global_store_dword v[0:1], v43, off
	v_mov_b32_e32 v53, v51
	v_add_nc_u32_e32 v56, s6, v50
	global_store_dword v[62:63], v44, off
	v_lshlrev_b64 v[0:1], 2, v[60:61]
	v_add_co_ci_u32_e32 v43, vcc_lo, s1, v57, vcc_lo
	v_add_co_u32_e64 v59, vcc_lo, s0, v54
	v_lshlrev_b64 v[52:53], 2, v[52:53]
	v_mov_b32_e32 v57, v51
	v_add_nc_u32_e32 v50, s2, v50
	v_add_co_ci_u32_e32 v60, vcc_lo, s1, v55, vcc_lo
	global_store_dword v[42:43], v45, off
	v_add_co_u32_e64 v0, vcc_lo, s0, v0
	v_lshlrev_b64 v[56:57], 2, v[56:57]
	v_add_nc_u32_e32 v54, 16, v50
	global_store_dword v[59:60], v46, off
	v_mov_b32_e32 v55, v51
	v_add_co_ci_u32_e32 v1, vcc_lo, s1, v1, vcc_lo
	v_add_co_u32_e64 v42, vcc_lo, s0, v52
	v_add_nc_u32_e32 v44, 48, v50
	v_mov_b32_e32 v45, v51
	v_add_co_ci_u32_e32 v43, vcc_lo, s1, v53, vcc_lo
	v_add_co_u32_e64 v59, vcc_lo, s0, v56
	v_lshlrev_b64 v[52:53], 2, v[50:51]
	global_store_dword v[0:1], v47, off
	v_add_nc_u32_e32 v56, 32, v50
	v_lshlrev_b64 v[44:45], 2, v[44:45]
	v_add_co_ci_u32_e32 v60, vcc_lo, s1, v57, vcc_lo
	global_store_dword v[42:43], v48, off
	v_mov_b32_e32 v57, v51
	global_store_dword v[59:60], v49, off
	v_lshlrev_b64 v[0:1], 2, v[54:55]
	v_add_co_u32_e64 v55, vcc_lo, s0, v52
	v_lshlrev_b64 v[42:43], 2, v[56:57]
	v_add_nc_u32_e32 v48, 64, v50
	v_mov_b32_e32 v49, v51
	v_add_nc_u32_e32 v52, s4, v50
	v_add_co_ci_u32_e32 v56, vcc_lo, s1, v53, vcc_lo
	v_mov_b32_e32 v53, v51
	v_add_co_u32_e64 v0, vcc_lo, s0, v0
	v_lshlrev_b64 v[48:49], 2, v[48:49]
	v_add_co_ci_u32_e32 v1, vcc_lo, s1, v1, vcc_lo
	v_add_co_u32_e64 v42, vcc_lo, s0, v42
	global_store_dword v[55:56], v34, off
	v_add_co_ci_u32_e32 v43, vcc_lo, s1, v43, vcc_lo
	v_add_co_u32_e64 v59, vcc_lo, s0, v44
	global_store_dword v[0:1], v35, off
	v_add_co_ci_u32_e32 v60, vcc_lo, s1, v45, vcc_lo
	global_store_dword v[42:43], v36, off
	v_mov_b32_e32 v45, v51
	global_store_dword v[59:60], v37, off
	v_lshlrev_b64 v[0:1], 2, v[52:53]
	v_add_nc_u32_e32 v34, s5, v50
	v_mov_b32_e32 v35, v51
	v_add_co_u32_e64 v36, vcc_lo, s0, v48
	v_add_nc_u32_e32 v42, s6, v50
	v_mov_b32_e32 v43, v51
	v_add_nc_u32_e32 v50, s2, v50
	v_lshlrev_b64 v[34:35], 2, v[34:35]
	v_add_co_ci_u32_e32 v37, vcc_lo, s1, v49, vcc_lo
	v_add_co_u32_e64 v0, vcc_lo, s0, v0
	v_lshlrev_b64 v[42:43], 2, v[42:43]
	v_add_nc_u32_e32 v44, 16, v50
	global_store_dword v[36:37], v38, off
	v_add_co_ci_u32_e32 v1, vcc_lo, s1, v1, vcc_lo
	v_lshlrev_b64 v[36:37], 2, v[50:51]
	v_add_co_u32_e64 v34, vcc_lo, s0, v34
	v_add_nc_u32_e32 v38, 32, v50
	v_add_co_ci_u32_e32 v35, vcc_lo, s1, v35, vcc_lo
	global_store_dword v[0:1], v39, off
	global_store_dword v[34:35], v40, off
	v_add_co_u32_e64 v34, vcc_lo, s0, v42
	v_lshlrev_b64 v[0:1], 2, v[44:45]
	v_mov_b32_e32 v39, v51
	v_add_nc_u32_e32 v42, 48, v50
	v_add_nc_u32_e32 v44, 64, v50
	v_add_co_ci_u32_e32 v35, vcc_lo, s1, v43, vcc_lo
	v_add_co_u32_e64 v36, vcc_lo, s0, v36
	v_lshlrev_b64 v[38:39], 2, v[38:39]
	v_mov_b32_e32 v43, v51
	v_add_nc_u32_e32 v40, s6, v50
	global_store_dword v[34:35], v41, off
	v_add_co_ci_u32_e32 v37, vcc_lo, s1, v37, vcc_lo
	v_add_co_u32_e64 v0, vcc_lo, s0, v0
	v_lshlrev_b64 v[34:35], 2, v[42:43]
	v_mov_b32_e32 v41, v51
	v_add_co_ci_u32_e32 v1, vcc_lo, s1, v1, vcc_lo
	global_store_dword v[36:37], v26, off
	global_store_dword v[0:1], v27, off
	v_add_co_u32_e64 v26, vcc_lo, s0, v38
	v_lshlrev_b64 v[0:1], 2, v[44:45]
	v_add_nc_u32_e32 v36, s4, v50
	v_mov_b32_e32 v37, v51
	v_add_nc_u32_e32 v38, s5, v50
	v_add_co_ci_u32_e32 v27, vcc_lo, s1, v39, vcc_lo
	v_add_co_u32_e64 v34, vcc_lo, s0, v34
	v_mov_b32_e32 v39, v51
	v_lshlrev_b64 v[36:37], 2, v[36:37]
	v_add_nc_u32_e32 v50, s2, v50
	global_store_dword v[26:27], v28, off
	v_add_co_ci_u32_e32 v35, vcc_lo, s1, v35, vcc_lo
	v_add_co_u32_e64 v0, vcc_lo, s0, v0
	v_lshlrev_b64 v[26:27], 2, v[38:39]
	v_add_nc_u32_e32 v38, 32, v50
	v_add_co_ci_u32_e32 v1, vcc_lo, s1, v1, vcc_lo
	v_add_co_u32_e64 v28, vcc_lo, s0, v36
	global_store_dword v[34:35], v29, off
	v_add_nc_u32_e32 v36, 16, v50
	global_store_dword v[0:1], v30, off
	v_lshlrev_b64 v[0:1], 2, v[40:41]
	v_add_co_ci_u32_e32 v29, vcc_lo, s1, v37, vcc_lo
	v_add_co_u32_e64 v26, vcc_lo, s0, v26
	v_lshlrev_b64 v[34:35], 2, v[50:51]
	v_mov_b32_e32 v37, v51
	v_add_co_ci_u32_e32 v27, vcc_lo, s1, v27, vcc_lo
	global_store_dword v[28:29], v31, off
	v_add_co_u32_e64 v43, vcc_lo, s0, v0
	v_add_co_ci_u32_e32 v44, vcc_lo, s1, v1, vcc_lo
	v_lshlrev_b64 v[0:1], 2, v[36:37]
	global_store_dword v[26:27], v32, off
	global_store_dword v[43:44], v33, off
	v_lshlrev_b64 v[26:27], 2, v[38:39]
	v_add_co_u32_e64 v39, vcc_lo, s0, v34
	v_add_nc_u32_e32 v28, 48, v50
	v_mov_b32_e32 v29, v51
	v_add_nc_u32_e32 v32, 64, v50
	v_mov_b32_e32 v33, v51
	v_add_co_ci_u32_e32 v40, vcc_lo, s1, v35, vcc_lo
	v_add_co_u32_e64 v0, vcc_lo, s0, v0
	v_lshlrev_b64 v[28:29], 2, v[28:29]
	v_add_nc_u32_e32 v34, s4, v50
	v_lshlrev_b64 v[32:33], 2, v[32:33]
	v_mov_b32_e32 v35, v51
	v_add_co_ci_u32_e32 v1, vcc_lo, s1, v1, vcc_lo
	v_add_co_u32_e64 v26, vcc_lo, s0, v26
	global_store_dword v[39:40], v18, off
	v_add_co_ci_u32_e32 v27, vcc_lo, s1, v27, vcc_lo
	v_add_co_u32_e64 v43, vcc_lo, s0, v28
	global_store_dword v[0:1], v19, off
	v_add_co_ci_u32_e32 v44, vcc_lo, s1, v29, vcc_lo
	global_store_dword v[26:27], v20, off
	v_mov_b32_e32 v29, v51
	global_store_dword v[43:44], v21, off
	v_lshlrev_b64 v[0:1], 2, v[34:35]
	v_add_nc_u32_e32 v18, s5, v50
	v_mov_b32_e32 v19, v51
	v_add_co_u32_e64 v20, vcc_lo, s0, v32
	v_add_nc_u32_e32 v26, s6, v50
	v_mov_b32_e32 v27, v51
	v_add_nc_u32_e32 v50, s2, v50
	v_lshlrev_b64 v[18:19], 2, v[18:19]
	v_add_co_ci_u32_e32 v21, vcc_lo, s1, v33, vcc_lo
	v_add_co_u32_e64 v0, vcc_lo, s0, v0
	v_lshlrev_b64 v[26:27], 2, v[26:27]
	v_add_nc_u32_e32 v28, 16, v50
	global_store_dword v[20:21], v22, off
	v_add_co_ci_u32_e32 v1, vcc_lo, s1, v1, vcc_lo
	v_lshlrev_b64 v[20:21], 2, v[50:51]
	v_add_co_u32_e64 v18, vcc_lo, s0, v18
	v_add_nc_u32_e32 v22, 32, v50
	v_add_co_ci_u32_e32 v19, vcc_lo, s1, v19, vcc_lo
	global_store_dword v[0:1], v23, off
	global_store_dword v[18:19], v24, off
	v_add_co_u32_e64 v18, vcc_lo, s0, v26
	v_lshlrev_b64 v[0:1], 2, v[28:29]
	v_mov_b32_e32 v23, v51
	v_add_nc_u32_e32 v26, 48, v50
	v_add_nc_u32_e32 v28, 64, v50
	v_add_co_ci_u32_e32 v19, vcc_lo, s1, v27, vcc_lo
	v_add_co_u32_e64 v20, vcc_lo, s0, v20
	v_lshlrev_b64 v[22:23], 2, v[22:23]
	v_mov_b32_e32 v27, v51
	v_add_nc_u32_e32 v24, s6, v50
	global_store_dword v[18:19], v25, off
	v_add_co_ci_u32_e32 v21, vcc_lo, s1, v21, vcc_lo
	v_add_co_u32_e64 v0, vcc_lo, s0, v0
	v_lshlrev_b64 v[18:19], 2, v[26:27]
	v_mov_b32_e32 v25, v51
	v_add_co_ci_u32_e32 v1, vcc_lo, s1, v1, vcc_lo
	global_store_dword v[20:21], v10, off
	global_store_dword v[0:1], v11, off
	v_add_co_u32_e64 v10, vcc_lo, s0, v22
	v_lshlrev_b64 v[0:1], 2, v[28:29]
	v_add_nc_u32_e32 v20, s4, v50
	v_mov_b32_e32 v21, v51
	v_add_nc_u32_e32 v22, s5, v50
	v_add_co_ci_u32_e32 v11, vcc_lo, s1, v23, vcc_lo
	v_add_co_u32_e64 v18, vcc_lo, s0, v18
	v_mov_b32_e32 v23, v51
	v_lshlrev_b64 v[20:21], 2, v[20:21]
	v_add_nc_u32_e32 v50, s2, v50
	global_store_dword v[10:11], v12, off
	v_add_co_ci_u32_e32 v19, vcc_lo, s1, v19, vcc_lo
	v_add_co_u32_e64 v0, vcc_lo, s0, v0
	v_lshlrev_b64 v[10:11], 2, v[22:23]
	v_add_nc_u32_e32 v22, 32, v50
	v_add_co_ci_u32_e32 v1, vcc_lo, s1, v1, vcc_lo
	v_add_co_u32_e64 v12, vcc_lo, s0, v20
	global_store_dword v[18:19], v13, off
	global_store_dword v[0:1], v14, off
	v_lshlrev_b64 v[0:1], 2, v[24:25]
	v_add_co_ci_u32_e32 v13, vcc_lo, s1, v21, vcc_lo
	v_add_co_u32_e64 v10, vcc_lo, s0, v10
	v_lshlrev_b64 v[20:21], 2, v[50:51]
	v_add_nc_u32_e32 v18, 16, v50
	v_mov_b32_e32 v19, v51
	global_store_dword v[12:13], v15, off
	v_add_co_ci_u32_e32 v11, vcc_lo, s1, v11, vcc_lo
	v_add_nc_u32_e32 v14, 48, v50
	v_add_co_u32_e64 v27, vcc_lo, s0, v0
	v_lshlrev_b64 v[12:13], 2, v[18:19]
	v_mov_b32_e32 v15, v51
	v_add_co_ci_u32_e32 v28, vcc_lo, s1, v1, vcc_lo
	v_add_co_u32_e64 v19, vcc_lo, s0, v20
	v_lshlrev_b64 v[0:1], 2, v[22:23]
	global_store_dword v[10:11], v16, off
	global_store_dword v[27:28], v17, off
	v_add_co_ci_u32_e32 v20, vcc_lo, s1, v21, vcc_lo
	v_add_co_u32_e64 v12, vcc_lo, s0, v12
	v_lshlrev_b64 v[10:11], 2, v[14:15]
	v_add_nc_u32_e32 v16, 64, v50
	v_mov_b32_e32 v17, v51
	global_store_dword v[19:20], v2, off
	v_add_co_ci_u32_e32 v13, vcc_lo, s1, v13, vcc_lo
	v_add_co_u32_e64 v22, vcc_lo, s0, v0
	v_add_nc_u32_e32 v2, s4, v50
	v_add_co_ci_u32_e32 v23, vcc_lo, s1, v1, vcc_lo
	v_lshlrev_b64 v[0:1], 2, v[16:17]
	global_store_dword v[12:13], v3, off
	v_add_co_u32_e64 v10, vcc_lo, s0, v10
	global_store_dword v[22:23], v4, off
	v_mov_b32_e32 v3, v51
	v_add_nc_u32_e32 v12, s5, v50
	v_mov_b32_e32 v13, v51
	v_add_co_ci_u32_e32 v11, vcc_lo, s1, v11, vcc_lo
	v_add_co_u32_e64 v0, vcc_lo, s0, v0
	v_lshlrev_b64 v[2:3], 2, v[2:3]
	v_add_nc_u32_e32 v50, s6, v50
	v_lshlrev_b64 v[12:13], 2, v[12:13]
	v_add_co_ci_u32_e32 v1, vcc_lo, s1, v1, vcc_lo
	global_store_dword v[10:11], v5, off
	v_lshlrev_b64 v[14:15], 2, v[50:51]
	v_add_co_u32_e64 v17, vcc_lo, s0, v2
	v_add_co_ci_u32_e32 v18, vcc_lo, s1, v3, vcc_lo
	v_add_co_u32_e64 v2, vcc_lo, s0, v12
	global_store_dword v[0:1], v6, off
	v_add_co_ci_u32_e32 v3, vcc_lo, s1, v13, vcc_lo
	v_add_co_u32_e64 v14, vcc_lo, s0, v14
	global_store_dword v[17:18], v7, off
	v_add_co_ci_u32_e32 v15, vcc_lo, s1, v15, vcc_lo
	global_store_dword v[2:3], v8, off
	global_store_dword v[14:15], v9, off
	s_endpgm
	.section	.rodata,#alloc
	.p2align	6
	.amdhsa_kernel mul
		.amdhsa_group_segment_fixed_size 16896
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
		.amdhsa_system_sgpr_workgroup_id_y 1
		.amdhsa_system_sgpr_workgroup_id_z 0
		.amdhsa_system_sgpr_workgroup_info 0
		.amdhsa_system_vgpr_workitem_id 1
		.amdhsa_next_free_vgpr 215
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
; codeLenInByte = 10740
; NumSgprs: 35
; NumVgprs: 215
; ScratchSize: 0
; MemoryBound: 0
; FloatMode: 240
; IeeeMode: 1
; LDSByteSize: 16896 bytes/workgroup (compile time only)
; SGPRBlocks: 4
; VGPRBlocks: 26
; NumSGPRsForWavesPerEU: 35
; NumVGPRsForWavesPerEU: 215
; Occupancy: 4
; WaveLimiterHint : 1
; COMPUTE_PGM_RSRC2:USER_SGPR: 6
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
    .group_segment_fixed_size: 16896
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
    .vgpr_count:     215
    .vgpr_spill_count: 0
    .wavefront_size: 32
amdhsa.version:
  - 1
  - 0
...

	.end_amdgpu_metadata
