//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __SHIPDEF_H__
#define __SHIPDEF_H__

#include "entity/EntityDef.h"
#include "entity/Entity.h"


class ShipComponent;

class ShipComponentDef : public i3d::RttiObject {
public:
    RTTI_CLASS_DECLARE( ShipComponentDef, i3d::RttiObject )
    
    typedef i3d::stl::Vector<ref_ptr_t> vector_t;
    
    ShipComponentDef();
    
    virtual ~ShipComponentDef();
    
    virtual const char * GetTypeName();
};

class ShipDef : public EntityDef {
public:
    
    RTTI_CLASS_DECLARE( ShipDef, EntityDef )
    
    ShipDef();
    
    virtual ~ShipDef();
    
    virtual const char * GetTypeName() override;
    
public:
    ShipComponentDef::vector_t::type    m_components;
    
    i3d::Model *                        m_model;
    i3d::MaterialResource *             m_materialMat;
    i3d::Quaternion                     m_modelOffsRot;
    i3d::Vector3                        m_modelOffsPos;
};

#endif
