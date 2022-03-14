//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "Builder.h"
#include "toolapp/RttiSerializerXml.h"

const char * Builder::FOLDER_NAMES[ Builder::FOLDER_COUNT ] = {
    "textures",
    "models"
};

//======================================================================================================================
void Builder::MetaFileCallback( std::string & path, void * context ) {
    
    MetaScanContext * scanContext = (MetaScanContext *) context;
    
    std::string srcPath = scanContext->m_basePath;
    fs->AppendPath( srcPath, path.c_str() );
    
    RttiSerializerXml ser;
    
    RttiObject::ref_ptr_t firstObj;
    bool ok = ser.Read( firstObj, srcPath.c_str(), scanContext->m_profile.c_str() );
    if ( ok == false ) {
        return;
    }
    
    uint32_t toolCount = 0;
    
    for ( RttiObject * obj : ser.GetObjects() ) {
        
        BuilderTool * tool = obj->SafeCast<BuilderTool>();
        if (tool != nullptr ) {
            
            tool->m_builder = scanContext->m_builder;
            tool->m_metaPath = path;
            
            scanContext->m_builders->push_back( tool );
            
            ++toolCount;
        }
    }
    
    ERROR( toolCount == 0, "Not a valid builder meta file. Contains no objects derived from BuilderTool.\n");
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
    
    // Store the folder containing the builder exe
    fs->GetApplicationPath(m_builderExeFolder);
    std::string temp;
    fs->ExtractFilename(temp, m_builderExeFolder);
    
    for ( uint32_t f = 0; f < FOLDER_COUNT; ++f ) {
        
        bool processFolder = true;
        if ( m_filterFolders == true ) {
            if ( m_folders[f] == 0) {
                processFolder = false;
            }
        }
        
        if (processFolder == true) {
        
            LOG("Scanning %s for meta files...", FOLDER_NAMES[f]);
            ScanMetaFilesForTool( FOLDER( f ) );
            LOG("Done.\n");
            LOG("Found %u meta files for folder %s.\n", m_builders[f].size(), FOLDER_NAMES[f] );
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
        
            std::string inputPath = m_inputPath;
            fs->AppendPath( inputPath, FOLDER_NAMES[f] );
            
            std::string outputPath = m_outputPath;
            fs->AppendPath( outputPath, FOLDER_NAMES[f] );
            
            for ( BuilderTool * t : m_builders[f] ) {
                
                ++totalCount;
                
                t->m_inputPath = inputPath;
                t->m_outputPath = outputPath;
                
                if ( t->CheckStale() == true ) {
                    
                    ++staleCount;
                    LOG(" Output for %s is stale. Building.\n", t->MakeInputFilePath( t->m_metaPath.c_str() ).c_str() );
                
                    t->Run();
                }
            }
        }
    }
    
    if ( staleCount > 0 ) {
        LOG( "Found %llu %s out of %llu total.\n", staleCount, (staleCount==1) ? "file" : "files", totalCount);
    }
    else {
        LOG( "No stale files found\n" );
    }
}

//======================================================================================================================
void Builder::ScanFiles( const char * path, const char * ext ) {
    
}

//======================================================================================================================
void Builder::ScanMetaFilesForTool( FOLDER folder ) {
    std::string fullPath = m_inputPath;
    fs->AppendPath( fullPath, FOLDER_NAMES[folder] );
    
    MetaScanContext ctxt;
    ctxt.m_basePath = fullPath;
    ctxt.m_builders = &m_builders[ folder ];
    ctxt.m_profile = m_profile;
    ctxt.m_builder = this;
    
    fs->EnumeratePathConents( fullPath.c_str(), MetaFileCallback, &ctxt, "xml");
}

//======================================================================================================================
std::string Builder::GetToolPath( const char * toolExeName ) {
    std::string path = m_builderExeFolder;
    fs->AppendPath( path, toolExeName );
    
    return path;
}

//======================================================================================================================
void Builder::SetFilterFolder( const std::bitset<FOLDER_COUNT>& filter ) {
    m_folders = filter;
    m_filterFolders = true;
}

