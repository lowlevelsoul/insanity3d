//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/gfx/GfxGpuFunction_metal.h"
#include "i3d/gfx/Gfx_metal.h"

namespace i3d { namespace gfx {

    //======================================================================================================================
    GpuFunctionMetal::GpuFunctionMetal() {
        
    }
    
    //======================================================================================================================
    GpuFunctionMetal::~GpuFunctionMetal() {
        
    }
    
    //======================================================================================================================
    void GpuFunctionMetal::Create( const char * name ) {
        
        id<MTLLibrary> defaultLibrary = [GetDevice() newDefaultLibrary];

        m_function = [defaultLibrary newFunctionWithName:[[NSString alloc] initWithUTF8String:name]];
        if ( m_function == nil ) {
            NSLog(@"Could not find gpu function %s", name);
            assert( m_function != nullptr);
        }
    }
}}
