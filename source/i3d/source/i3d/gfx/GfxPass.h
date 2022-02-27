//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_GFXPASS_H__
#define __I3D_GFXPASS_H__

#include "i3d/core/Types.h"

namespace i3d { namespace gfx {
    
    class Pass {
    public:
        virtual ~Pass() {}
        
        virtual void Create( const char * label ) = 0;
    };
}}


#endif
