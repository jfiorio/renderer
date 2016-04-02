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
del GLUTCallbacks.o
del mainCube.exe     mainCube.o     
del mainCubes.exe    mainCubes.o     
del mainCylinder.exe mainCylinder.o
del mainSphere.exe   mainSphere.o
del mainTriangle.exe mainTriangle.o
del mainShapes.exe   mainShapes.o