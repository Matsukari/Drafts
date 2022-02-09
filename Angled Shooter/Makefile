#OBJS specifies which files to compile as part of the project
ENGINE_OBJS = ../C++_Engine/Compiled/Base.o ../C++_Engine/Compiled/Graphics.o
MAIN_OBJS = Code/Main/Main.cpp

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -IC:\A_Programming\SDL2-2.0.12\i686-w64-mingw32\include\SDL2 -IC:\Game_Projects\C++_Engine

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LC:\A_Programming\SDL2-2.0.12\i686-w64-mingw32\lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -std=c++17 

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = Shooter

#This is the target that compiles our executable
all : $(ENGINE_OBJS) $(MAIN_OBJS)
	$(CC) $(ENGINE_OBJS) $(MAIN_OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)