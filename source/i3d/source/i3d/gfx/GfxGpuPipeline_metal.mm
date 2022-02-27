//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/gfx/GfxGpuPipeline_metal.h"
#include "i3d/gfx/GfxGpuFunction_metal.h"
#include "i3d/gfx/GfxVertexDesc_local.h"
#include "i3d/gfx/GfxContext_metal.h"
#include "i3d/gfx/Gfx_metal.h"
#include <MetalKit/MetalKit.h>

namespace i3d { namespace gfx {
    
    static const MTLVertexFormat TYPE_TABLE[] = {
        MTLVertexFormatInvalid,
        
        MTLVertexFormatFloat,
        MTLVertexFormatFloat2,
        MTLVertexFormatFloat3,
        MTLVertexFormatFloat4,
        
        MTLVertexFormatInt,
        MTLVertexFormatInt2,
        MTLVertexFormatInt3,
        MTLVertexFormatInt4,
        
        MTLVertexFormatUInt,
        MTLVertexFormatUInt2,
        MTLVertexFormatUInt3,
        MTLVertexFormatUInt4,
    };
    
    //======================================================================================================================
    GpuPipelineMetal::GpuPipelineMetal() {
        m_vertexDesc = nullptr;
        m_vertexFunc = nullptr;
        m_pixelFunc = nullptr;
    }
    
    //======================================================================================================================
    GpuPipelineMetal::~GpuPipelineMetal() {
        Destroy();
    }
    
    //======================================================================================================================
    void GpuPipelineMetal::Create( const Params * params ) {
        
        bool enableRasterise = false;
        
        // Create the gup functions
        if (params->m_vertexFuncName != nullptr ) {
            m_vertexFunc = (i3d::gfx::GpuFunctionMetal*) gfx::CreateGpuFunction( params->m_vertexFuncName );
        }
        
        if (params->m_pixelFuncName != nullptr ) {
            m_pixelFunc = (GpuFunctionMetal*) gfx::CreateGpuFunction( params->m_pixelFuncName );
            enableRasterise = true;
        }
        
        // Create the metal vertex description
        m_vertexDesc = [[MTLVertexDescriptor alloc] init];
        uint32_t attribCount = 0;
        
        for ( uint32_t i = 0; i < Params::MAX_VERTEX_DESC; ++i) {
            
            const VertexDescLocal * vd = (const VertexDescLocal *) params->m_vertexDesc[i];
            
            if ( vd != nullptr ) {
                
                for( const VertexDescLocal::ElementInfo * vi = vd->GetElements(); vi->m_type != TYPE_NIL; ++vi ) {
                    
                    m_vertexDesc.attributes[ attribCount ].bufferIndex = i;
                    m_vertexDesc.attributes[ attribCount ].format = TYPE_TABLE[ vi->m_type ];
                    m_vertexDesc.attributes[ attribCount ].offset = vi->m_offset;
                    
                    ++attribCount;
                }
                
                m_vertexDesc.layouts[i].stride = vd->GetStride();
                m_vertexDesc.layouts[i].stepRate = 1;
                m_vertexDesc.layouts[i].stepFunction = MTLVertexStepFunctionPerVertex;
            }
        }
        
        MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        if ( params->m_label != nullptr) {
            // Optional label.
            pipelineStateDescriptor.label               = [NSString stringWithUTF8String: params->m_label];
        }
        
        if ( m_vertexFunc != nullptr ) {
            pipelineStateDescriptor.vertexFunction = m_vertexFunc->m_function;
        }
        
        if (m_pixelFunc != nullptr ) {
            pipelineStateDescriptor.fragmentFunction = m_pixelFunc->m_function;
        }
        
        pipelineStateDescriptor.rasterizationEnabled = (enableRasterise == true) ? YES : NO;
        pipelineStateDescriptor.vertexDescriptor = m_vertexDesc;
        
        if ( enableRasterise == true && params->m_context != nullptr ) {
            
            MTKView * view = ((ContextMetal *) params->m_context)->m_view;
                            
            MTLRenderPipelineColorAttachmentDescriptor * att = pipelineStateDescriptor.colorAttachments[0];
            att.pixelFormat = view.colorPixelFormat;
            pipelineStateDescriptor.depthAttachmentPixelFormat = view.depthStencilPixelFormat;
            pipelineStateDescriptor.stencilAttachmentPixelFormat = view.depthStencilPixelFormat;
            pipelineStateDescriptor.sampleCount = view.sampleCount;
        }
        
        NSError *error = NULL;
        
        id<MTLRenderPipelineState> pipeline = [gfx::GetDevice() newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];
        if (pipeline == nil) {
            NSLog(@"Failed to created pipeline state, error %@", error);
        }
        
        m_pipeline = pipeline;
    }
    
    //======================================================================================================================
    void GpuPipelineMetal::Destroy() {
        m_vertexDesc = nil;
        m_pipeline = nil;
        
        SAFE_DELETE( m_vertexFunc );
        SAFE_DELETE( m_pixelFunc );
    }
}}
