//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2020, 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __TRANSFORMSKINTYPES_H__
#define __TRANSFORMSKINTYPES_H__

#ifdef __METAL_VERSION__
#define NS_ENUM(_type, _name) enum _name : _type _name; enum _name : _type
#define NSInteger metal::int32_t
#else
#import <Foundation/Foundation.h>
#endif

#define SKIN_MATRICES_MAX 96

typedef NS_ENUM(NSInteger, TransformStaticBufferIndex) {
    TransformSkinBuffer_DstVerts            = 0,
    TransformSkinBuffer_SrcStaticVerts      = 1,
    TransformSkinBuffer_SrcWeights          = 2,
    TransformSkinBuffer_JointMats           = 3,
    TransformSkinBuffer_BindMats            = 4,
};

// Unifmorm struct that is used by the draw vertex shader
typedef struct {

    matrix_float4x4     matrices[SKIN_MATRICES_MAX];
} TransformSkinJointMatrices;

#endif
