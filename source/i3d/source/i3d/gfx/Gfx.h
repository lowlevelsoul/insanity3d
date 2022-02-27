//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_GFX_H__
#define __I3D_GFX_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/GfxTypes.h"

#include "i3d/gfx/GfxVertexDesc.h"
#include "i3d/gfx/GfxGpuPipeline.h"
#include "i3d/gfx/GfxGpuTextureArray.h"
#include "i3d/gfx/GfxTexture.h"

namespace i3d { namespace gfx {
    
    class Buffer;
    class Pass;
    class Context;
    class GpuFunction;
    class GpuPipeline;
    class Pass;
    class Fence;
    class DepthState;
    class RasterizeState;
    class GpuTextureArray;
    
    Context * CreateContext();
    
    Buffer * CreateBuffer();
    
    Buffer * CreateBuffer( size_t size, uint64_t flags );
    
    VertexDesc * CreateVertexDesc();
    
    VertexDesc * CreateVertexDesc( const VertexDesc::Element * elements );
    
    GpuFunction * CreateGpuFunction();
    
    GpuFunction * CreateGpuFunction( const char * name );
    
    GpuPipeline * CreateGpuPipeline();
    
    GpuPipeline * CreateGpuPipeline( const GpuPipeline::Params * params );
    
    Pass * CreatePass( const char * name = nullptr );
    
    Fence * CreateFence( const char * label = nullptr );
    
    DepthState * CreateDepthState();
    
    DepthState * CreateDepthState( gfx::FUNC depthFunc, bool depthWrite, bool depthDest, float depthClear, uint8_t stencilClear );
    
    RasterizeState * CreateRasterizeState();
    
    RasterizeState * CreateRasterizeState( gfx::WINDING frontFace, gfx::CULL cullFace );
    
    Texture * CreateTexture();
    
    Texture * CreateTexture( Texture::FORMAT format, uint32_t width, uint32_t height, uint32_t mipCount );
    
    GpuTextureArray * CreateGpuTextureArray();
    
    GpuTextureArray * CreateGpuTextureArray( GpuTextureArray::Params & params );
        
    void Initialise();
    
    void Finalise();
    
    void GetCaps( Caps & caps );
    
    bool BeginScene( Context * ctxt );
    
    void EndScene();
    
    void Present();
    
    void BeginPass( const char * name );
    
    void BeginPass( Pass * pass );
    
    void EndPass( );
    
    void SetDepthState( DepthState * state );
    
    void SetRasterizeState( RasterizeState * state );
    
    void SetVertexBuffer( Buffer * buffer, uint32_t index, uint64_t offset );
    
    void SetVertexBufferOffset( uint32_t index, uint64_t offset );
    
    void SetPixelBuffer( Buffer * buffer, uint32_t index, uint64_t offset );
    
    void SetPixelBufferOffset( uint32_t index, uint64_t offset );
    
    void SetRenderPipeline( GpuPipeline * pl );
    
    void SetVertexTexture( Texture * tex, uint32_t index );
    
    void SetPixelTexture( Texture * tex, uint32_t index );
    
    void DrawIndexedPrim( PRIM primType, uintptr_t start, size_t count, Buffer * indices );
    
    void DrawPrim( PRIM primType, uintptr_t start, size_t count );
    
    void UpdateFence( Fence * fence, uint64_t stages );
    
    void WaitForFence( Fence * fence, uint64_t stages );
    
    // Resource heap api
    void InitResourceHeap( size_t size = 1024 * 1024 * 64 );
    
    void MoveTextureToResourceHeap( Texture * tex, bool moveNow = true );
}}


#endif 
