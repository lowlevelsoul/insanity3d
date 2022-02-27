//==========================================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//==========================================================================================================================================

#ifndef __I3D_LOADERBASE_H__
#define __I3D_LOADERBASE_H__

#include "i3d/core/Types.h"
#include "i3d/math/Math3dTypes.h"

namespace i3d {

    class Model;
    class File;

    class LoaderBase {
    public:
        LoaderBase();

        ~LoaderBase();

        void PrintError( const char * fmt, ... );

        void PrintWarning( const char * fmt, ... );

        void Reset();

        bool HasError() const {
            return m_errorCount > 0;
        }

        bool HasWarning() const {
            return m_warnCount > 0;
        }

        const char * GetLog() const {
            return m_log.c_str();
        }

        void IncrementLine() {
            ++m_line;
        }

        uint32_t GetLine() const {
            return m_line;
        }

    protected:
        uint32_t				m_line;
        stl::String::type		m_log;
        uint32_t				m_errorCount;
        uint32_t				m_warnCount;
    };
}

#endif
