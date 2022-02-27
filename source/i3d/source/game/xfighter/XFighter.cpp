//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "XFighter.h"
#include "i3d/res/Resource.h"
#include "i3d/render/Material.h"

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
    m_materialRes = res->Load<i3d::MaterialResource>( "~/materials/rx2/red.mat" );
    m_model = res->Load< i3d::Model >( "~/models/rx2/ship.mdl" );
    
    res->StartLoading();
    while ( res->HasPending() == true ) {
        
    }
}

//======================================================================================================================
void XFighter::Finalise() {
    
}

//======================================================================================================================
void XFighter::Think( float deltaTime ) {
    m_rot += 180.0f * deltaTime;
    while ( m_rot >= 360 ) m_rot -= 360.0f;
}

//======================================================================================================================
void XFighter::Draw( float deltaTime ) {
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
        render->BeginScene3d( projMat, viewMat, viewport );
        render->SubmitModel( m_model, modelMat, m_materialRes->GetMaterial() );
        render->EndScene3d();
        
    }
    render->EndScene();
}
