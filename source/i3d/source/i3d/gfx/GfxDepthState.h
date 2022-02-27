//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_GFXDEPTHSTATE_H__
#define __I3D_GFXDEPTHSTATE_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/GfxTypes.h"

namespace i3d { namespace gfx {
    
    class DepthState {
    public:
        virtual ~DepthState() {}
        
        virtual void Set( gfx::FUNC depthFunc, bool depthWrite, bool depthDest, float depthClear, uint8_t stencilClear ) = 0;
        
    protected:
    };
}}


#endif
