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
    class PrototypeResource;
}

class GameGlobals;
class EntityDefResource;
class Ship;

class XFighter : public i3d::Game {
public:
    XFighter();
    
    virtual ~XFighter();
    
    virtual void Initialise() override;
    
    virtual void Finalise() override;
    
    virtual void Think( float deltaTime, uint32_t viewWidth, uint32_t viewHeight ) override;
    
    virtual void Draw( float deltaTime, uint32_t viewWidth, uint32_t viewHeight ) override;
    
protected:
    
    void CreateSystemEnt();
    
public:    
    i3d::RttiResource *         m_globalsRes;
    GameGlobals *               m_globals;
    EntityDefResource *         m_playerDef;
    
    Ship *                      m_player;
};

extern XFighter * game;

#endif
