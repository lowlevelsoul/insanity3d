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
        FOLDER_COUNT,
    };
    
    struct MetaScanContext {
        Builder*            m_builder;
        std::string         m_basePath;
        std::vector<BuilderTool::ref_ptr_t> * m_builders;
        std::string         m_profile;
    };
    
    static const char * FOLDER_NAMES[ FOLDER_COUNT ];
    
    Builder();
    
    ~Builder();
    
    void Run( const char * inputPath, const char * outputPath, const char * profile );
    
    void ScanFiles( const char * path, const char * ext );
    
    void ScanMetaFilesForTool( FOLDER folder );
    
    static void MetaFileCallback( std::string & path, void * context );
    
    std::string GetToolPath( const char * toolExeName );
    
    void SetFilterFolder( const std::bitset<FOLDER_COUNT>& filter );
    
protected:
    std::string                                 m_inputPath;
    std::string                                 m_outputPath;
    
    std::string                                 m_profile;
    std::string                                 m_builderExeFolder;
    std::vector<BuilderTool::ref_ptr_t>         m_builders[FOLDER_COUNT];
    std::bitset<FOLDER_COUNT>                   m_folders;
    bool                                        m_filterFolders;
};

#endif
