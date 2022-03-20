//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#ifndef __TOOLMESH_H__
#define __TOOLMESH_H__

#include "i3d/util/RefObject.h"
#include "i3d/util/RefPointer.h"
#include "i3d/util/toolmodel/ToolSkin.h"
#include "i3d/util/toolmodel/ToolMaterial.h"

namespace i3d {

    class ToolModel;
    class ToolSkeleton;

    class ToolModel;
    class ToolSkeleton;
    class ToolJoint;

    class ToolMesh : public RefObject {
    public:
    typedef RefPointer<ToolMesh> ref_ptr_t;
        typedef std::vector<ToolMesh::ref_ptr_t> vector_t;
        typedef std::map<uint64_t, ref_ptr_t> map_t;
        
        ToolMesh( ToolModel * model );
        
        virtual ~ToolMesh();

        void CalcBounds();

        ToolSkin * AddSkin();

        void Copy( ToolMesh * rhs );

        void MakeSkinShell();

        bool IsSkinned() const;

        void SetMaterial( const char * name );
        
        void TransformByJoint( ToolJoint * joint );
        
        void ApplyTransform( const i3d::Matrix4 & transform );

        void FlipTriangles();
        
        void CalcSmoothNormals();
        
    protected:
        
        void CalcFaceNormals( const std::vector<uint32_t> & tris, const std::vector<i3d::Vector3> & verts, std::vector<i3d::Vector3> & normals );
        
        void CalcFaceNormals( std::vector<i3d::Vector3> & normals );
        
        i3d::Vector3 CalcVertexNormal( uint32_t vertex, const std::vector<uint32_t> & tris, const std::vector<i3d::Vector3> & faceNormals );

    public:
        std::string                 m_name;
        ToolModel *                 m_model;
        ToolSkin::ref_ptr_t         m_skin;
        ToolMaterial::ref_ptr_t     m_material;
        std::vector<i3d::Vector3>        m_verts;
        std::vector<i3d::Vector3>        m_normals;
        std::vector<i3d::Vector3>        m_texcoords;
        std::vector<uint32_t>           m_triangleIndices;
        i3d::Vector3                     m_boundsMin;
        i3d::Vector3                     m_boundsMax;
    };
}

#endif 
