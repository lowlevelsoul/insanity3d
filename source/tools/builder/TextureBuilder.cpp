//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "TextureBuilder.h"
#include "Builder.h"

RTTI_CLASS_BEGIN( TextureBuilder )
    RTTI_PROP( STRING, "input", m_input )
    RTTI_PROP( STRING, "block", m_block )
    RTTI_PROP( UINT, "width", m_width )
    RTTI_PROP( UINT, "height", m_height )
    RTTI_PROP( UINT, "mipcount", m_mipCount )
RTTI_CLASS_END( TextureBuilder )


//======================================================================================================================
TextureBuilder::TextureBuilder() {
    m_width = 0;
    m_height = 0;
    m_mipCount = 0;
}

//======================================================================================================================
TextureBuilder::~TextureBuilder() {
    
}

//======================================================================================================================
void TextureBuilder::GatherInputs( std::vector<std::string> & inputs ) {
    inputs.push_back( GetFullTexturePath() );
}

//======================================================================================================================
void TextureBuilder::GatherOutputs( std::vector<std::string> & outputs ) {
    outputs.push_back( MakeOutputFilePath( ".tex" ) );
}

//======================================================================================================================
const std::string TextureBuilder::GetFullTexturePath() {
    std::string inputFilePath = MakeInputFilePath( m_input.c_str() );
    return inputFilePath;
}

//======================================================================================================================
bool TextureBuilder::Run() {
    
    std::string inputFilePath = GetFullTexturePath();
    std::string outFilePath = MakeOutputFilePath( ".tex" );
    
    std::vector<std::string> args;
    args.push_back("+infile");
    args.push_back(inputFilePath);
    args.push_back("+outfile");
    args.push_back(outFilePath);
    
    if ( m_width != 0 && m_height != 0 ) {
        args.push_back( "+size" );
        args.push_back( sys->Vformat("%u", m_width ) );
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



