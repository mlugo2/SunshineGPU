OBJS = main.cpp gpu.cpp

#OBJ_NAME specifies the name of our executable
OBJ_NAME = emu

LIBS= -lSDL2

CC= g++

FLAGS= -std=c++17 -O3 -pthread

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -w $(LIBS) -o $(OBJ_NAME)

run:
	./$(OBJ_NAME)