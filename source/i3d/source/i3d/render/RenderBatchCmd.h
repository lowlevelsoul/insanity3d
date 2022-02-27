//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_RENDERBATCHCMD_H__
#define __I3D_RENDERBATCHCMD_H__

namespace i3d {
    class Model;
    class MaterialLocal;
    class ModelInstance;
    class Material;
}

namespace i3d {

    /// A useful template for implement singl-elinked lists
    template<typename __type__>
    struct BatchList {
        __type__ *  m_head;
        __type__ *  m_tail;
        
        void Reset() {
            m_head = nullptr;
            m_tail = nullptr;
        }
        
        void Add(__type__ * item ) {
            if (m_head == nullptr) {
                m_head = item;
            }
            else {
                m_tail->m_next = item;
            }
            
            m_tail = item;
            item->m_next = nullptr;
        }
    };
    
    struct CmdDrawModelMesh {
        uint32_t        m_vertexStart;
        uint32_t        m_vertexCount;
        uint32_t        m_indexStart;
        uint32_t        m_indexCount;
        
        uint64_t        m_vertexTransformConstOffs;
        uint64_t        m_indexTransformConstOffs;
    };
    
    struct CmdSkinTransform;
    
    struct CmdDrawModel {
        CmdDrawModel *      m_next;
        Matrix4             m_transform;
        Model *             m_model;                    ///< Pointer to the implementaton model pointer
        uint32_t            m_transformVertexStart;     ///< Start index of the transformed vertex buffer for this model
        typedef BatchList<CmdDrawModel> list_t;
    };
    
    struct CmdSkinTransform {
        CmdSkinTransform *  m_next;
        ModelInstance *     m_modelInst;
        uintptr_t           m_vertexStart;
        uintptr_t           m_vertexCount;
        CmdDrawModelMesh *  m_meshes;
        uint32_t            m_meshCount;
        
        uintptr_t           m_offsAnimMatConst;
        uintptr_t           m_offsBindMatConst;
        
        
        typedef BatchList<CmdSkinTransform> list_t;
    };
    
    struct CmdMaterialDraw {
        CmdMaterialDraw *       m_next;
        CmdDrawModel *          m_drawModel;
        
        typedef BatchList<CmdMaterialDraw> list_t;
    };
    
    struct CmdMaterial {
        Material *                  m_material;
        CmdDrawModel::list_t        m_drawList;
        uint32_t                    m_drawIndexStart;
        uint32_t                    m_drawIndexCount;
    };
    
    struct CmdLineVertex {
        Vector4         m_pos;
        Vector4         m_colour;
    };
    
    struct CmdScene3d {
        CmdScene3d *            m_next;
        Matrix4                 m_matProj;
        Matrix4                 m_matView;
        int32_t                 m_viewport[4];
        
        Vector4                 m_cameraPos;
        Vector4                 m_globalLightDir;
        Vector4                 m_globalLightColour;
        
        CmdLineVertex *         m_lineVerts;
        uint32_t *              m_lineIndices;
        size_t                  m_lineIndexCapacity;
        size_t                  m_lineIndexCount;
        size_t                  m_lineVertexCapacity;
        size_t                  m_lineVertexCount;

        uint32_t                m_lineIndexStart;
        uint64_t                m_sceneConstOffs;
        uint64_t                m_scenePixelConstOffs;          ///< Offset in the scene constant buffer for the 3d-scene's pixel constants
                                                                ///
                                                                ///
                                                                    
        uint32_t                m_materialListStart;
        uint32_t                m_materialListCount;
        
        CmdSkinTransform::list_t    m_skins;
        
        // The following members are used only when we're using material argument buffers / bindless textures
        CmdDrawModel::list_t    m_modelDraw;            ///< List of models to be drawn in in this scene
        uintptr_t               m_opaqueIndexStart;     ///< Offset in bytes of the indices for the opaqaue triangles for this scene
        size_t                  m_opaqueIndexCount;     ///< Number of indices for the opaqaue triangles in this scene
        
        typedef BatchList<CmdScene3d> list_t;
    };
    
    struct CmdScene {
        CmdScene3d::list_t      m_scene3dList;
        
        CmdMaterial *           m_materials;
        uint32_t                m_materialCapacity;
        uint32_t                m_materialCount;
        
        uint32_t                m_transformedVertexCount;
    };
}


#endif
