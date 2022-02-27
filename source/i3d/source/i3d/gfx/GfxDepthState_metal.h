//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_GFXDEPTHSTATE_METAL_H__
#define __I3D_GFXDEPTHSTATE_METAL_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/GfxTypes.h"
#include "i3d/gfx/GfxDepthState.h"
#include <Metal/Metal.h>

namespace i3d { namespace gfx {
    
    class DepthStateMetal : public DepthState {
    public:
        DepthStateMetal();
        
        virtual ~DepthStateMetal();
        
        virtual void Set( gfx::FUNC depthFunc, bool depthWrite, bool depthDest, float depthClear, uint8_t stencilClear ) override;
        
    public:
        id<MTLDepthStencilState>        m_state;
        gfx::FUNC                       m_depthFunc;
        float                           m_depthClear;
        uint8_t                         m_stencilClear;
        uint64_t                        m_changed : 1;
        uint64_t                        m_depthWrite : 1;
        uint64_t                        m_depthTest : 1;
    };
}}


#endif
