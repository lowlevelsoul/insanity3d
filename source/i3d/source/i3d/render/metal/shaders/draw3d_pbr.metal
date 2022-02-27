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
    float4  position_texu       [[ attribute( 0 ) ]];
    float4  normal_texv         [[ attribute( 1 ) ]];
} Vertex;

typedef struct {
    float4  position       [[ position ]];
    float4  positionWorld;
    float4  normalWorld;
    float2  texcoord;
} VertexInOut;


//======================================================================================================================
vertex VertexInOut Draw3d_Pbr_VertexShader( Vertex in [ [ stage_in ] ],
                                            constant Draw3dSceneConstants & scene [ [ buffer( Draw3dBuffer_SceneConstants ) ] ],
                                            constant Draw3dModelConstants & model [ [ buffer( Draw3dBuffer_ModelConstants ) ] ] ) {
    
    VertexInOut out;
    
    float4 tmp = model.modelMatrix * float4(in.position_texu.x, in.position_texu.y, in.position_texu.z, 1);
    out.positionWorld = tmp;
    
    out.normalWorld = model.modelMatrix * float4(in.normal_texv.x, in.normal_texv.y, in.normal_texv.z, 0);
    
    tmp = scene.matView * tmp;
    out.position = scene.matProj * tmp;
    out.texcoord = float2( in.position_texu.w, in.normal_texv.w );
    
    return out;
}

//======================================================================================================================
fragment float4 Draw3d_Pbr_FragmentShader(VertexInOut in                            [[ stage_in ] ],
                                          constant Draw3dFragmentScene & scene      [[ buffer( Draw3dFragmentBuffer_Scene ) ]],
                                          constant Material & material              [[ buffer( Draw3dFragmentBuffer_Material ) ]],
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
