//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//=======================================================================================================================

#ifndef __I3D_HASH_H__
#define __I3D_HASH_H__

#include "i3d/core/Types.h"

namespace i3d {

    namespace crc32 {
        uint32_t Calc(const void* buffer, size_t bufferSize);
        uint32_t CalcFromString(const char* string);
    }

    namespace fh64 {
        uint64_t Calc(const void* buffer, size_t bufferLen);
        uint64_t CalcFromString(const char* buffer);
    }
}

#endif
