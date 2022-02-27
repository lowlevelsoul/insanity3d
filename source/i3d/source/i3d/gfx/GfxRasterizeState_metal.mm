//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/gfx/GfxRasterizeState_metal.h"

namespace i3d { namespace gfx {
    
    //======================================================================================================================
    RasterizeStateMetal::RasterizeStateMetal() {
        m_changed = 1;
        m_cullFace = gfx::CULL_BACK;
        m_frontFace = gfx::WINDING_CW;        
    }
    
    //======================================================================================================================
    RasterizeStateMetal::~RasterizeStateMetal() {
        
    }
    
    //======================================================================================================================
    void RasterizeStateMetal::Set( gfx::WINDING frontFace, gfx::CULL cullFace ) {
        m_changed = 1;
        m_cullFace = cullFace;
        m_frontFace = frontFace;
    }
    
}}
