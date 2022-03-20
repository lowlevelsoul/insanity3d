//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __SHIPMODEL_H__
#define __SHIPMODEL_H__

namespace i3d {
    class Model;
    class Material;
    class MaterialResource;
}

struct ShipModel {
    i3d::Model *            m_model;
    i3d::Material *         m_material;
};

class ShipModelPrototype : public i3d::ComponentPrototype {
public:
    RTTI_CLASS_DECLARE_NAMED( ShipModelPrototype, i3d::ComponentPrototype )
    
    ShipModelPrototype();
    
    virtual ~ShipModelPrototype();
    
    virtual void CreateComponent( i3d::Entity ent ) override;
    
    virtual void Construct( i3d::Entity & ent )  override;
    
    virtual void DestroyComponent( i3d::Entity ent )  override;
    
public:
    i3d::Model *                m_model;
    i3d::MaterialResource *     m_materialRes;
};

#endif
