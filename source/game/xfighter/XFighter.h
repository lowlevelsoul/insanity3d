//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __XFIGHTER_H__
#define __XFIGHTER_H__

#include "i3d/i3d.h"
#include "i3d/input/Input.h"
#include "misc/PlayerTouchController.h"

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
    
    virtual void Think( float deltaTime, uint32_t viewWidth, uint32_t viewHeight, float displayScale ) override;
    
    virtual void Draw( float deltaTime, uint32_t viewWidth, uint32_t viewHeight, float displayScale ) override;
    
protected:
    
    void CreateSystemEnt();
    
    void ProcessInputs( float displayScale );
    
    void ProcessInputTouch( i3d::Event& ev, float displayScale );
    
    void ProceesInputKey( i3d::Event& ev );
    
    void CalcCollisionAreaBounds( i3d::Vector2& bmin, i3d::Vector2& bmax );
    
public:    
    i3d::RttiResource *         m_globalsRes;
    GameGlobals *               m_globals;
    EntityDefResource *         m_playerDef;
    EntityDefResource *         m_enemyDef;
    PlayerTouchController       m_touchController;
    int32_t                     m_viewWidth;
    int32_t                     m_viewHeight;
    
    i3d::Event *                 m_inputEvents;              ///< Input events
    uint32_t                     m_inputEventCount;          ///< Number of input events
    
    Ship *                      m_player;
    Ship *                      m_enemy;
};

extern XFighter * game;

#endif
