//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_GFXRASTERIZESTATE_H__
#define __I3D_GFXRASTERIZESTATE_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/GfxTypes.h"

namespace i3d { namespace gfx {
    
    class RasterizeState {
    public:
        virtual ~RasterizeState() {}
        
        virtual void Set( gfx::WINDING frontFace, gfx::CULL cullFace ) = 0;
        
    protected:
    };
}}


#endif
