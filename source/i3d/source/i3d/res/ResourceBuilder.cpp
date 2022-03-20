//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/res/ResourceBuilder.h"

namespace i3d {

    //======================================================================================================================
    ResourceBuilder::ResourceBuilder() {
        
    }
    
    //======================================================================================================================
    ResourceBuilder::~ResourceBuilder() {
        
    }
    
    //======================================================================================================================
    void ResourceBuilder::Compile( Resource * resource ) {
        
    }

    //======================================================================================================================
    void ResourceBuilder::GetDependencies( Resource * resource,
                                           stl::Vector<stl::String::type>::type & inputs ) {
        
    }

    //======================================================================================================================
    stl::String::type ResourceBuilder::GetAssetCompilePathFromResource( const char * path ) {
        stl::String::type assetPath = path;
        
        if (assetPath[0] == FileSystem::MOUNT_DATA_PREFIX ) {
            assetPath[0] = FileSystem::MOUNT_ASSET_PREFIX;
        }
        
        assetPath.append( ".build" );
        
        return assetPath;
    }
    
    //======================================================================================================================
    bool ResourceBuilder::CreateFolderAtPath( const char * path ) {
        
        stl::String::type actualPath = path;
        stl::String::type filename;
        fs->ExtractFilename(filename, actualPath);
        
        bool createOk = fs->CreateFolder( actualPath.c_str(), true );
        return createOk == true;
    }
}
