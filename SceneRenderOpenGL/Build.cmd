@PATH=C:\cs45500\MinGW32\bin;%PATH%
g++.exe -c -pipe -Wall %DEBUG% -I../gl -o SceneRenderOpenGlLib.o SceneRenderOpenGL.cpp
copy /Y SceneRenderOpenGlLib.o "%DESTFOLDER%"