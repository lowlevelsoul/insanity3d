//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __BUILDER_H__
#define __BUILDER_H__

#include "BuilderTool.h"

class Builder {
public:
    enum FOLDER {
        FOLDER_TEXTURES=0,
        FOLDER_MODELS,
        FOLDER_MATERIALS,
        FOLDER_PROTOTYPES,
        FOLDER_MISSIONS,
        FOLDER_COUNT,
    };
    
    struct MetaScanContext {
        Builder*            m_builder;
        i3d::stl::String::type         m_basePath;
        std::vector<BuilderTool::ref_ptr_t> * m_builders;
        i3d::stl::String::type         m_profile;
    };
    
    static const char * FOLDER_NAMES[ FOLDER_COUNT ];
    
    Builder();
    
    ~Builder();
    
    void Run( const char * inputPath, const char * outputPath, const char * profile );
    
    void ScanFiles( const char * path, const char * ext );
    
    void ScanMetaFilesForTool( FOLDER folder );
    
    static void MetaFileCallback( i3d::stl::String::type & path, void * context );
    
    i3d::stl::String::type GetToolPath( const char * toolExeName );
    
    void SetFilterFolder( const std::bitset<FOLDER_COUNT>& filter );
    
    i3d::PLATFORM_ID GetPlatform() const { return m_platform; }
    
protected:
    i3d::stl::String::type                      m_inputPath;
    i3d::stl::String::type                      m_outputPath;
    
    i3d::PLATFORM_ID                            m_platform;
    i3d::stl::String::type                      m_profile;
    i3d::stl::String::type                      m_builderExeFolder;
    std::vector<BuilderTool::ref_ptr_t>         m_builders[ FOLDER_COUNT ];
    std::bitset<FOLDER_COUNT>                   m_folders;
    bool                                        m_filterFolders;
};

#endif
