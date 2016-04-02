# Use
#     C:\cs45500\MinGW32\bin\mingw32-make.exe
#
CPP = C:\cs45500\MinGW32\bin\g++.exe
LD  = C:\cs45500\MinGW32\bin\ld.exe
CPPFLAGS = -pipe -Wall
OBJ  = SceneRender.o P1_Model2World.o  P2_World2View.o P3_BackFaceCulling.o P4_Lighting.o P5_View2Clip.o P6_Clipping.o P7_Clip2NDC.o P8_NDC2VP.o P9_Rasterize.o P10_FragmentShader.o P11_Fragments2FB.o FrameBuffer.o
#RM = rm -f
RM = cmd /c del

.PHONY: all clean

all: SceneRenderLib.o

clean:
	${RM} $(OBJ) SceneRenderLib.o "$(DESTFOLDER)\SceneRenderLib.o"

SceneRender.o: SceneRender.cpp SceneRender.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o SceneRender.o SceneRender.cpp

P1_Model2World.o: P1_Model2World.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o P1_Model2World.o P1_Model2World.cpp

P2_World2View.o: P2_World2View.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o P2_World2View.o P2_World2View.cpp

P3_BackFaceCulling.o: P3_BackFaceCulling.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o P3_BackFaceCulling.o P3_BackFaceCulling.cpp

P4_Lighting.o: P4_Lighting.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o P4_Lighting.o P4_Lighting.cpp

P5_View2Clip.o: P5_View2Clip.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o P5_View2Clip.o P5_View2Clip.cpp

P6_Clipping.o: P6_Clipping.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o P6_Clipping.o P6_Clipping.cpp

P7_Clip2NDC.o: P7_Clip2NDC.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o P7_Clip2NDC.o P7_Clip2NDC.cpp

P8_Switch2OpenGL.o: P8_Switch2OpenGL.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o P8_Switch2OpenGL.o P8_Switch2OpenGL.cpp

P8_NDC2VP.o: P8_NDC2VP.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o P8_NDC2VP.o P8_NDC2VP.cpp

P9_Rasterize.o: P9_Rasterize.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o P9_Rasterize.o P9_Rasterize.cpp

P10_FragmentShader.o: P10_FragmentShader.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o P10_FragmentShader.o P10_FragmentShader.cpp

P11_Fragments2FB.o: P11_Fragments2FB.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o P11_Fragments2FB.o P11_Fragments2FB.cpp

FrameBuffer.o: FrameBuffer.cpp FrameBuffer.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o FrameBuffer.o FrameBuffer.cpp

SceneRenderLib.o: $(OBJ)
	$(LD) -r -o SceneRenderLib.o $(OBJ)
	cmd.exe /c "copy /Y SceneRenderLib.o "$(DESTFOLDER)""