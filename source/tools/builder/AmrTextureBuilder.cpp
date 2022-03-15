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
    RTTI_PROP( INT, "width", m_width )
    RTTI_PROP( INT, "height", m_height )
    RTTI_PROP( INT, "mipcount", m_mipCount )
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
void AmrTextureBuilder::GatherInputs( std::vector<i3d::stl::String::type> & inputs ) {
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
void AmrTextureBuilder::GatherOutputs( std::vector<i3d::stl::String::type> & outputs ) {
    outputs.push_back( MakeOutputFilePath( ".btex" ) );
}

//======================================================================================================================
const i3d::stl::String::type AmrTextureBuilder::GetFullTexturePath( i3d::stl::String::type & tex ) {
    i3d::stl::String::type inputFilePath = MakeInputFilePath( tex.c_str() );
    return inputFilePath;
}

//======================================================================================================================
bool AmrTextureBuilder::Run() {
    
    i3d::stl::Vector<i3d::stl::String::type>::type args;
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
    args.push_back( MakeOutputFilePath( ".btex" ) );
    
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



