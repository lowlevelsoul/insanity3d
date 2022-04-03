//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __ENTITYRESOURCE_H__
#define __ENTITYRESOURCE_H__

#include "i3d/rtti/RttiStream.h"

class Entity;

class EntityDef : public i3d::Resource {
public:
    DECLARE_RESOURCE( EntityDef )
    
    EntityDef();
    
    virtual ~EntityDef();
    
    virtual void Load( i3d::File * file, const char * path ) override;
    
    Entity * Construct();
    
protected:
    i3d::RttiStream      m_stream;
};

#endif

