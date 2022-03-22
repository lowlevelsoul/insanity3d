//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#import <Foundation/Foundation.h>

#include <mutex>
#include <new>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>
#include <errno.h>
#include <dlfcn.h>

#include "i3d/core/Log.h"
#include "i3d/core/Sys_ios.h"

i3d::Sys * sys = nullptr;

namespace i3d {
    
    //======================================================================================================================
    void SysCreate() {
        if ( sys != nullptr ) {
            return;
        }
            
        sys = new SysIos;
    }
    
    //======================================================================================================================
    void SysDestroy() {
        SAFE_DELETE( sys );
    }
    
    //======================================================================================================================
    SysIos::SysIos() {
        timeIsMonotomic = false;
        
        //XE_LOG("==== System Init ====\n");
        
        kern_return_t res = mach_timebase_info(&timeBaseInfo);
               
        // Is the machine CPU clock constant, or does it change over time?
        timeIsMonotomic  = (res == 0);

        if (timeIsMonotomic ) {
            //XE_LOG("System clock is monotomic\n");
            // CPU clock is constant, so use the machine absolute time tick as our clock-tick
            machStartTime = mach_absolute_time();
        }
        else {
            //XE_LOG("System clock is non-monotomic\n");
            // CPU clock is not constant, so use the absolute system time
           gettimeofday(&tvStartTime, NULL);
        }
    }
    
    //======================================================================================================================
    SysIos::~SysIos() {
    }

    //======================================================================================================================
    uint64_t SysIos::GetTicks() {
        uint64_t ticks;
        
        if ( timeIsMonotomic == true) {
            uint64_t now = mach_absolute_time();
            ticks = (uint64_t)((((now - machStartTime) * timeBaseInfo.numer) / timeBaseInfo.denom) / 1000000);
        } else {
            struct timeval now;
            gettimeofday(&now, NULL);
            ticks = (uint64_t)((now.tv_sec - tvStartTime.tv_sec) * 1000 + (now.tv_usec - tvStartTime.tv_usec) / 1000);
        }
        
        return ticks;
    }

    //======================================================================================================================
    void SysIos::Breakpoint() {
        __builtin_trap();
    }

    //======================================================================================================================
    void SysIos::Exit(int32_t code) {
        exit(code);
    }

    //======================================================================================================================
    void SysIos::AlertMsgArgs(const char* title,  const char* fmt, va_list vaArgs ) {
    }

    //======================================================================================================================
    void SysIos::AssertMsgArgs( const char* file, int32_t line, const char* fmt, va_list vaArgs ) {
        
    }
    
    //======================================================================================================================
    bool SysIos::Exec( const char * appPath, stl::Vector<stl::String::type>::type & args ) {
        return false;
    }
    
    
}
