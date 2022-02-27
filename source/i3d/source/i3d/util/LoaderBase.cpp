//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//==========================================================================================================================================

#include "i3d/util/LoaderBase.h"
#include "i3d/core/Sys.h"
#include "i3d/core/FileSystem.h"

namespace i3d {

    //==========================================================================================================================================
    LoaderBase::LoaderBase() {
        Reset();
    }

    //==========================================================================================================================================
    LoaderBase::~LoaderBase() {

    }
          
    //==========================================================================================================================================
    void LoaderBase::PrintError( const char * fmt, ... ) {
        m_log.append( sys->Vformat( "ERROR: %u :", GetLine() ) );

        va_list args;
        va_start( args, fmt );

        stl::String::type msg = sys->Vformat( fmt, args );

        va_end( args );

        m_log.append( msg );
        m_log.append( "\n" );

        ++m_errorCount;
    }

    //==========================================================================================================================================
    void LoaderBase::PrintWarning( const char * fmt, ... ) {
        m_log.append( sys->Vformat( "WARNING: %u :", GetLine() ) );

        va_list args;
        va_start( args, fmt );

        stl::String::type msg = sys->Vformat( fmt, args );

        va_end( args );

        m_log.append( msg );
        m_log.append( "\n" );

        ++m_warnCount;
    }

    //==========================================================================================================================================
    void LoaderBase::Reset() {
        m_warnCount = 0;
        m_errorCount = 0;
        m_line = 1;
    }
}
