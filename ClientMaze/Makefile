# Use
#     C:\cs45500\MinGW32\bin\mingw32-make.exe
#
# You can use a Windows shell command like this
#     set DEBUG=-D DEBUG
# to pass extra C Preprocessor macros to the compiler.
#
CPP = g++
CPPFLAGS = -pipe -Wall -I../gl -std=c++11 -mavx -march=corei7-avx -Ofast
LIBS = -lopengl32 -lglu32 -lfreeglut
OBJ  = GLUTCallbacks.o GLUTMouseCallbacks.o MazeParser.o mainMaze.o
OBJ1 = SceneLib.o
OBJ2 = SceneRenderLib.o
OBJ3 = SceneRenderOpenGlLib.o
BIN  = mainMaze.exe
MAKE = make
RM = cmd /c del
DEBUG = -g

.PHONY: all clean

all: mainMaze.exe

clean:
	${RM} $(OBJ) $(BIN)
	cmd.exe /c "set DESTFOLDER=%CD%&& cd ..\Scene && $(MAKE) clean"
	cmd.exe /c "set DESTFOLDER=%CD%&& cd ..\SceneRender && $(MAKE) clean"
	cmd.exe /c "set DESTFOLDER=%CD%&& cd ..\SceneRenderOpenGL && $(MAKE) clean"

$(BIN): $(OBJ)
	$(CPP) $(CPPFLAGS) -o $(BIN) $(OBJ) $(OBJ1) $(OBJ2) $(OBJ3) $(LIBS)

GLUTCallbacks.o: GLUTCallbacks.cpp GLUTCallbacks.h Constants.h Timer.h MazeParser.h SceneLib.o SceneRenderLib.o SceneRenderOpenGlLib.o
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o GLUTcallbacks.o GLUTcallbacks.cpp

GLUTMouseCallbacks.o: GLUTMouseCallbacks.cpp GLUTMouseCallbacks.h Constants.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o GLUTMouseCallbacks.o GLUTMouseCallbacks.cpp

MazeParser.o: MazeParser.cpp MazeParser.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o MazeParser.o MazeParser.cpp

mainMaze.o: mainMaze.cpp Constants.h GLUTCallbacks.h GLUTMouseCallbacks.h MazeParser.h SceneLib.o SceneRenderLib.o
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o mainMaze.o mainMaze.cpp

SceneLib.o: force_rebuild
	cmd.exe /c "set DESTFOLDER=%CD%&& cd ..\Scene && $(MAKE)"

SceneRenderLib.o: force_rebuild
	cmd.exe /c "set DESTFOLDER=%CD%&& cd ..\SceneRender && $(MAKE)"

SceneRenderOpenGlLib.o: force_rebuild
	cmd.exe /c "set DESTFOLDER=%CD%&& cd ..\SceneRenderOpenGL && $(MAKE)"

force_rebuild :