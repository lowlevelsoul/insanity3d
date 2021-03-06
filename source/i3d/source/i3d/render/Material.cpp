//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/render/Material.h"
#include "i3d/rtti/RttiLoader.h"

namespace i3d {

    RTTI_CLASS_BEGIN( Material )
        RTTI_PROP( RESOURCE, "tex_albedo", m_albedo )
        RTTI_PROP( RESOURCE, "tex_amr", m_amr )
        RTTI_PROP( RESOURCE, "tex_glow", m_glow )
    RTTI_CLASS_END( Material )
    
    DEFINE_RESOURCE( MaterialResource, "mat;" )
    
    //======================================================================================================================
    Material::Material() {
        m_batchTimeStamp = 0;
        m_batchIndex = 0;
        m_albedo = nullptr;
        m_amr = nullptr;
        m_glow = nullptr;
    }
    
    //======================================================================================================================
    Material::~Material() {
        
    }
    
    //======================================================================================================================
    //======================================================================================================================
    
    //======================================================================================================================
    MaterialResource::MaterialResource() {
        m_material = nullptr;
    }
    
    //======================================================================================================================
    MaterialResource::~MaterialResource() {
        
    }
    
    //======================================================================================================================
    void MaterialResource::Load( File * file, const char * path ) {
        RttiStream str;
        
        str.ReadFromFile( file );        
        str.Construct();
        
        m_material = str.m_objects[0]->SafeCast<Material>();
        XE_ASSERT( m_material != nullptr );
    }
    
    //======================================================================================================================
    Material * MaterialResource::GetMaterial() {
        return m_material;
    }
}
