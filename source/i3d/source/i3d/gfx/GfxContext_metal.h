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
        dispatch_semaphore_t    m_inFlightSemaphore;
        NSWindow *              m_baseView;
        MTKView *               m_view;
        ContextViewDelegate *   m_viewDelegate;
        id<MTLCommandQueue>     m_cmdQueue;
        uint32_t                m_bufferCount;
        
    };
}}




#endif
