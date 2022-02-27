//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __XENGINE_AMRCOMPILER_H__
#define __XENGINE_AMRCOMPILER_H__

#include "xe/util/Image.h"
#include "xe/res/ResourceCompiler.h"

namespace xe {

    class AmrCompiler : public ResourceCompiler {
    public:
        RTTI_CLASS_DECLARE( AmrCompiler, ResourceCompiler )
        
        enum SRC_TEX {
            SRC_TEX_AO=0,
            SRC_TEX_ROUGHNESS,
            SRC_TEX_METALLIC,
            SRC_TEX_COUNT
        };
        
        static const uint8_t SRC_DEFAULT_VALUE[];
        
        xe::String                  m_block;            ///< The block format string
        uint32_t                    m_mipCount;         ///< How many mip levels the texture should have
        uint32_t                    m_maxSize;          ///< Maximum texture size (width or height)
        
        xe::String                  m_srcTexturePath[ SRC_TEX_COUNT ];
        Image::ref_ptr_t            m_srcTextures[ SRC_TEX_COUNT ];
        Image::ref_ptr_t            m_outTexture;
        
        AmrCompiler();
        
        ~AmrCompiler();
        
        virtual void GetDependencies( Resource * resource,
                                      xe::Array<xe::String> & inputs ) override;
        
        virtual void Compile( Resource * resource ) override;
        
        bool Process( xe::String & errorMsg );
        
        bool SetupMissingImages( xe::String & errorMsg );
        
        bool LoadSourceImages( xe::String & errorMsg );
        
        bool MakeOutputTexture( xe::String & errorMsg );
    };
}

#endif
