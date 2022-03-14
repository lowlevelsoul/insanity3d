//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//==========================================================================================================================================

#ifndef __MODELIMPORTER_H__
#define __MODELIMPORTER_H__

#include "toolmodel/ToolModel.h"

class ModelImporter  {
public:
    class Options {
    public:
        Options() {
            m_scale = 1;
            m_flipFaces = false;
        }

        ~Options() {

        }

    public:
        float		m_scale;
        bool		m_flipFaces;
        std::vector<std::string> m_meshFilter;
    };

    ModelImporter( Options & options );

    virtual ~ModelImporter();

    virtual ToolModel::ref_ptr_t Load( const char * path );

protected:
    Options			m_options;
};

#endif
