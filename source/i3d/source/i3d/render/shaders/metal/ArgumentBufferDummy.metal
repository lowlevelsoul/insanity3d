//
//  ArgumentBufferDummy.metal
//  xfighter-ios
//
//  Created by James Steele on 26.09.21.
//

#include <metal_stdlib>
using namespace metal;

#include "TextureArguments.h"

#define MAX_ARGUMENT_TEXTURES 256

typedef struct {
    float4  position       [[ position ]];
    float4  positionWorld;
    float4  normalWorld;
    float2  texcoord;
} VertexInOut;

fragment float4 ArgBufferDummy( VertexInOut in [[ stage_in ] ],
                                constant TextureArguments & textures [[ buffer( 0 ) ]] ) {

    return float4(0, 0, 0, 0);
}
                                    
