//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_RESOURCECOMPILER_H__
#define __I3D_RESOURCECOMPILER_H__

#include "i3d/core/Types.h"

namespace i3d {
    
    class Resource;

    class ResourceCompiler : public RttiObject {
    public:
        ResourceCompiler();
        
        virtual ~ResourceCompiler();
        
        virtual void GetDependencies( Resource * resource,
                                      std::vector<stl::String> & inputs );
        
        virtual void Compile( Resource * resource );
        
        static stl::String GetAssetCompilePathFromResource( const char * path );
        
        static bool CreateFolderAtPath( const char * path );
    };
}

#endif
