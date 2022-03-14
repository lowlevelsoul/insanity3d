//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __AMRBUILDER_H__
#define __AMRBUILDER_H__

#include "toolimage/ToolImage.h"

class AmrBuilder {
public:
    enum SRC_TEX {
        SRC_TEX_AO=0,
        SRC_TEX_ROUGHNESS,
        SRC_TEX_METALLIC,
        SRC_TEX_COUNT
    };
    
    static const uint8_t SRC_DEFAULT_VALUE[];
    
    std::string             m_srcTexturePath[SRC_TEX_COUNT];
    ToolImage::Ptr          m_srcTextures[SRC_TEX_COUNT];
    ToolImage::Ptr          m_outTexture;
    
    AmrBuilder();
    
    ~AmrBuilder();
    
    bool Process( std::string& errorMsg );
    
    bool SetupMissingImages( std::string& errorMsg );
    
    bool LoadSourceImages( std::string& errorMsg );
    
    bool MakeOutputTexture( std::string& errorMsg );
};

#endif
