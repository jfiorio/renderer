set DESTFOLDER=%CD%
pushd ..\Scene
cmd /C "BuildClean.cmd"
popd
pushd ..\SceneRender
cmd /C "BuildClean.cmd"
popd
pushd ..\SceneRenderOpenGL
cmd /C "BuildClean.cmd"
popd
del mainMaze.exe mainMaze.o GLUTCallbacks.o GLUTMouseCallbacks.o MazeParser.o
