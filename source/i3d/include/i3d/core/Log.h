//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_LOG_H__
#define __I3D_LOG_H__

#include "i3d/core/Types.h"
#include "i3d/core/Sys.h"

namespace i3d {

    struct LogCat_t {
        const char*     m_name;
        uint64_t        m_nameHash;
        
        LogCat_t(const char* name);
    };

    /// The interface for printing out the log messages
    class LogReporter {
    public:
        virtual ~LogReporter() {
        }

        virtual void Printf(const LogCat_t& cat, const char* fmt, va_list vargs) = 0;
    };

    class LogReporterDebug : public LogReporter {
    public:
        LogReporterDebug();
        
        virtual ~LogReporterDebug();
        
        virtual void Printf(const LogCat_t& cat, const char* fmt, va_list vargs) override;
    };
    
    class Log {
    public:
        virtual ~Log() {}
        
        virtual void PublishReporter( LogReporter* reporter ) = 0;
    
        virtual void PrintfArgs( const LogCat_t& cat, const char* fmt, va_list vaArgs ) = 0;
        
        inline void Printf(const LogCat_t& cat, const char* fmt, ...) {
            va_list vaArgs;
            va_start(vaArgs, fmt);
            PrintfArgs( cat, fmt, vaArgs );
        }
    };

    extern LogCat_t LOG_CAT_MISC;
    extern LogCat_t LOG_CAT_ERROR;
    extern LogCat_t LOG_CAT_ASSERT;
    extern LogCat_t LOG_CAT_WARN;
}
    
extern i3d::Log * logsys;


#define XE_LOG(...) logsys->Printf(i3d::LOG_CAT_MISC, __VA_ARGS__ )
#define XE_ERROR(C, ...) (void)((!(C)) || ( logsys->Printf( i3d::LOG_CAT_ERROR, __VA_ARGS__ ), sys->Exit( 0 ), 0 ) )
#define XE_WARN(C, ...) (void)((!(C)) || ( logsys->Printf( i3d::LOG_CAT_WARN, __VA_ARGS__ ), 0 ) )

#endif
