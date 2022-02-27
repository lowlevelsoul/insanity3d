//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_RESOURCE_H__
#define __I3D_RESOURCE_H__

#include "i3d/core/Types.h"
#include "i3d/core/Array.h"

namespace i3d {

    class Resource;
    class File;

    class ResourceFactory {
    public:
        ResourceFactory( const char * ext ) {
            m_ext = ext;
        }

        virtual ~ResourceFactory() {
        }

        virtual Resource * Create() = 0;

        virtual void OnPublish() {
        }

        virtual void OnDestroy() {
        }

        const char * GetExtensions() const {
            return m_ext;
        }

    protected:
        const char * m_ext;
    };

    /// The base class for anything that is a resource
    class Resource {
        friend class ResourceCacheLocal;
    public:
        virtual ~Resource() {
        }

        virtual void Load( File * file, const char * path ) {

        }

        void SetHash( uint64_t hash ) {
            m_hash = hash;
        }

        uint64_t Gethash() const {
            return m_hash;
        }
        
        const char * GetPath() const { return m_path.c_str(); }
            
    protected:
        stl::String::type       m_path;
        uint64_t		        m_hash;
    };

    class ResourceCache {
    public:
        virtual ~ResourceCache() {

        }

        template<typename __type__>
        void PublishFactory() {
            PublishFactory( &__type__::FACTORY );
        }

        virtual void PublishFactory( ResourceFactory * factory ) = 0;

        template<typename __type__>
        __type__ * Load( const char * path ) {
            return ( __type__ * ) Load( path );
        }

        virtual Resource * Load( const char * path ) = 0;

        virtual Resource * Find( const char * path ) = 0;

        virtual void Unload( Resource * res ) = 0;
        
        virtual void StartLoading() = 0;
        
        virtual bool HasPending() = 0;

    };
}

extern i3d::ResourceCache * res;

#define DECLARE_RESOURCE( NAME )\
	public:\
class ResourceFactory##NAME : public i3d::ResourceFactory {\
	public:\
        ResourceFactory##NAME( const char * ext ) : i3d::ResourceFactory( ext ) {\
		}\
        virtual ~ResourceFactory##NAME () {\
		}\
        virtual i3d::Resource * Create() { return new NAME; }\
	};\
    static ResourceFactory##NAME FACTORY;
	

#define DEFINE_RESOURCE( CLASS, EXT ) CLASS::ResourceFactory##CLASS CLASS::FACTORY(EXT);

#define DECLARE_RESOURCE_BEGIN( NAME )\
public:\
class ResourceFactory##NAME : public i3d::ResourceFactory {\
public:\
    ResourceFactory##NAME( const char * ext ) : i3d::ResourceFactory( ext ) {}\
    virtual ~ResourceFactory##NAME() {}\
    virtual i3d::Resource * Create() { return new NAME; }

#define DECLARE_RESOURCE_END(NAME)\
};\
static ResourceFactory##NAME FACTORY;

#endif
