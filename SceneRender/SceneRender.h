#ifndef SCENERENDER_H
#define SCENERENDER_H

#include <windows.h>

#include "SceneLib.h"
#include "Framebuffer.h"

void renderScene(Scene* s, FrameBuffer* fb);

void P1_Model2World(Scene *s);     // apply the model transformation
void P2_World2View(Scene *s);      // apply the view transformation
void P3_BackFaceCulling(Scene *s); // delete back facing triangles
void P4_Lighting(Scene *s);        // compute vertex shading
void P5_View2Clip(Scene *s);       // apply the projection transformation
void P6_Clipping(Scene *s);        // clip triangles to the standard volume
void P7_Clip2NDC(Scene *s);        // do the perspective division
//void P8_Switch2OpenGL(Scene *s);  // let OpenGL finish the pipeline
void P8_NDC2VP(Scene *s);          // apply the viewport transformation
void P9_Rasterize(Scene *s);       // rasterize each triangle into fragments
void P10_FragmentShaderTexture(Scene *s); // shade each fragment of each triangle
void P11_Fragments2FB(Scene *s, FrameBuffer* fb);

//void P11_FragmentOperations(Scene *s);
//void P12_Antialiasing(Scene *s);

#endif /* SCENERENDER_H */
