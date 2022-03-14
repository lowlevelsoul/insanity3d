//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "AmrTextureBuilder.h"
#include "Builder.h"

RTTI_CLASS_BEGIN( AmrTextureBuilder )
    RTTI_PROP( STRING, "ao", m_ambientOcclusion )
    RTTI_PROP( STRING, "metallic", m_metallic )
    RTTI_PROP( STRING, "roughness", m_roughness )
    RTTI_PROP( STRING, "block", m_block )
    RTTI_PROP( UINT, "width", m_width )
    RTTI_PROP( UINT, "height", m_height )
    RTTI_PROP( UINT, "mipcount", m_mipCount )
RTTI_CLASS_END( AmrTextureBuilder )


//======================================================================================================================
AmrTextureBuilder::AmrTextureBuilder() {
    m_width = 0;
    m_height = 0;
    m_mipCount = 0;
}

//======================================================================================================================
AmrTextureBuilder::~AmrTextureBuilder() {
    
}

//======================================================================================================================
void AmrTextureBuilder::GatherInputs( std::vector<std::string> & inputs ) {
    if ( m_ambientOcclusion.empty() == false ) {
        inputs.push_back( GetFullTexturePath( m_ambientOcclusion ) );
    }
    
    if ( m_metallic.empty() == false ) {
        inputs.push_back( GetFullTexturePath( m_metallic ) );
    }
    
    if ( m_roughness.empty() == false ) {
        inputs.push_back( GetFullTexturePath( m_roughness ) );
    }
}

//======================================================================================================================
void AmrTextureBuilder::GatherOutputs( std::vector<std::string> & outputs ) {
    outputs.push_back( MakeOutputFilePath( ".tex" ) );
}

//======================================================================================================================
const std::string AmrTextureBuilder::GetFullTexturePath( std::string & tex ) {
    std::string inputFilePath = MakeInputFilePath( tex.c_str() );
    return inputFilePath;
}

//======================================================================================================================
bool AmrTextureBuilder::Run() {
    
    std::vector<std::string> args;
    args.push_back( "+amr" );
    
    if (m_ambientOcclusion.empty() == false ) {
        args.push_back( "+ambient" );
        args.push_back( GetFullTexturePath( m_ambientOcclusion ) );
    }
    
    if (m_metallic.empty() == false ) {
        args.push_back( "+metallic" );
        args.push_back( GetFullTexturePath( m_metallic ) );
    }
    
    if (m_roughness.empty() == false ) {
        args.push_back( "+roughness" );
        args.push_back( GetFullTexturePath( m_roughness ) );
    }

    args.push_back( "+outfile" );
    args.push_back( MakeOutputFilePath( ".tex" ) );
    
    if ( m_width != 0 && m_height != 0 ) {
        args.push_back( "+size" );
        args.push_back( sys->Vformat( "%u", m_width ) );
        args.push_back( sys->Vformat( "%u", m_height ) );
    }
    
    if ( m_mipCount > 0 ) {
        args.push_back( "+genmips" );
        args.push_back( sys->Vformat( "%u", m_mipCount ) );
    }
    
    if ( m_block.empty() == false ) {
        args.push_back( "+block" );
        args.push_back( m_block );
    }
    
    sys->Exec( GetToolExePath().c_str(), args );
    
    return false;
}



