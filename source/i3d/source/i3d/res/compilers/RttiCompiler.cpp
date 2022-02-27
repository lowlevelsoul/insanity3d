//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "xe/res/compilers/RttiCompiler.h"
#include "xe/rtti/RttiLoader.h"

namespace xe {
    
    RTTI_CLASS_BEGIN( RttiCompiler )
        RTTI_PROP( STRING,  "input",           m_input )
    RTTI_CLASS_END( RttiCompiler )

    //======================================================================================================================
    RttiCompiler::RttiCompiler() {
        
    }

    //======================================================================================================================
    RttiCompiler::~RttiCompiler() {
        
    }

    //======================================================================================================================
    void RttiCompiler::GetDependencies( Resource * resource,
                                       xe::Array<xe::String> & inputs ) {
        inputs.PushBack( m_input );
    }

    //======================================================================================================================
    void RttiCompiler::Compile( Resource * resource ) {
        
        // Load and compile the rtti script into a binary stream
        RttiLoader rttiLoader;
        RttiStream rttiStream;
        rttiLoader.LoadAndCompileToStream(rttiStream, m_input.GetCStr() );
        
        // Save the stream to a file
        CreateFolderAtPath( resource->GetPath() );
        {
            xe::ScopedFile file( resource->GetPath(), "wb" );
            XE_ERROR( file.IsValid() == false, "Error opening %s for writing\n", resource->GetPath() );
            rttiStream.WriteToFile( file );
        }
    }
}
