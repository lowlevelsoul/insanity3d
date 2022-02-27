//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/res/ResourceCompiler.h"

namespace i3d {

    //======================================================================================================================
    ResourceCompiler::ResourceCompiler() {
        
    }
    
    //======================================================================================================================
    ResourceCompiler::~ResourceCompiler() {
        
    }
    
    //======================================================================================================================
    void ResourceCompiler::Compile( Resource * resource ) {
        
    }

    //======================================================================================================================
    void ResourceCompiler::GetDependencies( Resource * resource,
                                           i3d::Array<stl::String> & inputs ) {
        
    }

    //======================================================================================================================
    stl::String ResourceCompiler::GetAssetCompilePathFromResource( const char * path ) {
        stl::String assetPath = path;
        
        if (assetPath[0] == FileSystem::MOUNT_DATA_PREFIX ) {
            assetPath[0] = FileSystem::MOUNT_ASSET_PREFIX;
        }
        
        assetPath.Append( ".compile" );
        
        return assetPath;
    }
    
    //======================================================================================================================
    bool ResourceCompiler::CreateFolderAtPath( const char * path ) {
        
        stl::String actualPath = path;
        stl::String filename;
        fs->ExtractFilename(filename, actualPath);
        
        bool createOk = fs->CreateFolder( actualPath.c_str(), true );
        return createOk == true;
    }
}
