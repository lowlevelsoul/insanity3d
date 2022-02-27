//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2020, 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __STATICTRANSFORMTYPES_H__
#define __STATICTRANSFORMTYPES_H__

#ifdef __METAL_VERSION__
#define NS_ENUM(_type, _name) enum _name : _type _name; enum _name : _type
#define NSInteger metal::int32_t
#else
#import <Foundation/Foundation.h>
#endif

typedef NS_ENUM(NSInteger, TransformStaticBufferIndex) {
    TransformStaticBufferIndex_VertsDstBuffer      = 0,
    TransformStaticBufferIndex_VertsSrcBuffer      = 1,
    TransformStaticBufferIndex_VertsSceneConst     = 2,
    TransformStaticBufferIndex_VertsMeshConst      = 3,
    
    TransformStaticBufferIndex_IndexDstBuffer      = 0,
    TransformStaticBufferIndex_IndexSrcBuffer      = 1,
    TransformStaticBufferIndex_IndexMeshConst      = 2,
};

typedef struct {
    matrix_float4x4 projMatrix;
    matrix_float4x4 viewMatrix;
} TransformStaticVertexSceneConstants;

// Unifmorm struct that is used by the draw vertex shader
typedef struct {
    matrix_float4x4     modelMatrix;
    vector_int4         materialInfo;
} TransformStaticVertexMeshConstants;

typedef struct
{
    uint   vertexOffset;
} TransformStaticIndexConstants;

#endif
