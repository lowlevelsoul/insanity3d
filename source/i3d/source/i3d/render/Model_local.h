//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_MODEL_COMMON_H__
#define __I3D_MODEL_COMMON_H__

#include "i3d/render/Model.h"
#include "i3d/gfx/GfxBuffer.h"

namespace i3d {
    
    class ModelLocal : public Model {
    public:
        DECLARE_RESOURCE(ModelLocal)
        
        struct StaticVertex {
            float posTexU[4];
            float normalTexV[4];
        };
        
        struct Influence {
            uint32_t    m_joints[4];
            float       m_weights[4];
            uint32_t    m_params[4];
        };
        
        ModelLocal();
        
        virtual ~ModelLocal();
        
        virtual void Create( size_t vertexCount, size_t indexCount, size_t meshCount, uint64_t flags = 0) override;
        
        /// Write vertex data to a model
        virtual void WriteVertices( const void * src, uintptr_t start, size_t count ) override;

        /// Write index data to a model
        virtual void WriteIndices( const void * src, uintptr_t start, size_t count ) override;
        
        virtual void WriteInfluences( const void * src, uint32_t start, size_t count ) override;
        
        virtual void Load( File * file, const char * path ) override;
        
    public:        
        gfx::Buffer *           m_vertices;
        gfx::Buffer *           m_influences;
        gfx::Buffer *           m_indices;
    };
};

#endif
