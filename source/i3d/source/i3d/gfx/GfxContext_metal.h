//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __GFXCONTEXT_METAL_H__
#define __GFXCONTEXT_METAL_H__

#include "i3d/gfx/GfxContext.h"
#include <MetalKit/MetalKit.h>

namespace i3d { namespace gfx {
    class ContextMetal;
}}

@interface ContextViewDelegate : NSObject <MTKViewDelegate>
    {
        i3d::gfx::ContextMetal *      m_gfxContext;
    }
    -(nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)view;
@end

namespace i3d { namespace gfx {
    
    class ContextMetal : public Context {
    public:
        ContextMetal();
        
        virtual ~ContextMetal();
        
        virtual void Create( Params & params ) override;
        
        virtual void GetDrawableSize( uint32_t & width, uint32_t & height ) override;
        
    public:
        dispatch_semaphore_t _Nullable  m_inFlightSemaphore;
#if TARGET_OS_OSX
        NSWindow * _Nullable            m_baseView;
        MTKView *  _Nullable            m_view;
#elif TARGET_OS_IOS || TARGET_OS_TV
        UIView *                        m_baseView;
        MTKView *                       m_view;
#else
#   error Unknown Metal platform
#endif
        ContextViewDelegate * _Nullable m_viewDelegate;
        id<MTLCommandQueue> _Nullable   m_cmdQueue;
        uint32_t                        m_bufferCount;
        
    };
}}




#endif
