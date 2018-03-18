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

	// Check for arguments
	// if ( argc < 3 )
	// {
	// 	std::cout << "Usage: program [obj file] [vertex shader]\n";
	// 	return 1;
	// }

	// For vertex and face data
	std::vector<std::vector<float> > v;
	std::vector<std::vector<int> > f;

	// Vertex shader program
	std::vector<std::vector<int> > vs;

	// frame buffer
	u8 FB[SCREEN_HEIGHT][SCREEN_WIDTH][3];

	// Get all the vertex and face data
	object_file_interface(argv[1], &v, &f);

	// // Read vertex shader program into vector
	// vertex_shader_interface(argv[2], &vs);

	// Initialize the screen
	//std::thread t1 {thread_screen, FB};

	gpu *myGPU = new gpu();

	// GPU execution begins here
	// Run the GPU for the number of faces available
	int i = 0;
	while (f.size() != i)
	{
		myGPU->load_vab(v, f[i]);
		i++;
	}
	// myGPU->execute(FB);

	// t1.join();

	return 0;
}

