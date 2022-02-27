//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2020, 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __DEBUG3DTYPES_H__
#define __DEBUG3DTYPES_H__

#ifdef __METAL_VERSION__
#define NS_ENUM(_type, _name) enum _name : _type _name; enum _name : _type
#define NSInteger metal::int32_t
#else
#import <Foundation/Foundation.h>
#endif

// Indices of the atrtibute indices for the static vertices
typedef NS_ENUM(NSInteger, Debug3dVertexAttrib) {
    Debug3dVertexAttrib_Pos  = 0,
    Debug3dVertexAttrib_Colour = 1,
};

// Enums describing the indices of the buffers passed into the vertex program
typedef NS_ENUM(NSInteger, Debug3dBufferIndex) {
    Debug3dBufferIndex_Verts      = 0,
    Debug3dBufferIndex_Uniforms        = 1
};

typedef struct
{
    matrix_float4x4 projxeMatrix;
    matrix_float4x4 viewxeMatrix;
} Debug3dUniforms;


#endif /* Debug3d_h */
