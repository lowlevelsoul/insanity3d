//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_SYS_H__
#define __I3D_SYS_H__

#include "i3d/core/Types.h"

namespace i3d {
    
    class Sys {
    public:
        
        virtual ~Sys() {}

        template<typename __type__>
        static __type__ AlignValue(__type__ value, __type__ alignment) {
            __type__ rem = value % alignment;
            return (rem == 0) ? value : (value + (alignment - rem));
        }
        
        template<typename __value__, typename __index__>
        static bool BinarySearch( __index__& indexOut, const __value__& value, const __value__* valueArray, size_t arraySize ) {
            __index__ first = 0;
            __index__ last = (__index__)( arraySize - 1 );
            bool found = false;

            while ( first <= last ) {
                __index__ mid = ( ( last - first ) / 2 ) + first;
                if ( valueArray[ mid ] == value ) {
                    found = true;
                    first = mid;
                    break;
                } else if ( valueArray[ mid ] < value ) {
                    first = mid + 1;
                } else {
                    last = mid - 1;
                }
            }

            indexOut = first;
            return found;
        }
        
        static bool GetNextStringItem( const char *& strBegin, char * itemBuff, size_t itemBuffLen ) {
            // If we're at a terminating character, let's just bail
            if ( *strBegin == 0 ) {
                return false;
            }
            
            // Loop until we find a seperating semi-colon or the end of the string
            const char * strEnd = strBegin;
            while ( *strEnd != ';' && *strEnd != 0 ) {
                ++strEnd;
            }
            
            // Copy over the string
            size_t strLen = strEnd - strBegin;
            size_t cpyLen = (strLen < itemBuffLen) ? strLen : itemBuffLen - 1;
            strncpy( itemBuff, strBegin, cpyLen );
            itemBuff[ cpyLen ] = 0;
            
            // Move the string start pointer past the end of the string
            
            strBegin = (*strEnd == 0) ? strEnd : strEnd + 1;
            
            return true;
        }
        
        /// Returns the number of millisecond-ticks that have occured since system startup
        virtual uint64_t GetTicks() = 0;
        
        /// Calls the system breakiing function
        virtual void Breakpoint() = 0;
        
        /// Forces rhe application to exit with an error code
        virtual void Exit( int32_t code ) = 0;
        
        /// Displays a pop-up message
        virtual void AlertMsgArgs( const char * title, const char * fmt, va_list vaArgs ) = 0;
        
        /// Displays an assert message
        virtual void AssertMsgArgs( const char * file, int32_t line, const char * fmt, va_list vaArgs ) = 0;
        
        /// Makes a formated string from a variable-argument list
        virtual const stl::String::type VformatArgs( const char* fmt, va_list vaArgs ) = 0;

        /// Executes a program as a new process, waiting for the process to finish ececution
        virtual bool Exec( const char * appPath, stl::Vector< stl::String::type >::type & args ) = 0;
        
        /// Makes a formated string from a a format string and arguments
        inline const stl::String::type Vformat( const char* fmt, ... ) {
            va_list vaArgs;
            va_start(vaArgs, fmt);
            return VformatArgs( fmt, vaArgs );
        }
        
        /// Displays a pop-up message
        inline void AlertMsg( const char * title,  const char * fmt, ... ) {
            va_list vaArgs;
            va_start(vaArgs, fmt);
            AlertMsgArgs( title, fmt, vaArgs );
        }
        
        /// Displays an assert message
        inline void AssertMsg( const char * file, int32_t line, const char * fmt, ... ) {
            va_list vaArgs;
            va_start(vaArgs, fmt);
            AssertMsgArgs( file, line, fmt, vaArgs );
        }
    };
}

extern i3d::Sys * sys;

#if defined (DEBUG) || defined (_DEBUG)
#   define XE_ASSERT(C) (void)((C) || (sys->AssertMsg( __FILE__, __LINE__, #C), sys->Breakpoint(), 0))
#   define XE_ASSERT_MSG(C, ...) (void)((C) || (sys->AssertMsg( __FILE__, __LINE__, __VA_ARGS__), sys->Breakpoint(), 0))
#else
#   define XE_ASSERT(C)
#   define XE_ASSERT_MSG(C, ...)
#endif

#endif
