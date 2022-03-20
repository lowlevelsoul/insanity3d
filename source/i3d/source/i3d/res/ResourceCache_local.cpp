//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/core/FileSystem.h"
#include "i3d/core/Hash.h"
#include "i3d/core/Sys.h"
#include "i3d/res/ResourceCache_local.h"
#include "i3d/res/ResourceBuilder.h"
#include "i3d/rtti/RttiLoader.h"
#include <thread>

#define RESOURCE_COMPILE_STALE

i3d::ResourceCache * res = nullptr;
i3d::ResourceCacheLocal * resLocal = nullptr;

namespace i3d {
    
    CVar res_buildPlatform( "res_buildPlatform", "macos", "Platform used for building resources" );

    //======================================================================================================================
    void ResCreate() {
        if ( resLocal != nullptr ) {
            return;
        }
        
        resLocal = new i3d::ResourceCacheLocal;
        res = resLocal;
        
        resLocal->Initialise();
    }
    
    //======================================================================================================================
    void ResDestroy() {
        SAFE_DELETE( resLocal );
        res = nullptr;
    }
    
    //======================================================================================================================
    int ResourceCacheLocal::LoadThreadFunc( ) {
        
        XE_LOG("RESOURCE LOAD THEAD BEGIN \n");
    
        bool hasResource = true;
        do {
            hasResource = ::resLocal->PopPending();
            
            if ( hasResource == true ) {            
                Resource * res = resLocal->m_loading;

    #ifdef RESOURCE_COMPILE_STALE
        
                // Only attempt to check if the resource requires compilation if there
                // is a valid compile script associated with it. Otherwise, we'll just assume
                // that the resource exists and has just been magically placed in the data folder
                bool hasBuilder = resLocal->HasBuildSript( res );
                if ( hasBuilder == true ) {
                
                    ResourceBuilder * builder = nullptr;
                    bool isStale = resLocal->ResourceNeedsBuild( res, builder );
                    if ( isStale == true ) {
                        XE_LOG( "Resource %s is stale. Compiling.\n ", res->GetPath() );
                        
                        if ( builder == nullptr ) {
                            // If the target file does not exist, we most likely never loaded the compile
                            // script to create the builder. So make sure we load it now!
                            builder = resLocal->LoadBuildScript( res );
                        }

                        builder->Compile( res );
                    }
                    
                    SAFE_DELETE( builder );
                }
    #endif
                
                ScopedFile file( res->GetPath(), "rb" );
                XE_ERROR( file.IsValid() == false, "Could not open resource file %s for loading\n", res->GetPath() );
                
                res->Load( file, res->GetPath() );
            }
            
        } while ( hasResource == true );

        XE_LOG("RESOURCE LOAD THEAD END \n");
        return 0;
    }
    
    //======================================================================================================================
    ResourceCacheLocal::ResourceCacheLocal() {
        m_loading = nullptr;
        m_pending.reserve(2048);
        m_compileTargetPlatform = i3d::THIS_PLATFORM;
    }

    //======================================================================================================================
    ResourceCacheLocal::~ResourceCacheLocal() {
    }
    
    //======================================================================================================================
    void ResourceCacheLocal::Initialise() {

    }
        
    //======================================================================================================================
    void ResourceCacheLocal::PublishFactory( ResourceFactory * factory ) {
        //LOG( "Publishing resource factory '%s'\n", type->GetName() );
            
        const char * extensions = factory->GetExtensions();

        static char extListBuff[ 256 ];
        size_t extListSize = strlen( extensions );
        XE_ERROR( extListSize >= 256, "ResourceCacheLocal::PublishFactory - factory extension string size is too large" );
        XE_ERROR( extensions[extListSize-1] != ';', "ResourceCacheLocal::PublishFactory - factory extension string does not have a terminating ';' " );

        // Make a copy of the extension string from the factory and replace any
        // seperators with a zero char
        strcpy(extListBuff, extensions);
        for ( char * e = extListBuff; *e != 0; ++e) {
            if (*e == ';') {
                *e = '\0';
            }
        }

        // Register the resource factory for each of the extensions that it supports
        const char* currExt = extListBuff;
            
        while(true) {
                
            size_t len = strlen(currExt);
                
            if (len == 0) {
                break;
            }
            else {
                uint64_t hash = fh64::CalcFromString(currExt);
                m_factoryTable[hash] = factory;
                    
                currExt += (len + 1);
            }
        }
            
        factory->OnPublish();
    }
    
    //======================================================================================================================
    Resource * ResourceCacheLocal::Load( const char * path ) {
        
        // Check to see if the resource already exists or not. If it does, we'll just return with that
        // resource
        uint64_t nameHash = CalcPathHash( path );
        Resource * res = Find( nameHash );
        if ( res != nullptr ) {
            XE_LOG( "Resource %s already loaded. Using loaded resource.\n", path );
            return res;
        }
        
        XE_LOG( "Loading Resource %s\n", path );
            
        // Resource does not exist, so create it.
        res = CreateResource( path );
        XE_ERROR( res == nullptr, "Could not create resource for %s\n", path );
        
        res->m_path = path;
        
        
        // Add the resource to the internal list
        std::scoped_lock<std::mutex> lock( m_mutex );
        
        m_resourceTable[ nameHash ] = res;      // General book-keeping so we can track and find the resouece
        m_pending.push_back( res );              // Array of resources used to track which resources need to be loaded
        
        return res;
    }

    //======================================================================================================================
    Resource * ResourceCacheLocal::LoadImmediate( const char * path ) {
            
        uint64_t nameHash = CalcPathHash( path );
        Resource * res = Find( nameHash );
        if ( res != nullptr ) {
            XE_LOG( "Resource %s already loaded. Using loaded resource.\n", path );
            return res;
        }
        XE_LOG( "Loading Resource %s\n", path );
            
        res = CreateResource( path );
        XE_ERROR( res == nullptr, "Could not create resource for %s\n", path );
        
        res->m_path = path;
        
    #ifdef RESOURCE_COMPILE_STALE
        
        // Only attempt to check if the resource requires compilation if there
        // is a valid compile script associated with it. Otherwise, we'll just assume
        // that the resource exists and has just been magically placed in the data folder
        bool hasBuilder = HasBuildSript( res );
        if ( hasBuilder == true ) {
        
            ResourceBuilder * builder = nullptr;
            bool isStale = ResourceNeedsBuild( res, builder );
            if ( isStale == true ) {
                XE_LOG( "Resource %s is stale. Compiling.\n ", path );
                
                if ( builder == nullptr ) {
                    // If the target file does not exist, we most likely never loaded the compile
                    // script to create the builder. So make sure we load it now!
                    builder = LoadBuildScript( res );
                }

                builder->Compile( res );
            }
            
            SAFE_DELETE( builder );
    }
    #endif
        
        ScopedFile file( path, "rb" );
        XE_ERROR( file.IsValid() == false, "Could not open file for resource %s\n", path);
            
        res->Load( file, path );
            
        m_resourceTable[ nameHash ] = res;
            
        return res;
    }

    //======================================================================================================================
    void ResourceCacheLocal::Unload( Resource * res ) {

    }
    
    //======================================================================================================================
    bool ResourceCacheLocal::HasPending() {
        bool locked = m_mutex.try_lock();
        if ( locked == false ) {
            return true;
        }
        
        bool isPendingEmpty = m_pending.empty();
        Resource * currLoading = m_loading;
        
        m_mutex.unlock();
        
        return (isPendingEmpty == false || currLoading != nullptr );
    }
    
    //======================================================================================================================
    void ResourceCacheLocal::StartLoading() {
        std::thread loadThread( LoadThreadFunc );
        loadThread.detach();
    }

    //======================================================================================================================
    Resource * ResourceCacheLocal::Find( uint64_t hash ) {
        
        std::scoped_lock<std::mutex> lock( m_mutex );
            
        resource_table_t::iterator findIt = m_resourceTable.find( hash );
        if ( findIt == m_resourceTable.end() ) {
            return nullptr;
        }
            
        return findIt->second;
    }

    //======================================================================================================================
    Resource * ResourceCacheLocal::Find( const char* path ) {
        uint64_t hash = CalcPathHash( path );
        return Find(hash);
    }
        
    //======================================================================================================================
    void ResourceCacheLocal::MakePathCanonoical( stl::String::type & pathOut, const char * path ) {
        const char * actualPath = path;
        if (*actualPath == '~' ) {
            ++actualPath;
            if ( *actualPath == '/' || *actualPath == '\\' ) {
                ++actualPath;
            }
        }
            
        pathOut = actualPath;
        std::replace( pathOut.begin(), pathOut.end(), '\\', '/' );
        std::transform( pathOut.begin(), pathOut.end(), pathOut.begin(), std::tolower);
    }

    //======================================================================================================================
    uint64_t ResourceCacheLocal::CalcPathHash( const char * path ) {
        stl::String::type canPath;
        MakePathCanonoical( canPath,  path );
        uint64_t hash = fh64::CalcFromString( canPath.c_str() );
        return hash;
    }
        
    //======================================================================================================================
    Resource * ResourceCacheLocal::CreateResource( const char * path ) {
        
        std::scoped_lock<std::mutex> lock( m_mutex );
            
        // Get the extension from the path
        stl::String::type ext;
        bool gotExt = fs->GetExtension( ext, path );
        if (gotExt == false) {
            return nullptr;
        }
            
        uint64_t hash = fh64::CalcFromString( ext.c_str() );
            
        factory_table_t::iterator findIt = m_factoryTable.find( hash );
        if ( findIt == m_factoryTable.end() ) {
            return nullptr;
        }
            
        Resource * res = findIt->second->Create();
        return res;
    }
    
    //======================================================================================================================
    bool ResourceCacheLocal::HasBuildSript( Resource * res ) {
        stl::String::type resCompileScriptPath = ResourceBuilder::GetAssetCompilePathFromResource( res->m_path.c_str() );
        return fs->DoesFileExist( resCompileScriptPath.c_str() );
    }
    
    //======================================================================================================================
    ResourceBuilder * ResourceCacheLocal::LoadBuildScript( Resource * res ) {
        stl::String::type resCompileScriptPath = ResourceBuilder::GetAssetCompilePathFromResource( res->m_path.c_str() );
        XE_ERROR( fs->DoesFileExist( resCompileScriptPath.c_str() ) == false, "Could not find resource compile script at %s", resCompileScriptPath.c_str());
        
        i3d::PLATFORM_ID buildPlatformId = i3d::GetPlatformIdFromString( res_buildPlatform.GetString() );
        XE_ERROR( buildPlatformId == i3d::PLATFORM_ID_NONE, "Invalid resource build platform %s\n", res_buildPlatform.GetString() );
        
        RttiLoader loader;
        loader.Load( resCompileScriptPath.c_str(), buildPlatformId );
        
        XE_ERROR( loader.m_object == nullptr, "Uncaught error whilst loading compile script. No compile objects found\n");
        XE_ERROR( loader.m_object->IsOfType<ResourceBuilder>() == false, "Compile script does not contain a ResourceCompile object\n");
        
        return loader.m_object->SafeCast<ResourceBuilder>();
    }

    //======================================================================================================================
    bool ResourceCacheLocal::ResourceNeedsBuild( Resource * res, ResourceBuilder *& builder ) {
        stl::Vector<stl::String::type>::type inputs;
        
        if ( fs->DoesFileExist( res->m_path.c_str() )  == false ) {
            // The file we're checking, doesn't actually exist. So just bail with true,
            // meaning that the resource needs to be compiled
            return true;
        }
        
        builder = LoadBuildScript( res );
        XE_ASSERT( builder != nullptr );
        
        inputs.push_back( ResourceBuilder::GetAssetCompilePathFromResource( res->m_path.c_str() ) );
        
        builder->GetDependencies( res, inputs);
        
        // Get the respufe target file time stamp
        uint64_t resFileTimeStamp;
        fs->GetModifiedTimestamp( resFileTimeStamp, res->m_path.c_str());
        
        // Get the time stamps of the inputs
        stl::Vector<uint64_t>::type inputTimeStamps;
        
        for ( auto & i : inputs ) {
            if (fs->DoesFileExist( i.c_str() ) == true ) {
                uint64_t timestamp;
                fs->GetModifiedTimestamp(timestamp, i.c_str() );
                inputTimeStamps.push_back( timestamp );
            }
        }

        // Check all of the output file timestamp agains the input timestamps. If the
        // input timestamps are newere than any of the outputs, we need to compile the
        // resource
        for ( auto inputTs : inputTimeStamps) {
            if (resFileTimeStamp < inputTs) {
                return true;
            }
        }
        
        // If we get to hear, the resource is up-to-date.
        return false;
    }
    
    //======================================================================================================================
    bool ResourceCacheLocal::PopPending() {
        std::scoped_lock<std::mutex> lock( m_mutex );
        
        if ( m_pending.empty() ) {
            m_loading = nullptr;
            return false;
        }
        
        m_loading = *( m_pending.end() - 1);
        m_pending.pop_back();
        return true;
    }
}
