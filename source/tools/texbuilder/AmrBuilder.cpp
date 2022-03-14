//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "texbuilder/AmrBuilder.h"

const uint8_t AmrBuilder::SRC_DEFAULT_VALUE[] = {
    64,
    40,
    0
};

static const char* SRC_NAMES[] = {
    "ambient-occlusion",
    "roughess",
    "metallic"
};

//===============================================================================================================================
AmrBuilder::AmrBuilder() {
    
}

//===============================================================================================================================
AmrBuilder::~AmrBuilder() {
    
}

//===============================================================================================================================
bool AmrBuilder::Process( std::string& errorMsg ) {
    bool ok = LoadSourceImages(errorMsg);
    if (ok == false) {
        return false;
    }
    
    ok = SetupMissingImages( errorMsg );
    if (ok == false) {
        return false;
    }
    
    ok = MakeOutputTexture( errorMsg );
    if (ok == false) {
        return false;
    }
    
    return true;
}

//===============================================================================================================================
bool AmrBuilder::LoadSourceImages( std::string& errorMsg ) {
    
    uint32_t i = 0;
    for(auto path : m_srcTexturePath) {
        
        if (path.empty() == false) {
            
            XE_LOG("Loading %s texture %s\n", SRC_NAMES[i], path.c_str());
            
            m_srcTextures[i] = new ToolImage;
            
            bool loadOk = m_srcTextures[i]->Load(path.c_str());
            if (loadOk == false) {
                errorMsg = sys->Vformat("Error loading AMR texture %s\n", path.c_str());
                return false;
            }
        }
                 
                 ++i;
    }
    
    return true;
}

//===============================================================================================================================
bool AmrBuilder::SetupMissingImages( std::string& errorMsg ) {
    // First find a valid imahge to get the image size
    bool gotSize = false;
    uint32_t width=0, height=0;
    
    for(uint32_t i=0; i < SRC_TEX_COUNT; ++i) {
        if (m_srcTextures[i] != nullptr) {
            width = m_srcTextures[i]->GetWidth();
            height = m_srcTextures[i]->GetHeight();
            gotSize = true;
            break;
        }
    }
    
    if (gotSize == false) {
        errorMsg = "No AMR textures found!";
        return false;
    }
    
    // Fill in any missing textures with their default values
    for(uint32_t i=0; i < SRC_TEX_COUNT; ++i) {
        if (m_srcTextures[i] == nullptr) {
            
            XE_LOG("Creating default texture for %s\n", SRC_NAMES[i]);
            
            ToolImage* img = new ToolImage;
            img->Initialise(width, height, ToolImage::FORMAT_RGBA_U8);
            
            for(uint32_t y=0; y < height; ++y) {
                for(uint32_t x=0; x < width; ++x) {
                    img->SetRgba(x, y, SRC_DEFAULT_VALUE[i], SRC_DEFAULT_VALUE[i], SRC_DEFAULT_VALUE[i]);
                }
            }
            
            m_srcTextures[i] = img;
        }
    }
    
    return true;
}

//===============================================================================================================================
bool AmrBuilder::MakeOutputTexture( std::string& errorMsg ) {
    
    XE_LOG("Constructing final AMR texture\n");

    m_outTexture = new ToolImage;
    m_outTexture->Initialise(m_srcTextures[0]->GetWidth(), m_srcTextures[0]->GetHeight(), ToolImage::FORMAT_RGBA_U8);
    
    for(int32_t y=0; y < m_srcTextures[0]->GetHeight(); ++y) {
        for(int32_t x=0; x < m_srcTextures[0]->GetWidth(); ++x) {
            float ao = m_srcTextures[SRC_TEX_AO]->CalcIntensity(x, y);
            float metallic = m_srcTextures[SRC_TEX_METALLIC]->CalcIntensity(x, y);
            float roughness = m_srcTextures[SRC_TEX_ROUGHNESS]->CalcIntensity(x, y);
            
            ao = std::fmin(1, std::fmax(0, ao));
            metallic = std::fmin(1, std::fmax(0, metallic));
            roughness = std::fmin(1, std::fmax(0, roughness));
            
            m_outTexture->SetRgba(x, y, ao, metallic, roughness);
        }
    }
        
    return true;
}
