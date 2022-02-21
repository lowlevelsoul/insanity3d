/*
==========================================================================================================================================
CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
Copyright (C) 2021 James Steele. All Rights Reserved.
==========================================================================================================================================
*/

#include "i3d/Quat.h"

/*======================================================================================================================================*/
void _Quat_Concat( Quat_t * dst, const Quat_t * lhs, const Quat_t * rhs ) {

}

/*======================================================================================================================================*/
float _Quat_Normalise( Quat_t * q ) {
	float mag = Quat_Magnitude( *q );
	float fac = 1.0f / mag;
	assert( Scalar_IsValid( fac ) == true );

	Quat_MulScalar( *q, *q, fac );
	return mag;
}