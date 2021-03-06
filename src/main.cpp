#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>

#include <common.h>
#include <main.h>
#include <screen.h>
#include <gpu.h>

int main(int argc, char const *argv[])
{

	// Check for arguments
	if ( argc < 4 )
	{
		std::cout << "Usage: program [obj file] [vertex shader] [constants]\n";
		return 1;
	}

	// For vertex and face data
	std::vector<std::vector<float> > v;
	std::vector<std::vector<int> > f;

	// Vertex shader binary
	std::vector<unsigned long long int > vs;

	// Constant matrix data
	std::vector<float> c;

	// frame buffer
	u8 FB[SCREEN_HEIGHT][SCREEN_WIDTH][3] = {{{0}}};

	// Get all the vertex and face data
	object_file_interface(argv[1], &v, &f);

	// Read vertex shader binary into vector
	vertex_shader_interface(argv[2], &vs);

	// Read constant matrix data
	constant_file_interface(argv[3], &c);

	// Initialize the screen
	std::thread t1 {thread_screen, FB};

	// Instanciate gpu and load things..
	gpu *myGPU = new gpu();
	myGPU->load_const_mem(c);
	myGPU->load_microcode(vs);

	// GPU execution begins here
	// Run the GPU for the number of faces available
	int i = 0;
	while (f.size() != i)
	{
		myGPU->load_vab(v, f[i]); i++;
		myGPU->execute(FB);
	}

	t1.join();

	return 0;
}

