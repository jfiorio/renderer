# Use
#     C:\cs45500\MinGW32\bin\mingw32-make.exe
#
CPP = C:\cs45500\MinGW32\bin\g++.exe
LD  = C:\cs45500\MinGW32\bin\ld.exe
CPPFLAGS = -pipe -Wall
OBJ  = Scene.o Texture.o Triangle.o Vertex.o Vector.o Matrix.o
#RM = rm -f
RM = cmd /c del

.PHONY: all clean

all: SceneLib.o

clean:
	${RM} $(OBJ) SceneLib.o "$(DESTFOLDER)\SceneLib.o"

Scene.o: Scene.cpp Scene.h TriangleListNode.h Triangle.h Texture.h Light.h
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
	cmd.exe /c "copy /Y SceneLib.o "$(DESTFOLDER)""