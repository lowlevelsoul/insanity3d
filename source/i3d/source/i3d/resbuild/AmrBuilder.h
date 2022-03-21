//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_AMRBUILDER_H__
#define __I3D_AMRBUILDER_H__

#include "i3d/util/Image.h"
#include "i3d/res/ResourceBuilder.h"

namespace i3d {

    class AmrBuilder : public ResourceBuilder {
    public:
        RTTI_CLASS_DECLARE( AmrBuilder, ResourceBuilder )
        
        enum SRC_TEX {
            SRC_TEX_AO=0,
            SRC_TEX_ROUGHNESS,
            SRC_TEX_METALLIC,
            SRC_TEX_COUNT
        };
        
        static const uint8_t SRC_DEFAULT_VALUE[];
        
        stl::String::type           m_block;            ///< The block format string
        uint32_t                    m_mipCount;         ///< How many mip levels the texture should have
        uint32_t                    m_maxSize;          ///< Maximum texture size (width or height)
        
        stl::String::type           m_srcTexturePath[ SRC_TEX_COUNT ];
        Image::ref_ptr_t            m_srcTextures[ SRC_TEX_COUNT ];
        Image::ref_ptr_t            m_outTexture;
        
        AmrBuilder();
        
        ~AmrBuilder();
        
        virtual void GetDependencies( Resource * resource,
                                      stl::Vector<stl::String::type>::type & inputs ) override;
        
        virtual void Compile( Resource * resource ) override;
        
        bool Process( stl::String::type & errorMsg );
        
        bool SetupMissingImages( stl::String::type & errorMsg );
        
        bool LoadSourceImages( stl::String::type & errorMsg );
        
        bool MakeOutputTexture( stl::String::type & errorMsg );
    };
}

#endif
