//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/rtti/RttiScriptResource.h"
#include "i3d/rtti/RttiLoader.h"

namespace i3d {
    
    DEFINE_RESOURCE( RttiScriptResource, "rtti;" )
    
    //======================================================================================================================
    RttiScriptResource::RttiScriptResource() {
        m_object = nullptr;
    }
    
    //======================================================================================================================
    RttiScriptResource::~RttiScriptResource() {
        SAFE_DELETE( m_object );
    }
    
    //======================================================================================================================
    void RttiScriptResource::Load( File * file, const char * path ) {
        RttiLoader loader;
        loader.Load( file, path );
        
        m_object = loader.m_object;
    }
}
