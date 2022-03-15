//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "Builder.h"
#include "i3d/rtti/RttiLoader.h"

const char * Builder::FOLDER_NAMES[ Builder::FOLDER_COUNT ] = {
    "textures",
    "models"
};

//======================================================================================================================
void Builder::MetaFileCallback( i3d::stl::String::type & path, void * context ) {
    
    MetaScanContext * scanContext = (MetaScanContext *) context;
    
    i3d::stl::String::type srcPath = scanContext->m_basePath;
    fs->AppendPath( srcPath, path.c_str() );
    
    i3d::RttiLoader loader;
    loader.Load( srcPath.c_str(), scanContext->m_builder->GetPlatform() );
    
    if ( loader.m_object != nullptr ) {
        BuilderTool * tool = loader.m_object->SafeCast<BuilderTool>();
        tool->m_builder = scanContext->m_builder;
        tool->m_metaPath = path;
        
        scanContext->m_builders->push_back( tool );
    }
    else {
        XE_ERROR( false, "Not a valid builder meta file. Contains no objects derived from BuilderTool.\n");
    }
}

//======================================================================================================================
Builder::Builder() {
    m_filterFolders = false;
}

//======================================================================================================================
Builder::~Builder() {
    
}

//======================================================================================================================
void Builder::Run( const char * inputPath, const char * outputPath, const char * profile ) {
    m_inputPath = inputPath;
    m_outputPath = outputPath;
    m_profile = profile;
    m_platform = i3d::GetPlatformIdFromString( profile );
    
    // Store the folder containing the builder exe
    fs->GetApplicationPath(m_builderExeFolder);
    i3d::stl::String::type temp;
    fs->ExtractFilename(temp, m_builderExeFolder);
    
    for ( uint32_t f = 0; f < FOLDER_COUNT; ++f ) {
        
        bool processFolder = true;
        if ( m_filterFolders == true ) {
            if ( m_folders[f] == 0) {
                processFolder = false;
            }
        }
        
        if (processFolder == true) {
        
            XE_LOG("Scanning %s for meta files...", FOLDER_NAMES[f]);
            ScanMetaFilesForTool( FOLDER( f ) );
            XE_LOG("Done.\n");
            XE_LOG("Found %u meta files for folder %s.\n", m_builders[f].size(), FOLDER_NAMES[f] );
        }
    }
    
    uintptr_t staleCount = 0;
    uint32_t totalCount = 0;
    
    
    for ( uint32_t f = 0; f < FOLDER_COUNT; ++f ) {
        
        bool processFolder = true;
        if ( m_filterFolders == true ) {
            if ( m_folders[f] == 0) {
                processFolder = false;
            }
        }
        
        if (processFolder == true) {
        
            i3d::stl::String::type inputPath = m_inputPath;
            fs->AppendPath( inputPath, FOLDER_NAMES[f] );
            
            i3d::stl::String::type outputPath = m_outputPath;
            fs->AppendPath( outputPath, FOLDER_NAMES[f] );
            
            for ( BuilderTool * t : m_builders[f] ) {
                
                ++totalCount;
                
                t->m_inputPath = inputPath;
                t->m_outputPath = outputPath;
                
                if ( t->CheckStale() == true ) {
                    
                    ++staleCount;
                    XE_LOG(" Output for %s is stale. Building.\n", t->MakeInputFilePath( t->m_metaPath.c_str() ).c_str() );
                
                    t->Run();
                }
            }
        }
    }
    
    if ( staleCount > 0 ) {
        XE_LOG( "Found %llu %s out of %llu total.\n", staleCount, (staleCount==1) ? "file" : "files", totalCount);
    }
    else {
        XE_LOG( "No stale files found\n" );
    }
}

//======================================================================================================================
void Builder::ScanFiles( const char * path, const char * ext ) {
    
}

//======================================================================================================================
void Builder::ScanMetaFilesForTool( FOLDER folder ) {
    i3d::stl::String::type fullPath = m_inputPath;
    fs->AppendPath( fullPath, FOLDER_NAMES[folder] );
    
    MetaScanContext ctxt;
    ctxt.m_basePath = fullPath;
    ctxt.m_builders = &m_builders[ folder ];
    ctxt.m_profile = m_profile;
    ctxt.m_builder = this;
    
    fs->EnumeratePathConents( fullPath.c_str(), MetaFileCallback, &ctxt, "build");
}

//======================================================================================================================
i3d::stl::String::type Builder::GetToolPath( const char * toolExeName ) {
    i3d::stl::String::type path = m_builderExeFolder;
    fs->AppendPath( path, toolExeName );
    
    return path;
}

//======================================================================================================================
void Builder::SetFilterFolder( const std::bitset<FOLDER_COUNT>& filter ) {
    m_folders = filter;
    m_filterFolders = true;
}

