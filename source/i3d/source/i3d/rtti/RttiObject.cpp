//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//=======================================================================================================================

#include "i3d/rtti/RttiObject.h"

namespace i3d {
    
    RTTI_CLASS_BEGIN(RttiObject)
    RTTI_CLASS_END(RttiObject)

    //======================================================================================================================
    RttiObject::RttiObject() {
        m_refCount = 0;
        m_propsWereRead = 0;
    }

    //======================================================================================================================
    RttiObject::~RttiObject() {
        
    }

    //======================================================================================================================
    bool RttiObject::ReadComplete( const char * path ) {
        return true;
    }
}
