//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __XFIGHTER_H__
#define __XFIGHTER_H__

#include "i3d/i3d.h"

namespace i3d {
    class Material;
    class MaterialResource;
    class Model;
}

class XFighter : public i3d::Game {
public:
    XFighter();
    
    virtual ~XFighter();
    
    virtual void Initialise() override;
    
    virtual void Finalise() override;
    
    virtual void Think( float deltaTime ) override;
    
    virtual void Draw( float deltaTime ) override;
    
public:
    i3d::MaterialResource *     m_materialRes;
    i3d::Model *                m_model;
    float                       m_rot;
};

#endif
