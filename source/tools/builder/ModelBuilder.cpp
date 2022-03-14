//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "ModelBuilder.h"
#include "Builder.h"

RTTI_CLASS_BEGIN( ModelBuilder )
    RTTI_PROP( STRING, "input", m_input )
    RTTI_PROP( FLOAT, "scale", m_scale )
    RTTI_PROP( BOOL, "flipfaces", m_flipFaces )
    RTTI_PROP( BOOL, "gennormals", m_genNormals )
    RTTI_PROP( BOOL, "stripmixamo", m_stripMixamo )
    RTTI_PROP( STRING, "rootnode", m_rootNode )
RTTI_CLASS_END( ModelBuilder )

//======================================================================================================================
ModelBuilder::ModelBuilder() {
    m_scale = 1;
    m_flipFaces = false;
}

//======================================================================================================================
ModelBuilder::~ModelBuilder() {
    
}

//======================================================================================================================
void ModelBuilder::GatherInputs( std::vector<std::string> & inputs ) {
    inputs.push_back( MakeInputFilePath( m_input.c_str() ) );
    inputs.push_back( m_builder->GetToolPath("modelbuilder") );
}

//======================================================================================================================
void ModelBuilder::GatherOutputs( std::vector<std::string> & outputs ) {
    std::string outputFilePath = MakeOutputFilePath( ".mdl" );
    outputs.push_back( outputFilePath );
}

//======================================================================================================================
bool ModelBuilder::Run() {
    std::string exePath = GetToolExePath();
    
    std::string inputFilePath = MakeInputFilePath( m_input.c_str() );
    std::string outputFilePath = MakeOutputFilePath( ".mdl");
    
    std::vector<std::string> args;
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

    sys->Exec( exePath.c_str(), args );
    
    return true;
}
