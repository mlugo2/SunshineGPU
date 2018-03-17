#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>

#include "common.h"
#include "main.h"
#include "screen.h"
#include "gpu.h"

int main(int argc, char const *argv[])
{
	// if ( argc < 3 )
	// {
	// 	std::cout << "Usage: program [obj file] [vertex shader]\n";
	// 	return 1;
	// }

	// // For vertex and face data
	// std::vector<std::vector<float> > v;
	// std::vector<std::vector<int> > f;

	// std::vector<std::vector<int> > vs;

	// // Get all the vertex and face data
	// object_file_interface(argv[1], &v, &f);

	// // Read vertex shader program into vector
	// vertex_shader_interface(argv[2], &vs);

	// gpu *myGPU = new gpu();

	// myGPU->InitScreen();

	u8 FB[SCREEN_HEIGHT][SCREEN_WIDTH][3];

	//std::thread t1 {thread_screen, FB};

	gpu *myGPU = new gpu();
	myGPU->execute(FB);

	// t1.join();

	// Load data and microcode into gpu

	return 0;
}

