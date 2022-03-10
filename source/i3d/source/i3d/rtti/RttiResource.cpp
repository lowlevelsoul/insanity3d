//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/rtti/RttiResource.h"
#include "i3d/rtti/RttiStream.h"

namespace i3d {
    
    i3d::CVar rtti_logLoadedObjects("rtti_logLoadedObjects", false, "Spits out a list of the object types loaded from an rtti binary after loading / construction");
    i3d::CVar rtti_logLoad("rtti_logLoad", false, "Logs out info about rtti objects being loaded");
    
    DEFINE_RESOURCE( RttiResource, "brtti;" )
    
    //======================================================================================================================
    RttiResource::RttiResource() {
        m_object = nullptr;
    }
    
    //======================================================================================================================
    RttiResource::~RttiResource() {
        SAFE_DELETE( m_object );
    }
    
    //======================================================================================================================
    void RttiResource::Load( File * file, const char * path ) {
        RttiStream stream;
        
        if (rtti_logLoad.GetBool() ) {
            XE_LOG( "Reading rtti objects..." );
        }
        
        stream.ReadFromFile( file );
        
        if (rtti_logLoad.GetBool() ) {
            XE_LOG( "Done\n");
            XE_LOG( "Loaded %u objects\n", stream.m_objectInfo.size() );
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
        
        m_object = stream.m_objects[0];
    }
}
