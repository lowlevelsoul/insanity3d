//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/gfx/GfxGpuTextureArray_metal.h"
#include "i3d/gfx/GfxBuffer_metal.h"
#include "i3d/gfx/Gfx_metal.h"
#include "i3d/gfx/GfxTexture_metal.h"

namespace i3d { namespace gfx {

    //======================================================================================================================
    GpuTextureArrayMetal::GpuTextureArrayMetal() {
        
    }
    
    //======================================================================================================================
    GpuTextureArrayMetal::~GpuTextureArrayMetal() {
        
    }
    
    //======================================================================================================================
    void GpuTextureArrayMetal::Create( const Params & params ) {
        
        id<MTLLibrary> defaultLibrary = [ GetDevice() newDefaultLibrary];
        
        NSString * functionName = ( params.m_dummyFunctionName == nullptr ) ? @"ArgBufferDummy" : [[NSString alloc] initWithUTF8String: params.m_dummyFunctionName ];
        m_fakeFunction = [defaultLibrary newFunctionWithName: functionName ];
        
        m_encoder = [ m_fakeFunction newArgumentEncoderWithBufferIndex: params.m_bufferSlot ];
    }
    
    //======================================================================================================================
    size_t GpuTextureArrayMetal::GetBufferSize() {
        NSUInteger argumentBufferLength = m_encoder.encodedLength;
        return argumentBufferLength;
    }
    
    //======================================================================================================================
    void GpuTextureArrayMetal::EncodeToBuffer( Buffer * buffer, Texture ** textures, size_t textureCount ) {
        
        XE_ASSERT( textureCount <= CAPACITY );
        
        for ( uint32_t i =0; i != textureCount; i++) {
            m_textures[i] = ( ( i3d::gfx::TextureMetal * ) textures[ i ] )->m_texture;
        }
        
        BufferMetal * mtlBuffer = (BufferMetal *) buffer;
        
        [m_encoder setArgumentBuffer: mtlBuffer->m_buffer
                              offset: 0];
        
        [m_encoder setTextures: m_textures
                     withRange: NSMakeRange ( 0, textureCount ) ];
        
        for ( uint32_t i =0; i != textureCount; i++) {
            m_textures[i] = nil;
        }

    }
}}
