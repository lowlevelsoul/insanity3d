//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "XFighter.h"
#include "i3d/res/Resource.h"
#include "i3d/render/Material.h"
#include "i3d/ecs/PrototypeResource.h"

#include "components/Transform.h"
#include "components/ShipModel.h"


static uint8_t XFighterMem[ sizeof( XFighter ) ];
XFighter * game = nullptr;

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
    m_rot = 0;
}

//======================================================================================================================
XFighter::~XFighter() {
    
}

//======================================================================================================================
void XFighter::Initialise() {
    InitialiseEcs();
    
    CreateSystemEnt();
    
    m_player = res->Load<i3d::PrototypeResource>( "~/prototypes/player.proto" );
    
    res->StartLoading();
    while ( res->HasPending() == true ) {
        
    }
    
    i3d::Entity playerEnt = m_player->CreateEntity();
}


//======================================================================================================================
void XFighter::CreateSystemEnt() {
    i3d::PrototypeResource * systemProto = nullptr;
    
    bool loaded = res->Find("~/prototypes/system.proto");
    if ( loaded == false ) {
        systemProto = res->Load<i3d::PrototypeResource>("~/prototypes/system.proto");

        res->StartLoading();
        
        while ( res->HasPending() == true ) {
        
        }
    }
    
    m_systemEnt = systemProto->CreateEntity();
}

//======================================================================================================================
void XFighter::Finalise() {
    
}

//======================================================================================================================
void XFighter::Think( float timeStep, uint32_t viewWidth, uint32_t viewHeight ) {
    //m_rot += 180.0f * deltaTime;
    //while ( m_rot >= 360 ) m_rot -= 360.0f;
    
    m_playerManager.Think( timeStep );
    m_transformManager.Think( timeStep );
    
}

//======================================================================================================================
void XFighter::Draw( float timeStep, uint32_t viewWidth, uint32_t viewHeight ) {
    i3d::Matrix4 projMat, viewMat;
    
    projMat.SetProjectionPerspLH( i3d::scalar::DegToRad( 60 ), 4.0f / 3.0f, 5, 500 );
    viewMat = i3d::Matrix4::IDENTITY;
    viewMat.SetTranslation( i3d::Vector3( 0, 0, -20 ) );
    
    int32_t viewport[] = { 0, 0, 0, 0 };
    
    i3d::Matrix4 modelMat;
    modelMat = i3d::Matrix4::IDENTITY;
    modelMat.SetRotationAA( i3d::Vector3::UNIT_Y, i3d::scalar::DegToRad( m_rot ) );
    
    render->BeginScene();
    {
        //render->BeginScene3d( projMat, viewMat, viewport );
        //render->SubmitModel( m_model, modelMat, m_materialRes->GetMaterial() );
        
        m_sceneManager.BeginScene( timeStep, viewWidth, viewHeight );
        {
            m_shipManager.Draw( timeStep );
        }
        m_sceneManager.EndScene();
        
        //render->EndScene3d();
        
    }
    render->EndScene();
}
