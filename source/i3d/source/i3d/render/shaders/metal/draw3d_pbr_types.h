//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __DRAW3D_PBR_TYPES_H__
#define __DRAW3D_PBR_TYPES_H__

#ifdef __METAL_VERSION__
#define NS_ENUM(_type, _name) enum _name : _type _name; enum _name : _type
#define NSInteger metal::int32_t
#else
#import <Foundation/Foundation.h>
#endif

typedef NS_ENUM(NSInteger, Draw3dBuffer) {
    
    Draw3dBuffer_Verts = 0,
    Draw3dBuffer_ModelConstants = 1,
    Draw3dBuffer_SceneConstants = 2
};

typedef NS_ENUM(NSInteger, Draw3dFragmentBuffer) {
    Draw3dFragmentBuffer_Scene = 0,
    Draw3dFragmentBuffer_TextureArgs = 1,
    Draw3dFragmentBuffer_Material = 1
};

typedef NS_ENUM(NSInteger, Draw3dTexture) {
    Draw3dTexture_Albedo = 0,
    Draw3dTexture_Amr = 1,
    Draw3dTexture_Glow = 2,
};

typedef struct {
    uint    albedo;
    uint    amr;
    uint    glow;
    uint    pad;
} Material;

typedef struct {
    matrix_float4x4 matProj;
    matrix_float4x4 matView;

} Draw3dSceneConstants;

typedef struct {
    matrix_float4x4 modelMatrix;
} Draw3dModelConstants;


typedef struct {
    vector_float4          cameraPos;
    vector_float4          globalLightDir;
    vector_float4          globalLightColour;
} Draw3dFragmentScene;


#endif
