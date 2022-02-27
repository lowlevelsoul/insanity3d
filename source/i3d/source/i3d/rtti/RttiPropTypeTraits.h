//=======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021 James Steele. All Rights Reserved.
//=======================================================================================================================

#ifndef __I3D_RTTITYPETRAITS_H__
#define __I3D_RTTITYPETRAITS_H__

#include "i3d/rtti/RttiPropType.h"

namespace i3d {

class RttiObject;


/*

inline template<typename __type__> RTTI_PROP_TYPE RttiPropTraitsType() {}
inline template<typename __type__> size_t RttiPropTraitsSize() {}
inline template<typename __type__> bool RttiPropTraitsIsStdVector() {}
inline template<typename __type__> bool RttiPropTraitsIsStdMap() {}
inline template<typename __type__> bool RttiPropTraitsIsSigned() {}
inline template<typename __type__> bool RttiPropTraitsIsPOD() {}

#define RTTI_TYPE_TRAITS(TYPE, RTTI_TYPE, POD)\
inline template<> RTTI_PROP_TYPE RttiPropTraitsType<TYPE>() { return RTTI_TYPE; }\
inline template<> size_t RttiPropTraitsSize<TYPE>() { return sizeof(TYPE); }\
inline template<> bool RttiPropTraitsIsStdVector<TYPE>() { return false; }\
inline template<> bool RttiPropTraitsIsStdMap<TYPE>() { return false; }\
inline template<> bool RttiPropTraitsIsSigned<TYPE>() { return false; }\
inline template<> bool RttiPropTraitsIsPOD<TYPE>() { return POD; }

#define RTTI_TYPE_TRAITS_VECTOR(TYPE, RTTI_TYPE)\
inline template<> RTTI_PROP_TYPE RttiPropTraitsType<TYPE>() { return RTTI_TYPE; }\
inline template<> size_t RttiPropTraitsSize<TYPE>() { return sizeof(TYPE); }\
inline template<> bool RttiPropTraitsIsStdVector<TYPE>() { return true; }\
inline template<> bool RttiPropTraitsIsStdMap<TYPE>() { return false; }\
inline template<> bool RttiPropTraitsIsPOD<TYPE>() { return false; }

RTTI_TYPE_TRAITS(float, RTTI_PROP_TYPE_FLOAT, true)
RTTI_TYPE_TRAITS(int32_t, RTTI_PROP_TYPE_INT, true)
RTTI_TYPE_TRAITS(uint32_t, RTTI_PROP_TYPE_UINT, true)
RTTI_TYPE_TRAITS(int64_t, RTTI_PROP_TYPE_INT64, true)
RTTI_TYPE_TRAITS(uint64_t, RTTI_PROP_TYPE_UINT64, true)
RTTI_TYPE_TRAITS(bool, RTTI_PROP_TYPE_BOOL, true)

RTTI_TYPE_TRAITS(Vector3, RTTI_PROP_TYPE_VEC3, false)
RTTI_TYPE_TRAITS(Vector4, RTTI_PROP_TYPE_VEC4, false)
RTTI_TYPE_TRAITS(Quaternion, RTTI_PROP_TYPE_QUAT, false)
RTTI_TYPE_TRAITS(Matrix4, RTTI_PROP_TYPE_MAT4, false)

RTTI_TYPE_TRAITS(RttiObject*, RTTI_PROP_TYPE_OBJECT_PTR, false)
RTTI_TYPE_TRAITS(RefPointer<RttiObject>, RTTI_PROP_TYPE_OBJECT_REFPTR, false)
RTTI_TYPE_TRAITS(std::string, RTTI_PROP_TYPE_STRING, false)

RTTI_TYPE_TRAITS_VECTOR(std::vector<float>, RTTI_PROP_TYPE_FLOAT)
RTTI_TYPE_TRAITS_VECTOR(std::vector<int32_t>, RTTI_PROP_TYPE_INT)
RTTI_TYPE_TRAITS_VECTOR(std::vector<uint32_t>, RTTI_PROP_TYPE_UINT)
RTTI_TYPE_TRAITS_VECTOR(std::vector<int64_t>, RTTI_PROP_TYPE_INT)
RTTI_TYPE_TRAITS_VECTOR(std::vector<uint64_t>, RTTI_PROP_TYPE_UINT)
RTTI_TYPE_TRAITS_VECTOR(std::vector<bool>, RTTI_PROP_TYPE_BOOL)

RTTI_TYPE_TRAITS_VECTOR(std::vector<Vector3>, RTTI_PROP_TYPE_VEC3)
RTTI_TYPE_TRAITS_VECTOR(std::vector<Vector4>, RTTI_PROP_TYPE_VEC4)
RTTI_TYPE_TRAITS_VECTOR(std::vector<Quaternion>, RTTI_PROP_TYPE_QUAT)
RTTI_TYPE_TRAITS_VECTOR(std::vector<Matrix4>, RTTI_PROP_TYPE_MAT4)

RTTI_TYPE_TRAITS_VECTOR(std::vector<RttiObject*>, RTTI_PROP_TYPE_OBJECT_PTR)
RTTI_TYPE_TRAITS_VECTOR(std::vector<RefPointer<RttiObject> >, RTTI_PROP_TYPE_OBJECT_REFPTR)
RTTI_TYPE_TRAITS_VECTOR(std::vector<std::string>, RTTI_PROP_TYPE_STRING)
 
 */
    
}

#endif /* RttiTypeTraits_h */
