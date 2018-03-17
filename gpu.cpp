#include "gpu.h"

#include <iostream>
using namespace std;

gpu::gpu()
{

}

gpu::~gpu()
{
	
}

void gpu::execute( u8 FB[][SCREEN_WIDTH][3])
{
	// Initialize program counter
	u8 pc = 0;
	u8 opcode;
	u128 instruction;

	bool end = false;

	// DEBUG
	u64 w = 14183429296869597645;

	// u8 b = get_bits(w, 15, 1);

	u8 b = get_opcode(w);
	cout << (u16)b << endl;

	b = get_dest_type(w);
	cout << (u16)b << endl;

	b = get_dest_index(w);
	cout << (u16)b << endl;

	b = get_dest_w(w);
	cout << (u16)b << endl;

	b = get_dest_z(w);
	cout << (u16)b << endl;

	b = get_dest_y(w);
	cout << (u16)b << endl;

	b = get_dest_x(w);
	cout << (u16)b << endl;

	b = get_srcN_neg(w, 0);
	cout << (u16)b << endl;

	b = get_srcN_neg(w, 1);
	cout << (u16)b << endl;

	b = get_srcN_neg(w, 2);
	cout << (u16)b << endl;


	// DEBUG

	// Start decoding
	// while (!end)
	// {
	// 	// Fetch
	// 	instruction = instruction_mem[pc++];

	// 	// Decode
	// 	switch(opcode)
	// 	{
	// 		default:
	// 		end = true;
	// 		break;
	// 	}
	// }

}

void gpu::load_const_mem()
{

}

void gpu::load_microcode()
{

}

/********************************************************
 *					Helper Methods						*
 *******************************************************/

u8 gpu::get_opcode(u64 instr)
{
	instr >>= 58;
	return (u8) instr;
}

u8 gpu::get_dest_type(u64 instr)
{
	instr &= 0x100000000000000;
	instr >>= 56;
	return (u8)instr;
}

u8 gpu::get_dest_index(u64 instr)
{
	instr &= 0xF0000000000000;
	instr >>= 52;
	return (u8)instr;
}

u8 gpu::get_dest_w(u64 instr)
{
	instr &= 0x8000000000000;
	instr >>= 51;
	return (u8)instr;
}

u8 gpu::get_dest_z(u64 instr)
{
	instr &= 0x4000000000000;
	instr >>= 50;
	return (u8)instr;
}

u8 gpu::get_dest_y(u64 instr)
{
	instr &= 0x2000000000000;
	instr >>= 49;
	return (u8)instr;
}

u8 gpu::get_dest_x(u64 instr)
{
	instr &= 0x1000000000000;
	instr >>= 48;
	return (u8)instr;
}


u8 gpu::get_srcN_neg(u64 instr, u8 n)
{
	instr &= 0x800000000000 >> (15*n);
	instr >>= 47 - 15*n;
	return (u8)instr;
}

u8 gpu::get_srcN_type(u64 instr, u8)
{

}

u8 gpu::get_srcN_index(u64 instr, u8)
{

}

u8 gpu::get_srcN_w(u64 instr, u8)
{

}

u8 gpu::get_srcN_z(u64 instr, u8)
{

}

u8 gpu::get_srcN_y(u64 instr, u8)
{

}

u8 gpu::get_srcN_x(u64 instr, u8)
{

}



u8 gpu::get_bits(u64 num, u16 k, u16 p)
{
	return (((1 << k) - 1) & ( num >> (p - 1)));
}