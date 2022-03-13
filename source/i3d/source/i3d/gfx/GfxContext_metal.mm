//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/gfx/Gfx_metal.h"
#include "i3d/gfx/GfxContext_metal.h"

namespace i3d { namespace gfx {

    //======================================================================================================================
    ContextMetal::ContextMetal() {
        m_view = nil;
        m_baseView = nil;
    }

    //======================================================================================================================
    ContextMetal::~ContextMetal() {
        
    }

    //======================================================================================================================
    void ContextMetal::Create( Params & params ) {
        
#if TARGET_OS_OSX
        NSView * view = (__bridge NSView *) params.m_nativeView;
        bool baseIsMTKView = [view isKindOfClass:[MTKView class] ];
        if ( baseIsMTKView ) {
            // User has passed an MTK View class as the native window,
            // so we can just use that.
            m_view = (__bridge MTKView *) params.m_nativeView;
            m_baseView = nil;
        }
        else {
            // Native view is not a MTKView class, so we need to create our own
            m_baseView = (__bridge NSWindow * ) params.m_nativeView;

            m_view = [[MTKView alloc] initWithFrame: CGRectMake( 0, 0, m_baseView.frame.size.width, m_baseView.frame.size.height ) device: i3d::gfx::GetDevice() ];
            //m_view.autoresizingMask = (NSViewAutoresizingFlexibleWidth | NSViewAutoresizingFlexibleHeight);

            m_viewDelegate = [[ContextViewDelegate alloc] initWithMetalKitView: m_view];
            [m_view setDelegate: m_viewDelegate];
            [m_baseView.contentView addSubview: m_view ];
        }
#elif TARGET_OS_IOS || TARGET_OS_TV
        m_view = (__bridge MTKView *) params.m_nativeView;
        m_baseView = nil;
#endif
        
        m_cmdQueue = [GetDevice() newCommandQueue];
        
        m_inFlightSemaphore = dispatch_semaphore_create( params.m_bufferCount );
        m_bufferCount = params.m_bufferCount;
    }
    
    
    //======================================================================================================================
    void ContextMetal::GetDrawableSize( uint32_t & width, uint32_t & height ) {
        CGSize size = m_view.drawableSize;
        width = (uint32_t) size.width;
        height = (uint32_t) size.height;
    }
}}


@implementation ContextViewDelegate
    {
    }

    //======================================================================================================================
    -(nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)view;
    {
        self = [super init];
        if(self) {
            view.depthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
            view.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
            view.sampleCount = 1;
            view.clearDepth = 1;
            view.device = i3d::gfx::GetDevice();
            view.clearColor = MTLClearColorMake(0, 0.9f, 0.5f, 1);
            [ view setPaused: YES ];
            [ view setNeedsDisplay: NO ];
        }

        return self;
    }

    //======================================================================================================================
    - (void)_loadMetalWithView:(nonnull MTKView *)view
    {

    }

    //======================================================================================================================
    - (void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size {
        
    }

    //======================================================================================================================
    - (void) drawInMTKView:(nonnull MTKView *) view {
        
    }

@end
