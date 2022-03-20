//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#include "i3d/util/toolmodel/ToolModel.h"

namespace i3d {

    //==========================================================================================================================================
    ToolModel::ToolModel() {
        
    }

    //==========================================================================================================================================
    ToolModel::~ToolModel() {
        
    }

    //==========================================================================================================================================
    bool ToolModel::HasSkinnedMeshes() const {

        for ( auto m : m_meshes ) {
            if ( m->IsSkinned() == true ) {
                return true;
            }
        }

        return false;
    }

    //==========================================================================================================================================
    ToolMesh * ToolModel::AddMesh( const char * name ) {

        ToolMesh * mesh = new ToolMesh( this );
        mesh->m_name = name;
        m_meshes.push_back( mesh );

        return mesh;
    }

    //==========================================================================================================================================
    ToolMaterial * ToolModel::AddMaterial( const char * name ) {
        ToolMaterial * mat = FindMaterial( name );
        if ( mat != nullptr ) {
            return mat;
        }

        mat = new ToolMaterial( this );
        mat->m_name = name;

        uint64_t hash = i3d::fh64::CalcFromString( name );
        m_materials[ hash ] = mat;

        return mat;
    }

    //==========================================================================================================================================
    ToolMaterial * ToolModel::FindMaterial( const char * name ) {
        uint64_t hash = i3d::fh64::CalcFromString( name );

        ToolMaterial::map_t::iterator findIt = m_materials.find( hash );
        if ( findIt == m_materials.end() ) {
            return nullptr;
        }

        return findIt->second;
    }


    //==========================================================================================================================================
    ToolAnimation * ToolModel::AddAnimation() {
        ToolAnimation * anim = new ToolAnimation( this );
        m_anims.push_back( anim );
        
        return anim;
    }
}

