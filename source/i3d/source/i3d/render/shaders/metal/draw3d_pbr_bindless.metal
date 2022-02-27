//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include <metal_stdlib>
#include <simd/simd.h>
using namespace metal;

#include "draw3d_pbr_types.h"
#include "pbr.h"
#include "TextureArguments.h"

// Input vertex
typedef struct {
    float4  position_clip       [[ attribute( 0 ) ]];           // Projected position
    float4  position_texu       [[ attribute( 1 ) ]];           // Position in world space + texture u coordinate
    float4  normal_texv         [[ attribute( 2 ) ]];           // Normal in world space + texture v coordinate
    uint4   textureIds          [[ attribute( 3 ) ]];           // Indices into the texture pool to use for this triangle
} Vertex;

// Vertex data passed from vertex to fragment op
typedef struct {
    float4  position       [[ position ]];
    float4  positionWorld;
    float4  normalWorld;
    float2  texcoord;
    uint4   textureIds;
} VertexInOut;

//======================================================================================================================
vertex VertexInOut Draw3d_Pbr_Bindless_VertexShader( Vertex in [[ stage_in ]] ) {
    
    VertexInOut out;
    
    out.position = in.position_clip;
    out.positionWorld = float4(in.position_texu.xyz, 1);
    out.normalWorld = float4( in.normal_texv.xyz, 0);
    out.texcoord = float2( in.position_texu.w, in.normal_texv.w );
    out.textureIds = in.textureIds;
    
    return out;
}

//======================================================================================================================
fragment float4 Draw3d_Pbr_Bindless_FragmentShader(
                                          VertexInOut in                            [[ stage_in ] ],
                                          constant Draw3dFragmentScene & scene      [[ buffer( Draw3dFragmentBuffer_Scene ) ]],
                                          constant TextureArguments & texPool       [[ buffer( Draw3dFragmentBuffer_TextureArgs ) ]] ) {
    
    constexpr sampler textureSampler (mag_filter::linear,
                                      min_filter::linear);
    
    float3 ln = scene.globalLightDir.xyz;
    float3 n = normalize(in.normalWorld.xyz);
    float3 v = normalize(scene.cameraPos.xyz - in.positionWorld.xyz);
    
    // Grab the texture values 
    float4 albedo = texPool.textures[ in.textureIds.x ].sample(textureSampler, in.texcoord);
    float4 amr = texPool.textures[ in.textureIds.y ].sample(textureSampler, in.texcoord);
    float4 glow = texPool.textures[ in.textureIds.z ].sample(textureSampler, in.texcoord);
    
    float3 F0 = float3(0.04);
    F0 = mix(F0, albedo.xyz, amr.y);

    float3 Lo = float3(0);

    // Calculate global light contribution
    Lo = calcReflectanceGlobal(-ln, scene.globalLightColour.xyz, n, v, F0, albedo.xyz, amr.z, amr.y);
    
    float3 ambient = float(0.03) * albedo.xyz * amr.x;
    float3 color = ambient + Lo + glow.xyz;
    
    //color = color / (color + float3(1.0));
    //color = pow(color, float(1.0/2.2));
   
    return float4(color.xyz, 1.0);
}
