//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "xe/res/compilers/AmrCompiler.h"
#include "xe/res/compilers/TextureCompiler.h"

namespace xe {
    
    RTTI_CLASS_BEGIN( AmrCompiler )
        RTTI_PROP( STRING,  "ao",           m_srcTexturePath[ SRC_TEX_AO ] )
        RTTI_PROP( STRING,  "roughness",    m_srcTexturePath[ SRC_TEX_ROUGHNESS ] )
        RTTI_PROP( STRING,  "metallic",     m_srcTexturePath[ SRC_TEX_METALLIC ] )
        RTTI_PROP( STRING,  "block",        m_block )
        RTTI_PROP( INT,     "mip_count",    m_mipCount )
        RTTI_PROP( INT,     "max_size",     m_maxSize )
    RTTI_CLASS_END( AmrCompiler )

    const uint8_t AmrCompiler::SRC_DEFAULT_VALUE[] = {
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
    AmrCompiler::AmrCompiler() {
        m_mipCount = 0;
        m_maxSize = 0;
    }

    //===============================================================================================================================
    AmrCompiler::~AmrCompiler() {
        
    }
    
    //===============================================================================================================================
    void  AmrCompiler::GetDependencies( Resource * resource,
                                         xe::Array<xe::String> & inputs ) {
    
        for ( uint32_t i = 0; i < SRC_TEX_COUNT; ++i ) {
            if ( m_srcTexturePath[i].IsEmpty() == false ) {
                inputs.PushBack( m_srcTexturePath[i] );
            }
        }
    }
    
    //===============================================================================================================================
    void  AmrCompiler::Compile( Resource * resource ) {
        xe::String errorMsg;
        bool ok = Process( errorMsg );
        XE_ERROR( ok == false, "Error processing source textures. Reason: %s\n", errorMsg.GetCStr() );
        
        TextureCompiler texComp;
        texComp.m_baseImage     = m_outTexture;
        texComp.m_maxSize       = m_maxSize;
        texComp.m_block         = m_block;
        texComp.m_mipCount      = m_mipCount;
        
        texComp.Compile( resource );        
    }

    //===============================================================================================================================
    bool AmrCompiler::Process( xe::String & errorMsg ) {
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
    bool AmrCompiler::LoadSourceImages( xe::String & errorMsg ) {
        
        uint32_t i = 0;
        for( auto path : m_srcTexturePath ) {
            
            if ( path.IsEmpty() == false ) {
                
                XE_LOG( "Loading %s texture %s\n", SRC_NAMES[i], path.GetCStr() );
                
                m_srcTextures[i] = new Image;
                
                bool loadOk = m_srcTextures[i]->Load(path.GetCStr() );
                if ( loadOk == false ) {
                    errorMsg = sys->Vformat( "Error loading AMR texture %s\n", path.GetCStr() );
                    return false;
                }
            }
                     
                     ++i;
        }
        
        return true;
    }

    //===============================================================================================================================
    bool AmrCompiler::SetupMissingImages( xe::String & errorMsg ) {
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
                
                Image* img = new Image;
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
    bool AmrCompiler::MakeOutputTexture( xe::String & errorMsg ) {
        
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
