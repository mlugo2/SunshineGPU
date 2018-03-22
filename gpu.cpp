#include "gpu.h"

#include <iostream>
#include <cmath>
using namespace std;

gpu::gpu()
{

}

gpu::~gpu()
{
	
}

void gpu::execute( u8 FB[][SCREEN_WIDTH][3] )
{
	geometry_processor();
	rendering_engine(FB);
}

void gpu::load_const_mem()
{
	// Transform matrix
	constant_mem[0].x = 1.0; constant_mem[0].y = 0.0; constant_mem[0].z = 0.0; constant_mem[0].w = 0.0;
	constant_mem[1].x = 0.0; constant_mem[1].y = 1.0; constant_mem[1].z = 0.0; constant_mem[1].w = 0.0;
	constant_mem[2].x = 0.0; constant_mem[2].y = 0.0; constant_mem[2].z = 1.0; constant_mem[2].w = 0.0;
	constant_mem[3].x = 0.0; constant_mem[3].y = 0.0; constant_mem[3].z = 0.0; constant_mem[3].w = 1.0;

	// Light vector
	constant_mem[4].x = 0.0; constant_mem[4].y = 0.0; constant_mem[4].z = 0.0; constant_mem[4].w = 0.0;
}

void gpu::load_microcode(vector<u64> vs)
{
	int index = 0;
	for ( auto i : vs )
	{
		instruction_mem[index++] = i;
	}
}

void gpu::load_vab( vector<vector<float> > v, vector<int> f)
{
	// Load face data into VAB
	// Get x, y, z for first vertex
	main_mem[VERT_REG][0].x = v[f[1]][0]; 
	main_mem[VERT_REG][0].y = v[f[1]][1]; 
	main_mem[VERT_REG][0].z = v[f[1]][2];
	main_mem[VERT_REG][0].w = 1.0;

	// Get position for second vertex
	main_mem[VERT_REG][1].x = v[f[2]][0]; 
	main_mem[VERT_REG][1].y = v[f[2]][1]; 
	main_mem[VERT_REG][1].z = v[f[2]][2];
	main_mem[VERT_REG][1].w = 1.0;

	// Get position for thrid vertex
	main_mem[VERT_REG][2].x = v[f[3]][0]; 
	main_mem[VERT_REG][2].y = v[f[3]][1]; 
	main_mem[VERT_REG][2].z = v[f[3]][2];
	main_mem[VERT_REG][2].w = 1.0;

	// Calculate initial normal
	float V[] { main_mem[VERT_REG][1].x - main_mem[VERT_REG][0].x, main_mem[VERT_REG][1].y - main_mem[VERT_REG][0].y, main_mem[VERT_REG][1].z - main_mem[VERT_REG][0].z};
	float W[] { main_mem[VERT_REG][2].x - main_mem[VERT_REG][0].x, main_mem[VERT_REG][2].y - main_mem[VERT_REG][0].y, main_mem[VERT_REG][2].z - main_mem[VERT_REG][0].z};
	main_mem[VERT_REG][3].x = (V[1] * W[2]) - (V[2] * W[1]);
	main_mem[VERT_REG][3].y = (V[2] * W[0]) - (V[0] * W[2]);
	main_mem[VERT_REG][3].z = (V[0] * W[1]) - (V[1] * W[0]);
	main_mem[VERT_REG][3].w = 1.0;

	// Set default color
	main_mem[VERT_REG][4].x = 15;
	main_mem[VERT_REG][4].y = 90;
	main_mem[VERT_REG][4].z = 23;
	main_mem[VERT_REG][4].w = 1.0;

}


/********************************************************
 *					Pipeline Components					*
 *******************************************************/

void gpu::geometry_processor()
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
				mul_instr(instruction);
				break;

			case 0x03:		// ADD
				add_instr(instruction);
				break;

			case 0x04:		// MAD
				mad_instr(instruction);
				break;

			case 0x05:		// DST
				dst_instr(instruction);
				break;

			case 0x06:		// MIN
				min_instr(instruction);
				break;

			case 0x07:		// MAX
				max_instr(instruction);
				break;

			case 0x08:		// SLT
				slt_instr(instruction);
				break;

			case 0x09:		// SGE
				sge_instr(instruction);
				break;

			case 0x0A:		// RCP
				rcp_instr(instruction);
				break;

			case 0x0B:		// RSQ
				rsq_instr(instruction);
				break;

			case 0x0C:		// DP3
				dp3_instr(instruction);
				break;

			case 0x0D:		// DP4
				dp4_instr(instruction);
				break;

			case 0x0E:		// LOG
				log_instr(instruction);
				break;

			case 0x0F:		// EXP
				exp_instr(instruction);
				break;

			case 0x10:		// LIT
				lit_instr(instruction);
				break;

			case 0x11:		// ARL

				break;

			case 0x3F:
			default:
				end = true;
				break;
		}
	} // end of while
}

void gpu::rendering_engine(u8 FB[][SCREEN_WIDTH][3])
{
	// Here we do all the projection and other stuff..
	projection(VOB);
	rasteration(VOB, FB);

	// cout << "VOB stuff......." << endl;
	// cout << VOB[0].x << " " << VOB[0].y << " " << VOB[0].z << " " << VOB[0].w << " " << endl;
	// cout << VOB[1].x << " " << VOB[1].y << " " << VOB[1].z << " " << VOB[1].w << " " << endl;
	// cout << VOB[2].x << " " << VOB[2].y << " " << VOB[2].z << " " << VOB[2].w << " " << endl;
	// cout << VOB[3].x << " " << VOB[3].y << " " << VOB[3].z << " " << VOB[3].w << " " << endl;
	// cout << endl;
}

void gpu::projection(u128 *VOB)
{
	// Project V1
	VOB[0].x = ( ( ( VOB[0].x/4 ) + 1 ) / 2 ) * SCREEN_WIDTH;
	VOB[0].y = ( ( ( (-VOB[0].y)/4 ) + 1 ) / 2 ) * SCREEN_HEIGHT;
	VOB[0].z = ( ( ( VOB[0].z/4 ) + 1 ) / 2 ) * SCREEN_DEPTH;

	// Project V2
	VOB[1].x = ( ( ( VOB[1].x/4 ) + 1 ) / 2 ) * SCREEN_WIDTH;
	VOB[1].y = ( ( ( (-VOB[1].y)/4 ) + 1 ) / 2 ) * SCREEN_HEIGHT;
	VOB[1].z = ( ( ( VOB[1].z/4 ) + 1 ) / 2 ) * SCREEN_DEPTH;

	// Project V3
	VOB[2].x = ( ( ( VOB[2].x/4 ) + 1 ) / 2 ) * SCREEN_WIDTH;
	VOB[2].y = ( ( ( (-VOB[2].y)/4 ) + 1 ) / 2 ) * SCREEN_HEIGHT;
	VOB[2].z = ( ( ( VOB[2].z/4 ) + 1 ) / 2 ) * SCREEN_DEPTH;
}

void gpu::rasteration(u128 VOB[], u8 FB[][SCREEN_WIDTH][3])
{
	// Start rendering (≧д≦ヾ)
	// initialize 
	int c1, c2, c3, z;
	int xMin, xMax, yMin, yMax;

	// Get min and max from the triangle
	xMin = getXMin(VOB);
	xMax = getXMax(VOB);
	yMin = getYMin(VOB);
	yMax = getYMax(VOB);

	// Going through every pixel of the frame buffer..
	for ( int j = yMin; j < yMax; j++ )
	{
		for ( int k = xMin; k < xMax; k++)
		{
			// Calculate somethings
			c1 = perpDotProduct(VOB[0], VOB[1], k, j);
			c2 = perpDotProduct(VOB[1], VOB[2], k, j);
			c3 = perpDotProduct(VOB[2], VOB[0], k, j);

			// If true then pixel is in the triangle
			if ( (c1 > 0 && c2 > 0 && c3 > 0) || (c1 < 0 && c2 < 0 && c3 < 0) )
			{
				// Get z
				z = getZ(VOB, k, j);

				// Write to buffer or not
				if ( z < ZB[j][k])
				{
					ZB[j][k] = (u8)z;

					// Render baby, render
					FB[j][k][0] = VOB[3].x;	// R
					FB[j][k][1] = VOB[3].y; // G
					FB[j][k][2] = VOB[3].z; // B
				}
			}
		}
	}
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

	// Swizzle it
	temp = swizzle(main_mem[src][srcIndex], srcW, srcZ, srcY, srcX);

	// Negate it
	if (get_srcN_neg(instr, 0))
	{
		temp.x = -temp.x;
		temp.y = -temp.y;
		temp.z = -temp.z;
		temp.w = -temp.w;
	}

	// Bop it..
	if ( dest == TEMP_REG )
	{
		if (destW) main_mem[dest][destIndex].w = temp.w;
		if (destZ) main_mem[dest][destIndex].z = temp.z;
		if (destY) main_mem[dest][destIndex].y = temp.y;
		if (destX) main_mem[dest][destIndex].x = temp.x;
	}
	else
	{
		if (destW) VOB[destIndex].w = temp.w;
		if (destZ) VOB[destIndex].z = temp.z;
		if (destY) VOB[destIndex].y = temp.y;
		if (destX) VOB[destIndex].x = temp.x;
	}

}

void gpu::mul_instr(u64 instr)
{
	u128 temp0, temp1;

	// Determine destination and source paths
	u8 dest = get_dest_type(instr);
	u8 destIndex = get_dest_index(instr);
	u8 destW = get_dest_w(instr);
	u8 destZ = get_dest_z(instr);
	u8 destY = get_dest_y(instr);
	u8 destX = get_dest_x(instr);

	u8 src0 = get_srcN_type(instr, 0);
	u8 src0Index = get_srcN_index(instr, 0);
	u8 src0W = get_srcN_w(instr, 0);
	u8 src0Z = get_srcN_z(instr, 0);
	u8 src0Y = get_srcN_y(instr, 0);
	u8 src0X = get_srcN_x(instr, 0);

	u8 src1 = get_srcN_type(instr, 1);
	u8 src1Index = get_srcN_index(instr, 1);
	u8 src1W = get_srcN_w(instr, 1);
	u8 src1Z = get_srcN_z(instr, 1);
	u8 src1Y = get_srcN_y(instr, 1);
	u8 src1X = get_srcN_x(instr, 1);

	// Swizzle it
	temp0 = swizzle(main_mem[src0][src0Index], src0W, src0Z, src0Y, src0X);

	temp1 = swizzle(main_mem[src1][src1Index], src1W, src1Z, src1Y, src1X);

	// Negate it
	if (get_srcN_neg(instr, 0))
	{
		temp0.x = -temp0.x;
		temp0.y = -temp0.y;
		temp0.z = -temp0.z;
		temp0.w = -temp0.w;
	}

	if (get_srcN_neg(instr, 1))
	{
		temp1.x = -temp1.x;
		temp1.y = -temp1.y;
		temp1.z = -temp1.z;
		temp1.w = -temp1.w;
	}

	// Bop it..
	if ( dest == TEMP_REG )
	{
		if (destW) main_mem[dest][destIndex].w = temp0.w * temp1.w;
		if (destZ) main_mem[dest][destIndex].z = temp0.z * temp1.z;
		if (destY) main_mem[dest][destIndex].y = temp0.y * temp1.y;
		if (destX) main_mem[dest][destIndex].x = temp0.x * temp1.x;
	}
	else
	{
		if (destW) VOB[destIndex].w = temp0.w * temp1.w;
		if (destZ) VOB[destIndex].z = temp0.z * temp1.z;
		if (destY) VOB[destIndex].y = temp0.y * temp1.y;
		if (destX) VOB[destIndex].x = temp0.x * temp1.x;
	}

}

void gpu::add_instr(u64 instr)
{
	u128 temp0, temp1;

	// Determine destination and source paths
	u8 dest = get_dest_type(instr);
	u8 destIndex = get_dest_index(instr);
	u8 destW = get_dest_w(instr);
	u8 destZ = get_dest_z(instr);
	u8 destY = get_dest_y(instr);
	u8 destX = get_dest_x(instr);

	u8 src0 = get_srcN_type(instr, 0);
	u8 src0Index = get_srcN_index(instr, 0);
	u8 src0W = get_srcN_w(instr, 0);
	u8 src0Z = get_srcN_z(instr, 0);
	u8 src0Y = get_srcN_y(instr, 0);
	u8 src0X = get_srcN_x(instr, 0);

	u8 src1 = get_srcN_type(instr, 1);
	u8 src1Index = get_srcN_index(instr, 1);
	u8 src1W = get_srcN_w(instr, 1);
	u8 src1Z = get_srcN_z(instr, 1);
	u8 src1Y = get_srcN_y(instr, 1);
	u8 src1X = get_srcN_x(instr, 1);

	// Swizzle it
	temp0 = swizzle(main_mem[src0][src0Index], src0W, src0Z, src0Y, src0X);

	temp1 = swizzle(main_mem[src1][src1Index], src1W, src1Z, src1Y, src1X);

	// Negate it
	if (get_srcN_neg(instr, 0))
	{
		temp0.x = -temp0.x;
		temp0.y = -temp0.y;
		temp0.z = -temp0.z;
		temp0.w = -temp0.w;
	}

	if (get_srcN_neg(instr, 1))
	{
		temp1.x = -temp1.x;
		temp1.y = -temp1.y;
		temp1.z = -temp1.z;
		temp1.w = -temp1.w;
	}

	// Bop it..
	if ( dest == TEMP_REG )
	{
		if (destW) main_mem[dest][destIndex].w = temp0.w + temp1.w;
		if (destZ) main_mem[dest][destIndex].z = temp0.z + temp1.z;
		if (destY) main_mem[dest][destIndex].y = temp0.y + temp1.y;
		if (destX) main_mem[dest][destIndex].x = temp0.x + temp1.x;
	}
	else
	{
		if (destW) VOB[destIndex].w = temp0.w + temp1.w;
		if (destZ) VOB[destIndex].z = temp0.z + temp1.z;
		if (destY) VOB[destIndex].y = temp0.y + temp1.y;
		if (destX) VOB[destIndex].x = temp0.x + temp1.x;
	}
}

void gpu::mad_instr(u64 instr)
{
	u128 temp0, temp1, temp2;

	// Determine destination and source paths
	u8 dest = get_dest_type(instr);
	u8 destIndex = get_dest_index(instr);
	u8 destW = get_dest_w(instr);
	u8 destZ = get_dest_z(instr);
	u8 destY = get_dest_y(instr);
	u8 destX = get_dest_x(instr);

	u8 src0 = get_srcN_type(instr, 0);
	u8 src0Index = get_srcN_index(instr, 0);
	u8 src0W = get_srcN_w(instr, 0);
	u8 src0Z = get_srcN_z(instr, 0);
	u8 src0Y = get_srcN_y(instr, 0);
	u8 src0X = get_srcN_x(instr, 0);

	u8 src1 = get_srcN_type(instr, 1);
	u8 src1Index = get_srcN_index(instr, 1);
	u8 src1W = get_srcN_w(instr, 1);
	u8 src1Z = get_srcN_z(instr, 1);
	u8 src1Y = get_srcN_y(instr, 1);
	u8 src1X = get_srcN_x(instr, 1);

	u8 src2 = get_srcN_type(instr, 2);
	u8 src2Index = get_srcN_index(instr, 2);
	u8 src2W = get_srcN_w(instr, 2);
	u8 src2Z = get_srcN_z(instr, 2);
	u8 src2Y = get_srcN_y(instr, 2);
	u8 src2X = get_srcN_x(instr, 2);

	// Swizzle it
	temp0 = swizzle(main_mem[src0][src0Index], src0W, src0Z, src0Y, src0X);

	temp1 = swizzle(main_mem[src1][src1Index], src1W, src1Z, src1Y, src1X);

	temp2 = swizzle(main_mem[src2][src2Index], src2W, src2Z, src2Y, src2X);

	// Negate it
	if (get_srcN_neg(instr, 0))
	{
		temp0.x = -temp0.x;
		temp0.y = -temp0.y;
		temp0.z = -temp0.z;
		temp0.w = -temp0.w;
	}

	if (get_srcN_neg(instr, 1))
	{
		temp1.x = -temp1.x;
		temp1.y = -temp1.y;
		temp1.z = -temp1.z;
		temp1.w = -temp1.w;
	}

	if (get_srcN_neg(instr, 2))
	{
		temp2.x = -temp2.x;
		temp2.y = -temp2.y;
		temp2.z = -temp2.z;
		temp2.w = -temp2.w;
	}

	// Bop it..
	if ( dest == TEMP_REG )
	{
		if (destW) main_mem[dest][destIndex].w = temp0.w * temp1.w + temp2.w;
		if (destZ) main_mem[dest][destIndex].z = temp0.z * temp1.z + temp2.z;
		if (destY) main_mem[dest][destIndex].y = temp0.y * temp1.y + temp2.y;
		if (destX) main_mem[dest][destIndex].x = temp0.x * temp1.x + temp2.x;
	}
	else
	{
		if (destW) VOB[destIndex].w = temp0.w * temp1.w + temp2.w;
		if (destZ) VOB[destIndex].z = temp0.z * temp1.z + temp2.z;
		if (destY) VOB[destIndex].y = temp0.y * temp1.y + temp2.y;
		if (destX) VOB[destIndex].x = temp0.x * temp1.x + temp2.x;
	}
}

void gpu::dst_instr(u64 instr)
{
	u128 temp0, temp1;

	// Determine destination and source paths
	u8 dest = get_dest_type(instr);
	u8 destIndex = get_dest_index(instr);
	u8 destW = get_dest_w(instr);
	u8 destZ = get_dest_z(instr);
	u8 destY = get_dest_y(instr);
	u8 destX = get_dest_x(instr);

	u8 src0 = get_srcN_type(instr, 0);
	u8 src0Index = get_srcN_index(instr, 0);
	u8 src0W = get_srcN_w(instr, 0);
	u8 src0Z = get_srcN_z(instr, 0);
	u8 src0Y = get_srcN_y(instr, 0);
	u8 src0X = get_srcN_x(instr, 0);

	u8 src1 = get_srcN_type(instr, 1);
	u8 src1Index = get_srcN_index(instr, 1);
	u8 src1W = get_srcN_w(instr, 1);
	u8 src1Z = get_srcN_z(instr, 1);
	u8 src1Y = get_srcN_y(instr, 1);
	u8 src1X = get_srcN_x(instr, 1);

	// Swizzle it
	temp0 = swizzle(main_mem[src0][src0Index], src0W, src0Z, src0Y, src0X);

	temp1 = swizzle(main_mem[src1][src1Index], src1W, src1Z, src1Y, src1X);

	// Negate it
	if (get_srcN_neg(instr, 0))
	{
		temp0.y = -temp0.y;
		temp0.z = -temp0.z;
	}

	if (get_srcN_neg(instr, 1))
	{
		temp1.y = -temp1.y;
		temp1.w = -temp1.w;
	}

	// Bop it..
	if ( dest == TEMP_REG )
	{
		if (destW) main_mem[dest][destIndex].w = temp1.w;
		if (destZ) main_mem[dest][destIndex].z = temp0.z;
		if (destY) main_mem[dest][destIndex].y = temp0.y * temp1.y;
		if (destX) main_mem[dest][destIndex].x = 1.0;
	}
	else
	{
		if (destW) VOB[destIndex].w = temp1.w;
		if (destZ) VOB[destIndex].z = temp0.z;
		if (destY) VOB[destIndex].y = temp0.y * temp1.y;
		if (destX) VOB[destIndex].x = 1.0;
	}	
}

void gpu::min_instr(u64 instr)
{
	u128 temp0, temp1;

	// Determine destination and source paths
	u8 dest = get_dest_type(instr);
	u8 destIndex = get_dest_index(instr);
	u8 destW = get_dest_w(instr);
	u8 destZ = get_dest_z(instr);
	u8 destY = get_dest_y(instr);
	u8 destX = get_dest_x(instr);

	u8 src0 = get_srcN_type(instr, 0);
	u8 src0Index = get_srcN_index(instr, 0);
	u8 src0W = get_srcN_w(instr, 0);
	u8 src0Z = get_srcN_z(instr, 0);
	u8 src0Y = get_srcN_y(instr, 0);
	u8 src0X = get_srcN_x(instr, 0);

	u8 src1 = get_srcN_type(instr, 1);
	u8 src1Index = get_srcN_index(instr, 1);
	u8 src1W = get_srcN_w(instr, 1);
	u8 src1Z = get_srcN_z(instr, 1);
	u8 src1Y = get_srcN_y(instr, 1);
	u8 src1X = get_srcN_x(instr, 1);

	// Swizzle it
	temp0 = swizzle(main_mem[src0][src0Index], src0W, src0Z, src0Y, src0X);

	temp1 = swizzle(main_mem[src1][src1Index], src1W, src1Z, src1Y, src1X);

	// Negate it
	if (get_srcN_neg(instr, 0))
	{
		temp0.x = -temp0.x;
		temp0.y = -temp0.y;
		temp0.z = -temp0.z;
		temp0.w = -temp0.w;
	}

	if (get_srcN_neg(instr, 1))
	{
		temp1.x = -temp1.x;
		temp1.y = -temp1.y;
		temp1.z = -temp1.z;
		temp1.w = -temp1.w;
	}

	// Bop it..
	if ( dest == TEMP_REG )
	{
		if (destW) main_mem[dest][destIndex].w = (temp0.w < temp1.w) ? temp0.w : temp1.w;
		if (destZ) main_mem[dest][destIndex].z = (temp0.z < temp1.z) ? temp0.z : temp1.z;
		if (destY) main_mem[dest][destIndex].y = (temp0.y < temp1.y) ? temp0.y : temp1.y;
		if (destX) main_mem[dest][destIndex].x = (temp0.x < temp1.x) ? temp0.x : temp1.x;
	}
	else
	{
		if (destW) VOB[destIndex].w = (temp0.w < temp1.w) ? temp0.w : temp1.w;
		if (destZ) VOB[destIndex].z = (temp0.z < temp1.z) ? temp0.z : temp1.z;
		if (destY) VOB[destIndex].y = (temp0.y < temp1.y) ? temp0.y : temp1.y;
		if (destX) VOB[destIndex].x = (temp0.x < temp1.x) ? temp0.x : temp1.x;
	}
}

void gpu::max_instr(u64 instr)
{
	u128 temp0, temp1;

	// Determine destination and source paths
	u8 dest = get_dest_type(instr);
	u8 destIndex = get_dest_index(instr);
	u8 destW = get_dest_w(instr);
	u8 destZ = get_dest_z(instr);
	u8 destY = get_dest_y(instr);
	u8 destX = get_dest_x(instr);

	u8 src0 = get_srcN_type(instr, 0);
	u8 src0Index = get_srcN_index(instr, 0);
	u8 src0W = get_srcN_w(instr, 0);
	u8 src0Z = get_srcN_z(instr, 0);
	u8 src0Y = get_srcN_y(instr, 0);
	u8 src0X = get_srcN_x(instr, 0);

	u8 src1 = get_srcN_type(instr, 1);
	u8 src1Index = get_srcN_index(instr, 1);
	u8 src1W = get_srcN_w(instr, 1);
	u8 src1Z = get_srcN_z(instr, 1);
	u8 src1Y = get_srcN_y(instr, 1);
	u8 src1X = get_srcN_x(instr, 1);

	// Swizzle it
	temp0 = swizzle(main_mem[src0][src0Index], src0W, src0Z, src0Y, src0X);

	temp1 = swizzle(main_mem[src1][src1Index], src1W, src1Z, src1Y, src1X);

	// Negate it
	if (get_srcN_neg(instr, 0))
	{
		temp0.x = -temp0.x;
		temp0.y = -temp0.y;
		temp0.z = -temp0.z;
		temp0.w = -temp0.w;
	}

	if (get_srcN_neg(instr, 1))
	{
		temp1.x = -temp1.x;
		temp1.y = -temp1.y;
		temp1.z = -temp1.z;
		temp1.w = -temp1.w;
	}

	// Bop it..
	if ( dest == TEMP_REG )
	{
		if (destW) main_mem[dest][destIndex].w = (temp0.w >= temp1.w) ? temp0.w : temp1.w;
		if (destZ) main_mem[dest][destIndex].z = (temp0.z >= temp1.z) ? temp0.z : temp1.z;
		if (destY) main_mem[dest][destIndex].y = (temp0.y >= temp1.y) ? temp0.y : temp1.y;
		if (destX) main_mem[dest][destIndex].x = (temp0.x >= temp1.x) ? temp0.x : temp1.x;
	}
	else
	{
		if (destW) VOB[destIndex].w = (temp0.w >= temp1.w) ? temp0.w : temp1.w;
		if (destZ) VOB[destIndex].z = (temp0.z >= temp1.z) ? temp0.z : temp1.z;
		if (destY) VOB[destIndex].y = (temp0.y >= temp1.y) ? temp0.y : temp1.y;
		if (destX) VOB[destIndex].x = (temp0.x >= temp1.x) ? temp0.x : temp1.x;
	}
}

void gpu::slt_instr(u64 instr)
{
	u128 temp0, temp1;

	// Determine destination and source paths
	u8 dest = get_dest_type(instr);
	u8 destIndex = get_dest_index(instr);
	u8 destW = get_dest_w(instr);
	u8 destZ = get_dest_z(instr);
	u8 destY = get_dest_y(instr);
	u8 destX = get_dest_x(instr);

	u8 src0 = get_srcN_type(instr, 0);
	u8 src0Index = get_srcN_index(instr, 0);
	u8 src0W = get_srcN_w(instr, 0);
	u8 src0Z = get_srcN_z(instr, 0);
	u8 src0Y = get_srcN_y(instr, 0);
	u8 src0X = get_srcN_x(instr, 0);

	u8 src1 = get_srcN_type(instr, 1);
	u8 src1Index = get_srcN_index(instr, 1);
	u8 src1W = get_srcN_w(instr, 1);
	u8 src1Z = get_srcN_z(instr, 1);
	u8 src1Y = get_srcN_y(instr, 1);
	u8 src1X = get_srcN_x(instr, 1);

	// Swizzle it
	temp0 = swizzle(main_mem[src0][src0Index], src0W, src0Z, src0Y, src0X);

	temp1 = swizzle(main_mem[src1][src1Index], src1W, src1Z, src1Y, src1X);

	// Negate it
	if (get_srcN_neg(instr, 0))
	{
		temp0.x = -temp0.x;
		temp0.y = -temp0.y;
		temp0.z = -temp0.z;
		temp0.w = -temp0.w;
	}

	if (get_srcN_neg(instr, 1))
	{
		temp1.x = -temp1.x;
		temp1.y = -temp1.y;
		temp1.z = -temp1.z;
		temp1.w = -temp1.w;
	}

	// Bop it..
	if ( dest == TEMP_REG )
	{
		if (destW) main_mem[dest][destIndex].w = (temp0.w < temp1.w) ? 1.0 : 0.0;
		if (destZ) main_mem[dest][destIndex].z = (temp0.z < temp1.z) ? 1.0 : 0.0;
		if (destY) main_mem[dest][destIndex].y = (temp0.y < temp1.y) ? 1.0 : 0.0;
		if (destX) main_mem[dest][destIndex].x = (temp0.x < temp1.x) ? 1.0 : 0.0;
	}
	else
	{
		if (destW) VOB[destIndex].w = (temp0.w < temp1.w) ? 1.0 : 0.0;
		if (destZ) VOB[destIndex].z = (temp0.z < temp1.z) ? 1.0 : 0.0;
		if (destY) VOB[destIndex].y = (temp0.y < temp1.y) ? 1.0 : 0.0;
		if (destX) VOB[destIndex].x = (temp0.x < temp1.x) ? 1.0 : 0.0;
	}
}

void gpu::sge_instr(u64 instr)
{
	u128 temp0, temp1;

	// Determine destination and source paths
	u8 dest = get_dest_type(instr);
	u8 destIndex = get_dest_index(instr);
	u8 destW = get_dest_w(instr);
	u8 destZ = get_dest_z(instr);
	u8 destY = get_dest_y(instr);
	u8 destX = get_dest_x(instr);

	u8 src0 = get_srcN_type(instr, 0);
	u8 src0Index = get_srcN_index(instr, 0);
	u8 src0W = get_srcN_w(instr, 0);
	u8 src0Z = get_srcN_z(instr, 0);
	u8 src0Y = get_srcN_y(instr, 0);
	u8 src0X = get_srcN_x(instr, 0);

	u8 src1 = get_srcN_type(instr, 1);
	u8 src1Index = get_srcN_index(instr, 1);
	u8 src1W = get_srcN_w(instr, 1);
	u8 src1Z = get_srcN_z(instr, 1);
	u8 src1Y = get_srcN_y(instr, 1);
	u8 src1X = get_srcN_x(instr, 1);

	// Swizzle it
	temp0 = swizzle(main_mem[src0][src0Index], src0W, src0Z, src0Y, src0X);

	temp1 = swizzle(main_mem[src1][src1Index], src1W, src1Z, src1Y, src1X);

	// Negate it
	if (get_srcN_neg(instr, 0))
	{
		temp0.x = -temp0.x;
		temp0.y = -temp0.y;
		temp0.z = -temp0.z;
		temp0.w = -temp0.w;
	}

	if (get_srcN_neg(instr, 1))
	{
		temp1.x = -temp1.x;
		temp1.y = -temp1.y;
		temp1.z = -temp1.z;
		temp1.w = -temp1.w;
	}

	// Bop it..
	if ( dest == TEMP_REG )
	{
		if (destW) main_mem[dest][destIndex].w = (temp0.w >= temp1.w) ? 1.0 : 0.0;
		if (destZ) main_mem[dest][destIndex].z = (temp0.z >= temp1.z) ? 1.0 : 0.0;
		if (destY) main_mem[dest][destIndex].y = (temp0.y >= temp1.y) ? 1.0 : 0.0;
		if (destX) main_mem[dest][destIndex].x = (temp0.x >= temp1.x) ? 1.0 : 0.0;
	}
	else
	{
		if (destW) VOB[destIndex].w = (temp0.w >= temp1.w) ? 1.0 : 0.0;
		if (destZ) VOB[destIndex].z = (temp0.z >= temp1.z) ? 1.0 : 0.0;
		if (destY) VOB[destIndex].y = (temp0.y >= temp1.y) ? 1.0 : 0.0;
		if (destX) VOB[destIndex].x = (temp0.x >= temp1.x) ? 1.0 : 0.0;
	}
}

void gpu::rcp_instr(u64 instr)
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

	// Swizzle it
	temp = swizzle(main_mem[src][srcIndex], srcW, srcZ, srcY, srcX);

	if (get_srcN_neg(instr, 0))
	{
		temp.x = -temp.x;
	}

	if ( temp.x == 1.0 )
		temp.x = 1;
	else {
		temp.x = 1.0 / temp.x;
	}

	// Bop it..
	if ( dest == TEMP_REG )
	{
		if (destW) main_mem[dest][destIndex].w = temp.x;
		if (destZ) main_mem[dest][destIndex].z = temp.x;
		if (destY) main_mem[dest][destIndex].y = temp.x;
		if (destX) main_mem[dest][destIndex].x = temp.x;
	}
	else
	{
		if (destW) VOB[destIndex].w = temp.x;
		if (destZ) VOB[destIndex].z = temp.x;
		if (destY) VOB[destIndex].y = temp.x;
		if (destX) VOB[destIndex].x = temp.x;
	}
}

void gpu::rsq_instr(u64 instr)
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

	// Swizzle it
	temp = swizzle(main_mem[src][srcIndex], srcW, srcZ, srcY, srcX);

	if (get_srcN_neg(instr, 0))
	{
		temp.x = -temp.x;
	}

	temp.x = 1.0 / sqrt(abs(temp.x));

	// Bop it..
	if ( dest == TEMP_REG )
	{
		if (destW) main_mem[dest][destIndex].w = temp.x;
		if (destZ) main_mem[dest][destIndex].z = temp.x;
		if (destY) main_mem[dest][destIndex].y = temp.x;
		if (destX) main_mem[dest][destIndex].x = temp.x;
	}
	else
	{
		if (destW) VOB[destIndex].w = temp.x;
		if (destZ) VOB[destIndex].z = temp.x;
		if (destY) VOB[destIndex].y = temp.x;
		if (destX) VOB[destIndex].x = temp.x;
	}
}

void gpu::dp3_instr(u64 instr)
{
	u128 temp0, temp1;

	// Determine destination and source paths
	u8 dest = get_dest_type(instr);
	u8 destIndex = get_dest_index(instr);
	u8 destW = get_dest_w(instr);
	u8 destZ = get_dest_z(instr);
	u8 destY = get_dest_y(instr);
	u8 destX = get_dest_x(instr);

	u8 src0 = get_srcN_type(instr, 0);
	u8 src0Index = get_srcN_index(instr, 0);
	u8 src0W = get_srcN_w(instr, 0);
	u8 src0Z = get_srcN_z(instr, 0);
	u8 src0Y = get_srcN_y(instr, 0);
	u8 src0X = get_srcN_x(instr, 0);

	u8 src1 = get_srcN_type(instr, 1);
	u8 src1Index = get_srcN_index(instr, 1);
	u8 src1W = get_srcN_w(instr, 1);
	u8 src1Z = get_srcN_z(instr, 1);
	u8 src1Y = get_srcN_y(instr, 1);
	u8 src1X = get_srcN_x(instr, 1);

	// Swizzle it
	temp0 = swizzle(main_mem[src0][src0Index], src0W, src0Z, src0Y, src0X);

	temp1 = swizzle(main_mem[src1][src1Index], src1W, src1Z, src1Y, src1X);

	// Negate it
	if (get_srcN_neg(instr, 0))
	{
		temp0.x = -temp0.x;
		temp0.y = -temp0.y;
		temp0.z = -temp0.z;
	}

	if (get_srcN_neg(instr, 1))
	{
		temp1.x = -temp1.x;
		temp1.y = -temp1.y;
		temp1.z = -temp1.z;
	}

	temp0.x = temp0.x * temp1.x + temp0.y * temp1.y + temp0.z * temp1.z;
	// Bop it..
	if ( dest == TEMP_REG )
	{
		if (destW) main_mem[dest][destIndex].w = temp0.x;
		if (destZ) main_mem[dest][destIndex].z = temp0.x;
		if (destY) main_mem[dest][destIndex].y = temp0.x;
		if (destX) main_mem[dest][destIndex].x = temp0.x;
	}
	else
	{
		if (destW) VOB[destIndex].w = temp0.x;
		if (destZ) VOB[destIndex].z = temp0.x;
		if (destY) VOB[destIndex].y = temp0.x;
		if (destX) VOB[destIndex].x = temp0.x;
	}
}

void gpu::dp4_instr(u64 instr)
{
	u128 temp0, temp1;

	// Determine destination and source paths
	u8 dest = get_dest_type(instr);
	u8 destIndex = get_dest_index(instr);
	u8 destW = get_dest_w(instr);
	u8 destZ = get_dest_z(instr);
	u8 destY = get_dest_y(instr);
	u8 destX = get_dest_x(instr);

	u8 src0 = get_srcN_type(instr, 0);
	u8 src0Index = get_srcN_index(instr, 0);
	u8 src0W = get_srcN_w(instr, 0);
	u8 src0Z = get_srcN_z(instr, 0);
	u8 src0Y = get_srcN_y(instr, 0);
	u8 src0X = get_srcN_x(instr, 0);

	u8 src1 = get_srcN_type(instr, 1);
	u8 src1Index = get_srcN_index(instr, 1);
	u8 src1W = get_srcN_w(instr, 1);
	u8 src1Z = get_srcN_z(instr, 1);
	u8 src1Y = get_srcN_y(instr, 1);
	u8 src1X = get_srcN_x(instr, 1);

	// Swizzle it
	temp0 = swizzle(main_mem[src0][src0Index], src0W, src0Z, src0Y, src0X);

	temp1 = swizzle(main_mem[src1][src1Index], src1W, src1Z, src1Y, src1X);

	// Negate it
	if (get_srcN_neg(instr, 0))
	{
		temp0.x = -temp0.x;
		temp0.y = -temp0.y;
		temp0.z = -temp0.z;
		temp0.w = -temp0.w;
	}

	if (get_srcN_neg(instr, 1))
	{
		temp1.x = -temp1.x;
		temp1.y = -temp1.y;
		temp1.z = -temp1.z;
		temp1.w = -temp1.w;
	}

	temp0.x = temp0.x * temp1.x + temp0.y * temp1.y + temp0.z * temp1.z + temp0.w + temp1.w;
	// Bop it..
	if ( dest == TEMP_REG )
	{
		if (destW) main_mem[dest][destIndex].w = temp0.x;
		if (destZ) main_mem[dest][destIndex].z = temp0.x;
		if (destY) main_mem[dest][destIndex].y = temp0.x;
		if (destX) main_mem[dest][destIndex].x = temp0.x;
	}
	else
	{
		if (destW) VOB[destIndex].w = temp0.x;
		if (destZ) VOB[destIndex].z = temp0.x;
		if (destY) VOB[destIndex].y = temp0.x;
		if (destX) VOB[destIndex].x = temp0.x;
	}
}

void gpu::log_instr(u64 instr)
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

	// Swizzle it
	temp = swizzle(main_mem[src][srcIndex], srcW, srcZ, srcY, srcX);

	// Negate it
	if (get_srcN_neg(instr, 0))
	{
		temp.x = -temp.x;
	}

	temp.x = (log(temp.x)/log(2));

	// Bop it..
	if ( dest == TEMP_REG )
	{
		if (destW) main_mem[dest][destIndex].w = 1.0;
		if (destZ) main_mem[dest][destIndex].z = temp.x;
		if (destY) main_mem[dest][destIndex].y = temp.x;
		if (destX) main_mem[dest][destIndex].x = temp.x;
	}
	else
	{
		if (destW) VOB[destIndex].w = 1.0;
		if (destZ) VOB[destIndex].z = temp.x;
		if (destY) VOB[destIndex].y = temp.x;
		if (destX) VOB[destIndex].x = temp.x;
	}
}

void gpu::exp_instr(u64 instr)
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

	// Swizzle it
	temp = swizzle(main_mem[src][srcIndex], srcW, srcZ, srcY, srcX);

	// Negate it
	if (get_srcN_neg(instr, 0))
	{
		temp.x = -temp.x;
	}

	temp.x = pow(2, temp.x);

	// Bop it..
	if ( dest == TEMP_REG )
	{
		if (destW) main_mem[dest][destIndex].w = 1.0;
		if (destZ) main_mem[dest][destIndex].z = temp.x;
		if (destY) main_mem[dest][destIndex].y = temp.x;
		if (destX) main_mem[dest][destIndex].x = temp.x;
	}
	else
	{
		if (destW) VOB[destIndex].w = 1.0;
		if (destZ) VOB[destIndex].z = temp.x;
		if (destY) VOB[destIndex].y = temp.x;
		if (destX) VOB[destIndex].x = temp.x;
	}
}

void gpu::lit_instr(u64 instr)
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

	// Swizzle it
	temp = swizzle(main_mem[src][srcIndex], srcW, srcZ, srcY, srcX);

	// Negate it
	if (get_srcN_neg(instr, 0))
	{
		temp.x = -temp.x;
	}

	if (temp.w < -(128.0-(1.0/256.0))) temp.w = -(128.0 - (1.0/256.0));
	else if ( temp.w > 128-(1.0/256.0)) temp.w = 128.0 - (1.0/256.0);
	if (temp.x < 0.0) temp.x = 0.0;
	if (temp.y < 0.0) temp.y = 0.0;

	// Bop it..
	if ( dest == TEMP_REG )
	{
		if (destW) main_mem[dest][destIndex].w = 1.0;
		if (destZ) main_mem[dest][destIndex].z = temp.x;
		if (destY) main_mem[dest][destIndex].y = temp.x;
		if (destX) main_mem[dest][destIndex].x = temp.x;
	}
	else
	{
		if (destW) VOB[destIndex].w = 1.0;
		if (destZ) VOB[destIndex].z = (temp.x > 0.0) ? pow(2, temp.w*(log(temp.y)/log(2))) : 0.0;
		if (destY) VOB[destIndex].y = temp.x;
		if (destX) VOB[destIndex].x = 1.0;
	}

}

void gpu::arl_instr(u64)
{

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

int gpu::getXMin(u128 v[])
{
	int min = v[0].x;

	for (int i = 1; i < 3; i++)
	{
		if (v[i].x < min)
			min = v[i].x;
	}	

	return min;
}

int gpu::getXMax(u128 v[])
{
	int max = v[0].x;

	for (int i = 1; i < 3; i++)
	{
		if (v[i].x > max)
			max = v[i].x;
	}	

	return max;
}

int gpu::getYMin(u128 v[])
{
	int min = v[0].y;

	for (int i = 1; i < 3; i++)
	{
		if (v[i].y < min)
			min = v[i].y;
	}	

	return min;
}

int gpu::getYMax(u128 v[])
{
	int max = v[0].y;

	for (int i = 1; i < 3; i++)
	{
		if (v[i].y > max)
			max = v[i].y;
	}	

	return max;
}


int gpu::perpDotProduct(u128 A, u128 B, int x, int y)
{
	return ( (x - A.x)*(B.y - A.y) ) - ( (y - A.y)*(B.x - A.x) );
}

int gpu::getZ(u128 v[], int x, int y)
{
	// if all z's same return one of them
	if (v[0].z == v[1].z && v[1].z == v[2].z)
		return v[0].z;

	// if not, then do calculations
	// plane equation
	return
	( -((v[0].z*( v[1].x - v[2].x )+
		   v[1].z*( v[2].x - v[0].x )+
		   v[2].z*( v[0].x - v[1].x)) * x +
	
		  (v[0].y*( v[1].z - v[2].z )+
		   v[1].y*( v[2].z - v[0].z )+
		   v[2].y*( v[0].z - v[1].z)) * y) /

		(v[0].x*( v[1].y - v[2].y )+
		   v[1].x*( v[2].y - v[0].y )+
		   v[2].x*( v[0].y - v[1].y))

	);

}