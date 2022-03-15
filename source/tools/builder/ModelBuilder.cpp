//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ModelBuilder.h"
#include "Builder.h"

RTTI_CLASS_BEGIN( ModelBuilder )
    RTTI_PROP( STRING, "input", m_input )
    RTTI_PROP( FLOAT, "scale", m_scale )
    RTTI_PROP( BOOL, "flip_faces", m_flipFaces )
    RTTI_PROP( BOOL, "gen_normals", m_genNormals )
    RTTI_PROP( BOOL, "strip_mixamo", m_stripMixamo )
    RTTI_PROP( STRING, "rootnode", m_rootNode )
    RTTI_PROP_ARRAY(STRING, "mesh_filter", m_meshFilter)
RTTI_CLASS_END( ModelBuilder )

//======================================================================================================================
ModelBuilder::ModelBuilder() {
    m_scale = 1;
    m_flipFaces = false;
    m_genNormals = false;
    m_stripMixamo = false;
}

//======================================================================================================================
ModelBuilder::~ModelBuilder() {
    
}

//======================================================================================================================
void ModelBuilder::GatherInputs( std::vector<i3d::stl::String::type> & inputs ) {
    inputs.push_back( MakeInputFilePath( m_input.c_str() ) );
    inputs.push_back( m_builder->GetToolPath("modelbuilder") );
}

//======================================================================================================================
void ModelBuilder::GatherOutputs( std::vector<i3d::stl::String::type> & outputs ) {
    i3d::stl::String::type outputFilePath = MakeOutputFilePath( ".bmdl" );
    outputs.push_back( outputFilePath );
}

//======================================================================================================================
bool ModelBuilder::Run() {
    i3d::stl::String::type exePath = GetToolExePath();
    
    i3d::stl::String::type inputFilePath = MakeInputFilePath( m_input.c_str() );
    i3d::stl::String::type outputFilePath = MakeOutputFilePath( ".bmdl");
    
    const char * outPath = outputFilePath.c_str();
    
    i3d::stl::Vector<i3d::stl::String::type>::type args;
    args.push_back( "+infile" );
    args.push_back( inputFilePath.c_str() );
    args.push_back( "+outfile" );
    args.push_back( outputFilePath.c_str() );
    
    if (m_scale != 1) {
        args.push_back("+scale");
        args.push_back( sys->Vformat( "%5.5f", m_scale ) );
    }
    
    if (m_flipFaces == true) {
        args.push_back("+flipfaces");
    }
    
    if (m_genNormals == true) {
        args.push_back("+gennormals");
    }
    
    if (m_stripMixamo == true) {
        args.push_back("+stripmixamo");
    }
    
    if ( m_meshFilter.empty() == false ) {
        args.push_back("+meshfilter");
        for ( auto & m : m_meshFilter ) {
            args.push_back( m );
        }
    }

    sys->Exec( exePath.c_str(), args );
    
    return true;
}
