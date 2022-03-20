//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "Builder.h"
#include "RttiBuilder.h"
#include "i3d/rtti/RttiLoader.h"
#include "i3d/rtti/RttiStream.h"

RTTI_CLASS_BEGIN( RttiBuilder )
    RTTI_PROP( STRING, "input", m_input )
    RTTI_PROP( STRING, "output_ext", m_outputExt )
RTTI_CLASS_END( RttiBuilder )

//======================================================================================================================
RttiBuilder::RttiBuilder() {
    m_outputExt = ".brtti";
}

//======================================================================================================================
RttiBuilder::~RttiBuilder() {
    
}

//======================================================================================================================
void RttiBuilder::GatherInputs( std::vector<i3d::stl::String::type> & inputs ) {
    inputs.push_back( MakeInputFilePath( m_input.c_str() ) );
}

//======================================================================================================================
void RttiBuilder::GatherOutputs( std::vector<i3d::stl::String::type> & outputs ) {
    i3d::stl::String::type outputFilePath = MakeOutputFilePath( m_outputExt.c_str() );
    outputs.push_back( outputFilePath );
}

//======================================================================================================================
bool RttiBuilder::Run() {
    i3d::stl::String::type exePath = GetToolExePath();
    
    i3d::stl::String::type inputFilePath = MakeInputFilePath( m_input.c_str() );
    i3d::stl::String::type outputFilePath = MakeOutputFilePath( m_outputExt.c_str() );
    
    i3d::RttiStream rttiStream;
    
    i3d::RttiLoader rttiLoader;
    rttiLoader.LoadAndCompileToStream( rttiStream, inputFilePath.c_str() );
    
    XE_LOG("Writing rtti stream to %s", outputFilePath.c_str() );
    i3d::ScopedFile file(m_outputPath.c_str(), "wb");
    rttiStream.WriteToFile( file );
    
    return true;
}
