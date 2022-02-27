//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/core/Sys_common.h"

namespace i3d {
    
    //======================================================================================================================
    const stl::String::type SysCommon::VformatArgs( const char* fmt, va_list vaArgs ) {
        // initialize use of the variable argument array
        va_list vaArgsCopy;
        
        // reliably acquire the size
        // from a copy of the variable argument array
        // and a functionally reliable call to mock the formatting
        va_copy(vaArgsCopy, vaArgs);
        size_t len = std::vsnprintf(NULL, 0, fmt, vaArgsCopy);
        va_end(vaArgsCopy);
        
        stl::String::type ret;
        ret.resize(len);

        // return a formatted string without risking memory mismanagement
        // and without assuming any compiler or platform specific behavior
        std::vsnprintf(&ret[0], len+1, fmt, vaArgs);
        va_end(vaArgs);
        
        return ret;
    }
}
