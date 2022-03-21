//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/resbuild/AmrBuilder.h"
#include "i3d/resbuild/TextureBuilder.h"

namespace i3d {
    
    RTTI_CLASS_BEGIN( AmrBuilder )
        RTTI_PROP( STRING,  "ao",           m_srcTexturePath[ SRC_TEX_AO ] )
        RTTI_PROP( STRING,  "roughness",    m_srcTexturePath[ SRC_TEX_ROUGHNESS ] )
        RTTI_PROP( STRING,  "metallic",     m_srcTexturePath[ SRC_TEX_METALLIC ] )
        RTTI_PROP( STRING,  "block",        m_block )
        RTTI_PROP( INT,     "mip_count",    m_mipCount )
        RTTI_PROP( INT,     "max_size",     m_maxSize )
    RTTI_CLASS_END( AmrBuilder )

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
        m_mipCount = 0;
        m_maxSize = 0;
    }

    //===============================================================================================================================
    AmrBuilder::~AmrBuilder() {
        
    }
    
    //===============================================================================================================================
    void  AmrBuilder::GetDependencies( Resource * resource,
                                       stl::Vector<stl::String::type>::type & inputs ) {
    
        for ( uint32_t i = 0; i < SRC_TEX_COUNT; ++i ) {
            if ( m_srcTexturePath[i].empty() == false ) {
                inputs.push_back( m_srcTexturePath[i] );
            }
        }
    }
    
    //===============================================================================================================================
    void  AmrBuilder::Compile( Resource * resource ) {
        stl::String::type errorMsg;
        bool ok = Process( errorMsg );
        XE_ERROR( ok == false, "Error processing source textures. Reason: %s\n", errorMsg.c_str() );
        
        TextureBuilder texComp;
        texComp.m_baseImage     = m_outTexture;
        texComp.m_maxSize       = m_maxSize;
        texComp.m_block         = m_block;
        texComp.m_mipCount      = m_mipCount;
        
        texComp.Compile( resource );        
    }

    //===============================================================================================================================
    bool AmrBuilder::Process( stl::String::type & errorMsg ) {
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
    bool AmrBuilder::LoadSourceImages( stl::String::type & errorMsg ) {
        
        uint32_t i = 0;
        for( auto path : m_srcTexturePath ) {
            
            if ( path.empty() == false ) {
                
                XE_LOG( "Loading %s texture %s\n", SRC_NAMES[i], path.c_str() );
                
                m_srcTextures[i] = new Image;
                
                bool loadOk = m_srcTextures[i]->Load(path.c_str() );
                if ( loadOk == false ) {
                    errorMsg = sys->Vformat( "Error loading AMR texture %s\n", path.c_str() );
                    return false;
                }
            }
                     
                     ++i;
        }
        
        return true;
    }

    //===============================================================================================================================
    bool AmrBuilder::SetupMissingImages( stl::String::type & errorMsg ) {
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
                
                Image * img = new Image;
                img->Initialise(width, height, Image::FORMAT_RGBA_U8);
                
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
    bool AmrBuilder::MakeOutputTexture( stl::String::type & errorMsg ) {
        
        XE_LOG("Constructing final AMR texture\n");

        m_outTexture = new Image;
        m_outTexture->Initialise( m_srcTextures[0]->GetWidth(), m_srcTextures[0]->GetHeight(), Image::FORMAT_RGBA_U8 );
        
        for( int32_t y=0; y < m_srcTextures[0]->GetHeight(); ++y ) {
            for( int32_t x=0; x < m_srcTextures[0]->GetWidth(); ++x ) {
                
                float ao = m_srcTextures[SRC_TEX_AO]->CalcIntensity( x, y );
                float metallic = m_srcTextures[SRC_TEX_METALLIC]->CalcIntensity(x, y);
                float roughness = m_srcTextures[SRC_TEX_ROUGHNESS]->CalcIntensity(x, y);
                
                ao = std::fmin( 1, std::fmax(0, ao));
                metallic = std::fmin(1, std::fmax(0, metallic));
                roughness = std::fmin(1, std::fmax(0, roughness));
                
                m_outTexture->SetRgba(x, y, ao, metallic, roughness);
            }
        }
            
        return true;
    }
}
