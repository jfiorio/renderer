@PATH=C:\cs45500\MinGW32\bin;%PATH%
g++.exe -c -pipe -Wall %DEBUG% -o Scene.o    Scene.cpp
g++.exe -c -pipe -Wall %DEBUG% -o Texture.o  Texture.cpp
g++.exe -c -pipe -Wall %DEBUG% -o Triangle.o Triangle.cpp
g++.exe -c -pipe -Wall %DEBUG% -o Vertex.o   Vertex.cpp
g++.exe -c -pipe -Wall %DEBUG% -o Vector.o   Vector.cpp
g++.exe -c -pipe -Wall %DEBUG% -o Matrix.o   Matrix.cpp
ld.exe -r -o SceneLib.o Scene.o Texture.o Triangle.o Vertex.o Vector.o Matrix.o
copy /Y SceneLib.o "%DESTFOLDER%"