# Use
#     C:\cs45500\MinGW32\bin\mingw32-make.exe
#
# You can use a Windows shell command like this
#     set DEBUG=-D DEBUG
# to pass extra C Preprocessor macros to the compiler.
#
CPP = C:\cs45500\MinGW32\bin\g++.exe
CPPFLAGS = -Wall -I../gl
LIBS = -lopengl32 -lglu32 -lglut32
OBJ  = GLUTCallbacks.o mainCube.o
OBJ1 = SceneLib.o
OBJ2 = SceneRenderLib.o
OBJ3 = SceneRenderOpenGlLib.o
BIN  = mainCube.exe
MAKE = C:\cs45500\MinGW32\bin\mingw32-make.exe
RM = cmd /c del

.PHONY: all clean

all: $(BIN)

clean:
	${RM} $(OBJ) $(BIN)
	cmd.exe /c "set DESTFOLDER=%CD%&& cd ..\Scene && $(MAKE) clean"
	cmd.exe /c "set DESTFOLDER=%CD%&& cd ..\SceneRender && $(MAKE) clean"
	cmd.exe /c "set DESTFOLDER=%CD%&& cd ..\SceneRenderOpenGL && $(MAKE) clean"

$(BIN): $(OBJ)
	$(CPP) $(CPPFLAGS) -o $(BIN) $(OBJ) $(OBJ1) $(OBJ2) $(OBJ3) $(LIBS)

GLUTCallbacks.o: GLUTCallbacks.cpp GLUTCallbacks.h SceneLib.o SceneRenderLib.o SceneRenderOpenGlLib.o
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o GLUTcallbacks.o GLUTcallbacks.cpp

mainCube.o: mainCube.cpp GLUTCallbacks.h SceneLib.o SceneRenderLib.o
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o mainCube.o mainCube.cpp

SceneLib.o: force_rebuild
	cmd.exe /c "set DESTFOLDER=%CD%&& cd ..\Scene && $(MAKE)"

SceneRenderLib.o: force_rebuild
	cmd.exe /c "set DESTFOLDER=%CD%&& cd ..\SceneRender && $(MAKE)"

SceneRenderOpenGlLib.o: force_rebuild
	cmd.exe /c "set DESTFOLDER=%CD%&& cd ..\SceneRenderOpenGL && $(MAKE)"

force_rebuild :