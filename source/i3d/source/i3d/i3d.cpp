//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/i3d.h"
#include "i3d/render/Render.h"
#include "i3d/core/Log.h"
#include "i3d/render/Material.h"

namespace i3d {
    
    void FsCreate();
    void FsDestroy();
    
    void SysCreate();
    void SysDestroy();
    
    void FsCreate();
    void FsDestroy();
    
    void CVarCreate();
    void CVarDestroy();
    
    void LogCreate();
    void LogDestroy();
    
    void ResCreate();
    void ResDestroy();
    
    void RttiCreate();
    void RttiDestroy();
    
    class EngineState {
    public:
        EngineState() {
            m_lastTick = 0;
            m_firstFrame = true;
            m_game = nullptr;
        }
        
        ~EngineState() {
            
        }
        
    public:
        bool                m_firstFrame;
        uint64_t            m_lastTick;
        Game *              m_game;
        LogReporterDebug    m_debugReporter;
        
    };
    
    static uint8_t EngineStateMem[ sizeof( EngineState ) ];
    EngineState * engine = nullptr;
    
    static void EngineThink( float deltaTime );
    
    static void EngineDraw( float deltaTime );
    

    //======================================================================================================================
    void EngineInitialise( int argc, const char ** argv ) {
        
        engine = new( EngineStateMem ) EngineState;
                
        LogCreate();
#if defined ( _DEBUG ) || defined( DEBUG )
        logsys->PublishReporter( &engine->m_debugReporter );
#endif
        SysCreate();
        FsCreate();
        CVarCreate();
        RttiCreate();
        ResCreate();
        
        res->PublishFactory< i3d::MaterialResource >();
    }
    
    //======================================================================================================================
    void EnginePostRenderInitialise() {
        engine->m_game = GameCreate();
        engine->m_game->Initialise();
    }
    
    //======================================================================================================================
    void EngineFinalise() {
        
        engine->m_game->Finalise();
        GameDestroy();
        
        ResDestroy();
        RttiDestroy();
        CVarDestroy();
        SysDestroy();
        FsDestroy();
        LogDestroy();
        
        engine->~EngineState();
    }
    
    //======================================================================================================================
    void EngineThink() {
        float deltaTime = 1.0f / 60.0f;
        if ( engine->m_firstFrame == true ) {
            engine->m_firstFrame = false;
            engine->m_lastTick = sys->GetTicks();
        }
        else {
            uint64_t currTick = sys->GetTicks();
            uint64_t deltaTick = ( currTick - engine->m_lastTick );
            deltaTime = 0.001f * (float)( deltaTick );
            engine->m_lastTick = currTick;
        }
        
        EngineThink( deltaTime );
        
        EngineDraw( deltaTime );
    }
    
    //======================================================================================================================
    void EngineThink( float deltaTime ) {
        engine->m_game->Think( deltaTime );
    }
    
    //======================================================================================================================
    void EngineDraw( float deltaTime ) {
        
        engine->m_game->Draw( deltaTime );
        render->Submit();
    }
}
