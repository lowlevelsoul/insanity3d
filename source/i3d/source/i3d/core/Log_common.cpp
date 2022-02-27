//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/core/Hash.h"
#include "i3d/core/Log.h"
#include <mutex>

namespace i3d {

    LogCat_t LOG_CAT_MISC   ("   LOG");
    LogCat_t LOG_CAT_ERROR  (" ERROR");
    LogCat_t LOG_CAT_ASSERT ("ASSERT");
    LogCat_t LOG_CAT_WARN   ("  WARN");
    
    //======================================================================================================================
    LogCat_t::LogCat_t(const char* name) {
        assert(name != nullptr);
        m_name = name;
        m_nameHash = i3d::fh64::CalcFromString(name);
    }

    //======================================================================================================================
    LogReporterDebug::LogReporterDebug() {
    }

    //======================================================================================================================
    LogReporterDebug::~LogReporterDebug() {
        
    }

    //======================================================================================================================
    void LogReporterDebug::Printf(const LogCat_t& cat, const char* fmt, va_list vargs) {
        // initialize use of the variable argument array
        va_list vaCopy;
        
        va_copy(vaCopy, vargs);
        vprintf(fmt, vaCopy);                   // Use vprintf to print the log message
        va_end(vaCopy);
    }
}

