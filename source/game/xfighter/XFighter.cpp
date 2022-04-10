//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "XFighter.h"
#include "i3d/res/Resource.h"
#include "i3d/render/Material.h"
#include "i3d/ecs/PrototypeResource.h"

#include "entity/EntityDefResource.h"
#include "entity/EntityDefResource.h"
#include "entity/EntityManager.h"
#include "misc/GameGlobals.h"
#include "ship/Ship.h"
#include "col/CollideSys.h"


static uint8_t XFighterMem[ sizeof( XFighter ) ];
XFighter * game = nullptr;

void EntityManagerCreate();
void EntityManagerDestroy();

void CollideCreate();
void CollideDestroy();

i3d::CVar   in_Player1MoveX("in_player1MoveX", 0.0f, "Player1 move input in x-direction");
i3d::CVar   in_Player1MoveZ("in_player1MoveZ", 0.0f, "Player1 move input in z-direction");
i3d::CVar   in_Player1Fire("in_player1Fire", 0.0f, "Player1 move input in z-direction");

i3d::CVar   in_Player2MoveX("in_player2MoveX", 0.0f, "Player2 move input in x-direction");
i3d::CVar   in_Player2MoveZ("in_player2MoveZ", 0.0f, "Player2 move input in z-direction");
i3d::CVar   in_Player2Fire("in_player2Fire", 0.0f, "Player2 move input in z-direction");

i3d::CVar   in_enabled("in_enabled", 1, "Enables / disables player input");

extern i3d::CVar   col_dbgDraw;
extern i3d::CVar   col_dbgDrawShapes;
extern i3d::CVar   col_dbgDrawCells;
extern i3d::CVar   col_dbgDrawPairs;

extern i3d::CVar   col_dbgDrawShapesGroups;
extern i3d::CVar   col_dbgDrawCellGroups;
extern i3d::CVar   col_dbgDrawPairGroups;

//======================================================================================================================
i3d::Game * i3d::GameCreate() {
    if ( game == nullptr ) {
        game = new( XFighterMem ) XFighter;
    }
    
    return game;
}

//======================================================================================================================
void i3d::GameDestroy() {
    if ( game != nullptr ) {
        ((XFighter*) game)->~XFighter();
        game = nullptr;
    }
}

//======================================================================================================================
XFighter::XFighter() {
}

//======================================================================================================================
XFighter::~XFighter() {
    
}

//======================================================================================================================
void XFighter::Initialise() {
    EntityManagerCreate();
    
    res->PublishFactory<EntityDefResource>();
    
    CollideCreate();
    i3d::Vector2 bmin(-256, -256), bmax(256, 256);
    colSys->Initialise( bmin, bmax );

    col_dbgDraw.Set( true );
    col_dbgDrawShapes.Set( true );
    col_dbgDrawCells.Set( true );
    col_dbgDrawPairs.Set( true );
    
    col_dbgDrawShapesGroups.Set( "player;enemy;player_bullet;enemy_bullet" );
    col_dbgDrawCellGroups.Set( "player;enemy" );
    col_dbgDrawPairGroups.Set( "player;enemy" );
    
    m_globalsRes = res->Load<i3d::RttiResource>("~/rtti/GameGlobals.brtti");
    m_playerDef = res->Load<EntityDefResource>("~/entities/Player.ent");
    m_enemyDef = res->Load<EntityDefResource>("~/entities/Enemy_gunship.ent");

    res->StartLoading();
    while ( res->HasPending() == true ) {
        
    }
    
    m_globals = m_globalsRes->GetObject()->SafeCast<GameGlobals>();
    
    m_player = m_playerDef->Construct()->SafeCast<Ship>();
    m_enemy = m_enemyDef->Construct()->SafeCast<Ship>();
    
    entityMgr->AddEntity( m_player );
    entityMgr->AddEntity( m_enemy );
}

//======================================================================================================================
void XFighter::CreateSystemEnt() {
}

//======================================================================================================================
void XFighter::Finalise() {
    
}

//======================================================================================================================
void XFighter::CalcCollisionAreaBounds( i3d::Vector2& bmin, i3d::Vector2& bmax) {
    
    m_globals->m_camera->Update( 160, 90 );
    m_globals->m_playfield->Update( m_globals->m_camera );
    
    // Determine the bounds of the frustum
    float minZ, maxZ;
    m_globals->m_playfield->CalcZLimits(minZ, maxZ, 0, 0);
    
    float left, right;
    m_globals->m_playfield->CalcXLimits(left, right, 0, maxZ);
    
    // Workout the largest bounds component
    int32_t halfSize = (maxZ > right) ? maxZ : right;
    halfSize = (halfSize > -minZ) ? halfSize : -minZ;
    assert(halfSize > 0);
    
    halfSize = i3d::scalar::GetNextPowerOfTwo(halfSize);
    bmin.Set((float)-halfSize, (float)-halfSize);
    bmax.Set((float)halfSize, (float)halfSize);
}

//======================================================================================================================
void XFighter::Think( float timeStep, uint32_t viewWidth, uint32_t viewHeight, float displayScale ) {
    m_viewWidth = viewWidth;
    m_viewHeight = viewHeight;
    
    m_globals->m_camera->Update( viewWidth, viewHeight );
    m_globals->m_playfield->Update( m_globals->m_camera );
    
    ProcessInputs( displayScale );
    
    colSys->Think( timeStep );
    
    entityMgr->Think( timeStep );
}

//======================================================================================================================
void XFighter::ProcessInputTouch( i3d::Event& ev, float displayScale ) {
#ifdef XF_USE_TOUCH_INPUT
    i3d::TouchEvent tv = ev.m_event.m_touch;

    if (tv.m_type == i3d::TouchEvent::TYPE_BEGIN) {
        in_Player1Fire.Set(1.0f);
        in_Player1MoveX.Set(0.0f);
        in_Player1MoveZ.Set(0.0f);
    
        m_touchController.Begin(tv.m_x * displayScale, tv.m_y * displayScale);
        
    }
    else if (tv.m_type == i3d::TouchEvent::TYPE_END) {
        in_Player1Fire.Set(0.0f);
        in_Player1MoveX.Set(0.0f);
        in_Player1MoveZ.Set(0.0f);
        
        m_touchController.End(0, 0);
    }
    else if (tv.m_type == i3d::TouchEvent::TYPE_MOVED) {
        m_touchController.Moved(tv.m_x * displayScale, tv.m_y * displayScale);
        
        in_Player1MoveX.Set( m_touchController.m_moveDelta.X() ) ;
        in_Player1MoveZ.Set( m_touchController.m_moveDelta.Z() );
    }
#endif
}

//======================================================================================================================
void XFighter::ProceesInputKey( i3d::Event& ev ) {
#ifndef XF_USE_TOUCH_INPUT
    switch(ev.m_event.m_key.m_key) {
        case KEY_W:
            PLAYER1_FORWARD = (ev.m_event.m_key.m_value != 0) ? 1 : 0;
            break;
        case KEY_S:
            PLAYER1_BACK = (ev.m_event.m_key.m_value != 0) ? 1 : 0;
            break;
        case KEY_A:
            PLAYER1_LEFT = (ev.m_event.m_key.m_value != 0) ? 1 : 0;
            break;
        case KEY_D:
            PLAYER1_RIGHT = (ev.m_event.m_key.m_value != 0) ? 1 : 0;
            break;
        //case KEY_L:
           // PLAYER1_FIRE = (ev.m_event.m_key.m_value != 0) ? 1 : 0;
          //  break;
        default:
            break;
    }
#endif
}

//======================================================================================================================
void XFighter::ProcessInputs( float displayScale ) {
    
    input->Think( m_inputEvents, m_inputEventCount);
    
    for(uint32_t i=0; i < m_inputEventCount; ++i) {
        
        i3d::Event& ev = m_inputEvents[i];
        switch(ev.m_type) {
                
            case i3d::EVENT_TYPE_KEY:
                ProceesInputKey(ev);
                break;
                
            case i3d::EVENT_TYPE_TOUCH:
                ProcessInputTouch(ev, displayScale );
                break;
                
            default:
                break;
        }
    }
}

//======================================================================================================================
void XFighter::Draw( float timeStep, uint32_t viewWidth, uint32_t viewHeight, float displayScale ) {
    
    int32_t viewport[] = { 0, 0, (int32_t)viewWidth, (int32_t)viewHeight };
    
    render->BeginScene();
    {
        if ( m_globals != nullptr ) {
            
            render->BeginScene3d( m_globals->m_camera->m_projection,
                                  m_globals->m_camera->m_transform,
                                  viewport );
            
            entityMgr->Draw( timeStep, viewWidth, viewHeight);
            
            if ( col_dbgDraw.GetBool() == true ) {
                colSys->DebugDraw();
            }
            
            render->EndScene3d();
        }        
    }
    render->EndScene();
}
