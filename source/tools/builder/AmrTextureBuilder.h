//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __AMRTEXTUREBUILDER_H__
#define __AMRTEXTUREBUILDER_H__

#include "BuilderTool.h"

class AmrTextureBuilder : public BuilderTool {
    RTTI_CLASS_DECLARE( AmrTextureBuilder, BuilderTool )
public:
    AmrTextureBuilder();
    
    virtual ~AmrTextureBuilder();
    
    virtual bool Run();
    
    virtual void GatherInputs( std::vector<i3d::stl::String::type> & inputs );
    
    virtual void GatherOutputs( std::vector<i3d::stl::String::type> & outputs );
    
    virtual const char * GetExeName() const { return "texbuilder"; }
    
    const i3d::stl::String::type GetFullTexturePath( i3d::stl::String::type & tex );
    
public:
    uint32_t        m_width;
    uint32_t        m_height;
    uint32_t        m_mipCount;
    i3d::stl::String::type     m_block;
    i3d::stl::String::type     m_ambientOcclusion;
    i3d::stl::String::type     m_metallic;
    i3d::stl::String::type     m_roughness;
};

#endif
