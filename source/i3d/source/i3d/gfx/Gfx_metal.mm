//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/gfx/Gfx_metal.h"
#include "i3d/gfx/GfxPass_metal.h"
#include "i3d/gfx/GfxBuffer_metal.h"
#include "i3d/gfx/GfxContext_metal.h"
#include "i3d/gfx/GfxGpuFunction_metal.h"
#include "i3d/gfx/GfxGpuPipeline_metal.h"
#include "i3d/gfx/GfxPass_metal.h"
#include "i3d/gfx/GfxFence_metal.h"
#include "i3d/gfx/GfxDepthState_metal.h"
#include "i3d/gfx/GfxRasterizeState_metal.h"
#include "i3d/gfx/GfxTexture_metal.h"
#include "i3d/gfx/GfxGpuTextureArray_metal.h"

#include "i3d/gfx/GfxVertexDesc_local.h"

#include <MetalKit/MetalKit.h>

namespace i3d { namespace gfx {
    
    struct GfxState {
        
        static constexpr size_t MAX_PENDING = 1024;
        //typedef ArrayStatic<TextureMetal*,  MAX_PENDING> pending_array_t;

        
        ContextMetal *      m_currCtxt;
        PassMetal *         m_currPass;
        
        id<MTLDevice>                   m_device;
        id<MTLRenderCommandEncoder>     m_encoder;
        id<MTLCommandQueue>             m_cmdQueue;
        id<MTLCommandBuffer>            m_cmdBuffer;
        MTLRenderPassDescriptor *       m_renderPassDescriptor;
        
        id<MTLHeap>                     m_resourceHeap;
        TextureMetal *                  m_pendingTextures[ MAX_PENDING ];
        size_t                          m_pendingTextureCount;
        
        Caps                            m_caps;
        
        GfxState() {
            m_currPass = nullptr;
            m_renderPassDescriptor = nil;
            m_device = MTLCreateSystemDefaultDevice();
            m_pendingTextureCount = 0;
        }
    };
    
    static uint8_t s_stateMem[sizeof(GfxState)];
    static GfxState * gfxstate = nullptr;
    
    static const MTLPrimitiveType PRIM_TABLE[] = {
        MTLPrimitiveTypePoint,          // PRIM_POINTS,
        MTLPrimitiveTypeTriangle,       // PRIM_TRIANGLES,
        MTLPrimitiveTypeTriangleStrip,  // PRIM_TRISTRIPS,
        MTLPrimitiveTypeLine,           // PRIM_LINES,
        MTLPrimitiveTypeLineStrip,      // PRIM_LINESTRIP
    };
    
    static const MTLCompareFunction FUNC_TABLE[] = {
        MTLCompareFunctionAlways,       // FUNC_ALWAYS = 0,
        MTLCompareFunctionNever,        // FUNC_NEVER,
        MTLCompareFunctionEqual,        // FUNC_EQUAL,
        MTLCompareFunctionNotEqual,     // FUNC_NEQUAL,
        MTLCompareFunctionLess,         // FUNC_LESS,
        MTLCompareFunctionGreater,      // FUNC_GREATER,
        MTLCompareFunctionLessEqual,    // FUNC_LEQUAL,
        MTLCompareFunctionGreaterEqual, // FUNC_GEQUAL
    };
    
    static const  MTLWinding WINDING_TABLE[] = {
        MTLWindingClockwise,
        MTLWindingCounterClockwise
    };
    
    static const MTLCullMode CULL_TABLE[] = {
        MTLCullModeFront,
        MTLCullModeBack,
        MTLCullModeNone,
    };
    
    void _TransferPendingTextures( id<MTLCommandBuffer> cmdbuffer );
    void _TransferPendingHeapTexturesNow();
    
    //======================================================================================================================
    static MTLTextureDescriptor* _NewDescriptorFromTexture( id<MTLTexture> texture, MTLStorageMode storageMode ) {
        MTLTextureDescriptor * descriptor = [MTLTextureDescriptor new];

        descriptor.textureType      = texture.textureType;
        descriptor.pixelFormat      = texture.pixelFormat;
        descriptor.width            = texture.width;
        descriptor.height           = texture.height;
        descriptor.depth            = texture.depth;
        descriptor.mipmapLevelCount = texture.mipmapLevelCount;
        descriptor.arrayLength      = texture.arrayLength;
        descriptor.sampleCount      = texture.sampleCount;
        descriptor.storageMode      = storageMode;

        return descriptor;
    }
    
    //======================================================================================================================
    id<MTLDevice> GetDevice() {
        return gfxstate->m_device;
    }
    
    //======================================================================================================================
    Context * CreateContext() {
        return new ContextMetal;
    }
    
    //======================================================================================================================
    Buffer * CreateBuffer() {
        return new BufferMetal;
    }
    
    //======================================================================================================================
    Buffer * CreateBuffer( size_t size, uint64_t flags ) {
        Buffer * buff = CreateBuffer();
        buff->Create( size, flags );
        return buff;
    }
    
    //======================================================================================================================
    Texture * CreateTexture() {
        return new TextureMetal;
    }
    
    //======================================================================================================================
    Texture * CreateTexture( Texture::FORMAT format, uint32_t width, uint32_t height, uint32_t mipCount ) {
        Texture * tex = CreateTexture();
        tex->Create( format, width, height, mipCount );
        return tex;
    }
    
    //======================================================================================================================
    VertexDesc * CreateVertexDesc() {
        return new VertexDescLocal;
    }
    
    //======================================================================================================================
    VertexDesc * CreateVertexDesc( const VertexDesc::Element * elements ) {
        VertexDesc * vd = CreateVertexDesc();
        vd->Create( elements );
        return vd;
    }
    
    //======================================================================================================================
    GpuFunction * CreateGpuFunction() {
        return new GpuFunctionMetal;
    }
    
    //======================================================================================================================
    GpuFunction * CreateGpuFunction( const char * name ) {
        GpuFunction * func = CreateGpuFunction();
        func->Create( name );
        return func;
    }
    
    //======================================================================================================================
    GpuPipeline * CreateGpuPipeline() {
        return new GpuPipelineMetal;
    }
    
    //======================================================================================================================
    GpuPipeline * CreateGpuPipeline( const GpuPipeline::Params * params  ) {
        GpuPipeline * pl = CreateGpuPipeline();
        pl->Create( params );
        return pl;
    }
    
    //======================================================================================================================
    Pass * CreatePass( const char * name ) {
        PassMetal * pass = new PassMetal;
        
        if (name != nullptr ) {
            pass->Create( name );
        }
        
        return pass;        
    }
    
    //======================================================================================================================
    Fence * CreateFence( const char * label ) {
        FenceMetal * fence = new FenceMetal;
        
        if ( label != nullptr ) {
            fence->m_fence.label = [[NSString alloc] initWithUTF8String: label];
        }
        
        return fence;
    }
    
    //======================================================================================================================
    DepthState * CreateDepthState() {
        return new DepthStateMetal;
    }
    
    //======================================================================================================================
    DepthState * CreateDepthState( gfx::FUNC depthFunc, bool depthWrite, bool depthTest, float depthClear, uint8_t stencilClear ) {
        DepthState * state = CreateDepthState();
        state->Set( depthFunc, depthWrite, depthTest, depthClear, stencilClear );
        return state;
    }
    
    //======================================================================================================================
    RasterizeState * CreateRasterizeState() {
        return new RasterizeStateMetal;
    }
    
    //======================================================================================================================
    RasterizeState * CreateRasterizeState( gfx::WINDING frontFace, gfx::CULL cullFace ) {
        RasterizeState * state = CreateRasterizeState();
        state->Set( frontFace, cullFace );
        return state;
    }
    
    //======================================================================================================================
    id<MTLCommandBuffer> GetCommandBuffer() {
        return gfxstate->m_cmdBuffer;
    }
    
    //======================================================================================================================
    id<MTLCommandQueue> GetCommandQueue() {
        return gfxstate->m_cmdQueue;
    }
    
    //======================================================================================================================
    id<MTLRenderCommandEncoder> GetRenderEncoder() {
        return gfxstate->m_encoder;
    }
    
    //======================================================================================================================
    MTLRenderPassDescriptor * GetRenderPassDescriptor() {
        return gfxstate->m_renderPassDescriptor;
    }
    
    //======================================================================================================================
    GpuTextureArray * CreateGpuTextureArray() {
        return new GpuTextureArrayMetal;
    }
    
    //======================================================================================================================
    GpuTextureArray * CreateGpuTextureArray( GpuTextureArray::Params & params ) {
        GpuTextureArray * texArray = CreateGpuTextureArray();
        texArray->Create( params );
        return texArray;
    }

    //======================================================================================================================
    void _InitCaps() {
        MTLArgumentBuffersTier argBufferTier = GetDevice().argumentBuffersSupport;
    
        if ( argBufferTier >= MTLArgumentBuffersTier2 ) {
            // This device supports lots of bindless textures.
            gfxstate->m_caps.m_bindlessTexture.m_supportsBindless = 1;
            gfxstate->m_caps.m_bindlessTexture.m_useResourceHeap = 1;
            gfxstate->m_caps.m_bindlessTexture.m_maxTextures = 5000000;
        }
        
    }

    //======================================================================================================================
    void Initialise() {
        if ( gfxstate != nullptr ) {
            return;
        }
        
        gfxstate = new( s_stateMem ) GfxState;
        _InitCaps();
        
        gfx::Caps deviceCaps;
        GetCaps( deviceCaps );
    }

    //======================================================================================================================
    void Finalise() {
        if (gfxstate != nullptr ) {
            gfxstate->~GfxState();
            gfxstate = nullptr;
        }
    }
    
    //======================================================================================================================
    void GetCaps( Caps & caps ) {
        caps = gfxstate->m_caps;
    }
    
    //======================================================================================================================
    bool BeginScene( Context * ctxt ) {
        assert( ctxt != nullptr );
        assert( gfxstate->m_currCtxt == nullptr );
        
        gfxstate->m_currCtxt = (ContextMetal * ) ctxt;
        gfxstate->m_cmdQueue = gfxstate->m_currCtxt->m_cmdQueue;
        
        dispatch_semaphore_wait( gfxstate->m_currCtxt->m_inFlightSemaphore, DISPATCH_TIME_FOREVER);
        
        gfxstate->m_cmdBuffer = [gfxstate->m_cmdQueue commandBuffer];

        // Add a completion call back to say that this command buffer has
        // completed.
        __block dispatch_semaphore_t block_sema = gfxstate->m_currCtxt->m_inFlightSemaphore;
        [ gfxstate->m_cmdBuffer addCompletedHandler:^(id<MTLCommandBuffer> buffer)
        {
            dispatch_semaphore_signal(block_sema);
        }];
        
        gfxstate->m_renderPassDescriptor = gfxstate->m_currCtxt->m_view.currentRenderPassDescriptor;
        if ( gfxstate->m_renderPassDescriptor == nil ) {
            return false;
        }
        
        return true;
    }
    
    //======================================================================================================================
    void EndScene() {
        [ gfxstate->m_cmdBuffer commit];
        gfxstate->m_currCtxt = nullptr;
    }
    
    //======================================================================================================================
    void Present() {
        [ gfxstate->m_cmdBuffer presentDrawable: gfxstate->m_currCtxt->m_view.currentDrawable];
    }
    
    //======================================================================================================================
    void BeginPass( const char * name ) {
        assert( gfxstate->m_encoder == nullptr );
        
        gfxstate->m_encoder = [gfxstate->m_cmdBuffer renderCommandEncoderWithDescriptor: gfxstate->m_renderPassDescriptor];
        gfxstate->m_encoder.label = [[NSString alloc] initWithUTF8String: name];
        
        
    }
    
    //======================================================================================================================
    void BeginPass( Pass * pass ) {
        PassMetal * passMetal = (PassMetal *) pass;
        
        assert( gfxstate->m_encoder == nullptr );
        
        // At the start of each pass, we'll see if we need to transfer any resources. If so, we'll
        // queue them for transfer before the pass uses them. 
        if (gfxstate->m_pendingTextureCount != 0 ) {
            _TransferPendingTextures( gfxstate->m_cmdBuffer );
        }
                        
        gfxstate->m_encoder = [gfxstate->m_cmdBuffer renderCommandEncoderWithDescriptor: gfxstate->m_renderPassDescriptor];
        passMetal->m_encoder = gfxstate->m_encoder;
        
        if ( gfxstate->m_resourceHeap ) {
            [ gfxstate->m_encoder useHeap: gfxstate->m_resourceHeap ];
        }
        
        if ( passMetal->m_label != nullptr ) {
            gfxstate->m_encoder.label = [[NSString alloc] initWithUTF8String: passMetal->m_label];
        }
    }

    //======================================================================================================================
    void EndPass( ) {
        assert( gfxstate->m_encoder != nil );
        [ gfxstate->m_encoder endEncoding ];
        
        gfxstate->m_encoder = nil;
    }
    
    //======================================================================================================================
    void SetDepthState( DepthState * state ) {
        DepthStateMetal * stateMtl = (DepthStateMetal * ) state;
        
        if ( stateMtl->m_changed == 1 ) {
            // We'll handle the creation of the depth state here.
            MTLDepthStencilDescriptor *depthStateDesc = [[MTLDepthStencilDescriptor alloc] init];
            
            MTLCompareFunction mtlCompare = FUNC_TABLE[ stateMtl->m_depthFunc ];
            if ( stateMtl->m_depthTest == 0 ) {
                mtlCompare = MTLCompareFunctionAlways;
            }
            
            depthStateDesc.depthCompareFunction = mtlCompare;
            depthStateDesc.depthWriteEnabled = (stateMtl->m_depthWrite == 1) ? YES : NO;
            
            stateMtl->m_state = [gfx::GetDevice() newDepthStencilStateWithDescriptor:depthStateDesc];
            
            // Update changes status of the state
            stateMtl->m_changed = 0;
        }
        
        [ gfxstate->m_encoder setDepthStencilState: stateMtl->m_state ];
    }
    
    //======================================================================================================================
    void SetRasterizeState( RasterizeState * state ) {
        
        RasterizeStateMetal * stateMtl = (RasterizeStateMetal *) state;
        
        MTLWinding windingMtl = WINDING_TABLE[ stateMtl->m_frontFace ];
        MTLCullMode cullMtl = CULL_TABLE[ stateMtl->m_cullFace ];
        
        [ gfxstate->m_encoder setFrontFacingWinding: windingMtl ];
        [ gfxstate->m_encoder setCullMode: cullMtl ];
        stateMtl->m_changed = 0;            
    }

    //======================================================================================================================
    void SetVertexBuffer( Buffer * buffer, uint32_t index, uint64_t offset ) {
        [ gfxstate->m_encoder setVertexBuffer: ((BufferMetal*) buffer)->m_buffer
                                       offset: offset
                                      atIndex: index ];
    }

    //======================================================================================================================
    void SetVertexBufferOffset( uint32_t index, uint64_t offset ) {
        [ gfxstate->m_encoder setVertexBufferOffset: offset
                                            atIndex: index ];
    }

    //======================================================================================================================
    void SetPixelBuffer( Buffer * buffer, uint32_t index, uint64_t offset ) {
        [ gfxstate->m_encoder setFragmentBuffer: ((BufferMetal*) buffer)->m_buffer
                                         offset: offset
                                        atIndex: index ];
    }

    //======================================================================================================================
    void SetPixelBufferOffset( uint32_t index, uint64_t offset ) {
        [ gfxstate->m_encoder setFragmentBufferOffset: offset
                                              atIndex: index ];
    }
    
    //======================================================================================================================
    void SetRenderPipeline( GpuPipeline * pl ) {
        [ gfxstate->m_encoder setRenderPipelineState: ((GpuPipelineMetal*)pl)->m_pipeline ];
    }
    
    //======================================================================================================================
    void SetVertexTexture( Texture * tex, uint32_t index ) {
        [ gfxstate->m_encoder setVertexTexture: ((TextureMetal*)tex)->m_texture  atIndex:index ];
    }
    
    //======================================================================================================================
    void SetPixelTexture( Texture * tex, uint32_t index ) {
        [ gfxstate->m_encoder setFragmentTexture: ((TextureMetal*)tex)->m_texture  atIndex:index ];
    }

    //======================================================================================================================
    void DrawIndexedPrim( PRIM primType, uintptr_t start, size_t count, Buffer * indices ) {
        
        MTLPrimitiveType mtlPrimType = PRIM_TABLE[ primType ];
        
        [ gfxstate->m_encoder drawIndexedPrimitives: mtlPrimType
                                         indexCount: count
                                          indexType: MTLIndexTypeUInt32
                                        indexBuffer: ((BufferMetal*) indices)->m_buffer
                                  indexBufferOffset: sizeof(uint32_t ) * start ];
    }
    
    //======================================================================================================================
    void DrawPrim( PRIM primType, uintptr_t start, size_t count ) {
        
        MTLPrimitiveType mtlPrimType = PRIM_TABLE[ primType ];
        
        [ gfxstate->m_encoder drawPrimitives: mtlPrimType
                                 vertexStart: start
                                 vertexCount: count ];
    }
    
    //======================================================================================================================
    void UpdateFence( Fence * fence, uint64_t stages ) {
        
        uint64_t metalStages = ( ( stages & gfx::RENDER_STAGE_VERTEX) != 0 ) ? MTLRenderStageVertex : 0;
        metalStages |= ( ( stages & gfx::RENDER_STAGE_PIXEL) != 0 ) ? MTLRenderStageFragment : 0;
        
        FenceMetal * fenceMetal = ( FenceMetal * ) fence;
        
        [ gfxstate->m_encoder  updateFence: fenceMetal->m_fence
                               afterStages: metalStages ];
    }
    
    //======================================================================================================================
    void WaitForFence( Fence * fence, uint64_t stages ) {
        
        uint64_t metalStages = ( ( stages & gfx::RENDER_STAGE_VERTEX) != 0 ) ? MTLRenderStageVertex : 0;
        metalStages |= ( ( stages & gfx::RENDER_STAGE_PIXEL) != 0 ) ? MTLRenderStageFragment : 0;
        
        FenceMetal * fenceMetal = ( FenceMetal * ) fence;
        
        [ gfxstate->m_encoder waitForFence: fenceMetal->m_fence
                              beforeStages: metalStages ];
    }
    
    
    //======================================================================================================================
    void InitResourceHeap( size_t size ) {
        
        MTLHeapDescriptor *heapDescriptor = [MTLHeapDescriptor new];
        heapDescriptor.storageMode = MTLStorageModePrivate;
        heapDescriptor.size = size;
        
        // Create a heap large enough to store all resources
        gfxstate->m_resourceHeap = [GetDevice() newHeapWithDescriptor:heapDescriptor];
    }
    
    
    //======================================================================================================================
    void MoveTextureToResourceHeap( Texture * tex, bool moveNow  ) {
        TextureMetal * texMtl = (TextureMetal *) tex;
        
        // If it's already a heap texture, we can just bail
        if ( texMtl->m_heapTexture == 1 ) {
            return;
        }
        
        texMtl->m_heapTexture = 1;
        
        XE_ASSERT( gfxstate->m_pendingTextureCount < GfxState::MAX_PENDING );
        
        gfxstate->m_pendingTextures[ (unsigned int) gfxstate->m_pendingTextureCount ] = texMtl;
        ++gfxstate->m_pendingTextureCount;
        
        if ( moveNow == true ) {
            _TransferPendingHeapTexturesNow();
        }
    }
    
    //======================================================================================================================
    void _TransferPendingHeapTexturesNow() {
        id<MTLCommandQueue> cmdQueue = [ GetDevice() newCommandQueue];
        id<MTLCommandBuffer> cmdBuffer = [ cmdQueue commandBuffer ];
        
        _TransferPendingTextures( cmdBuffer );
        
        [cmdBuffer commit];
        [cmdBuffer waitUntilCompleted];
    }
    
    //======================================================================================================================
    void _TransferPendingTextures( id<MTLCommandBuffer> cmdbuffer ) {
        id <MTLBlitCommandEncoder> blitEncoder = cmdbuffer.blitCommandEncoder;
        blitEncoder.label = @"Heap Transfer Blit Encoder";
        

        for ( uint32_t i =0; i < gfxstate->m_pendingTextureCount; ++i ) {
            TextureMetal * tex = gfxstate->m_pendingTextures[ i ];
 
            
            MTLRegion region = MTLRegionMake2D(0, 0, tex->GetWidth(), tex->GetHeight());
            
            MTLTextureDescriptor *descriptor = _NewDescriptorFromTexture( tex->m_texture, gfxstate->m_resourceHeap.storageMode );
                        
            // Create a texture from the heap
            id<MTLTexture> heapTexture = [gfxstate->m_resourceHeap newTextureWithDescriptor:descriptor];
            
            for( uint32_t level = 0; level <= tex->GetMipCount(); ++level ) {
                
                for( NSUInteger slice = 0; slice < tex->m_texture.arrayLength; slice++) {
                    [blitEncoder copyFromTexture: tex->m_texture
                                     sourceSlice: slice
                                     sourceLevel: level
                                    sourceOrigin: region.origin
                                      sourceSize: region.size
                                       toTexture: heapTexture
                                destinationSlice: slice
                                destinationLevel: level
                               destinationOrigin: region.origin];
                }
                region.size.width /= 2;
                region.size.height /= 2;
            }
            
            tex->m_heapTexturePending = 0;
            tex->m_heapTexture = 1;
            tex->m_texture = heapTexture;
        }
        
        [blitEncoder endEncoding];
        
        gfxstate->m_pendingTextureCount = 0;
    }

}}
