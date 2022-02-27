//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2020, 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include <metal_stdlib>
#include <simd/simd.h>
#include "TransformStaticTypes.h"

using namespace metal;

#define STATIC_VERTEX_FLOAT_COUNT 8
#define DRAW_VERTEX_FLOAT_COUNT 16

//======================================================================================================================
vertex void TransformStatic_Vertex(device float* dstBuffer [[ buffer( TransformStaticBufferIndex_VertsDstBuffer) ]],
                                   device const float* srcBuffer [[ buffer( TransformStaticBufferIndex_VertsSrcBuffer) ]],
                                   constant TransformStaticVertexSceneConstants& sceneConstants [[buffer(TransformStaticBufferIndex_VertsSceneConst) ]],
                                   constant TransformStaticVertexMeshConstants& meshConstants [[buffer(TransformStaticBufferIndex_VertsMeshConst) ]],
                                   uint vid [[ vertex_id ]] ) {
    
    uint srcFloatIndex = vid * STATIC_VERTEX_FLOAT_COUNT;
    uint dstFloatIndex = vid * DRAW_VERTEX_FLOAT_COUNT;
    
    // Pullin the vertex data from the float stream
    float4 modelPos = float4(srcBuffer[srcFloatIndex],
                             srcBuffer[srcFloatIndex+1],
                             srcBuffer[srcFloatIndex+2],
                             1);
    
    float4 modelNorm = float4(srcBuffer[srcFloatIndex+4],
                             srcBuffer[srcFloatIndex+5],
                             srcBuffer[srcFloatIndex+6],
                             0);
    
    float2 texCoord = float2(srcBuffer[srcFloatIndex+3], srcBuffer[srcFloatIndex+7]);
    
    // Tansform position and normal into world space
    float4 worldPos = meshConstants.modelMatrix * modelPos;
    float4 worldNorm = meshConstants.modelMatrix * modelNorm;
    
    // Transform the world position into clip space (world->view matrix and then by the projection matrix)
    float4 clipPos = sceneConstants.viewMatrix * worldPos;
    clipPos = sceneConstants.projMatrix * clipPos;
    
    // Pack the info into the output buffer
    dstBuffer[dstFloatIndex+0] = clipPos.x;
    dstBuffer[dstFloatIndex+1] = clipPos.y;
    dstBuffer[dstFloatIndex+2] = clipPos.z;
    dstBuffer[dstFloatIndex+3] = clipPos.w;
    
    // Pack position and tex u
    dstBuffer[dstFloatIndex+4] = worldPos.x;
    dstBuffer[dstFloatIndex+5] = worldPos.y;
    dstBuffer[dstFloatIndex+6] = worldPos.z;
    dstBuffer[dstFloatIndex+7] = texCoord.x;
    
    // Pack normal and tex v
    dstBuffer[dstFloatIndex+8] = worldNorm.x;
    dstBuffer[dstFloatIndex+9] = worldNorm.y;
    dstBuffer[dstFloatIndex+10] = worldNorm.z;
    dstBuffer[dstFloatIndex+11] = texCoord.y;
    
    // Pack the material info into the vertex
    device int * materialParams = (device int *) &dstBuffer[dstFloatIndex+12];
    materialParams[0] = meshConstants.materialInfo.x;
    materialParams[1] = meshConstants.materialInfo.y;
    materialParams[2] = meshConstants.materialInfo.z;
    materialParams[3] = meshConstants.materialInfo.w;
    
    // All done!
}

//======================================================================================================================
vertex void TransformStatic_Index(device uint* dstBuffer [[ buffer(0) ]],
                                  device const uint* srcBuffer [[ buffer(1) ]],
                                  constant TransformStaticIndexConstants& constants [[buffer(2) ]],
                                  uint vid [[ vertex_id ]] ) {
    dstBuffer[vid] = srcBuffer[vid] + constants.vertexOffset;
}
