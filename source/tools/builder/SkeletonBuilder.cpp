//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "SkeletonBuilder.h"
#include "Builder.h"

RTTI_CLASS_BEGIN( SkeletonBuilder )
    RTTI_PROP( STRING, "input", m_input )
    RTTI_PROP( FLOAT, "scale", m_scale )
    RTTI_PROP( BOOL, "stripmixamo", m_stripMixamo )
    RTTI_PROP( STRING, "rootnode", m_rootNode )
RTTI_CLASS_END( SkeletonBuilder )

//======================================================================================================================
SkeletonBuilder::SkeletonBuilder() {
    m_scale = 1;
}

//======================================================================================================================
SkeletonBuilder::~SkeletonBuilder() {
    
}

//======================================================================================================================
void SkeletonBuilder::GatherInputs( std::vector<i3d::stl::String::type> & inputs ) {
    inputs.push_back( MakeInputFilePath( m_input.c_str() ) );
    inputs.push_back( m_builder->GetToolPath("modelbuilder") );
}

//======================================================================================================================
void SkeletonBuilder::GatherOutputs( std::vector<i3d::stl::String::type> & outputs ) {
    i3d::stl::String::type outputFilePath = MakeOutputFilePath( ".skl" );
    outputs.push_back( outputFilePath );
}

//======================================================================================================================
bool SkeletonBuilder::Run() {
    i3d::stl::String::type exePath = GetToolExePath();
    
    i3d::stl::String::type inputFilePath = MakeInputFilePath( m_input.c_str() );
    i3d::stl::String::type outputFilePath = MakeOutputFilePath( ".skl");
    
    i3d::stl::Vector<i3d::stl::String::type>::type args;
    args.push_back( "+infile" );
    args.push_back( inputFilePath.c_str() );
    args.push_back( "+outfile" );
    args.push_back( outputFilePath.c_str() );
    args.push_back( "+skeleton" );
    
    if (m_scale != 1) {
        args.push_back("+scale");
        args.push_back( sys->Vformat( "%5.5f", m_scale ) );
    }
    
    if (m_stripMixamo == true) {
        args.push_back("+stripmixamo");
    }
    
    if ( m_rootNode.empty() == false ) {
        args.push_back("+rootnode");
        args.push_back( m_rootNode );
    }

    sys->Exec( exePath.c_str(), args );
    
    return true;
}
