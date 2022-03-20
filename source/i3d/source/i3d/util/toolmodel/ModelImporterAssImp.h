//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#ifndef __MODELIMPORTERASSIMP_H__
#define __MODELIMPORTERASSIMP_H__

#include "i3d/util/toolmodel/ToolModel.h"
#include "i3d/util/toolmodel/ModelImporter.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace i3d {

    class ModelImporterAssImp : public ModelImporter  {
    public:
        ModelImporterAssImp( Options & options );

        virtual ~ModelImporterAssImp();

        virtual ToolModel::ref_ptr_t Load( const char * path );

    protected:

        void ImportMaterials();

        void ImportSkeleton();

        void ImportJoint( aiNode * );

        void ImportMeshes();

        void ImportMesh( aiMesh * srcMesh );
        
        void ImportAnim( aiScene * scene );

        bool LoadScene( const char * path );

        i3d::Matrix4 Convert(const aiMatrix4x4& aiMat);

    protected:
        Assimp::Importer*			m_importer;
        const aiScene*				m_scene;
        ToolModel::ref_ptr_t		m_model;			///< The model that we're importing to
    };

    //================================================================================================================================
    inline i3d::Matrix4 ModelImporterAssImp::Convert( const aiMatrix4x4& aiMat ) {
        i3d::Matrix4 mat;
        
        mat.m_rows[0].Set(aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1);
        mat.m_rows[1].Set(aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2);
        mat.m_rows[2].Set(aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3);
        mat.m_rows[3].Set(aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4);
        
        return mat;
    }
}

#endif
