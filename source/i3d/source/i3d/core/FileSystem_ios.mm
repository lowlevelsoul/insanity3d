//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include <stdlib.h>
#include <stdio.h>
#include <new>
#include <mutex>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>

#include "i3d/core/FileSystem.h"
#include "i3d/core/Log.h"
#include "i3d/core/FileSystem_common.h"

#import <foundation/Foundation.h>

i3d::FileSystem * fs = nullptr;

namespace i3d {

    /// Internal file class
    class FileStd : public File
    {
    public:
        FileStd() :
            m_file(nullptr)
        {}
        
        virtual ~FileStd() {
            Close();
        }
        
        /// std FILE instance
        FILE*           m_file;
        
        //=========================================================================================================================================
        void Close() {
            if (m_file != nullptr) {
                fclose(m_file);
                m_file = nullptr;
            }
        }
        
        //=========================================================================================================================================
        size_t Read(void* buffer, size_t size, size_t count) {
            assert(buffer != NULL);
            assert(m_file != NULL);
            return fread(buffer, size, count, m_file);
        }
        
        //=========================================================================================================================================
        size_t Write(const void* buffer, size_t size, size_t count) {
            assert(buffer != NULL);
            assert(m_file != NULL);
            return fwrite(buffer, size, count, m_file);
        }
        
        //=========================================================================================================================================
        uintptr_t Pos() {
            assert(m_file != NULL);
            return ftell(m_file);
        }
        
        //=========================================================================================================================================
        size_t Length() {
            assert(m_file != NULL);
            struct stat buf;
            fstat(fileno(m_file), &buf);
            off_t size = buf.st_size;
            
            return size;
        }
        
        //=========================================================================================================================================
        void Seek(uintptr_t pos) {
            assert(m_file != NULL);
            fseek(m_file, pos, SEEK_SET);
        }
    };
    
    class FileSystemIos : public FileSystemCommon {
    public:
        FileSystemIos();
        
        virtual ~FileSystemIos();
        
        bool MakeFullPath( stl::String::type & pathOut, const char* pathIn);
        bool GetCurrentDirectory( stl::String::type & pathOut );
        char GetPlatformSeparator();
        char GetOtherSeparator();
        bool DoesFileExist( const char* path);
        bool DoesFolderExist( const char* path);
        bool CreateFolder( const char* path, bool createIntermediate );
        bool EnumeratePathConents( const char * path, PathEnumeratorFunc enumFunc, void * context, const char * ext );
        bool GetApplicationPath( stl::String::type & pathOut );
        bool GetModifiedTimestamp(uint64_t& timeStamp, const char* filePath);
        virtual void SetDataMountFolder( const char * path );
        virtual void SetAssetMountFolder( const char * path );
        
        File * FileOpen( const char * path, const char * mode );
        void FileClose( File * file );
        
        
        void Lock() { m_mutex.lock(); }
        void Unlock() { m_mutex.unlock(); }
        
        FileStd * AllocFile() {
            return new FileStd;
        }
        
        void FreeFile( FileStd * file ) {
            delete ((FileStd*)file);
        }
        
    public:
        stl::String::type       m_mountData;
        stl::String::type       m_pathBuff;
        std::mutex              m_mutex;
    };

    //=========================================================================================================================================
    void FsCreate() {
        if ( fs != nullptr ) {
            return;
        }
        
        fs = new FileSystemIos;
    }

    //=========================================================================================================================================
    void FsDestroy() {
        SAFE_DELETE( fs );
    }

    //=========================================================================================================================================
    FileSystemIos::FileSystemIos() {
        
        /* Setup the data directory */
        NSBundle* mainBundle = [NSBundle mainBundle];
        NSString* bundlePath = [ mainBundle resourcePath];
        
        const char* pathUtf8 = [ bundlePath UTF8String];
        m_mountData = pathUtf8;
        AppendPath( m_mountData, "/data");
        
        XE_LOG("    Data folder path is %s\n", m_mountData.c_str() );
    }
        
    //=========================================================================================================================================
    FileSystemIos::~FileSystemIos() {
    }
    
    //=========================================================================================================================================
    void FileSystemIos::SetDataMountFolder( const char * path ) {
        
    }
    
    //=========================================================================================================================================
    void FileSystemIos::SetAssetMountFolder( const char * path ) {
        
    }
    
    //=========================================================================================================================================
    File* FileSystemIos::FileOpen( const char*  path, const char * mode ) {
        assert(path != NULL);
        assert(mode != NULL);
        
        /* Construct the full path */
        stl::String::type fullPath;
        MakeFullPath( fullPath, path );
        
        const char* fullPathCStr = fullPath.c_str();
        
        /* Try to open the file */
        FILE* stdFile = NULL;
        stdFile = fopen(fullPathCStr, mode);
        if (stdFile == nullptr) {
            return nullptr;
        }
        
        FileStd* file = AllocFile();
        file->m_file = stdFile;
        
        return file;
    }


    //=========================================================================================================================================
    void FileSystemIos::FileClose( File * file ) {
        fclose( ((FileStd*)file)->m_file );
        FreeFile( ((FileStd*)file) );
    }

    //=========================================================================================================================================
    bool FileSystemIos::GetCurrentDirectory( stl::String::type & pathOut ) {
        NSFileManager * fileMgr = [NSFileManager defaultManager];
        NSString* currDirPath = [fileMgr currentDirectoryPath];
        
        if (currDirPath != nil) {
            const char* pathStrUtf8 = [currDirPath UTF8String];
            pathOut = pathStrUtf8;
            return true;
        }
        
        return false;
    }

    //=========================================================================================================================================
    bool FileSystemIos::MakeFullPath( stl::String::type & pathOut, const char* pathIn)
    {
        switch( pathIn[0] ) {
                // pathIn has '~' prefix, so append to data path
            case MOUNT_DATA_PREFIX : {
                std::lock_guard<std::mutex> lock( m_mutex );
                JoinPath( pathOut, m_mountData.c_str(), pathIn+1);
                break;
            }
                
                // No prefix, so just copy
            default : {
                stl::String::type emptyStr = "";
                pathOut = pathIn;
            }
        }
        
        return true;
    }

    //=========================================================================================================================================
    char FileSystemIos::GetPlatformSeparator() {
        return '/';
    }

    //=========================================================================================================================================
    char FileSystemIos::GetOtherSeparator() {
        return '\\';
    }

    //=========================================================================================================================================
    bool FileSystemIos::DoesFileExist( const char* path) {
        
        stl::String::type fullPath;
        MakeFullPath(fullPath, path);
        
        NSFileManager* fileMgr = [NSFileManager defaultManager];
        NSString *pathNs = [NSString stringWithUTF8String:fullPath.c_str()];
        
        BOOL isFolder;
        BOOL res = [fileMgr fileExistsAtPath:pathNs isDirectory:&isFolder];
        return (res == YES) && (isFolder == NO);
    }

    //=========================================================================================================================================
    bool FileSystemIos::DoesFolderExist( const char* path) {
        
        stl::String::type fullPath;
        MakeFullPath(fullPath, path);
        
        NSFileManager* fileMgr = [NSFileManager defaultManager];
        NSString *pathNs = [NSString stringWithUTF8String:fullPath.c_str()];
        
        BOOL isFolder;
        BOOL res = [fileMgr fileExistsAtPath:pathNs isDirectory:&isFolder];
        return (res == YES) && (isFolder == YES);
    }

    //=========================================================================================================================================
    bool FileSystemIos::CreateFolder( const char* path, bool createIntermediate ) {
        
        stl::String::type fullPath;
        MakeFullPath(fullPath, path);
        
        NSFileManager* fileMgr = [NSFileManager defaultManager];
        NSString *pathNs = [NSString stringWithUTF8String:fullPath.c_str()];
        
        BOOL res = [fileMgr createDirectoryAtPath:pathNs withIntermediateDirectories:createIntermediate==true attributes:nil error:nil];
        return (res == YES);
    }
    
    //=========================================================================================================================================
    bool FileSystemIos::EnumeratePathConents( const char * path, PathEnumeratorFunc enumFunc, void * context, const char * ext ) {
        NSString * extStr = nil;
        if (ext != nullptr) {
            extStr = [[NSString alloc] initWithUTF8String: ext];
        }
        
        NSString* folder = [[NSString alloc] initWithUTF8String: path];
        NSDirectoryEnumerator *dirEnum = [[NSFileManager defaultManager] enumeratorAtPath:folder];
        
        NSString *file;
        do {
            
            file = [dirEnum nextObject];
            if (file) {
                bool processFile = false;
                if ( extStr != nil ) {
                    if ( [[file pathExtension] isEqualToString: extStr] ) {
                        processFile = true;
                    }
                }
                else {
                    processFile = true;
                }
                
                if (processFile == true) {
                    stl::String::type thisFileStr = [file UTF8String];
                    enumFunc( thisFileStr, context);
                }
            }
            
        } while (file != nil);
        
        return true;
    }
    
    //======================================================================================================================
    bool FileSystemIos::GetApplicationPath( stl::String::type & pathOut ) {
        NSString* pathNs = [[NSBundle mainBundle] executablePath];
        if (pathNs != nil) {
            const char* pathStrUtf8 = [pathNs UTF8String];
            pathOut = pathStrUtf8;
            return true;
        }
        return false;
    }
    
    //======================================================================================================================
    bool FileSystemIos::GetModifiedTimestamp(uint64_t& timeStamp, const char* filePath) {
        // Get the full path of the file
        stl::String::type fullPath;
        MakeFullPath(fullPath, filePath);
        
        // Get the last modified attribute of the file
        NSFileManager* fileMgr = [NSFileManager defaultManager];
        NSString *pathNs = [NSString stringWithUTF8String:fullPath.c_str()];
        
        NSError * error = nil;
        NSDictionary * attrs = [fileMgr attributesOfItemAtPath:pathNs error:&error];
        
        if (attrs && !error)
        {
            // We got the arribs, so query the modified date and convert into microseconds since 1970
            // to give us a single timestamp
            NSDate * fileLastModifiedDate = nil;
            fileLastModifiedDate = [attrs fileModificationDate];
            
            NSTimeInterval interval = [fileLastModifiedDate timeIntervalSince1970];
            uint64_t time64 = (uint64_t) (interval * 1000.0);
            timeStamp = time64;
            return true;
        }
        
        timeStamp = 0;
        return false;
    }
}
