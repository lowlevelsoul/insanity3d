//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "BuilderApp.h"

TOOL_APP(BuilderApp)

enum ARG {
    ARG_INPATH = 0,
    ARG_OUTPATH,
    ARG_PLATFORM,
    ARG_FOLDERS,
};

static const uint32_t VERSION_MAJOR = 0;
static const uint32_t VERSION_MINOR = 1;
static const uint32_t VERSION_PATCH = 0;

//======================================================================================================================
BuilderApp::BuilderApp() {
    PublishArgEnum( "inpath", ARG_INPATH );
    PublishArgEnum( "outpath", ARG_OUTPATH );
    PublishArgEnum( "platform", ARG_PLATFORM );
    PublishArgEnum( "folders", ARG_FOLDERS );
    
    m_dataFolderName    = "data";
    m_displayVersion    = false;
    m_filterFolders     = false;
    m_platform          = i3d::THIS_PLATFORM;
}

//======================================================================================================================
BuilderApp::~BuilderApp() {
    
}

//======================================================================================================================
bool BuilderApp::HandleArg( ToolArg& arg, uint32_t argEnum ) {
    
    switch( argEnum ) {
            
        case ARG_INPATH:
            m_inputPath = arg.m_params[0];
            break;
            
        case ARG_OUTPATH:
            m_outputPath = arg.m_params[0];
            break;
            
        case ARG_PLATFORM:
            m_platform = arg.m_params[0];
            break;
            
        case ARG_FOLDERS:
            for ( auto & p : arg.m_params ) {
                Builder::FOLDER folderId = GetFolderId( p );
                if ( folderId < Builder::FOLDER_COUNT ) {
                    m_filterFolders = true;
                    m_folders[ folderId ] = 1;
                }
            }
            break;
    }
    
    return true;
}

//======================================================================================================================
bool BuilderApp::Run() {
    
    if (m_displayVersion == true) {
        PrintVersionNumber( false );
    }
    
    
    // Display version number
    PrintVersionNumber( true );
    
    if (m_platform.empty() == true ) {
        XE_ERROR( true, "No platform specified. Please use builder --help for details\n");
    }
    
    // Build the output folder path
    m_outputBasePath = m_outputPath;
    
    if  ( m_platform.empty() == false ) {
        fs->AppendPath( m_outputBasePath, m_platform.c_str() );
    }
    
    fs->AppendPath( m_outputBasePath, m_dataFolderName.c_str() );
    
    XE_LOG("Builder output base path: %s\n", m_outputBasePath.c_str() );
    
    // Setup the folder filters if any were specified.
    if ( m_filterFolders == true ) {
        m_builder.SetFilterFolder( m_folders );
    }
    
    m_builder.Run( m_inputPath.c_str(), m_outputBasePath.c_str(), m_platform.c_str() );
    
    return true;
}

//======================================================================================================================
void BuilderApp::PrintVersionNumber( bool asHeader ) {
    if ( asHeader == true ) {
        XE_LOG( "Builder Version " );
    }
    
    XE_LOG( "%u.%u.%u\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH );
}

//======================================================================================================================
Builder::FOLDER BuilderApp::GetFolderId( const char * str ) {
    
    for ( uint32_t i = 0; i < Builder::FOLDER_COUNT; ++i ) {
        if ( strcasecmp( str, Builder::FOLDER_NAMES[ i ] ) == 0 ) {
            return Builder::FOLDER(i);
        }
    }
    
    return Builder::FOLDER_COUNT;
}
