//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "XFighter.h"
#include "i3d/res/Resource.h"
#include "i3d/render/Material.h"
#include "i3d/ecs/PrototypeResource.h"

#include "entity/EntityDef.h"
#include "entity/EntityManager.h"
#include "misc/GameGlobals.h"
#include "ship/Ship.h"


static uint8_t XFighterMem[ sizeof( XFighter ) ];
XFighter * game = nullptr;

void EntityManagerCreate();
void EntityManagerDestroy();

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
    
    res->PublishFactory<EntityDef>();
    
    m_globalsRes = res->Load<i3d::RttiResource>("~/rtti/GameGlobals.brtti");
    m_playerDef = res->Load<EntityDef>("~/entities/Player.ent");
    
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
    m_globals->m_camera->Update( viewWidth, viewHeight );
    m_globals->m_playfield->Update( m_globals->m_camera );
    
    entityMgr->Think( timeStep );
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
