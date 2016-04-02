@PATH=C:\cs45500\MinGW32\bin;%PATH%
g++.exe -c -pipe -Wall %DEBUG% -o FrameBuffer.o         FrameBuffer.cpp
g++.exe -c -pipe -Wall %DEBUG% -o P1_Model2World.o      P1_Model2World.cpp
g++.exe -c -pipe -Wall %DEBUG% -o P2_World2View.o       P2_World2View.cpp
g++.exe -c -pipe -Wall %DEBUG% -o P3_BackFaceCulling.o  P3_BackFaceCulling.cpp
g++.exe -c -pipe -Wall %DEBUG% -o P4_Lighting.o         P4_Lighting.cpp
g++.exe -c -pipe -Wall %DEBUG% -o P5_View2Clip.o        P5_View2Clip.cpp
g++.exe -c -pipe -Wall %DEBUG% -o P6_Clipping.o         P6_Clipping.cpp
g++.exe -c -pipe -Wall %DEBUG% -o P6_ClippingSimple.o   P6_ClippingSimple.cpp
g++.exe -c -pipe -Wall %DEBUG% -o P7_Clip2NDC.o         P7_Clip2NDC.cpp
g++.exe -c -pipe -Wall %DEBUG% -o P8_Switch2OpenGL.o    P8_Switch2OpenGL.cpp
g++.exe -c -pipe -Wall %DEBUG% -o P8_NDC2VP.o           P8_NDC2VP.cpp
g++.exe -c -pipe -Wall %DEBUG% -o P9_Rasterize.o        P9_Rasterize.cpp
g++.exe -c -pipe -Wall %DEBUG% -o P10_FragmentShader.o  P10_FragmentShader.cpp
g++.exe -c -pipe -Wall %DEBUG% -o P10_FragmentShaderTexture.o  P10_FragmentShaderTexture.cpp
g++.exe -c -pipe -Wall %DEBUG% -o P11_Fragments2FB.o    P11_Fragments2FB.cpp
g++.exe -c -pipe -Wall %DEBUG% -o SceneRender.o         SceneRender.cpp
ld.exe -r -o SceneRenderLib.o FrameBuffer.o P1_Model2World.o P2_World2View.o P3_BackFaceCulling.o P4_Lighting.o  P5_View2Clip.o P6_Clipping.o P7_Clip2NDC.o P8_NDC2VP.o P9_Rasterize.o P10_FragmentShader.o P10_FragmentShaderTexture.o P11_Fragments2FB.o SceneRender.o
copy /Y SceneRenderLib.o "%DESTFOLDER%"