#include "PipelineStage.h"PipelineStage::PipelineStage(){  this->attributes = STAGE_ATTRIB_READ_TRIANGLE_LIST; // all stages walk the triangle list}int PipelineStage::getAttributes(){  return attributes;}