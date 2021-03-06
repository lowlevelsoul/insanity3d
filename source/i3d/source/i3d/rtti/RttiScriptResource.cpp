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
        RttiStream stream;
        stream.ReadFromFile( file );
        stream.Construct();
        
        m_object = stream.m_objects[0];
    }
}
