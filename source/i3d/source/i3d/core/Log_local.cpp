//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/core/Hash.h"
#include "i3d/core/Log.h"
#include <mutex>

i3d::Log * logsys = nullptr;

namespace i3d {
    
    class LogLocal : public Log {
    public:
        static const size_t REPORTER_CAPACITY = 16;
        
        LogLocal();
        
        virtual ~LogLocal();
        
        virtual void PublishReporter( LogReporter* reporter ) override;
    
        virtual void PrintfArgs( const LogCat_t& cat, const char* fmt, va_list vaArgs ) override;
        
        std::mutex          m_mutex;
        LogReporter *       m_reporters[REPORTER_CAPACITY];
        size_t              m_reporterCount;
    };
    
    //======================================================================================================================
    void LogCreate() {
        if ( logsys != nullptr ) {
            return;
        }
        
        logsys = new LogLocal;
    }
    
    //======================================================================================================================
    void LogDestroy() {
        SAFE_DELETE( logsys );
    }
    
    
    //======================================================================================================================
    LogLocal::LogLocal() {
        m_reporterCount = 0;
    }
    
    //======================================================================================================================
    LogLocal::~LogLocal() {
        
    }


    //======================================================================================================================
    void LogLocal::PublishReporter( LogReporter* reporter ) {
        std::scoped_lock lock ( m_mutex );
        
        assert(m_reporterCount < LogLocal::REPORTER_CAPACITY);
        
        m_reporters[m_reporterCount] = reporter;
        ++m_reporterCount;
    }

    //======================================================================================================================
    void LogLocal::PrintfArgs( const LogCat_t& cat, const char* fmt, va_list vaArgs ) {
        std::scoped_lock lock ( m_mutex );
    
        for(uint32_t i=0; i < m_reporterCount; ++i) {
            m_reporters[i]->Printf(cat, fmt, vaArgs);
        }
        
        va_end(vaArgs);
    }

}

