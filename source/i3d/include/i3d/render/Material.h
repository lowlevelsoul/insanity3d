//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_MATERIAL_H__
#define __I3D_MATERIAL_H__

#include "i3d/core/Types.h"
#include "i3d/res/Resource.h"
#include "i3d/rtti/RttiObject.h"
#include "i3d/rtti/RttiResource.h"

namespace i3d {
    
    class Texture;
    
    class Material : public RttiObject {
    public:
        RTTI_CLASS_DECLARE( Material, RttiObject )
        
        enum STAGE {
            STAGE_ALBEDO=0,
            STAGE_AMR,
            STAGE_GLOW,
            STAGE_COUNT = 4,
            STAGE_FORCE32 = 0xffffffff,
        };
        
        Material();
        
        virtual ~Material();
        
    public:
        uint32_t        m_texturePoolId[STAGE_COUNT];
        uint64_t        m_batchTimeStamp;
        uint64_t        m_batchIndex;
        Texture *       m_albedo;
        Texture *       m_amr;
        Texture *       m_glow;
    };
    
    class MaterialResource : public Resource {
        DECLARE_RESOURCE( MaterialResource )
    public:
        MaterialResource();
        
        virtual ~MaterialResource();
        
        virtual void Load( File * file, const char * path ) override;
        
        Material * GetMaterial();
        
    protected:
        Material *      m_material;
    };
}

#endif
