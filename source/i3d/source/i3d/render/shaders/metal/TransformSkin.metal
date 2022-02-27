//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2020, 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include <metal_stdlib>
#include <simd/simd.h>
#include "TransformSkinTypes.h"

using namespace metal;

typedef struct {
    float4      posTexU         [[ attribute( 0 ) ]];
    float4      normTexV        [[ attribute( 1 ) ]];
} StaticVertex;

typedef struct {
    uint4       joints          [[ attribute( 0 ) ]];
    float4      weights         [[ attribute( 1 ) ]];
    uint4       params          [[ attribute( 2 ) ]];
} VertexInfluence;

//======================================================================================================================
vertex void TransformSkin_Vertex( device StaticVertex * dstBuffer [[ buffer( 0 ) ]],
                                  device const StaticVertex * srcVerts [[ buffer( 1 ) ]],
                                  device const VertexInfluence * srcInf [[ buffer ( 2 ) ]],
                                  constant TransformSkinJointMatrices& jointMats [[ buffer( 3 ) ]],
                                  constant TransformSkinJointMatrices& bindMats [[ buffer( 4 ) ]],
                                  uint vid [[ vertex_id ]] ) {

    device StaticVertex * dstVert = &dstBuffer[ vid ];
    
    const device StaticVertex * vert = &srcVerts[ vid ];
    const device VertexInfluence * inf = &srcInf[ vid ];
    
    // Transform the vertices by the skin matrices
    float3 posAccum = float3(0, 0, 0);
    float3 normAccum = float3(0, 0, 0);
    
    uint infCount = inf->params[0];
        
    for (uint i = 0; i < infCount; ++i ) {
        
        uint jointIndex = inf->joints[ i ];
        
        constant matrix_float4x4 & bindMat = bindMats.matrices[ jointIndex ];
        constant matrix_float4x4 & jointMat = jointMats.matrices[ jointIndex ];
        
        float4 localP = bindMat * float4( vert->posTexU.xyz, 1 );
        float4 localN = bindMat * float4( vert->normTexV.xyz, 0 );
        
        float4 tmpP = jointMat * localP;
        float4 tmpN = jointMat * localN;
        
        tmpP = tmpP * inf->weights[ i ];
        posAccum = posAccum + tmpP.xyz;
        normAccum = normAccum + tmpN.xyz;
    }
    
    normAccum = normalize( normAccum );
    
    // Pack the transformed verts into the destination buffer
    dstVert->posTexU = float4( posAccum, vert->posTexU.w );
    dstVert->normTexV = float4( normAccum, vert->normTexV.w );
}

