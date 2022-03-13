//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_TYPES_H__
#define __I3D_TYPES_H__

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include <cmath>
#include <array>
#include <queue>
#include <bitset>
#include <type_traits>

#ifdef __APPLE__
#   include <TargetConditionals.h>
#endif

namespace i3d {

	typedef uint64_t id_t;
    
	typedef uint64_t handle_t;
    
	static const handle_t HANDLE_INVALID = 0;
    
    inline constexpr uint32_t FOURCC( const char * str ) {
        return (((uint32_t)str[0]) << 24) | (((uint32_t)str[1]) << 16) | (((uint32_t)str[2]) << 8) | ((uint32_t)str[0]);
    }
    
    enum PLATFORM_ID {
        PLATFORM_ID_NONE = 0,
        PLATFORM_ID_MACOS,
        PLATFORM_ID_IOS,
        PLATFORM_ID_TVOS,
        
        PLATFORM_ID_WIN,
        
        PLATFORM_ID_COUNT,
    };
    
    inline PLATFORM_ID GetPlatformIdFromString( const char * str ) {
        if (strcmp( str, "macos" ) == 0 ) {
            return PLATFORM_ID_MACOS;
        }
        else if (strcmp( str, "tvos" ) == 0 ) {
            return PLATFORM_ID_TVOS;
        }
        else if (strcmp( str, "ios" ) == 0 ) {
            return PLATFORM_ID_IOS;
        }
        else if (strcmp( str, "win" ) == 0 ) {
            return PLATFORM_ID_WIN;
        }
        else if (strcmp( str, "none" ) == 0 ) {
            return PLATFORM_ID_NONE;
        }
        
        return PLATFORM_ID_NONE;
    }
    
#if defined( _WIN32 ) || defined( WIN32 )
    static constexpr PLATFORM_ID THIS_PLATFORM = PLATFORM_WIN;
#elif defined( __APPLE__ )
#   if TARGET_OS_OSX
        static constexpr PLATFORM_ID THIS_PLATFORM = PLATFORM_ID_MACOS;
#   elif TARGET_OS_TV
        static constexpr PLATFORM_ID THIS_PLATFORM = PLATFORM_ID_TVOS;
#   elif TARGET_OS_IOS
        static constexpr PLATFORM_ID THIS_PLATFORM = PLATFORM_ID_IOS;
#   endif
#endif
}

#ifndef SAFE_DELETE
#   define SAFE_DELETE(X) if ((X)) { delete (X); }
#endif

#ifndef SAFE_DELETEARRAY
#   define SAFE_DELETEARRAY(X) if ((X)) { delete [](X); }
#endif

#define DISALLOW_COPY_MOVE(C)\
    protected: C( const C & ) {}\
    protected: C( C && ) {}

#define needs_construct(__type__) (std::is_pod<__type__>() == false)
#define needs_destruct(__type__) (std::is_pod<__type__>() == false)

#define BIT(X) (1 << (X))
#define BIT64(X) ((uint64_t)(1 << (X)))
#define XE_UNUSED(V) (void)V

#endif
