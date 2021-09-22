OBJS = main.cpp ../src/*.cpp ../src/objects/*.cpp ../src/controls/*.cpp

CC = g++

INCLUDE_PATHS = -I../include
WIN_INCLUDE_PATHS = -IC:/mingw_dev_lib/include -IC:/mingw_dev_lib/include/SDL2

WIN_LIBRARY_PATHS = -LC:/mingw_dev_lib/lib

COMPILER_FLAGS = -g -w

LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf
WIN_LINKER_FLAGS = -lmingw32 -lSDL2main

OBJ_NAME = example

all: $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

linux: $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

windows: $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(WIN_INCLUDE_PATHS) $(WIN_LIBRARY_PATHS) $(COMPILER_FLAGS) $(WIN_LINKER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

clean:
	rm -f *.o
	rm -f example

clean-win:
	del example.exe
