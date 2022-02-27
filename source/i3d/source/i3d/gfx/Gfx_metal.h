//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_GFX_METAL_H__
#define __I3D_GFX_METAL_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/Gfx.h"
#import <Metal/Metal.h>

namespace i3d { namespace gfx {
    id<MTLDevice> GetDevice();
    
    // TEMP: Use during transition from metal -> gfx usage
    id<MTLCommandBuffer> GetCommandBuffer();
    
    // TEMP: Use during transition from metal -> gfx usage
    id<MTLCommandQueue> GetCommandQueue();
    
    id<MTLRenderCommandEncoder> GetRenderEncoder();
    
    MTLRenderPassDescriptor * GetRenderPassDescriptor();
}}


#endif
