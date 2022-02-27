//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "xe/input/ActionResource.h"
#include "xe/rtti/RttiStream.h"
#include "xe/input/ActionManager.h"

namespace xe {
    
    extern CVar rtti_logLoad;
    extern CVar rtti_logLoadedObjects;
    
    DEFINE_RESOURCE( ActionResource, "action;" );

    //======================================================================================================================
    ActionResource::ActionResource() {
        
    }

    //======================================================================================================================
    ActionResource::~ActionResource() {
        
    }

    //======================================================================================================================
    void ActionResource::Load( File * file, const char * path ) {
        RttiStream stream;
        
        if (rtti_logLoad.GetBool() ) {
            XE_LOG( "Reading rtti objects..." );
        }
        
        stream.ReadFromFile( file );
        
        if (rtti_logLoad.GetBool() ) {
            XE_LOG( "Done\n");
            XE_LOG( "Loaded %u objects\n", stream.m_objectInfo.GetSize() );
        }
    
        if (rtti_logLoad.GetBool() ) {
            XE_LOG( "Constructing...");
        }
        
        stream.Construct();
        
        if ( rtti_logLoad.GetBool() == true && rtti_logLoadedObjects.GetBool() == true ) {
            XE_LOG( "\n");
            for ( auto o : stream.m_objects ) {
                XE_LOG( "    %s\n", o->GetType()->GetName() );
            }
        }
        if (rtti_logLoad.GetBool() ) {
            XE_LOG( "Done\n");
        }
        
        for ( auto o : stream.m_objects ) {
            if ( o->IsOfType<ActionProfile>() == true ) {
                ActionProfile * profile = o->SafeCast<ActionProfile>();
                if ( profile != nullptr ) {
                    XE_LOG("Added action profile '%s'\n", profile->m_name.GetCStr() );
                    m_profiles.PushBack( profile );
                    action->AddProfile( profile );
                }
            }
        }
    }
}
