#ifndef _GPU_H
#define _GPU_H

#include <iostream>
#include "common.h"

class gpu
{
private:
	u8 ZB[SCREEN_DEPTH];

	u128 register_file[12];
	u128 constant_mem[96];
	u64 instruction_mem[128];

	u128 VAB[16];
	u128 VOB[16];

	// Helper methods
	u8 get_opcode(u64);

	u8 get_dest_type(u64);
	u8 get_dest_index(u64);
	u8 get_dest_w(u64);
	u8 get_dest_z(u64);
	u8 get_dest_y(u64);
	u8 get_dest_x(u64);

	u8 get_srcN_neg(u64, u8);
	u8 get_srcN_type(u64, u8);
	u8 get_srcN_index(u64, u8);
	u8 get_srcN_w(u64, u8);
	u8 get_srcN_z(u64, u8);
	u8 get_srcN_y(u64, u8);
	u8 get_srcN_x(u64, u8);

	void debug(u64);

	u128 swizzle(u128, u8, u8, u8, u8);
	// u32 get_w_comp(u128);
	// u32 get_z_comp(u128);
	// u32 get_y_comp(u128);
	// u32 get_x_comp(u128);

	// Instuction execution methods
	void mov_instr(u64);

public:
	gpu();
	~gpu();

	void execute( u8[][SCREEN_WIDTH][3]);
	void load_const_mem();
	void load_microcode();
	void load_vab();
	
};

#endif