#OBJS specifies which files to compile as part of the project
OBJS = src/main.cpp src/gpu.cpp

#OBJ_NAME specifies the name of our executable
OBJ_NAME = gpu

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS= -lSDL2

#CC specifies which compiler we're using
CC= g++

INC= -Iinc/ -Isrc/

FLAGS= -std=c++17 -O3 -pthread

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -w $(LINKER_FLAGS) $(INC) -o $(OBJ_NAME)
