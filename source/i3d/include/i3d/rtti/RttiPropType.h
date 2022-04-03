//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//=======================================================================================================================

#ifndef __I3D_RTTIPROPTYPE_H__
#define __I3D_RTTIPROPTYPE_H__

#include "i3d/core/Types.h"
#include "i3d/math/Math3dTypes.h"

namespace i3d {

    class Resource;
    class RttiObject;

    enum RTTI_PROP_TYPE {
        RTTI_PROP_TYPE_NULL = 0,
        
        RTTI_PROP_TYPE_INT,
        RTTI_PROP_TYPE_FLOAT,
        RTTI_PROP_TYPE_BOOL,
        
        RTTI_PROP_TYPE_VEC2,
        RTTI_PROP_TYPE_VEC3,
        RTTI_PROP_TYPE_VEC4,
        RTTI_PROP_TYPE_QUAT,
           
        RTTI_PROP_TYPE_OBJECT_PTR,
        RTTI_PROP_TYPE_OBJECT_REFPTR,
        RTTI_PROP_TYPE_STRING,
        RTTI_PROP_TYPE_RESOURCE,
        
        RTTI_PROP_TYPE_COUNT
    };


    struct RttiEnumElement {
        const char*     m_name;
        uint32_t        m_value;
    };

    struct RttiPropDef {
        RTTI_PROP_TYPE      m_type;
        const char*         m_name;
        uint32_t            m_dim;
        uint32_t            m_offset;
        uint64_t            m_isVector : 1;             ///< if 1, this prop is a std::vector of types
    };
}

#endif
