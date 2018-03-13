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

public:
	gpu();
	~gpu();

	void execute();
	void load_const_mem();
	void load_microcode();
	
};

#endif