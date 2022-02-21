/*
==========================================================================================================================================
CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
Copyright (C) 2021 James Steele. All Rights Reserved.
==========================================================================================================================================
*/

#include "i3d/Fs.h"
#include <string.h>

/*======================================================================================================================================*/
bool_t FS_AppendPath( String_t * lhs, const char * rhs ) {

	size_t rhsLen, lhsLen, totalLen;
	bool_t lhsSep, rhsSep;
	char separatorStr[ 2 ] = { FS_GetSeparator(), 0 };

	if ( rhs == NULL || rhs[ 0 ] == 0 ) {
		return true;
	}

	if ( *lhs == NULL ) {
		/* String we;'re appending to is NULL, so just copy rhs */
		Str_Copy( lhs, rhs );
		return;
	}

	lhsLen = Str_Length( *lhs );
	rhsLen = strlen( rhs );

	lhsSep = (*lhs)[ lhsLen - 1 ] == FS_GetSeparator() || (*lhs)[ lhsLen - 1 ] == FS_GetOtherSeparator();
	rhsSep = rhs[ 0 ] == FS_GetSeparator() || rhs[ 0 ] == FS_GetOtherSeparator();
	
	if ( lhsSep == rhsSep ) {
		if ( lhsSep == true ) {
			/* Both strings have a seperator, so we only want one. Just trim
			   the leading char from rhs */
			Str_Append( lhs, rhs + 1 );
		}
		else {
			/* Neither string has a separator - so add one of our own */
			totalLen = rhsLen + lhsLen + 1;

			Str_Reserve( lhs, totalLen );
			Str_Append( lhs, separatorStr );
			Str_Append( lhs, rhs );
		}
	}
	else {
		/* Only one string has a separator - so we can just joint the two */
		Str_Append( lhs, rhs );
	}

	return true;
}

/*======================================================================================================================================*/
bool_t FS_JoinPath( String_t * dst, const char * lhs, const char * rhs ) {

	if ( lhs == NULL || lhs[ 0 ] == NULL ) {
		/* Lhs is null or zero length but rhs is valid so just copy rshs */
		Str_Copy( dst, rhs );
		return true;
	}
	else if ( rhs == NULL || rhs[ 0 ] == NULL ) {
		/* Rhs is null or zero length but lhs is valid so just copy lhs */
		Str_Copy( dst, lhs );
		return true;
	}
	else {
		/* Both paths are valid, so join them together */

		size_t totalLen = strlen( lhs ) + strlen( rhs ) + 1;
		Str_Reserve( dst, totalLen );
		Str_Copy( dst, lhs );
		FS_AppendPath( dst, rhs );
	}

	return true;
}