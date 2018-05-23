#ifndef __MAIN_H
#define __MAIN_H

#include <SDL2/SDL.h>

#include "stringproc.h"
void object_file_interface(std::string fileName, 
	std::vector<std::vector<float> > *vertex, 
	std::vector<std::vector<int> > *face);

void vertex_shader_interface(std::string fileName, 
	std::vector<unsigned long long int > *vertexShader );

void constant_file_interface(std::string fileName,
	std::vector<float> *constants);

void object_file_interface(std::string fileName, 
	std::vector<std::vector<float> > *vertex, 
	std::vector<std::vector<int> > *face)
{
	// Opening file
	std::ifstream myFile;
	myFile.open(fileName.data());

	// Read file into vector for easier processing
	std::vector<std::string> data;
	std::string line;
	while( myFile.good() )
	{
		getline(myFile, line);
		line[line.length()] = '\0';
		data.push_back(line);
	}

	myFile.close();

	// Validate file
	if ( data[0] != "OFF" )
	{
		std::cout << "ERORR(1): incorrect file format." << std::endl;
		return;
	}

	// Header not needed anymore
	data.erase(data.begin());

	// Check that they are numbers
	// #verticies #faces #edges
	for ( auto i : data[0] )
	{
		// Skip over spaces
		if ( i == ' ')
			continue;
		
		if (!isdigit(i))
		{
			std::cout << "ERORR(2): incorrect file format." << std::endl;
			return;
		}
	}


	// Now to get all the data out..
	std::string delims = " \t";
	std::vector<std::string> tempData;
	std::vector<float> temp;

	// Get the number of vertices, faces, and edges
	// in that order.
	tokenize(data[0], tempData, delims);
	int vertNum;
	int faceNum;
	int edgeNum;

	int index = 0;
	for ( auto i : tempData )
	{
		switch(index)
		{
			case 0: vertNum = stoi(i); index++; break;
			case 1: faceNum = stoi(i); index++; break;
			case 2: edgeNum = stoi(i); index++; break;
			default: return;
		}
	}

	// Erase more of the header
	data.erase(data.begin());
	tempData.clear();

	// Go through the rows of vertex data
	for (int i = 0; i < vertNum; ++i)
	{
		tokenize(data[i], tempData, delims);

		for ( auto j : tempData )
		{
			temp.push_back( std::strtof((j).c_str(), 0) );
		}

		vertex->push_back(temp);

		tempData.clear();
		temp.clear();
	}

	for (int i = 0; i < vertNum; ++i)
		data.erase(data.begin());


	std::vector<int> int_temp;
	for ( int i = 0; i < faceNum; ++i )
	{
		tokenize(data[i], tempData, delims);

		for ( auto j : tempData )
		{
			int_temp.push_back( (int)strtof((j).c_str(), 0) );
		}

		face->push_back(int_temp);

		tempData.clear();
		int_temp.clear();
	}

}

#include <iostream>
using namespace std;

void vertex_shader_interface(std::string fileName, 
	std::vector<unsigned long long int > *vertexShader )
{
	// Opening file
	std::ifstream myFile;
	myFile.open(fileName.data());

	// Read file into vector for easier processing
	std::vector<std::string> data;
	std::string line;
	while( myFile.good() )
	{
		getline(myFile, line);
		line[line.length()] = '\0';
		data.push_back(line);
	}

	myFile.close();

	for ( auto i : data )
	{
		vertexShader->push_back(stoull(i,0,0));
	}

}

#include <iostream>
using namespace std;

void constant_file_interface(std::string fileName,
	std::vector<float> *constants)
{
	// Opening file
	std::ifstream myFile;
	myFile.open(fileName.data());

	// Read file into vector for easier processing
	std::vector<std::string> data;
	std::string line;
	while( myFile.good() )
	{
		getline(myFile, line);
		line[line.length()] = '\0';
		data.push_back(line);
	}

	myFile.close();

	// Tokenize data
	std::string delims = " \t\n,";
	std::vector<std::string> tempData;

	// Store tokenized data into tempData
	for ( auto i : data )
	{
		tokenize(i, tempData, delims);
	}

	for ( auto i : tempData )
	{
		constants->push_back(std::stof(i));
	}		
}

#endif