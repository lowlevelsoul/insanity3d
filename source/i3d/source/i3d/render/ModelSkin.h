//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_MODELSKIN_H__
#define __I3D_MODELSKIN_H__

#include "i3d/rtti/RttiObject.h"
#include "i3d/render/Material.h"
#include "i3d/rtti/RttiResource.h"

namespace i3d {
    
    class ModelInstance;
    
    class MeshSkin : public RttiObject {
        RTTI_CLASS_DECLARE( MeshSkin, RttiObject)
    public:
        MeshSkin();
        
        virtual ~MeshSkin();
        
    public:
        xe::String              m_mesh;
        xe::MaterialResource *  m_material;        
    };

    class ModelSkin : public RttiObject {
        RTTI_CLASS_DECLARE( ModelSkin, RttiObject )
    public:
        ModelSkin();
        
        virtual ~ModelSkin();
        
        void ApplyToModel( ModelInstance * model );
        
    public:
        xe::Array<MeshSkin::ref_ptr_t> m_meshes;
    };
    
    class ModelSkinResource : public RttiResource {
        DECLARE_RESOURCE( ModelSkinResource )
    public:
        ModelSkinResource();
        
        virtual ~ModelSkinResource();
        
        void ApplyToModel( ModelInstance * model );        
    };
}

#endif
