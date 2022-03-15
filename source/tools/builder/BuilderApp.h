//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __BUILDERAPP_H__
#define __BUILDERAPP_H__

#include "Builder.h"

class BuilderApp : public ToolApp {
public:
    BuilderApp();
    
    virtual ~BuilderApp();
    
    virtual bool Run();
    
    virtual bool HandleArg( ToolArg& arg, uint32_t argEnum );
    
    void PrintVersionNumber( bool asHeader = true );
    
    Builder::FOLDER GetFolderId( const char * str );
    
protected:
    i3d::stl::String::type         m_inputPath;                ///< Where does builder get the files from?
    i3d::stl::String::type         m_outputPath;               ///< The base path for where builder will write files to
    i3d::stl::String::type         m_platform;                 ///< The platform outoput folder (appended to output path)
    i3d::stl::String::type         m_outputBasePath;           ///< Full output bath
    i3d::stl::String::type         m_dataFolderName;           ///< Name of the data folder
    bool                m_filterFolders;
    std::bitset<Builder::FOLDER_COUNT> m_folders;
    
    bool                m_displayVersion;
    
    Builder             m_builder;                  ///< 
};

#endif
