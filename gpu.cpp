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

u8 gpu::get_srcN_type(u64 instr, u8 n)
{
	instr &= 0x600000000000 >> (15*n);
	instr >>= 45 - 15*n;
	return (u8)instr;
}

u8 gpu::get_srcN_index(u64 instr, u8 n)
{
	instr &= 0x1E0000000000 >> (15*n);
	instr >>= 41 - 15*n;
	return (u8)instr;
}

u8 gpu::get_srcN_w(u64 instr, u8 n)
{
	instr &= 0x18000000000 >> (15*n);
	instr >>= 39 - 15*n;
	return (u8)instr;
}

u8 gpu::get_srcN_z(u64 instr, u8 n)
{
	instr &= 0x6000000000 >> (15*n);
	instr >>= 37 - 15*n;
	return (u8)instr;
}

u8 gpu::get_srcN_y(u64 instr, u8 n)
{
	instr &= 0x1800000000 >> (15*n);
	instr >>= 35 - 15*n;
	return (u8)instr;
}

u8 gpu::get_srcN_x(u64 instr, u8 n)
{
	instr &= 0x600000000 >> (15*n);
	instr >>= 33 - 15*n;
	return (u8)instr;
}

void gpu::debug(u64 instr)
{
// DEBUG
	u64 w = instr;

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
	cout <<"source0 neg: "<< (u16)b << endl;

	b = get_srcN_type(w, 0);
	cout <<"source0 type: "<< (u16)b << endl;

	b = get_srcN_index(w, 0);
	cout <<"source0 index: "<< (u16)b << endl;

	b = get_srcN_w(w, 0);
	cout <<"source0 w: "<< (u16)b << endl;

	b = get_srcN_z(w, 0);
	cout <<"source0 z: "<< (u16)b << endl;

	b = get_srcN_y(w, 0);
	cout <<"source0 y: "<< (u16)b << endl;

	b = get_srcN_x(w, 0);
	cout <<"source0 x: "<< (u16)b << endl;

	cout << "-------------------" << endl;

	b = get_srcN_neg(w, 1);
	cout <<"source1 neg: "<< (u16)b << endl;

	b = get_srcN_type(w, 1);
	cout <<"source1 type: "<< (u16)b << endl;

	b = get_srcN_index(w, 1);
	cout <<"source1 index: "<< (u16)b << endl;

	b = get_srcN_w(w, 1);
	cout <<"source1 w: "<< (u16)b << endl;

	b = get_srcN_z(w, 1);
	cout <<"source1 z: "<< (u16)b << endl;

	b = get_srcN_y(w, 1);
	cout <<"source1 y: "<< (u16)b << endl;

	b = get_srcN_x(w, 1);
	cout <<"source1 x: "<< (u16)b << endl;

	cout << "-------------------" << endl;

	b = get_srcN_neg(w, 2);
	cout <<"source2 neg: "<< (u16)b << endl;

	b = get_srcN_type(w, 2);
	cout <<"source2 type: "<< (u16)b << endl;

	b = get_srcN_index(w, 2);
	cout <<"source2 index: "<< (u16)b << endl;

	b = get_srcN_w(w, 2);
	cout <<"source2 w: "<< (u16)b << endl;

	b = get_srcN_z(w, 2);
	cout <<"source2 z: "<< (u16)b << endl;

	b = get_srcN_y(w, 2);
	cout <<"source2 y: "<< (u16)b << endl;

	b = get_srcN_x(w, 2);
	cout <<"source2 x: "<< (u16)b << endl;


	// DEBUG
}