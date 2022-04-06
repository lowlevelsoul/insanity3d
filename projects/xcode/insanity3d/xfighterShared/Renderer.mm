//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//=======================================================================================================================

#import <simd/simd.h>
#import <ModelIO/ModelIO.h>

#import "Renderer.h"
#import "i3d/i3d.h"
#import "i3d/render/Render.h"
#import "i3d/gfx/Gfx_metal.h"

// Include header shared between C code here, which executes Metal API commands, and .metal files
#import "ShaderTypes.h"

namespace i3d {
    void EnginePostRenderInitialise();
    void EngineThink( uint32_t viewWidth, uint32_t viewHeight, float displayScale );
    
    void RenderCreate( RenderParams * params );
    void RenderDestroy();
}

@implementation Renderer {
    
}

//=======================================================================================================================
-(nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)view;
{
    self = [super init];
    if(self)
    {
    
        i3d::RenderParams renderParams;
        renderParams.m_nativeView = (__bridge void *) view;
        
        view.depthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
        view.colorPixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
        view.sampleCount = 1;
        view.clearDepth = 1;
        
        
        i3d::RenderCreate( &renderParams );
        view.device = i3d::gfx::GetDevice();
        
        i3d::EnginePostRenderInitialise();
    }

    return self;
}

//=======================================================================================================================
- (void)_loadMetalWithView:(nonnull MTKView *)view;
{
}

//=======================================================================================================================
- (void)_loadAssets
{
}

//=======================================================================================================================
- (void)_updateGameState
{
}

//=======================================================================================================================
- (void)drawInMTKView:(nonnull MTKView *)view
{
    CGSize size = view.drawableSize;
    CGFloat scale = view.contentScaleFactor;
    
    i3d::EngineThink( size.width, size.height, scale );
}

//=======================================================================================================================
- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
 
}

@end
