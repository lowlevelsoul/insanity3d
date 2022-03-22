//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_FILESYSTEM_H__
#define __I3D_FILESYSTEM_H__

namespace i3d {
    
    typedef void (*PathEnumeratorFunc)( stl::String::type & path, void * context );
    
    class File {
    protected:
        virtual ~File() {
        }
    public:
        
        virtual void       Close() = 0;
        virtual size_t     Read( void* buffer, size_t size, size_t count ) = 0;
        virtual size_t     Write( const void* buffer, size_t size, size_t count ) = 0;
        virtual uintptr_t  Pos() = 0;
        virtual size_t     Length() = 0;
        virtual void       Seek( uintptr_t pos ) = 0;
        
        template<typename __type__>
        size_t Write( const __type__ * items, size_t itemCount = 1 ) {
            size_t writeAmt = Write((const void*) items, sizeof(__type__), itemCount);
            return writeAmt;
        }
        
        bool WriteAlignPadding(size_t alignment) {
            uint8_t byte = 0;
            uintptr_t amt = Pos() % alignment;
            if (amt != 0) {
                amt = alignment - amt;
                for(uintptr_t i=0; i < amt; ++i) {
                    Write<uint8_t>(&byte);
                }
            }
            
            return true;
        }
    };
    
    class FileSystem {
    public:
        enum class MOUNT {
             DATA = 0,
             ASSETS,
             COUNT,
         };
        
        virtual ~FileSystem() {}
        
        static const char MOUNT_DATA_PREFIX = '~';
        static const char MOUNT_ASSET_PREFIX = '@';
        
        virtual bool HasExtension ( const stl::String::type& path ) = 0;
        virtual bool HasExtensionType ( const stl::String::type& path, const char* ext ) = 0;
        virtual bool RemoveExtension ( stl::String::type& path ) = 0;
        virtual bool GetFilename ( stl::String::type& filename, const stl::String::type& path ) = 0;
        virtual bool ExtractFilename ( stl::String::type& filename, stl::String::type& path ) = 0;
        virtual bool AppendExtension ( stl::String::type& path, const char* ext) = 0;
        virtual void StripExtension ( stl::String::type& path ) = 0;
        virtual bool AppendPath ( stl::String::type& lhs, const char* rhs ) = 0;
        virtual bool JoinPath ( stl::String::type& path, const char* lhs, const char* rhs ) = 0;
        virtual void MakePathCononical ( stl::String::type& path ) = 0;
        virtual bool GetExtension ( stl::String::type& ext, const stl::String::type& path ) = 0;
        virtual bool MakeFullPath ( stl::String::type& pathOut, const char* pathIn) = 0;
        virtual bool GetCurrentDirectory ( stl::String::type& pathOut ) = 0;
        virtual bool DoesFileExist ( const char* path ) = 0;
        virtual bool DoesFolderExist ( const char* path ) = 0;
        virtual bool CreateFolder ( const char* path, bool createIntermediate ) = 0;
        virtual char GetPlatformSeparator () = 0;
        virtual char GetOtherSeparator () = 0;
        virtual bool EnumeratePathConents ( const char * path, PathEnumeratorFunc enumFunc, void * context, const char * ext ) = 0;
        virtual bool GetApplicationPath ( stl::String::type& pathOut ) = 0;
        virtual bool GetModifiedTimestamp ( uint64_t& timeStamp, const char* filePath ) = 0;
        
        virtual void SetDataMountFolder( const char * path ) = 0;
        virtual void SetAssetMountFolder( const char * path ) = 0 ;
        
        virtual File * FileOpen ( const char * path, const char * mode ) = 0;
        virtual void FileClose ( File * file ) = 0;
    };
}
 
extern i3d::FileSystem * fs;

namespace i3d {
    
    class ScopedFile {
    public:
        ScopedFile( const char * path, const char * mode ) {
            m_file = fs->FileOpen( path, mode);
        }
        
        ~ScopedFile() {
            if ( m_file != nullptr ) {
                m_file->Close();
            }
        }
        
        bool IsValid() const { return m_file != nullptr; }
        
        operator const File *() const { return m_file; }
        
        operator File *() { return m_file; }
        
        File * operator->() { return m_file; }
        
        const File * operator->() const { return m_file; }
        
    protected:
        File *        m_file;
    };
}

#endif
