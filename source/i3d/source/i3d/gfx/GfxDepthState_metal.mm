//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/gfx/GfxDepthState_metal.h"

namespace i3d { namespace gfx {

    //======================================================================================================================
    DepthStateMetal::DepthStateMetal() {
        m_depthFunc = gfx::FUNC_LESS;
        m_changed = 1;
        m_depthWrite = 1;
        m_depthTest = 1;
        m_depthClear = 1;
        m_stencilClear = 0;
    }
    
    //======================================================================================================================
    DepthStateMetal::~DepthStateMetal() {
        
    }
    
    //======================================================================================================================
    void DepthStateMetal::Set( gfx::FUNC depthFunc, bool depthWrite, bool depthDest, float depthClear, uint8_t stencilClear ) {
        
        m_depthFunc = depthFunc;
        m_changed = 1;
        m_depthWrite = (depthWrite == true) ? 1 : 0;
        m_depthTest = (m_depthTest == true) ? 1 : 0;
        m_depthClear = depthClear;
        m_stencilClear = stencilClear;
    }
    
}}
