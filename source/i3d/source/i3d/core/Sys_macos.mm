//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

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

#include "i3d/core/Sys_macos.h"
#include "i3d/core/Log.h"

class i3d::Sys * sys = nullptr;

namespace i3d {
    
    //======================================================================================================================
    void SysCreate() {
        if ( sys != nullptr ) {
            return;
        }
            
        sys = new SysMacos;
    }
    
    //======================================================================================================================
    void SysDestroy() {
        SAFE_DELETE( sys );
    }
    
    //======================================================================================================================
    SysMacos::SysMacos() {
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
    SysMacos::~SysMacos() {
    }

    //======================================================================================================================
    uint64_t SysMacos::GetTicks() {
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
    void SysMacos::Breakpoint() {
        __builtin_trap();
    }

    //======================================================================================================================
    void SysMacos::Exit(int32_t code) {
        exit(code);
    }

    //======================================================================================================================
    void SysMacos::AlertMsgArgs(const char* title,  const char* fmt, va_list vaArgs ) {

        stl::String::type messageText = VformatArgs(fmt, vaArgs);
        va_end(vaArgs);
        
        /* Display the dialog for the message */
        NSString* nsMsg = [ NSString stringWithUTF8String:messageText.c_str() ];
        NSString* nsTitle = [ NSString stringWithUTF8String:title ];
        dispatch_sync(dispatch_get_main_queue(), ^(){
            NSAlert *alert = [[NSAlert alloc] init];
            [alert setMessageText:nsTitle];
            [alert setInformativeText:nsMsg];
            [alert addButtonWithTitle:@"Ok"];
            [alert runModal]; }
        );
    }

    //======================================================================================================================
    void SysMacos::AssertMsgArgs( const char* file, int32_t line, const char* fmt, va_list vaArgs ) {
        
        stl::String::type assertText = Vformat("%File: %s\nLine: %i\n", file, line );
        
        stl::String::type messageText = VformatArgs(fmt, vaArgs);
        va_end(vaArgs);
        
        assertText.append(messageText);
        
        /* Display the dialog for the message */
        NSString* nsMsg = [NSString stringWithUTF8String: assertText.c_str() ];
        
        dispatch_sync(dispatch_get_main_queue(), ^(){
            NSAlert *alert = [[NSAlert alloc] init];
            [alert setMessageText: @"Assert"];
            [alert setInformativeText:nsMsg];
            [alert addButtonWithTitle:@"Ok"];
            [alert runModal]; }
                      );
    }
    
    //======================================================================================================================
    bool SysMacos::Exec( const char * appPath, stl::Vector<stl::String::type>::type & args ) {
        @try
        {
            // Set up the process
            NSTask *t = [[NSTask alloc] init];
            [t setLaunchPath: [[NSString alloc] initWithCString: appPath encoding: NSASCIIStringEncoding]];
            
            NSMutableArray * argArray = [[NSMutableArray alloc] init];
            for ( auto & a : args ) {
                NSString * thisArg = [[NSString alloc] initWithCString: a.c_str() encoding:NSASCIIStringEncoding];
                [argArray addObject:thisArg];
            }
            
            [t setArguments: argArray];
            
            // Launch (forks) the process
            [t launch]; // raises an exception if something went wrong
            [t waitUntilExit];
        }
        @catch (NSException *e)
        {
            NSLog(@"Expection occurred %@", [e reason]);
            return false;
        
        }

        return true;
    }
}


