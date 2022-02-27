//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __GFXPUFUNCTION_METAL_H__
#define __GFXPUFUNCTION_METAL_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/GfxGpuFunction.h"
#include <Metal/Metal.h>

namespace i3d { namespace gfx {
    
    class GpuFunctionMetal : public GpuFunction {
    public:
        GpuFunctionMetal();
        
        virtual ~GpuFunctionMetal();
        
        virtual void Create( const char * name ) override;
        
    public:
        id<MTLFunction>     m_function;
    };
}}

#endif
