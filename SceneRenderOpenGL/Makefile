# Use
#     C:\cs45500\MinGW32\bin\mingw32-make.exe
#
CPP = C:\cs45500\MinGW32\bin\g++.exe
CPPFLAGS = -pipe -Wall -I../gl
LIBS = -lopengl32 -lglu32 -lglut32
OBJ  = SceneRenderOpenGlLib.o
#RM = rm -f
RM = cmd /c del

.PHONY: all clean

all: $(OBJ)

clean:
	${RM} $(OBJ) "$(DESTFOLDER)\SceneRenderOpenGlLib.o"  null

$(OBJ): SceneRenderOpenGL.cpp SceneRenderOpenGL.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o $(OBJ) SceneRenderOpenGL.cpp $(LIBS)
	cmd.exe /c "copy /Y $(OBJ) "$(DESTFOLDER)""
