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



typedef struct {
    float4  position_clip       [[ attribute( 0 ) ]];
    float4  position_texu       [[ attribute( 1 ) ]];
    float4  normal_texv         [[ attribute( 2 ) ]];
    uint4   textureIds          [[ attribute( 3 ) ]];           // Indices into the texture pool to use for this triangle
} Vertex;

typedef struct {
    float4      position       [[ position ]];
    float4      positionWorld;
    float4      normalWorld;
    float2      texcoord;
    uint4       textureIds;
} VertexInOut;


//======================================================================================================================
vertex VertexInOut Draw3d_Pbr_VertexShader( Vertex in [ [ stage_in ] ] ) {
    
    VertexInOut out;
    
    out.position = in.position_clip;
    out.positionWorld = float4(in.position_texu.xyz, 1);
    out.normalWorld = float4( in.normal_texv.xyz, 0);
    out.texcoord = float2( in.position_texu.w, in.normal_texv.w );
    out.textureIds = in.textureIds;
    
    return out;
}

//======================================================================================================================
fragment float4 Draw3d_Pbr_FragmentShader(VertexInOut in                            [[ stage_in ] ],
                                          constant Draw3dFragmentScene & scene      [[ buffer( Draw3dFragmentBuffer_Scene ) ]],
                                          texture2d<float> albedoTexture            [[ texture( Draw3dTexture_Albedo ) ]],
                                          texture2d<float> amrTexture               [[ texture( Draw3dTexture_Amr ) ]],
                                          texture2d<float> glowTexture              [[ texture( Draw3dTexture_Glow ) ]] ) {
    
    constexpr sampler textureSampler (mag_filter::linear,
                                      min_filter::linear);
    
    float3 ln = scene.globalLightDir.xyz;
    float3 n = normalize(in.normalWorld.xyz);
    float3 v = normalize(scene.cameraPos.xyz - in.positionWorld.xyz);
    
    float4 albedo = albedoTexture.sample(textureSampler, in.texcoord);
    float4 amr = amrTexture.sample(textureSampler, in.texcoord);
    float4 glow = glowTexture.sample(textureSampler, in.texcoord);
    
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
