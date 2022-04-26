//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include <cmath>
#include "i3d/math/Math3d.h"

namespace i3d {
    
    const Vector2 Vector2::UNIT_X( 1, 0 );
    const Vector2 Vector2::UNIT_Y( 0, 1 );
    const Vector2 Vector2::ZERO( 0, 0 );

    const Vector3 Vector3::UNIT_X( 1, 0, 0 );
    const Vector3 Vector3::UNIT_Y( 0, 1, 0 );
    const Vector3 Vector3::UNIT_Z( 0, 0, 1 );
    const Vector3 Vector3::ZERO( 0, 0, 0 );

    const Vector4 Vector4::UNIT_X( 1, 0, 0, 0 );
    const Vector4 Vector4::UNIT_Y( 0, 1, 0, 0 );
    const Vector4 Vector4::UNIT_Z( 0, 0, 1, 0 );
    const Vector4 Vector4::UNIT_W( 0, 0, 0, 1 );
    const Vector4 Vector4::ZERO( 0, 0, 0, 0 );

    const Quaternion Quaternion::IDENTITY( 0, 0, 0, 1 );

    const Matrix2 Matrix2::IDENTITY( 1, 1 );
    const Matrix3 Matrix3::IDENTITY( 1, 1, 1 );
    const Matrix4 Matrix4::IDENTITY( 1, 1, 1, 1 );
}
