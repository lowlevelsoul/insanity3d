//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __RENDERBUFFERS_METAL_H__
#define __RENDERBUFFERS_METAL_H__

#include "i3d/core/Types.h"
#include "i3d/gfx/GfxVertexDesc.h"

namespace i3d { namespace gfx {
    class Buffer;
    class Fence;
}}

namespace i3d {

    class RenderBuffers {
    public:
        static constexpr size_t BUFFER_COUNT = 3;
        
        struct BufferInfo {
            gfx::Buffer *       m_vertexBuffer;
            gfx::Buffer *       m_indexBuffer;
            
            gfx::Buffer *       m_vertexTransformConstants;
            gfx::Buffer *       m_indexTransformConstants;
            
            gfx::Buffer *       m_sceneConstantBuffer;
            
            gfx::Buffer *       m_skinTargetBuffer;
            gfx::Buffer *       m_skinUniforms;
            
            gfx::Buffer *       m_bindlessTextureArray;
            
            gfx::Buffer *       m_lineVertices;
            gfx::Buffer *       m_lineIndices;

            gfx::Fence *        m_staticTransformFence;
            gfx::Fence *        m_skinTransformFence;

            uintptr_t           m_vertexBufferOffs;
            uintptr_t           m_indexBufferOffs;
            uintptr_t           m_skinBufferOffs;
            uintptr_t           m_skinUniformOffs;
            
            uintptr_t           m_vertexTransformOffs;
            uintptr_t           m_indexTransformOffs;
            
            uintptr_t           m_sceneConstantOffs;
            
            uint8_t *           m_vertexTransformMem;
            uint8_t *           m_indexTransformMem;
            uint8_t *           m_sceneConstantMem;
            uint8_t *           m_skinConstantMem;
        };
        
        template<typename __type__>
        struct AllocInfo {
            __type__ *      m_memory;
            uintptr_t       m_offs;
        };
        
        RenderBuffers();
        
        ~RenderBuffers();
        
        void Initialise( size_t vertexBufferSize, size_t indexBufferSize, size_t skinTargetBufferSize, size_t skinUniformBufferSize );
        
        void BeginFrame();
        
        bool AllocVertex( uintptr_t & offset, size_t size );
        
        bool AllocIndex( uintptr_t & offset, size_t size );
        
        bool AllocSkinVertex( uintptr_t & offset, size_t size );
        
        bool AllocSkinUniforms( uintptr_t & offset, size_t size );
        
        template<typename __type__>
        bool Alloc( AllocInfo<__type__> & info, size_t size, uintptr_t & offs, size_t count, uint8_t * bufferMem ) {
            size_t allocSize = sizeof(__type__) * count;
            uintptr_t newOffs = Sys::AlignValue<uintptr_t>(offs + allocSize, m_constantOffsetAlign);
            if ( newOffs > size ) {
                return false;
            }
            
            info.m_offs = offs;
            info.m_memory = (__type__ *) (bufferMem + offs);
            offs = newOffs;
            
            return true;
        }
        
        template<typename __type__>
        bool AllocVertexTransformConstant(AllocInfo<__type__> & info, size_t count ) {
            return Alloc<__type__>( info, m_vertexTransformConstantBufferSize,
                                   m_currBuffer->m_vertexTransformOffs, count,
                                   m_currBuffer->m_vertexTransformMem);
        }
        
        template<typename __type__>
        bool AllocIndexTransformConstant(AllocInfo<__type__> & info, size_t count ) {
            return Alloc<__type__>( info, m_indexTransformConstantBufferSize,
                                   m_currBuffer->m_indexTransformOffs, count,
                                   m_currBuffer->m_indexTransformMem);
        }
        
        template<typename __type__>
        bool AllocSceneConstant(AllocInfo<__type__> & info, size_t count ) {
            return Alloc<__type__>( info, m_sceneConstantBufferSize,
                                   m_currBuffer->m_sceneConstantOffs, count,
                                   m_currBuffer->m_sceneConstantMem);
        }
        
        template<typename __type__>
        bool AllocSkinConstant(AllocInfo<__type__> & info, size_t count ) {
            return Alloc<__type__>( info, m_skinUniformBufferSize,
                                   m_currBuffer->m_skinUniformOffs, count,
                                   m_currBuffer->m_skinConstantMem);
        }
        
        void BeginConstants();
        
        void EndConstants();
        
    public:
        BufferInfo      m_buffers[ BUFFER_COUNT ];          ///< Array of avilable buffers
        BufferInfo *    m_currBuffer;                       ///< Pointer to the currently active buffer
        uint32_t        m_nextBufferIndex;                  ///< Index to the next buffer

        gfx::VertexDesc * m_lineVertexDesc;
                                                    
        
        uintptr_t       m_constantOffsetAlign;
        
        size_t          m_vertexBufferSize;                 ///< Size of the vertex buffer (in bytes)
        size_t          m_indexBufferSize;                  ///< Size of the index buffer (in bytes)
        size_t          m_skinTargetBufferSize;             ///< Size of the skin target buffer (in bytes)
        size_t          m_skinUniformBufferSize;
        
        size_t          m_vertexTransformConstantBufferSize;
        size_t          m_indexTransformConstantBufferSize;
        size_t          m_sceneConstantBufferSize;
    };

    //======================================================================================================================
    inline bool RenderBuffers::AllocVertex( uintptr_t & offset, size_t size ) {
        assert( m_currBuffer->m_vertexBufferOffs + size <= m_vertexBufferSize );
        
        offset = m_currBuffer->m_vertexBufferOffs;
        m_currBuffer->m_vertexBufferOffs += size;
        
        return true;
    }

    //======================================================================================================================
    inline bool RenderBuffers::AllocIndex( uintptr_t & offset, size_t size ) {
        assert( m_currBuffer->m_indexBufferOffs + size <= m_indexBufferSize );
        
        offset = m_currBuffer->m_indexBufferOffs;
        m_currBuffer->m_indexBufferOffs += size;
        
        return true;
    }
    
    //======================================================================================================================
    inline bool RenderBuffers::AllocSkinVertex( uintptr_t & offset, size_t size ) {
        assert( m_currBuffer->m_skinBufferOffs + size <= m_skinTargetBufferSize );
        
        offset = m_currBuffer->m_skinBufferOffs;
        m_currBuffer->m_skinBufferOffs += size;
        
        return true;
    }
    
    //======================================================================================================================
    inline bool RenderBuffers::AllocSkinUniforms( uintptr_t & offset, size_t size ) {
        assert( m_currBuffer->m_skinUniformOffs + size <= m_skinUniformBufferSize );
        
        offset = m_currBuffer->m_skinUniformOffs;
        m_currBuffer->m_skinUniformOffs += size;
        
        return true;
    }
}

#endif
