//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/resbuild/RttiBuilder.h"
#include "i3d/rtti/RttiLoader.h"


namespace i3d {
    
    RTTI_CLASS_BEGIN( RttiBuilder )
        RTTI_PROP( STRING,  "input",           m_input )
    RTTI_CLASS_END( RttiBuilder )
    
    extern CVar res_buildPlatform;

    //======================================================================================================================
    RttiBuilder::RttiBuilder() {
        
    }

    //======================================================================================================================
    RttiBuilder::~RttiBuilder() {
        
    }

    //======================================================================================================================
    void RttiBuilder::GetDependencies( Resource * resource,
                                       stl::Vector<stl::String::type>::type & inputs ) {
        inputs.push_back( m_input );
    }

    //======================================================================================================================
    void RttiBuilder::Compile( Resource * resource ) {
        
        // Load and compile the rtti script into a binary stream
        RttiLoader rttiLoader;
        RttiStream rttiStream;
        
        i3d::PLATFORM_ID buildPlatformId = i3d::GetPlatformIdFromString( res_buildPlatform.GetString() );
        XE_ERROR( buildPlatformId == i3d::PLATFORM_ID_NONE, "Invalid resource build platform %s\n", res_buildPlatform.GetString() );
        
        rttiLoader.LoadAndCompileToStream( rttiStream, m_input.c_str(), buildPlatformId );
        
        // Save the stream to a file
        CreateFolderAtPath( resource->GetPath() );
        {
            ScopedFile file( resource->GetPath(), "wb" );
            XE_ERROR( file.IsValid() == false, "Error opening %s for writing\n", resource->GetPath() );
            rttiStream.WriteToFile( file );
        }
    }
}
