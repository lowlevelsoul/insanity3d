//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//=======================================================================================================================

#ifndef __I3D_FILESYSTEM_COMMON_H__
#define __I3D_FILESYSTEM_COMMON_H__

namespace i3d {
    
    class FileSystemCommon : public FileSystem {
    public:
        virtual ~FileSystemCommon() {}
        
        virtual bool GetFilename( stl::String::type& filename, const stl::String::type& path ) override;
        virtual bool ExtractFilename( stl::String::type& filename, stl::String::type& path ) override;
        virtual bool HasExtension( const stl::String::type& path ) override;
        virtual bool GetExtension( stl::String::type& ext, const stl::String::type& path ) override;
        virtual bool RemoveExtension( stl::String::type& path ) override;
        virtual bool HasExtensionType( const stl::String::type& path, const char* ext ) override;
        virtual void StripExtension( stl::String::type& path ) override;
        virtual bool AppendExtension( stl::String::type& path, const char* ext ) override;
        virtual bool AppendPath( stl::String::type& lhs, const char* rhs ) override;
        virtual bool JoinPath( stl::String::type& path, const char* lhs, const char* rhs ) override;
        virtual void MakePathCononical( stl::String::type& path ) override;
    };
}

#endif
