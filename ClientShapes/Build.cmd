@PATH=C:\cs45500\MinGW32\bin;%PATH%
rem set DEBUG=-D DEBUG
rem set PrintLevel=3
rem set PipeLine3=1
rem set PipeLine7=1
set DESTFOLDER=%CD%
pushd ..\Scene
cmd /C "Build.cmd"
popd
pushd ..\SceneRender
cmd /C "Build.cmd"
popd
pushd ..\SceneRenderOpenGL
cmd /C "Build.cmd && PING -n 4 127.0.0.1>null"
popd
g++.exe -c -pipe -Wall %DEBUG% -I../gl -o GLUTCallbacks.o GLUTCallbacks.cpp
g++.exe -c -pipe -Wall %DEBUG% -I../gl -o mainCube.o      mainCube.cpp
g++.exe -c -pipe -Wall %DEBUG% -I../gl -o mainCubes.o     mainCubes.cpp
g++.exe -c -pipe -Wall %DEBUG% -I../gl -o mainCylinder.o  mainCylinder.cpp
g++.exe -c -pipe -Wall %DEBUG% -I../gl -o mainSphere.o    mainSphere.cpp
g++.exe -c -pipe -Wall %DEBUG% -I../gl -o mainTriangle.o  mainTriangle.cpp
g++.exe -c -pipe -Wall %DEBUG% -I../gl -o mainShapes.o    mainShapes.cpp
g++.exe -pipe -Wall -o mainCube.exe     mainCube.o     GLUTCallbacks.o SceneLib.o SceneRenderLib.o SceneRenderOpenGlLib.o -lopengl32 -lglu32 -lglut32
g++.exe -pipe -Wall -o mainCubes.exe    mainCubes.o    GLUTCallbacks.o SceneLib.o SceneRenderLib.o SceneRenderOpenGlLib.o -lopengl32 -lglu32 -lglut32
g++.exe -pipe -Wall -o mainCylinder.exe mainCylinder.o GLUTCallbacks.o SceneLib.o SceneRenderLib.o SceneRenderOpenGlLib.o -lopengl32 -lglu32 -lglut32
g++.exe -pipe -Wall -o mainSphere.exe   mainSphere.o   GLUTCallbacks.o SceneLib.o SceneRenderLib.o SceneRenderOpenGlLib.o -lopengl32 -lglu32 -lglut32
g++.exe -pipe -Wall -o mainTriangle.exe mainTriangle.o GLUTCallbacks.o SceneLib.o SceneRenderLib.o SceneRenderOpenGlLib.o -lopengl32 -lglu32 -lglut32
g++.exe -pipe -Wall -o mainShapes.exe   mainShapes.o   GLUTCallbacks.o SceneLib.o SceneRenderLib.o SceneRenderOpenGlLib.o -lopengl32 -lglu32 -lglut32