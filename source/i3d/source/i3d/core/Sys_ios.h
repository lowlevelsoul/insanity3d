//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_SYS_IOS_H__
#define __I3D_SYS_IOS_H__

#include "i3d/core/Types.h"
#include "i3d/core/Sys_common.h"

namespace i3d {
    
    class SysIos : public SysCommon {
    public:
        SysIos();
        
        virtual ~SysIos();
        
        /// Returns the number of millisecond-ticks that have occured since system startup
        virtual uint64_t GetTicks() override;
        
        /// Calls the system breakiing function
        virtual void Breakpoint() override;
        
        /// Forces rhe application to exit with an error code
        virtual void Exit( int32_t code ) override;
        
        /// Displays a pop-up message
        virtual void AlertMsgArgs( const char * title, const char * fmt, va_list vaArgs ) override;
        
        /// Displays an assert message
        virtual void AssertMsgArgs( const char * file, int32_t line, const char * fmt, va_list vaArgs ) override;
        
        /// Executes a program as a new process, waiting for the process to finish ececution
        virtual bool Exec( const char * appPath, stl::Vector<stl::String::type>::type & args ) override;
        
    public:
        bool                         timeIsMonotomic;
        mach_timebase_info_data_t    timeBaseInfo;
        uint64_t                     machStartTime;
        struct timeval               tvStartTime;
    };
}

#endif
