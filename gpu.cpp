#include "gpu.h"

#include <iostream>
using namespace std;

#include <stdio.h>

gpu::gpu()
{

}

gpu::~gpu()
{
	
}

void gpu::execute( u8 FB[][SCREEN_WIDTH][3] )
{
	// Initialize program counter
	u8 pc = 0;
	u8 opcode;
	u64 instruction;

	bool end = false;

	// Start decoding
	while (!end)
	{
		// Fetch
		instruction = instruction_mem[pc++];

		opcode = get_opcode(instruction);

		// Decode
		switch(opcode)
		{
			case 0x01:		// MOV
				mov_instr(instruction);

				break;

			case 0x02:		// MUL

				break;

			case 0x03:		// ADD

				break;

			case 0x04:		// MAD

				break;

			case 0x05:		// DST

				break;

			case 0x06:		// MIN

				break;

			case 0x07:		// MAX

				break;

			case 0x08:		// SLT

				break;

			case 0x09:		// SGE

				break;

			case 0x0A:		// RCP

				break;

			case 0x0B:		// RSQ

				break;

			case 0x0C:		// DP3

				break;

			case 0x0D:		// DP4

				break;

			case 0x0E:		// LOG

				break;

			case 0x0F:		// EXP

				break;

			case 0x10:		// LIT

				break;

			case 0x11:		// ARL

				break;

			case 0xFF:
			default:
				end = true;
				break;
		}
	}

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

void gpu::mov_instr(u64 instr)
{

	u128 temp;

	// Determine destination and source paths
	u8 dest = get_dest_type(instr);
	u8 destIndex = get_dest_index(instr);
	u8 destW = get_dest_w(instr);
	u8 destZ = get_dest_z(instr);
	u8 destY = get_dest_y(instr);
	u8 destX = get_dest_x(instr);

	u8 src = get_srcN_type(instr, 0);
	u8 srcIndex = get_srcN_index(instr, 0);
	u8 srcW = get_srcN_w(instr, 0);
	u8 srcZ = get_srcN_z(instr, 0);
	u8 srcY = get_srcN_y(instr, 0);
	u8 srcX = get_srcN_x(instr, 0);

	// mov r[], r[]
	if ( dest == 0 && src == 0 )
	{
		// We gotta swizzle now..
		temp = swizzle(register_file[srcIndex], srcW, srcZ, srcY, srcX);

		// Negate?
		if (get_srcN_neg(instr, 0))
		{
			temp.x = -temp.x;
			temp.y = -temp.y;
			temp.z = -temp.z;
			temp.w = -temp.w;
		}

		// Finally any masks?
		if (destW) register_file[destIndex].w = temp.w;
		if (destZ) register_file[destIndex].z = temp.z;
		if (destY) register_file[destIndex].y = temp.y;
		if (destX) register_file[destIndex].x = temp.x;

	}
	// mov r[], v[] 
	else if ( dest == 0 && src == 1 )
	{
		// We gotta swizzle now..
		temp = swizzle(VAB[srcIndex], srcW, srcZ, srcY, srcX);

		// Negate?
		if (get_srcN_neg(instr, 0))
		{
			temp.x = -temp.x;
			temp.y = -temp.y;
			temp.z = -temp.z;
			temp.w = -temp.w;
		}

		// Finally any masks?
		if (destW) register_file[destIndex].w = temp.w;
		if (destZ) register_file[destIndex].z = temp.z;
		if (destY) register_file[destIndex].y = temp.y;
		if (destX) register_file[destIndex].x = temp.x;

	}
	// mov r[], c[]
	else if ( dest == 0 && src == 2 )
	{
		// We gotta swizzle now..
		temp = swizzle(constant_mem[srcIndex], srcW, srcZ, srcY, srcX);

		// Negate?
		if (get_srcN_neg(instr, 0))
		{
			temp.x = -temp.x;
			temp.y = -temp.y;
			temp.z = -temp.z;
			temp.w = -temp.w;
		}

		// Finally any masks?
		if (destW) register_file[destIndex].w = temp.w;
		if (destZ) register_file[destIndex].z = temp.z;
		if (destY) register_file[destIndex].y = temp.y;
		if (destX) register_file[destIndex].x = temp.x;
	}
	// mov o[], r[]
	else if ( dest == 1 && src == 0 )
	{
		// We gotta swizzle now..
		temp = swizzle(register_file[srcIndex], srcW, srcZ, srcY, srcX);

		// Negate?
		if (get_srcN_neg(instr, 0))
		{
			temp.x = -temp.x;
			temp.y = -temp.y;
			temp.z = -temp.z;
			temp.w = -temp.w;
		}

		// Finally any masks?
		if (destW) VOB[destIndex].w = temp.w;
		if (destZ) VOB[destIndex].z = temp.z;
		if (destY) VOB[destIndex].y = temp.y;
		if (destX) VOB[destIndex].x = temp.x;	
	}
	// mov o[], v[] 
	else if ( dest == 1 && src == 1 )
	{
		// We gotta swizzle now..
		temp = swizzle(VAB[srcIndex], srcW, srcZ, srcY, srcX);

		// Negate?
		if (get_srcN_neg(instr, 0))
		{
			temp.x = -temp.x;
			temp.y = -temp.y;
			temp.z = -temp.z;
			temp.w = -temp.w;
		}

		// Finally any masks?
		if (destW) VOB[destIndex].w = temp.w;
		if (destZ) VOB[destIndex].z = temp.z;
		if (destY) VOB[destIndex].y = temp.y;
		if (destX) VOB[destIndex].x = temp.x;
	}
	// mov o[], c[]
	else if ( dest == 1 && src == 2 )
	{
		// We gotta swizzle now..
		temp = swizzle(constant_mem[srcIndex], srcW, srcZ, srcY, srcX);

		// Negate?
		if (get_srcN_neg(instr, 0))
		{
			temp.x = -temp.x;
			temp.y = -temp.y;
			temp.z = -temp.z;
			temp.w = -temp.w;
		}

		// Finally any masks?
		if (destW) VOB[destIndex].w = temp.w;
		if (destZ) VOB[destIndex].z = temp.z;
		if (destY) VOB[destIndex].y = temp.y;
		if (destX) VOB[destIndex].x = temp.x;
	}


}

u128 gpu::swizzle(u128 data, u8 w, u8 z, u8 y, u8 x)
{
	// No swizzle 
	if (w == 3 && z == 2 && y == 1 && x == 0)
		return data;

	u32 temp[4];

	// Tear apart all componets, put them in array..
	temp[0] = data.x;
	temp[1] = data.y;
	temp[2] = data.z;
	temp[3] = data.w;

	data.x = temp[x];
	data.y = temp[y];
	data.z = temp[z];
	data.w = temp[w];

	return data;
}
