//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#ifndef __TOOLMATERIAL_H__
#define __TOOLMATERIAL_H__

#include "i3d/core/Types.h"
#include "toolapp/RefObject.h"
#include "toolapp/RefPointer.h"

class ToolModel;

class ToolMaterial : public RefObject {
public:
    typedef RefPointer<ToolMaterial> ref_ptr_t;
    typedef std::vector<ref_ptr_t> vector_t;
    typedef std::map<uint64_t, ToolMaterial * > map_t;

    enum STAGE {
        STAGE_ALBEDO = 0,
        STAGE_METALLIC,
        STAGE_ROUGHNESS,
        STAGE_METALLICROUGHNESS,
        STAGE_AMBIENT_OCCLUSION,
        STAGE_AMBIENT,
        STAGE_GLOW,
        STAGE_SPECULAR,
        STAGE_COUNT
    };
    
    ToolMaterial( ToolModel * model );
    
    virtual ~ToolMaterial();

public:
    ToolModel *             m_model;
    std::string             m_name;                         ///< Joint name
    std::string             m_textures[ STAGE_COUNT ];      ///< Textures for this stage
};

#endif
