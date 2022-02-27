//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_ID_H__
#define __I3D_ID_H__

#define XE_ID_MUL_0 (uint64_t) (1)
#define XE_ID_MUL_1 (uint64_t) (38)
#define XE_ID_MUL_2 (uint64_t) ((XE_ID_MUL_1) * (uint64_t) 38)
#define XE_ID_MUL_3 (uint64_t) ((XE_ID_MUL_2) * (uint64_t) 38)
#define XE_ID_MUL_4 (uint64_t) ((XE_ID_MUL_3) * (uint64_t) 38)
#define XE_ID_MUL_5 (uint64_t) ((XE_ID_MUL_4) * (uint64_t) 38)
#define XE_ID_MUL_6 (uint64_t) ((XE_ID_MUL_5) * (uint64_t) 38)
#define XE_ID_MUL_7 (uint64_t) ((XE_ID_MUL_6) * (uint64_t) 38)
#define XE_ID_MUL_8 (uint64_t) ((XE_ID_MUL_7) * (uint64_t) 38)
#define XE_ID_MUL_9 (uint64_t) ((XE_ID_MUL_8) * (uint64_t) 38)
#define XE_ID_MUL_10 (uint64_t) ((XE_ID_MUL_9) * (uint64_t) 38)
#define XE_ID_MUL_11 (uint64_t) ((XE_ID_MUL_10) * (uint64_t) 38)

#define XE_ID_CHAR_0 0
#define XE_ID_CHAR_1 1
#define XE_ID_CHAR_2 2
#define XE_ID_CHAR_3 3
#define XE_ID_CHAR_4 4
#define XE_ID_CHAR_5 5
#define XE_ID_CHAR_6 6
#define XE_ID_CHAR_7 7
#define XE_ID_CHAR_8 8
#define XE_ID_CHAR_9 9
#define XE_ID_CHAR__ 10
#define XE_ID_CHAR_SPACE 11
#define XE_ID_CHAR_space 11

#define XE_ID_CHAR_A 12
#define XE_ID_CHAR_B 13
#define XE_ID_CHAR_C 14
#define XE_ID_CHAR_D 15
#define XE_ID_CHAR_E 16
#define XE_ID_CHAR_F 17
#define XE_ID_CHAR_G 18
#define XE_ID_CHAR_H 19
#define XE_ID_CHAR_I 20
#define XE_ID_CHAR_J 21
#define XE_ID_CHAR_K 22
#define XE_ID_CHAR_L 23
#define XE_ID_CHAR_M 24
#define XE_ID_CHAR_N 25
#define XE_ID_CHAR_O 26
#define XE_ID_CHAR_P 27
#define XE_ID_CHAR_Q 28
#define XE_ID_CHAR_R 29
#define XE_ID_CHAR_S 30
#define XE_ID_CHAR_T 31
#define XE_ID_CHAR_U 32
#define XE_ID_CHAR_V 33
#define XE_ID_CHAR_W 34
#define XE_ID_CHAR_X 35
#define XE_ID_CHAR_Y 36
#define XE_ID_CHAR_Z 37

#define XE_ID_CHAR_a 12
#define XE_ID_CHAR_b 13
#define XE_ID_CHAR_c 14
#define XE_ID_CHAR_d 15
#define XE_ID_CHAR_e 16
#define XE_ID_CHAR_f 17
#define XE_ID_CHAR_g 18
#define XE_ID_CHAR_h 19
#define XE_ID_CHAR_i 20
#define XE_ID_CHAR_j 21
#define XE_ID_CHAR_k 22
#define XE_ID_CHAR_l 23
#define XE_ID_CHAR_m 24
#define XE_ID_CHAR_n 25
#define XE_ID_CHAR_o 26
#define XE_ID_CHAR_p 27
#define XE_ID_CHAR_q 28
#define XE_ID_CHAR_r 29
#define XE_ID_CHAR_s 30
#define XE_ID_CHAR_t 31
#define XE_ID_CHAR_u 32
#define XE_ID_CHAR_v 33
#define XE_ID_CHAR_w 34
#define XE_ID_CHAR_x 35
#define XE_ID_CHAR_y 36
#define XE_ID_CHAR_z 37

#define XE_MAKE_ID(c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11)\
	(uint64_t) ((XE_ID_CHAR_##c0 * XE_ID_MUL_0) +\
			    (XE_ID_CHAR_##c1 * XE_ID_MUL_1) +\
				(XE_ID_CHAR_##c2 * XE_ID_MUL_2) +\
				(XE_ID_CHAR_##c3 * XE_ID_MUL_3) +\
			    (XE_ID_CHAR_##c4 * XE_ID_MUL_4) +\
				(XE_ID_CHAR_##c5 * XE_ID_MUL_5) +\
				(XE_ID_CHAR_##c6 * XE_ID_MUL_6) +\
				(XE_ID_CHAR_##c7 * XE_ID_MUL_7) +\
				(XE_ID_CHAR_##c8 * XE_ID_MUL_8) +\
				(XE_ID_CHAR_##c9 * XE_ID_MUL_9) +\
				(XE_ID_CHAR_##c10 * XE_ID_MUL_10) +\
				(XE_ID_CHAR_##c11 * XE_ID_MUL_11))

#endif
