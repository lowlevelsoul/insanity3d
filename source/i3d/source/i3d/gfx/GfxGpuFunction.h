//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __GFXPUFUNCTION_H__
#define __GFXPUFUNCTION_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/GfxTypes.h"

namespace i3d { namespace gfx {
    
    class GpuFunction {
    public:
        virtual ~GpuFunction() {}
        
        virtual void Create( const char * name ) = 0;
    };
}}

#endif 
