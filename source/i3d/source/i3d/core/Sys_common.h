//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_SYS_BASE_H__
#define __I3D_SYS_BASE_H__

#include "i3d/core/Types.h"
#include "i3d/core/Sys.h"

namespace i3d {
    
    class SysCommon : public Sys {
    public:
        virtual ~SysCommon() {}
               
        /// Makes a formated string from a variable-argument list
        virtual const stl::String::type VformatArgs( const char* fmt, va_list vaArgs ) override;
    };
}

#endif
