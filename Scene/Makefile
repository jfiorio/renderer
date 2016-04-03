# Use
#     C:\cs45500\MinGW32\bin\mingw32-make.exe
#
CPP = g++
LD  = ld
CPPFLAGS = -pipe -Wall -mavx -march=corei7-avx -std=c++11 -Wl,-Map=./map.map -Ofast
OBJ  = Scene.o Texture.o Triangle.o Vertex.o Vector.o Matrix.o
#RM = rm -f
RM = cmd /c del
DEBUG = -g

.PHONY: all clean

all: SceneLib.o

clean:
	${RM} $(OBJ) SceneLib.o "$(DESTFOLDER)\SceneLib.o"

Scene.o: Scene.cpp Scene.h Triangle.h Texture.h Light.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o Scene.o Scene.cpp

Texture.o: Texture.cpp Texture.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o Texture.o Texture.cpp

Triangle.o: Triangle.cpp Triangle.h Vertex.h Texture.h Material.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o Triangle.o Triangle.cpp

Vertex.o: Vertex.cpp Vertex.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o Vertex.o Vertex.cpp

Vector.o: Vector.cpp Vector.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o Vector.o Vector.cpp

Matrix.o: Matrix.cpp Matrix.h Vector.h Vertex.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o Matrix.o Matrix.cpp
  
SceneLib.o: $(OBJ)
	$(LD) -r -o SceneLib.o $(OBJ) 
	cp SceneLib.o $(DESTFOLDER)