//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/gfx/Gfx_metal.h"
#include "i3d/gfx/GfxFence_metal.h"

namespace i3d { namespace gfx {

    //======================================================================================================================
    FenceMetal::FenceMetal() {
        m_fence = [gfx::GetDevice() newFence];
    }

    //======================================================================================================================
    FenceMetal::~FenceMetal() {
        
    }

}}
