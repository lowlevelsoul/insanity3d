//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_GFXFENCE_METAL_H__
#define __I3D_GFXFENCE_METAL_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/GfxFence.h"
#include <Metal/Metal.h>

namespace i3d { namespace gfx {
    
    class FenceMetal : public Fence{
    public:
        FenceMetal();
        
        virtual ~FenceMetal();
        
    public:
        id<MTLFence>        m_fence;
    };
}}


#endif
