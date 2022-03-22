//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_RESOURCECOMPILER_H__
#define __I3D_RESOURCECOMPILER_H__

#include "i3d/core/Types.h"

namespace i3d {
    
    class Resource;

    class ResourceBuilder : public RttiObject {
    public:
        ResourceBuilder();
        
        virtual ~ResourceBuilder();
        
        virtual void GetDependencies( Resource * resource,
                                      stl::Vector<stl::String::type>::type & inputs );
        
        virtual void Compile( const char * path );
        
        static stl::String::type GetAssetCompilePathFromResource( const char * path );
        
        static bool CreateFolderAtPath( const char * path );
    };
}

#endif
