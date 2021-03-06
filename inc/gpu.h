#ifndef _GPU_H
#define _GPU_H

#include <iostream>
#include <vector>
#include "common.h"

using namespace std;

class gpu
{
private:
	u8 ZB[SCREEN_HEIGHT][SCREEN_WIDTH];

	u128 register_file[12];
	u128 constant_mem[96];

	u128 main_mem[3][16];

	u64 instruction_mem[128];

	u128 VAB[16];
	u128 VOB[16];

	// Pipeline components
	void geometry_processor();
	void rendering_engine(u8[][SCREEN_WIDTH][3]);

	// Sub pipeline methods
	void projection (u128 *);
	void rasteration(u128[], u8[][SCREEN_WIDTH][3]);

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

	int getXMin(u128[]);
	int getXMax(u128[]);
	int getYMin(u128[]);
	int getYMax(u128[]);

	int perpDotProduct(u128, u128, int, int);

	int getZ(u128 [], int, int);


	// Instuction execution methods
	void mov_instr(u64);
	void mul_instr(u64);
	void add_instr(u64);
	void mad_instr(u64);
	void dst_instr(u64);
	void min_instr(u64);
	void max_instr(u64);
	void slt_instr(u64);
	void sge_instr(u64);
	void rcp_instr(u64);
	void rsq_instr(u64);
	void dp3_instr(u64);
	void dp4_instr(u64);
	void log_instr(u64);
	void exp_instr(u64);
	void lit_instr(u64);
	void arl_instr(u64);

public:
	gpu();
	~gpu();

	void execute( u8[][SCREEN_WIDTH][3]);
	void load_const_mem(vector<float> constants);
	void load_microcode(vector<u64>);
	void load_vab( vector<vector<float> >, vector<int>);

	void rotate(float *, float*, float*, float*);

	void refresh_screen(u128[], u8[][SCREEN_WIDTH][3]);
	
};

#endif