//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2020, 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include <metal_stdlib>
#include <simd/simd.h>
#include "Debug3dTypes.h"

using namespace metal;


// The static vertex format
typedef struct {
    float4      pos             [[ attribute(Debug3dVertexAttrib_Pos) ]];                // Position in world-space with a w of 1
    float4      colour          [[ attribute(Debug3dVertexAttrib_Colour) ]];             // Colour at this vertex
} Debug3dVertex;

// In/out struct
typedef struct {
    float4 position             [[ position ]];       // Position in clip space
    float4 colour;
} Debug3dInOut;

//======================================================================================================================
vertex Debug3dInOut Debug3dVertexShader(Debug3dVertex in [[stage_in]],
                                        constant Debug3dUniforms & uniforms [[ buffer(Debug3dBufferIndex_Uniforms) ]]) {
    
    Debug3dInOut out;
    float4 tmp = uniforms.viewxeMatrix * in.pos;
    out.position = uniforms.projxeMatrix * tmp;
    out.colour = in.colour;
    
    return out;
}

//======================================================================================================================
fragment float4 Debug3dFragmentShader( Debug3dInOut in [[stage_in]] ) {
    return in.colour;
}
