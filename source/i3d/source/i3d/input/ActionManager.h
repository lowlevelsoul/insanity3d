//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __XENGINE_ACTIONMANAGER_H__
#define __XENGINE_ACTIONMANAGER_H__

#include "xe/input/ActionProfile.h"

namespace xe {

    class ActionManager {
    public:
        ActionManager();
        
        ~ActionManager();
        
        ActionProfile * AddProfile( const char * name );
        
        void AddProfile( ActionProfile * profile );
        
        ActionProfile * FindProfle( const char * name );
        
        void BroadcastAction( Key key, float value );
        
        void BeginFrame();
        
    protected:
        
        uint64_t MakeProfileNameHash( const char * name );
        
    protected:
        xe::Array<uint64_t>                     m_nameHashArray;
        xe::Array<ActionProfile::ref_ptr_t>     m_profiles;
    };
}

extern xe::ActionManager * action;
    
#endif
