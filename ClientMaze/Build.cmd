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
g++.exe -c -pipe -Wall %DEBUG% -I../gl -o GLUTMouseCallbacks.o GLUTMouseCallbacks.cpp
g++.exe -c -pipe -Wall %DEBUG% -I../gl -o MazeParser.o MazeParser.cpp
g++.exe -c -pipe -Wall %DEBUG% -I../gl -o mainMaze.o mainMaze.cpp
g++.exe -pipe -Wall -o mainMaze.exe mainMaze.o GLUTCallbacks.o GLUTMouseCallbacks.o MazeParser.o SceneLib.o SceneRenderLib.o SceneRenderOpenGlLib.o -lopengl32 -lglu32 -lglut32
