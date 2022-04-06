//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_H__
#define __I3D_H__

#include "i3d/core/Types.h"
#include "i3d/render/Render.h"

namespace i3d {
    
    
    class Game {
    public:
        virtual ~Game() {
            // Empty
        }
        
        virtual void Initialise() = 0;
        
        virtual void Finalise() = 0;
        
        virtual void Think( float deltaTime, uint32_t viewWidth, uint32_t viewHeight, float displayScale ) = 0;
        
        virtual void Draw( float deltaTime, uint32_t viewWidth, uint32_t viewHeight, float displayScale ) = 0;
    };
    
    void EngineInitialise( int argc, const char ** argv );
    
    void EngineFinalise();
    
    Game * GameCreate();
    
    void GameDestroy();
}


#endif
