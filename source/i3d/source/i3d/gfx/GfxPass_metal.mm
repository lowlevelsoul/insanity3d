//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/gfx/Gfx_metal.h"
#include "i3d/gfx/GfxPass_metal.h"

namespace i3d { namespace gfx {

    //======================================================================================================================
    PassMetal::PassMetal() {
        m_label = nullptr;
    }
    
    //======================================================================================================================
    PassMetal::~PassMetal() {
        m_label = nullptr;
        m_encoder = nil;
    }
    
    //======================================================================================================================
    void PassMetal::Create( const char * label ) {
        m_label = label;
    }
}}
