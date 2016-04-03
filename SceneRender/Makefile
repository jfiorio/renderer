# Use
#     C:\cs45500\MinGW32\bin\mingw32-make.exe
#
CPP = g++.exe
LD  = ld.exe 
CPPFLAGS = -pipe -Wall -g -mavx -march=corei7-avx -std=c++11 -Ofast
OBJ  = SceneRender.o Pipeline.o PipelineMultithreaded.o PipelineJob.o PipelineStage.o Context.o Thread.o P1_Model2World.o  P2_World2View.o P3_BackFaceCulling.o P4_Lighting.o P5_View2Clip.o P6_Clipping.o P7_Clip2NDC.o P8_NDC2VP.o P9_Rasterize.o P10_FragmentShaderTexture.o P11_Fragments2FB.o FrameBuffer.o
#RM = rm -f
RM = cmd /c del

.PHONY: all clean

all: SceneRenderLib.o

clean:
	${RM} $(OBJ) SceneRenderLib.o "$(DESTFOLDER)\SceneRenderLib.o"

SceneRender.o: SceneRender.cpp SceneRender.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o SceneRender.o SceneRender.cpp

Pipeline.o: Pipeline.cpp Pipeline.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o Pipeline.o Pipeline.cpp

PipelineMultithreaded.o: PipelineMultithreaded.cpp PipelineMultithreaded.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o PipelineMultithreaded.o PipelineMultithreaded.cpp

PipelineJob.o: PipelineJob.cpp PipelineJob.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o PipelineJob.o PipelineJob.cpp
  
PipelineStage.o: PipelineStage.cpp PipelineStage.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o PipelineStage.o PipelineStage.cpp

Context.o: Context.cpp Context.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o Context.o Context.cpp
  
Thread.o: Thread.cpp Thread.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o Thread.o Thread.cpp
  
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

P10_FragmentShaderTexture.o: P10_FragmentShaderTexture.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o P10_FragmentShaderTexture.o P10_FragmentShaderTexture.cpp

P11_Fragments2FB.o: P11_Fragments2FB.cpp
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o P11_Fragments2FB.o P11_Fragments2FB.cpp

FrameBuffer.o: FrameBuffer.cpp FrameBuffer.h
	$(CPP) $(CPPFLAGS) $(DEBUG) -c -o FrameBuffer.o FrameBuffer.cpp

SceneRenderLib.o: $(OBJ)
	$(LD) -r -o SceneRenderLib.o $(OBJ)
	cp SceneRenderLib.o $(DESTFOLDER)