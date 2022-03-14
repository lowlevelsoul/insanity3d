//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/math/Math3d.h"
#include "i3d/render/Model.h"
#include "i3d/core/FileSystem.h"

#include "i3d/render/ModelStream.h"
#include "i3d/render/Model_local.h"

#include "i3d/gfx/Gfx.h"

namespace i3d {
    
    DEFINE_RESOURCE(ModelLocal, "bmdl;")
    
    //======================================================================================================================
    ModelLocal::ModelLocal() {
        m_vertices = nullptr;
        m_influences = nullptr;
        m_indices = nullptr;        
    }

    //======================================================================================================================
    ModelLocal::~ModelLocal() {
        SAFE_DELETE( m_vertices );
        SAFE_DELETE( m_influences );
        SAFE_DELETE( m_indices );
    }
    
    //======================================================================================================================
    void ModelLocal::Create( size_t vertexCount, size_t indexCount, size_t meshCount, uint64_t flags ) {
        
        m_vertexCount = vertexCount;
        m_vertexStride = 32;
        m_vertexSize = vertexCount * m_vertexStride;
        m_indexCount = indexCount;
        m_indexStride = 4;
        m_indexSize = indexCount * m_indexStride;
        
        m_vertices = gfx::CreateBuffer( m_vertexSize, 0 );
        m_indices = gfx::CreateBuffer( m_indexSize, 0);
        
        if ( ( flags & F_SKINNED ) != 0 ) {
            
            size_t infSize = vertexCount * sizeof(Influence);
            m_influences = gfx::CreateBuffer( infSize, 0);
        }
        
        m_boundsMin = Vector3::ZERO;
        m_boundsMax = Vector3::ZERO;
    }
    
    //======================================================================================================================
    void ModelLocal::WriteVertices( const void * src, uintptr_t start, size_t count ) {
        assert(start + count <= m_vertexCount);
        uintptr_t dstOffset =  ( m_vertexStride * start );
        size_t dstSize =  ( m_vertexStride * count );
        
        uint8_t * bufferMem = (uint8_t *) m_vertices->Map( gfx::BUFFER_MAP_WRITEONLY );
        
        uint8_t * bufferPtr = ((uint8_t*) bufferMem ) + dstOffset;
        memcpy( bufferPtr, src, dstSize );
        
        m_vertices->Unmap();
    }

    //======================================================================================================================
    void ModelLocal::WriteIndices( const void * src, uintptr_t start, size_t count ) {
        assert(start + count <= m_indexCount);
        uintptr_t dstOffset =  ( m_indexStride * start );
        size_t dstSize =  ( m_indexStride * count );
        
        uint8_t * bufferMem = ( uint8_t * ) m_indices->Map( gfx::BUFFER_MAP_WRITEONLY );
        
        uint8_t * bufferPtr = ( ( uint8_t * ) bufferMem ) + dstOffset;
        memcpy( bufferPtr, src, dstSize );
        
        m_indices->Unmap();
    }
    
    //======================================================================================================================
    void ModelLocal::WriteInfluences( const void * src, uint32_t start, size_t count ) {
        assert( start + count <= m_vertexCount );
        uintptr_t dstOffset = ( sizeof(Influence) * start );
        size_t dstSize = ( sizeof(Influence) * count );
        
        uint8_t * bufferMem = ( uint8_t * ) m_influences->Map( gfx::BUFFER_MAP_WRITEONLY );
        
        uint8_t * bufferPtr = ( ( uint8_t* ) bufferMem ) + dstOffset;
        memcpy( bufferPtr, src, dstSize );
        
        m_indices->Unmap();
    }
    
    //======================================================================================================================
    void ModelLocal::Load( File * file, const char * path ) {
        uint8_t * buffer = nullptr;
        
        size_t fileLen = file->Length();
        
        buffer = new uint8_t[ fileLen ];
        XE_ASSERT(buffer != nullptr );
        
        size_t readAmt = file->Read( buffer, sizeof(uint8_t), fileLen );
        XE_ERROR( readAmt != fileLen, "Error reading model file" );
        
        i3d::ModelStream * mdlStr = (i3d::ModelStream * ) buffer;
        XE_ASSERT(mdlStr->m_version == i3d::ModelStream::VERSION);
        
        uint64_t modelFlags = 0;
        if ( mdlStr->m_flags & ModelStream::F_SKINNED ) {
            modelFlags |= Model::F_SKINNED;
        }
        
        const MeshStream * meshStr = mdlStr->GetMeshes();

        Create( meshStr->m_vertexCount, meshStr->m_indexCount, 0, modelFlags );
        
        WriteVertices( mdlStr->GetVertices(), 0, meshStr->m_vertexCount );
        WriteIndices( mdlStr->GetIndices(), 0, meshStr->m_indexCount );

        if ( mdlStr->m_flags & ModelStream::F_SKINNED ) {
            WriteInfluences( mdlStr->GetInfluences(), 0, meshStr->m_vertexCount );
        }

        SetBounds( mdlStr->GetBoundsMin(), mdlStr->GetBoundsMax() );
        
        SAFE_DELETEARRAY( buffer );
    }
}
