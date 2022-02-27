//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/render/ModelSkin.h"

namespace i3d {
    
    RTTI_CLASS_BEGIN( MeshSkin )
        RTTI_PROP( STRING, "mesh", m_mesh)
        RTTI_PROP( RESOURCE, "material", m_material )
    RTTI_CLASS_END( MeshSkin )
    
    RTTI_CLASS_BEGIN( ModelSkin )
        RTTI_PROP_ARRAY( OBJECT_REFPTR, "meshes", m_meshes )
    RTTI_CLASS_END( ModelSkin )
    
    DEFINE_RESOURCE( ModelSkinResource, "skin;" )
    
    //======================================================================================================================
    MeshSkin::MeshSkin() {
        m_material = nullptr;
    }
    
    //======================================================================================================================
    MeshSkin::~MeshSkin() {
    }
    
    //======================================================================================================================
    ModelSkin::ModelSkin() {
    }
    
    //======================================================================================================================
    ModelSkin::~ModelSkin() {
    }
    
    //======================================================================================================================
    void ModelSkin::ApplyToModel( ModelInstance * model ) {
        for ( auto ms : m_meshes ) {
            int32_t index = model->FindMeshIndex( ms->m_mesh.GetCStr() );
            if ( index >= 0 ) {
                model->SetMeshMaterial( index, ms->m_material->GetMaterial() );
            }
        }
    }
    
    //======================================================================================================================
    ModelSkinResource::ModelSkinResource() {
        
    }
    
    //======================================================================================================================
    ModelSkinResource::~ModelSkinResource() {
        
    }
    
    //======================================================================================================================
    void ModelSkinResource::ApplyToModel( ModelInstance * model ) {
        ModelSkin * skin = m_object->SafeCast<ModelSkin>();
        XE_ASSERT_MSG(skin != nullptr, "ModelSkinResource object is not of type ModelSkin\n");
        skin->ApplyToModel( model );
    }
}
