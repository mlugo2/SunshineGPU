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
	u128 instruction_mem[128];

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

	u8 get_bits(u64, u16, u16);

public:
	gpu();
	~gpu();

	void execute( u8[][SCREEN_WIDTH][3]);
	void load_const_mem();
	void load_microcode();
	void load_vab();
	
};

#endif