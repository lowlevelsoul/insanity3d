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


static uint8_t XFighterMem[ sizeof( XFighter ) ];
XFighter * game = nullptr;

void EntityManagerCreate();
void EntityManagerDestroy();

i3d::CVar   in_Player1MoveX("in_player1MoveX", 0.0f, "Player1 move input in x-direction");
i3d::CVar   in_Player1MoveZ("in_player1MoveZ", 0.0f, "Player1 move input in z-direction");
i3d::CVar   in_Player1Fire("in_player1Fire", 0.0f, "Player1 move input in z-direction");

i3d::CVar   in_Player2MoveX("in_player2MoveX", 0.0f, "Player2 move input in x-direction");
i3d::CVar   in_Player2MoveZ("in_player2MoveZ", 0.0f, "Player2 move input in z-direction");
i3d::CVar   in_Player2Fire("in_player2Fire", 0.0f, "Player2 move input in z-direction");

i3d::CVar   in_enabled("in_enabled", 1, "Enables / disables player input");

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
    
    m_globalsRes = res->Load<i3d::RttiResource>("~/rtti/GameGlobals.brtti");
    m_playerDef = res->Load<EntityDefResource>("~/entities/Player.ent");
    
    res->StartLoading();
    while ( res->HasPending() == true ) {
        
    }
    
    m_globals = m_globalsRes->GetObject()->SafeCast<GameGlobals>();
    
    m_player = m_playerDef->Construct()->SafeCast<Ship>();
    res->StartLoading();
    while ( res->HasPending() == true ) {
        
    }
    
    entityMgr->AddEntity( m_player );
    
}

//======================================================================================================================
void XFighter::CreateSystemEnt() {
}

//======================================================================================================================
void XFighter::Finalise() {
    
}

//======================================================================================================================
void XFighter::Think( float timeStep, uint32_t viewWidth, uint32_t viewHeight ) {
    m_viewWidth = viewWidth;
    m_viewHeight = viewHeight;
    
    m_globals->m_camera->Update( viewWidth, viewHeight );
    m_globals->m_playfield->Update( m_globals->m_camera );
    
    ProcessInputs();
    
    entityMgr->Think( timeStep );
}

//======================================================================================================================
void XFighter::ProcessInputTouch( i3d::Event& ev ) {
#ifdef XF_USE_TOUCH_INPUT
    i3d::TouchEvent tv = ev.m_event.m_touch;

    if (tv.m_type == i3d::TouchEvent::TYPE_BEGIN) {
        in_Player1Fire.Set(1.0f);
        in_Player1MoveX.Set(0.0f);
        in_Player1MoveZ.Set(0.0f);
    
        m_touchController.Begin(tv.m_x, tv.m_y);
        
    }
    else if (tv.m_type == i3d::TouchEvent::TYPE_END) {
        in_Player1Fire.Set(0.0f);
        in_Player1MoveX.Set(0.0f);
        in_Player1MoveZ.Set(0.0f);
        
        m_touchController.End(0, 0);
    }
    else if (tv.m_type == i3d::TouchEvent::TYPE_MOVED) {
        m_touchController.Moved(tv.m_x, tv.m_y);
        
        in_Player1MoveX.Set( m_touchController.m_moveDelta.X()) ;
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
void XFighter::ProcessInputs() {
    
    input->Think( m_inputEvents, m_inputEventCount);
    
    for(uint32_t i=0; i < m_inputEventCount; ++i) {
        
        i3d::Event& ev = m_inputEvents[i];
        switch(ev.m_type) {
                
            case i3d::EVENT_TYPE_KEY:
                ProceesInputKey(ev);
                break;
                
            case i3d::EVENT_TYPE_TOUCH:
                ProcessInputTouch(ev);
                break;
                
            default:
                break;
        }
    }
}


//======================================================================================================================
void XFighter::Draw( float timeStep, uint32_t viewWidth, uint32_t viewHeight ) {
    i3d::Matrix4 projMat, viewMat;
    
    projMat.SetProjectionPerspLH( i3d::scalar::DegToRad( 60 ), 4.0f / 3.0f, 5, 500 );
    viewMat = i3d::Matrix4::IDENTITY;
    viewMat.SetTranslation( i3d::Vector3( 0, 0, -20 ) );
    
    int32_t viewport[] = { 0, 0, (int32_t)viewWidth, (int32_t)viewHeight };
    
    render->BeginScene();
    {
        if ( m_globals != nullptr ) {
            render->BeginScene3d( m_globals->m_camera->m_projection,
                                  m_globals->m_camera->m_transform,
                                  viewport );
            
            entityMgr->Draw( timeStep, viewWidth, viewHeight);
            
            render->EndScene3d();
        }        
    }
    render->EndScene();
}
