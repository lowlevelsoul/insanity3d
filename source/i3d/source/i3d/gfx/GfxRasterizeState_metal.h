//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_GFXRASTERIZESTATE_METAL_H__
#define __I3D_GFXRASTERIZESTATE_METAL_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/GfxTypes.h"
#include "i3d/gfx/GfxRasterizeState.h"

namespace i3d { namespace gfx {
    
    class RasterizeStateMetal : public RasterizeState {
    public:
        RasterizeStateMetal();
        
        virtual ~RasterizeStateMetal();
        
        virtual void Set( gfx::WINDING frontFace, gfx::CULL cullFace ) override;
        
    public:
        uint64_t        m_changed : 1;
        gfx::WINDING    m_frontFace;
        gfx::CULL       m_cullFace;
    };
}}


#endif
