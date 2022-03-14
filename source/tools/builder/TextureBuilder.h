//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __TEXTUREBUILDER_H__
#define __TEXTUREBUILDER_H__

#include "BuilderTool.h"

class TextureBuilder : public BuilderTool {
    RTTI_CLASS_DECLARE( TextureBuilder, BuilderTool )
public:
    TextureBuilder();
    
    virtual ~TextureBuilder();
    
    virtual bool Run();
    
    virtual void GatherInputs( std::vector<std::string> & inputs );
    
    virtual void GatherOutputs( std::vector<std::string> & outputs );
    
    virtual const char * GetExeName() const { return "texbuilder"; }
    
    const std::string GetFullTexturePath();
    
public:
    uint32_t        m_width;
    uint32_t        m_height;
    uint32_t        m_mipCount;
    std::string     m_block;
    std::string     m_input;
};

#endif
