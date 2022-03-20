//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#ifndef __TOOLMODEL_H__
#define __TOOLMODEL_H__

#include "i3d/util/RefObject.h"
#include "i3d/util/RefPointer.h"
#include "i3d/util/toolmodel/ToolMesh.h"
#include "i3d/util/toolmodel/ToolSkeleton.h"
#include "i3d/util/toolmodel/ToolMaterial.h"
#include "i3d/util/toolmodel/ToolAnimation.h"

namespace i3d {

    class ToolModel : public RefObject {
    public:
        typedef RefPointer<ToolModel> ref_ptr_t;

        ToolModel();
        
        virtual ~ToolModel();

        ToolMesh * AddMesh( const char * name );

        ToolSkeleton * AddSkeleton() {
            ToolSkeleton * skeleton = new ToolSkeleton( this );
            m_skeleton = skeleton;
            return skeleton;
        }

        ToolMaterial * AddMaterial( const char * name );

        ToolMaterial * FindMaterial( const char * name );
        
        ToolAnimation * AddAnimation();

        bool HasSkinnedMeshes() const;
        
    public:
        ToolSkeleton::ref_ptr_t     m_skeleton;
        ToolMesh::vector_t          m_meshes;
        ToolMaterial::map_t         m_materials;
        ToolAnimation::vector_t     m_anims;
    };
}

#endif
