# Use
#     C:\cs45500\MinGW32\bin\mingw32-make.exe
#
CPP = g++
CPPFLAGS = -pipe -Wall -I../gl -std=c++11 -static -static-libgcc -static-libstdc++ -Ofast
LIBS = -lopengl32 -lglu32 -lfreeglut
OBJ  = SceneRenderOpenGlLib.o
#RM = rm -f
RM = cmd /c del

.PHONY: all clean

all: $(OBJ)

clean:
	${RM} $(OBJ) "$(DESTFOLDER)\SceneRenderOpenGlLib.o"  null

$(OBJ): SceneRenderOpenGL.cpp SceneRenderOpenGL.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o $(OBJ) SceneRenderOpenGL.cpp $(LIBS)
	cp $(OBJ) $(DESTFOLDER)
