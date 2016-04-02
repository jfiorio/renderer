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
del mainCube2.exe mainCube2.o GLUTCallbacks.o GLUTMouseCallbacks.o
