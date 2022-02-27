//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __XENGINE_ACTIONRESOURCE_H__
#define __XENGINE_ACTIONERESOURCE_H__

#include "xe/input/ActionProfile.h"

namespace xe {
    
    class ActionProfile;
    
    class ActionResource : public Resource {
    public:
        DECLARE_RESOURCE( ActionResource )
        
        ActionResource();
        
        virtual ~ActionResource();
        
        virtual void Load( File * file, const char * path ) override;
        
    public:
        xe::Array<ActionProfile::ref_ptr_t>     m_profiles;
    };
}

#endif
