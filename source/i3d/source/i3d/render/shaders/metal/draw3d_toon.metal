//
//  draw3d_pbr.metal
//  r3d-macos
//
//  Created by James Steele on 17.08.21.
//

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
vertex VertexInOut Draw3d_Toon_Bindless_VertexShader( Vertex in [[ stage_in ]] ) {
    
    VertexInOut out;
    
    out.position = in.position_clip;
    out.positionWorld = float4(in.position_texu.xyz, 1);
    out.normalWorld = float4( in.normal_texv.xyz, 0);
    out.texcoord = float2( in.position_texu.w, in.normal_texv.w );
    out.textureIds = in.textureIds;
    
    return out;
}

//======================================================================================================================
fragment float4 Draw3d_Toon_Bindless_FragmentShader(
                                          VertexInOut in                            [[ stage_in ] ],
                                          constant Draw3dFragmentScene & scene      [[ buffer( Draw3dFragmentBuffer_Scene ) ]],
                                          constant TextureArguments & texPool       [[ buffer( Draw3dFragmentBuffer_TextureArgs ) ]] ) {
    
    constexpr sampler textureSampler (mag_filter::linear,
                                      min_filter::linear);
    
    float3 ln = scene.globalLightDir.xyz;
    float3 n = normalize(in.normalWorld.xyz);
    
    // Grab the texture values
    float4 albedo = texPool.textures[ in.textureIds.x ].sample(textureSampler, in.texcoord);
    float4 amr = texPool.textures[ in.textureIds.y ].sample(textureSampler, in.texcoord);
    float4 glow = texPool.textures[ in.textureIds.z ].sample(textureSampler, in.texcoord);
    
    float i = max(-dot(ln, n), 0.0);
    if ( i <= 0.3) {
        i = 0.3;
    } else {
        i = 1.0;
    }
    
    float3 Lo = albedo.xyz * i;
    float3 color = Lo + glow.xyz;
    
    //color = color / (color + float3(1.0));
    //color = pow(color, float(1.0/2.2));
   
    return float4(color.xyz, 1.0);
}
