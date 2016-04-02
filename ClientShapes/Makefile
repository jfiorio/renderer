# Use
#     C:\cs45500\MinGW32\bin\mingw32-make.exe
#
# You can use a Windows shell command like this
#     set DEBUG=-D DEBUG
# to pass extra C Preprocessor macros to the compiler.
#
CPP = C:\cs45500\MinGW32\bin\g++.exe
CPPFLAGS = -pipe -Wall -I../gl
LIBS = -lopengl32 -lglu32 -lglut32
OBJ  = GLUTCallbacks.o mainCube.o mainCubes.o mainCylinder.o mainSphere.o mainTriangle.o mainShapes.o
OBJ1 = SceneLib.o
OBJ2 = SceneRenderLib.o
OBJ3 = SceneRenderOpenGlLib.o
BIN  = mainCube.exe mainCubes.exe mainCylinder.exe mainSphere.exe mainTriangle.exe mainShapes.exe
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
	$(CPP) $(CPPFLAGS) -o mainCube.exe     mainCube.o     GLUTCallbacks.o $(OBJ1) $(OBJ2) $(OBJ3) $(LIBS)
	$(CPP) $(CPPFLAGS) -o mainCubes.exe    mainCubes.o    GLUTCallbacks.o $(OBJ1) $(OBJ2) $(OBJ3) $(LIBS)
	$(CPP) $(CPPFLAGS) -o mainCylinder.exe mainCylinder.o GLUTCallbacks.o $(OBJ1) $(OBJ2) $(OBJ3) $(LIBS)
	$(CPP) $(CPPFLAGS) -o mainSphere.exe   mainSphere.o   GLUTCallbacks.o $(OBJ1) $(OBJ2) $(OBJ3) $(LIBS)
	$(CPP) $(CPPFLAGS) -o mainTriangle.exe mainTriangle.o GLUTCallbacks.o $(OBJ1) $(OBJ2) $(OBJ3) $(LIBS)
	$(CPP) $(CPPFLAGS) -o mainShapes.exe   mainShapes.o   GLUTCallbacks.o $(OBJ1) $(OBJ2) $(OBJ3) $(LIBS)

GLUTCallbacks.o: GLUTCallbacks.cpp GLUTCallbacks.h SceneLib.o SceneRenderLib.o SceneRenderOpenGlLib.o
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o GLUTcallbacks.o GLUTcallbacks.cpp

mainCube.o: mainCube.cpp GLUTCallbacks.h SceneLib.o SceneRenderLib.o
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o mainCube.o mainCube.cpp

mainCubes.o: mainCubes.cpp GLUTCallbacks.h SceneLib.o SceneRenderLib.o
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o mainCubes.o mainCubes.cpp

mainCylinder.o: mainCylinder.cpp GLUTCallbacks.h SceneLib.o SceneRenderLib.o
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o mainCylinder.o mainCylinder.cpp

mainSphere.o: mainSphere.cpp GLUTCallbacks.h SceneLib.o SceneRenderLib.o
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o mainSphere.o mainSphere.cpp

mainTriangle.o: mainTriangle.cpp GLUTCallbacks.h SceneLib.o SceneRenderLib.o
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o mainTriangle.o mainTriangle.cpp

mainShapes.o: mainShapes.cpp GLUTCallbacks.h Sphere.h Cylinder.h Cube.h Triangle.h SceneLib.o SceneRenderLib.o
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o mainShapes.o mainShapes.cpp

SceneLib.o: force_rebuild
	cmd.exe /c "set DESTFOLDER=%CD%&& cd ..\Scene && $(MAKE)"

SceneRenderLib.o: force_rebuild
	cmd.exe /c "set DESTFOLDER=%CD%&& cd ..\SceneRender && $(MAKE)"

SceneRenderOpenGlLib.o: force_rebuild
	cmd.exe /c "set DESTFOLDER=%CD%&& cd ..\SceneRenderOpenGL && $(MAKE)"

force_rebuild :