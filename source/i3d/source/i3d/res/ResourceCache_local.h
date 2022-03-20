//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_RESOURCE_LOCAL_H__
#define __I3D_RESOURCE_LOCAL_H__

#include "i3d/res/Resource.h"

namespace i3d {
    
    class ResourceBuilder;

    class ResourceCacheLocal : public ResourceCache {
    public:
        ResourceCacheLocal();
            
        virtual ~ResourceCacheLocal();
        
        void Initialise();
            
        virtual void PublishFactory( ResourceFactory * factory ) override;
        
        virtual Resource * Load( const char * path ) override;
            
        virtual Resource * LoadImmediate( const char * path );

        virtual void Unload( Resource * res ) override;

        virtual Resource * Find( const char* path ) override;
        
        virtual void StartLoading() override;
        
        virtual bool HasPending() override;
            
        Resource * Find( uint64_t hash );
            
        Resource * CreateResource( const char * path );
            
    protected:
        
        static int LoadThreadFunc( );
        
        bool PopPending();
        
        bool ResourceNeedsBuild( Resource * res, ResourceBuilder *& compiler );
            
        void MakePathCanonoical( stl::String::type & pathOut, const char * path );
            
        uint64_t CalcPathHash( const char * path );
        
        ResourceBuilder * LoadBuildScript( Resource * res );
        
        bool HasBuildSript( Resource * res );
            
    protected:
        static const size_t FACTORY_BUCKET_COUNT = 8;
        static const size_t RESOURCE_BUCKET_COUNT = 64;
            
        typedef stl::Map< uint64_t, ResourceFactory *> factory_table_t;
        typedef stl::Map< uint64_t, Resource *> resource_table_t;
            
        factory_table_t::type           m_factoryTable;
        resource_table_t::type          m_resourceTable;
        
        i3d::PLATFORM_ID                m_compileTargetPlatform;    ///< The target platform used for compiling resources
        std::mutex                      m_mutex;                    ///< Sync object used for multi-threading
        stl::Vector<Resource *>::type   m_pending;                  ///< List of pending resources that we want to load
        Resource *                      m_loading;                  ///< The resource that is currently being loaded
    };
}

#endif 
